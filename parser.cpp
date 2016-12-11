#include "parser.h"

Parser::Parser()
{
    headerRegular = new QRegularExpression("(?=HEADER)(.|\n)*?(?<=VALUE)", QRegularExpression::MultilineOption);
    valuesRegular = new QRegularExpression("(?=\"time\")(?>.*\n){7}", QRegularExpression::MultilineOption);
}

void Parser::parse(QString toParse)
{
    if(toParse.isEmpty()){
        throw ParseException("Нечего парсить");
    }

    QRegularExpressionMatchIterator headerMatch = headerRegular->globalMatch(toParse);
    if(headerMatch.hasNext()){
        QRegularExpressionMatch match = headerMatch.next();
        if(match.hasMatch()){

            qDebug() << match.capturedRef().toString();
            qDebug() << match.lastCapturedIndex();
        }
    } else {
        qDebug() << "Header not found";
        throw ParseException("HEADER не найден");
    }
    //ебашу как мудак
    QStringRef *header;
    int indexValues = toParse.indexOf("VALUE");
    if(indexValues > 5){
        header = new QStringRef(&toParse, 0, indexValues + 5);
        //        qDebug() << header->toString();
    } else {
        throw ParseException("VALUE не найден");
    }

    TransientAnalysisEntity result = Parser::parseHeader(TransientAnalysisEntity(), *header);

    QStringRef *valuesString = new QStringRef(&toParse, indexValues - 5, toParse.size() - indexValues);

    //TODO обработка значенией
    QRegularExpressionMatchIterator match = valuesRegular->globalMatch(*valuesString);
    //TODO сделать уведомление о том что, что-то не распарсилось
    while(match.hasNext()){
        QRegularExpressionMatch localMatch = match.next();
        if(localMatch.hasMatch()){
            QString captured = localMatch.captured(0);

            QStringList list = captured.split("\n");

            QString timeValue = list.at(0).split(" ").at(1);
            double time = timeValue.toDouble();

            // TODO сделать для множества точек (не хардкод кол-во)
            QString values = list.at(3);
            double value = values.toDouble();
            //            *series << QPointF()
            //                       series->append((float)time, (float)value);
            //            qDebug() << "time: " << time;
            //            qDebug() << "value: " << value;
            //            qDebug() << captured;
        }
    }
}

TransientAnalysisEntity Parser::parseHeader(TransientAnalysisEntity entity, QStringRef header)
{
    if(header == NULL || header.isEmpty()){
        throw ParseException("HEADER пуст");
    }
    QRegularExpression regular("(?<=TRACE\n)(.|\n)*?(?=\nVALUE)",QRegularExpression::MultilineOption);

    QRegularExpressionMatch match = regular.match(header.toString());
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
            chartHeader.push_back(chartName.replace("\"", ""));
        }

        entity.setHeaders(chartHeader);
    }

    qDebug() << "Заголовок заполнен следующими значениями "<< entity.getHeaders();
    qDebug() << "что-то нашел";

    return entity;
}

TransientAnalysisEntity Parser::parseValues(TransientAnalysisEntity entity, QStringRef values)
{
    return entity;
}


