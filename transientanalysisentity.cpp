#include "transientanalysisentity.h"

TransientAnalysisEntity::TransientAnalysisEntity()
{
    groupSize = -1;
}

int TransientAnalysisEntity::getGroupSize() const
{
    return groupSize;
}

void TransientAnalysisEntity::setGroupSize(int value)
{
    groupSize = value;
}

void TransientAnalysisEntity::setHeaders(const QStringList &value)
{
    headers = value;
}

QStringList TransientAnalysisEntity::getHeaders() const
{
    return headers;
}

QString TransientAnalysisEntity::getSweep() const
{
    return sweep;
}

void TransientAnalysisEntity::setSweep(const QString &value)
{
    sweep = value;
}

QHash<QString, QList<QPointF> > TransientAnalysisEntity::getPoints() const
{
    return points;
}

void TransientAnalysisEntity::setPoints(const QHash<QString, QList<QPointF> > &value)
{
    points = value;
}
