#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../Thermal/ThermalMap.h"
#include "Obstacle.h"
#include <vector>

class Environment
{
private:
    ThermalMap thermalMap;
    std::vector<Obstacle> obstacles;

public:
    void setThermalMap(const ThermalMap& map);

    ThermalMap& getThermalMap();
    const ThermalMap& getThermalMap() const;

    void addObstacle(const Obstacle& obstacle);

    const std::vector<Obstacle>& getObstacles() const;

    bool isBlocked(const Point3D& point) const;
};

#endif