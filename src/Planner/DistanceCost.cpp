#include "DistanceCost.h"

double DistanceCost::calculate(
    const GraphNode& current,
    const GraphNode& next) const
{
    return current.getPosition()
        .distanceTo(next.getPosition());
}