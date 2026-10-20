#ifndef DISTANCECOST_H
#define DISTANCECOST_H

#include "CostFunction.h"

class DistanceCost : public CostFunction
{
public:
    double calculate(
        const GraphNode& current,
        const GraphNode& next
    ) const override;
};

#endif