#ifndef TRANSIENTANALYSISENTITY_H
#define TRANSIENTANALYSISENTITY_H

#include <QString>
#include<QStringList>
#include <QHash>
#include <QList>
#include <QPointF>
#include <QColor>

class TransientAnalysisEntity
{
public:
    TransientAnalysisEntity();

    int getGroupSize() const;

    //TODO возможно лишнее
    void setGroupSize(int value);

    void setHeaders(const QStringList &value);

    QStringList getHeaders() const;

    QString getSweep() const;
    void setSweep(const QString &value);

    QHash<QString, QList<QPointF> > getPoints() const;
    void setPoints(const QHash<QString, QList<QPointF> > &value);

    QColor getChartColor(QString chartName) const;
    void setChartColor(QString chartName, QColor color);

private:
    int groupSize;
    QStringList headers;
    QString sweep;
    QHash<QString, QList<QPointF>> points;
    QHash<QString, QColor> chartColor;
};

#endif // TRANSIENTANALYSISENTITY_H
