#include "parser.h"

Parser::Parser()
{
    headerRegular = new QRegularExpression("(?=HEADER)(.|\n)*?(?<=VALUE)(?=VALUE)(.|\n)*?(?<=END)", QRegularExpression::MultilineOption);
    valuesRegular = new QRegularExpression("(?=\"time\")(?>.*\n){7}", QRegularExpression::MultilineOption);
}

TransientAnalysisEntity Parser::parse(QString toParse)
{
    if(toParse.isEmpty()){
        throw ParseException("Нечего парсить");
    }

//    QRegularExpressionMatchIterator headerMatch = headerRegular->globalMatch(toParse);
//    if(headerMatch.hasNext()){
//        QRegularExpressionMatch match = headerMatch.next();
//        if(match.hasMatch()){

//            qDebug() << match.capturedRef().toString();

////            qDebug() << match.lastCapturedIndex();
//        }
//    } else {
//        qDebug() << "Header not found";
//        throw ParseException("HEADER не найден");
//    }

//    while(headerMatch.hasNext()){
//        qDebug() << "----";
//        qDebug() << headerMatch.next().capturedTexts();
//    }

    //ебашу как мудак, с регуляркой не срослось, поэтому оставлю как есть
    QStringRef *header;
    int indexValues = toParse.indexOf("VALUE");
    if(indexValues > 5){
        header = new QStringRef(&toParse, 0, indexValues + 5);
    } else {
        throw ParseException("VALUE не найден");
    }

    TransientAnalysisEntity result = parseHeader(TransientAnalysisEntity(), *header);

    QStringRef *valuesString = new QStringRef(&toParse, indexValues - 5, toParse.size() - indexValues);

    result = parseValues(result, *valuesString);
    return result;
}

void Parser::setColorManager(ColorManager *colorManager)
{
    this->colorManager = colorManager;
}

TransientAnalysisEntity Parser::parseHeader(TransientAnalysisEntity entity, QStringRef header)
{
    if(header == NULL || header.isEmpty()){
        throw ParseException("HEADER пуст");
    }
    QRegularExpression regular("(?<=TRACE\n)(.|\n)*?(?=\n(?>VALUE|TYPE|SWEEP))",QRegularExpression::MultilineOption);

    QRegularExpressionMatch match = regular.match(header);
    if(!match.hasMatch()){
        throw ParseException("Не содержится блока TRACE ... VALUE");
    }

    QString trace = match.captured(0);
    if(trace.isEmpty()){
        throw ParseException("Блок TRACE ... VALUE пуст");
    }
    QStringList lines = trace.split("\n");
    if(lines.size() == 0){
        throw ParseException("TRACE не содержит строк");
    } else if(lines.size() == 1){
        qDebug() << "TRACE" << "найденна лишь одна строка";
    }
    int groupSize = 0;
    if(lines.size() > 1){
        QString groupLine = lines.at(0);
        int groupIndex = groupLine.lastIndexOf("GROUP");
        if(groupIndex == -1){
            throw ParseException("Первая строка TRACE должна содержать GROUP {n}");
        } else if(groupIndex + 5 > groupLine.size()){
            throw ParseException("Не удается распознать размер GROUP");
        } else {
            groupIndex += 5;
        }

        bool check = false;
        QStringRef test(&groupLine, groupIndex, groupLine.size() - groupIndex);
        groupSize = QStringRef(&groupLine, groupIndex, groupLine.size() - groupIndex).toInt(&check, 10);
        if(!check){
            throw ParseException("Не удалось распознать размер GROUP, см. TRACE -> 1-ю строку");
        }
        if(groupSize < 0){
            throw ParseException("Размер GROUP меньше 0");
        }
        if(groupSize - 1 > lines.size()){
            throw ParseException("Ошибка в размере GROUP");
        }
        QStringList chartHeader;
        for(int count = 1; count < groupSize + 1; count ++){
            QString line = lines.at(count);
            QStringList splitLine = line.split(" ");
            if(splitLine.size() != 2){
                throw ParseException("Ошибка распознавания строки " + QString::number(count) + " TRACE Блока");
            }
            QString chartName = splitLine.at(0);
            if(chartName.isEmpty()){
                throw ParseException("Ошибка распознования названия графика, он пустой, строка " + QString::number(count) + " TRACE блок");
            }
            QString name = chartName.replace("\"", "");
            entity.setChartColor(name, colorManager->getColor(count - 1));
            chartHeader.push_back(name);
        }

        entity.setHeaders(chartHeader);
        entity.setGroupSize(chartHeader.size());
    }

    QRegularExpression sweepRegular("(?<=SWEEP\n)(\".*?\")(.|\n)*?(?=\n(?>VALUE|TYPE|SWEEP))", QRegularExpression::MultilineOption);

    match = sweepRegular.match(header);
    if(!match.hasMatch()){
        throw ParseException("Не найден блок SWEEP");
    }

    if(match.capturedTexts().size() < 2 || (match.capturedTexts().size() == 1 && match.captured(1).isEmpty())){
        throw ParseException("SWEEP не распознан");
    }

    QString sweepName = match.captured(1).replace("\"", "");
    entity.setSweep(sweepName);

    return entity;
}

TransientAnalysisEntity Parser::parseValues(TransientAnalysisEntity entity, QStringRef valuesString)
{
    if(valuesString.isEmpty()){
        return entity;
    }
    QRegularExpression *valuesRegular = new QRegularExpression("(?=\"time\")(?>.*\n){" + QString::number(entity.getHeaders().size() + 2) + "}", QRegularExpression::MultilineOption);

    QRegularExpressionMatchIterator match = valuesRegular->globalMatch(valuesString);

    QHash<QString, QList<QPointF>> values;

    //init map values;
    QStringListIterator headerIterator(entity.getHeaders());
    while(headerIterator.hasNext()){
        values.insert(headerIterator.next(), QList<QPointF>());
    }

    while(match.hasNext()){
        QRegularExpressionMatch localMatch = match.next();
        if(localMatch.hasMatch()){

            QStringList list = localMatch.captured(0).split("\n");
            if(list.size() < entity.getGroupSize() + 2){
                throw ParseException("Ошибка парсинга значений, не достаточно значений");
            }

            QString timeValue = list.at(0).split(" ").at(1);
            bool check = false;
            float time = timeValue.toFloat(&check);
            if(!check){
                throw ParseException("Не распозналось значение времение");
            }

            for(int count = 2; count < entity.getGroupSize() + 2; count ++){
                QString chartName = entity.getHeaders().at(count - 2);
                float value = list.at(count).toFloat(&check);
                if(!check){
                    throw ParseException("Не распознано значение переменной");
                }
                QList<QPointF> points = values.value(chartName);
                points.push_back(QPointF(time, value));
                values.insert(chartName, points);
            }
        }
    }
    entity.setPoints(values);

    QHashIterator<QString, QList<QPointF>> iterator(values);
    while(iterator.hasNext()){
        iterator.next();
    }
    return entity;
}


