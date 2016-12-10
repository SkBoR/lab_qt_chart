#ifndef TRANSIENTANALYSISENTITY_H
#define TRANSIENTANALYSISENTITY_H

#include <QString>
#include<QStringList>
#include <QHash>
#include <QList>
#include <QPointF>

class TransientAnalysisEntity
{
public:
    TransientAnalysisEntity();

    int getGroupSize() const;

    //TODO возможно лишнее
    void setGroupSize(int value);

private:
    int groupSize;
    QStringList headers;
    QString sweep;
    QHash<QString, QList<QPointF>> points;
};

#endif // TRANSIENTANALYSISENTITY_H
