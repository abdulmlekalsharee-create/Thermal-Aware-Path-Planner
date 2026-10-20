#include "Environment.h"

void Environment::setThermalMap(const ThermalMap& map)
{
    thermalMap = map;
}

ThermalMap& Environment::getThermalMap()
{
    return thermalMap;
}

const ThermalMap& Environment::getThermalMap() const
{
    return thermalMap;
}

void Environment::addObstacle(const Obstacle& obstacle)
{
    obstacles.push_back(obstacle);
}

const std::vector<Obstacle>& Environment::getObstacles() const
{
    return obstacles;
}

bool Environment::isBlocked(const Point3D& point) const
{
    for (const Obstacle& obstacle : obstacles)
    {
        if (obstacle.contains(point))
            return true;
    }

    return false;
}