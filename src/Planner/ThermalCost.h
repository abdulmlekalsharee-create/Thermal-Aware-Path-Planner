#ifndef THERMALCOST_H
#define THERMALCOST_H

#include "CostFunction.h"

class ThermalCost : public CostFunction
{
private:
    double thermalWeight;
    double safeTemperature;

public:
    ThermalCost(
        double thermalWeight = 5.0,
        double safeTemperature = 40.0
    );

    double calculate(
        const GraphNode& current,
        const GraphNode& next
    ) const override;
};

#endif