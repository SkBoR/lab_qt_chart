#ifndef PARSEEXCEPTION_H
#define PARSEEXCEPTION_H

#include <QString>
#include <QException>

class ParseException : public QException
{
public:
    ParseException(QString message = "");
    QString getMessage();
    void setMessage(QString message);
private:
    QString *message;
};

#endif // PARSEEXCEPTION_H
