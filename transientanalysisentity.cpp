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
