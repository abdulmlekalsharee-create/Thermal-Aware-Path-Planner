#ifndef COSTFUNCTION_H
#define COSTFUNCTION_H

#include "../Graph/GraphNode.h"

class CostFunction
{
public:
    virtual ~CostFunction() = default;

    virtual double calculate(
        const GraphNode& current,
        const GraphNode& next
    ) const = 0;
};

#endif