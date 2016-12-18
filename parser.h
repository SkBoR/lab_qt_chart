#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <parseexception.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QLineSeries>
#include <QDebug>
#include <transientanalysisentity.h>
#include <QStringRef>
#include <QStringListIterator>
#include <colormanager.h>

/**
 * @brief The Parser class
 * Исполльзовать для парсинга .tr0 файла, подобного примеру.
 * Парсит на глаз, спецификацию не читал, только координаты достает
 */
class Parser
{
public:
    Parser();
    //TODO продумать что будет возвращать
    TransientAnalysisEntity parse(QString toParse);
    void setColorManager(ColorManager *colorManager);
private:
    QRegularExpression *headerRegular;
    QRegularExpression *valuesRegular;

    ColorManager *colorManager = new ColorManager;

    TransientAnalysisEntity parseHeader(TransientAnalysisEntity entity, QStringRef header);
    TransientAnalysisEntity parseValues(TransientAnalysisEntity entity, QStringRef values);
};

#endif // PARSER_H
