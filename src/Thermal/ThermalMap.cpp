#include "ThermalMap.h"

void ThermalMap::addNode(const ThermalNode& node)
{
    nodes.push_back(node);
}

const std::vector<ThermalNode>& ThermalMap::getNodes() const
{
    return nodes;
}

ThermalNode* ThermalMap::findNode(const Point3D& position)
{
    for (ThermalNode& node : nodes)
    {
        if (node.getPosition() == position)
            return &node;
    }

    return nullptr;
}

bool ThermalMap::empty() const
{
    return nodes.empty();
}

std::size_t ThermalMap::size() const
{
    return nodes.size();
}