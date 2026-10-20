#include "ThermalNode.h"

ThermalNode::ThermalNode()
    : position(), temperature(0.0), blocked(false)
{
}

ThermalNode::ThermalNode(
    const Point3D& position,
    double temperature)
    : position(position),
      temperature(temperature),
      blocked(false)
{
}

const Point3D& ThermalNode::getPosition() const
{
    return position;
}

double ThermalNode::getTemperature() const
{
    return temperature;
}

bool ThermalNode::isBlocked() const
{
    return blocked;
}

void ThermalNode::setTemperature(double temperature)
{
    this->temperature = temperature;
}

void ThermalNode::setBlocked(bool blocked)
{
    this->blocked = blocked;
}

double ThermalNode::getThermalRisk(
    double safeTemperature) const
{
    if (temperature <= safeTemperature)
        return 0.0;

    return temperature - safeTemperature;
}
