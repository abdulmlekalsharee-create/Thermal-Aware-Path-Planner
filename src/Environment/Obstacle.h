#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../Geometry/Point3D.h"

class Obstacle
{
private:
    Point3D minPoint;
    Point3D maxPoint;

public:
    Obstacle();
    Obstacle(
        const Point3D& minPoint,
        const Point3D& maxPoint
    );

    bool contains(const Point3D& point) const;

    const Point3D& getMinPoint() const;
    const Point3D& getMaxPoint() const;
};

#endif