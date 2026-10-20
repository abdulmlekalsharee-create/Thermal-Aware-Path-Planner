#ifndef GRAPH_H
#define GRAPH_H

#include "GraphNode.h"
#include "../Environment/Environment.h"
#include <vector>

class Graph
{
private:
    std::vector<GraphNode> nodes;

public:
    void buildFromEnvironment(
        const Environment& environment
    );

    GraphNode* findNode(
        const Point3D& position
    );

    const std::vector<GraphNode>& getNodes() const;

    std::vector<GraphNode*> getNeighbors(
        GraphNode* node
    ) const;

    void reset();
};

#endif