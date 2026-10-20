#include "Graph.h"

#include <cmath>
#include <limits>

void Graph::buildFromEnvironment(
    const Environment& environment)
{
    nodes.clear();

    const std::vector<ThermalNode>& thermalNodes =
        environment.getThermalMap().getNodes();

    for (const ThermalNode& thermalNode : thermalNodes)
    {
        GraphNode node(
            thermalNode.getPosition(),
            thermalNode.getTemperature()
        );

        node.setBlocked(
            thermalNode.isBlocked() ||
            environment.isBlocked(
                thermalNode.getPosition()
            )
        );

        nodes.push_back(node);
    }
}

GraphNode* Graph::findNode(
    const Point3D& position)
{
    const double tolerance = 1e-6;

    for (GraphNode& node : nodes)
    {
        if (node.getPosition().distanceTo(position)
            <= tolerance)
        {
            return &node;
        }
    }

    return nullptr;
}

const std::vector<GraphNode>& Graph::getNodes() const
{
    return nodes;
}

std::vector<GraphNode*> Graph::getNeighbors(
    GraphNode* node) const
{
    std::vector<GraphNode*> neighbors;

    if (node == nullptr)
        return neighbors;

    /*
        The thermal data coming from SOLIDWORKS
        is not guaranteed to be a perfect 1-unit grid.

        Therefore, instead of assuming:

            x + 1
            x - 1
            y + 1
            y - 1

        we search for the closest existing nodes.
    */

    // The SolidWorks mesh spacing in thermal_grid.csv is
    // approximately 0.714 units (measured), not ~0.01. This
    // tolerance must sit comfortably above the true spacing
    // (so real neighbors are found) and below the next mesh
    // "shell" (~1.43 units), so we don't skip past an actual
    // neighbor to grab a more distant node instead.
    
    const double maxNeighborDistance = 1.0;

    double bestDistance[6] =
    {
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity()
    };

    GraphNode* bestNeighbor[6] =
    {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };

    const Point3D& currentPosition =
        node->getPosition();

    for (const GraphNode& candidate : nodes)
    {
        GraphNode* candidatePtr =
            const_cast<GraphNode*>(&candidate);

        if (candidatePtr == node)
            continue;

        if (candidate.isBlocked())
            continue;

        double dx =
            candidate.getPosition().getX()
            - currentPosition.getX();

        double dy =
            candidate.getPosition().getY()
            - currentPosition.getY();

        double dz =
            candidate.getPosition().getZ()
            - currentPosition.getZ();

        double distance =
            currentPosition.distanceTo(
                candidate.getPosition()
            );

        if (distance > maxNeighborDistance)
            continue;

        /*
            Determine the dominant direction.
        */

        double ax = std::abs(dx);
        double ay = std::abs(dy);
        double az = std::abs(dz);

        int direction = -1;

        if (ax >= ay && ax >= az)
        {
            direction = (dx > 0.0) ? 0 : 1;
        }
        else if (ay >= ax && ay >= az)
        {
            direction = (dy > 0.0) ? 2 : 3;
        }
        else
        {
            direction = (dz > 0.0) ? 4 : 5;
        }

        if (distance < bestDistance[direction])
        {
            bestDistance[direction] = distance;
            bestNeighbor[direction] = candidatePtr;
        }
    }

    for (int i = 0; i < 6; ++i)
    {
        if (bestNeighbor[i] != nullptr)
            neighbors.push_back(bestNeighbor[i]);
    }

    return neighbors;
}

void Graph::reset()
{
    for (GraphNode& node : nodes)
        node.reset();
}