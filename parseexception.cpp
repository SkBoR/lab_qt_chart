#include "parseexception.h"

ParseException::ParseException(QString message)
{
    this->message = new QString(message);
}

QString ParseException::getMessage()
{
    if(this->message == NULL){
        return QString("Ошибка в парсинге");
    } else return *message;
}

void ParseException::setMessage(QString message)
{
    this->message = new QString(message);
}
