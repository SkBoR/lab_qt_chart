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

    QRegularExpressionMatchIterator trace = regular.globalMatch(header.toString());
    if(trace.hasNext()){
        qDebug()<< "все ок";
        QRegularExpressionMatch match =  trace.next();
        if(match.hasMatch()){
            qDebug() << match.captured(0);
        }
    } else {

        QRegularExpressionMatch match = regular.match(header.toString());
        if(match.hasMatch()){
            qDebug() << match.captured(0);
            qDebug() << "что-то нашел";
        } else {
            qDebug() << "TRACE не найден";
            throw ParseException("TRACE не найден");
        }
    }

    QRegularExpressionMatch match = regular.match(header.toString());
    if(match.hasMatch()){
        QString trace = match.captured(0);

        qDebug() << trace;
        qDebug() << "что-то нашел";
    }
//    trace.

    return entity;
}

TransientAnalysisEntity Parser::parseValues(TransientAnalysisEntity entity, QStringRef values)
{
    return entity;
}


