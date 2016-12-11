#include "transientanalysisentity.h"

TransientAnalysisEntity::TransientAnalysisEntity()
{

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
