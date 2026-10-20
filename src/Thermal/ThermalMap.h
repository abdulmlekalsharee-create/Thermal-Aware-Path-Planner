#ifndef THERMALMAP_H
#define THERMALMAP_H

#include "ThermalNode.h"
#include <vector>

class ThermalMap
{
private:
    std::vector<ThermalNode> nodes;

public:
    void addNode(const ThermalNode& node);

    const std::vector<ThermalNode>& getNodes() const;

    ThermalNode* findNode(const Point3D& position);

    bool empty() const;

    std::size_t size() const;
};

#endif