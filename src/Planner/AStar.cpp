#include "AStar.h"
#include "DistanceCost.h"

#include <algorithm>
#include <utility>

AStar::AStar()
    : costFunction(std::make_unique<DistanceCost>())
{
}

void AStar::setCostFunction(
    std::unique_ptr<CostFunction> costFunction)
{
    this->costFunction = std::move(costFunction);
}

double AStar::heuristic(
    const GraphNode& current,
    const GraphNode& goal) const
{
    return current.getPosition()
        .distanceTo(goal.getPosition());
}

std::vector<GraphNode*> AStar::reconstructPath(
    GraphNode* goal) const
{
    std::vector<GraphNode*> path;

    GraphNode* current = goal;

    while (current != nullptr)
    {
        path.push_back(current);
        current = current->getParent();
    }

    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<GraphNode*> AStar::findPath(
    Graph& graph,
    const Point3D& start,
    const Point3D& goal)
{
    graph.reset();

    GraphNode* startNode = graph.findNode(start);
    GraphNode* goalNode = graph.findNode(goal);

    if (startNode == nullptr || goalNode == nullptr)
        return {};

    if (startNode->isBlocked() || goalNode->isBlocked())
        return {};

    std::vector<GraphNode*> openList;
    std::vector<GraphNode*> closedList;

    startNode->setGCost(0.0);

    startNode->setHCost(
        heuristic(*startNode, *goalNode)
    );

    openList.push_back(startNode);

    while (!openList.empty())
    {
        auto bestIterator = std::min_element(
            openList.begin(),
            openList.end(),
            [](GraphNode* a, GraphNode* b)
            {
                return a->getFCost() < b->getFCost();
            }
        );

        GraphNode* current = *bestIterator;

        openList.erase(bestIterator);

        if (current == goalNode)
            return reconstructPath(goalNode);

        closedList.push_back(current);

        std::vector<GraphNode*> neighbors =
            graph.getNeighbors(current);

        for (GraphNode* neighbor : neighbors)
        {
            if (neighbor->isBlocked())
                continue;

            if (std::find(
                    closedList.begin(),
                    closedList.end(),
                    neighbor)
                != closedList.end())
            {
                continue;
            }

            double movementCost =
                costFunction->calculate(
                    *current,
                    *neighbor
                );

            double newGCost =
                current->getGCost() + movementCost;

            bool inOpenList =
                std::find(
                    openList.begin(),
                    openList.end(),
                    neighbor)
                != openList.end();

            if (!inOpenList ||
                newGCost < neighbor->getGCost())
            {
                neighbor->setGCost(newGCost);

                neighbor->setHCost(
                    heuristic(
                        *neighbor,
                        *goalNode
                    )
                );

                neighbor->setParent(current);

                if (!inOpenList)
                    openList.push_back(neighbor);
            }
        }
    }

    return {};
}