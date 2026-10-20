#include "ThermalCost.h"

ThermalCost::ThermalCost(
    double thermalWeight,
    double safeTemperature)
    : thermalWeight(thermalWeight),
      safeTemperature(safeTemperature)
{
}

double ThermalCost::calculate(
    const GraphNode& current,
    const GraphNode& next) const
{
    double risk = 0.0;

    if (next.getTemperature() > safeTemperature)
    {
        risk = next.getTemperature() - safeTemperature;
    }

    return thermalWeight * risk;
}