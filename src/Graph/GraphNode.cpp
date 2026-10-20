#include "GraphNode.h"

GraphNode::GraphNode()
    : position(),
      temperature(25.0),
      gCost(std::numeric_limits<double>::infinity()),
      hCost(0.0),
      parent(nullptr),
      blocked(false)
{
}

GraphNode::GraphNode(
    const Point3D& position,
    double temperature)
    : position(position),
      temperature(temperature),
      gCost(std::numeric_limits<double>::infinity()),
      hCost(0.0),
      parent(nullptr),
      blocked(false)
{
}

const Point3D& GraphNode::getPosition() const
{
    return position;
}

double GraphNode::getTemperature() const
{
    return temperature;
}

double GraphNode::getGCost() const
{
    return gCost;
}

double GraphNode::getHCost() const
{
    return hCost;
}

double GraphNode::getFCost() const
{
    return gCost + hCost;
}

GraphNode* GraphNode::getParent() const
{
    return parent;
}

bool GraphNode::isBlocked() const
{
    return blocked;
}

void GraphNode::setGCost(double cost)
{
    gCost = cost;
}

void GraphNode::setHCost(double cost)
{
    hCost = cost;
}

void GraphNode::setParent(GraphNode* parent)
{
    this->parent = parent;
}

void GraphNode::setBlocked(bool blocked)
{
    this->blocked = blocked;
}

void GraphNode::reset()
{
    gCost = std::numeric_limits<double>::infinity();
    hCost = 0.0;
    parent = nullptr;
}