#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "../Geometry/Point3D.h"
#include <limits>

class GraphNode
{
private:
    Point3D position;
    double temperature;

    double gCost;
    double hCost;

    GraphNode* parent;
    bool blocked;

public:
    GraphNode();

    GraphNode(
        const Point3D& position,
        double temperature
    );

    const Point3D& getPosition() const;
    double getTemperature() const;

    double getGCost() const;
    double getHCost() const;
    double getFCost() const;

    GraphNode* getParent() const;

    bool isBlocked() const;

    void setGCost(double cost);
    void setHCost(double cost);
    void setParent(GraphNode* parent);
    void setBlocked(bool blocked);

    void reset();
};

#endif