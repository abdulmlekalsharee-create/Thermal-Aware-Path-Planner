#ifndef THERMALNODE_H
#define THERMALNODE_H

#include "../Geometry/Point3D.h"

class ThermalNode
{
private:
    Point3D position;
    double temperature;
    bool blocked;

public:
    ThermalNode();
    ThermalNode(const Point3D& position, double temperature);

    const Point3D& getPosition() const;
    double getTemperature() const;
    bool isBlocked() const;

    void setTemperature(double temperature);
    void setBlocked(bool blocked);

    double getThermalRisk(double safeTemperature) const;
};

#endif