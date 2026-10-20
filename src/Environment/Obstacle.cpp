#include "Obstacle.h"

Obstacle::Obstacle()
    : minPoint(), maxPoint()
{
}

Obstacle::Obstacle(
    const Point3D& minPoint,
    const Point3D& maxPoint)
    : minPoint(minPoint),
      maxPoint(maxPoint)
{
}

bool Obstacle::contains(const Point3D& point) const
{
    return point.getX() >= minPoint.getX() &&
           point.getX() <= maxPoint.getX() &&
           point.getY() >= minPoint.getY() &&
           point.getY() <= maxPoint.getY() &&
           point.getZ() >= minPoint.getZ() &&
           point.getZ() <= maxPoint.getZ();
}

const Point3D& Obstacle::getMinPoint() const
{
    return minPoint;
}

const Point3D& Obstacle::getMaxPoint() const
{
    return maxPoint;
}