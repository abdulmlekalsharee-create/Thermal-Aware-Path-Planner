#ifndef ASTAR_H
#define ASTAR_H

#include "../Graph/Graph.h"
#include "CostFunction.h"

#include <memory>
#include <vector>

class AStar
{
private:
    std::unique_ptr<CostFunction> costFunction;

    double heuristic(
        const GraphNode& current,
        const GraphNode& goal
    ) const;

    std::vector<GraphNode*> reconstructPath(
        GraphNode* goal
    ) const;

public:
    AStar();

    void setCostFunction(
        std::unique_ptr<CostFunction> costFunction
    );

    std::vector<GraphNode*> findPath(
        Graph& graph,
        const Point3D& start,
        const Point3D& goal
    );
};

#endif