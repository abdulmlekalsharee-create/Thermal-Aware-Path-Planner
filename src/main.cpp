#include <iostream>
#include <memory>
#include <vector>

#include "Geometry/Point3D.h"

#include "Thermal/ThermalMap.h"
#include "Thermal/ThermalNode.h"

#include "Environment/Environment.h"
#include "Environment/Obstacle.h"

#include "Graph/Graph.h"

#include "Planner/AStar.h"
#include "Planner/ThermalCost.h"

#include "IO/CSVReader.h"
#include "IO/CSVWriter.h"
#include "IO/ThermalDataReader.h"


int main()
{
    std::cout
        << "========================================\n"
        << "        THERMAL PATH PLANNER\n"
        << "========================================\n\n";


    // ========================================
    // 1. Load Thermal Map
    // ========================================

    std::cout << "Loading thermal map...\n";

    ThermalMap thermalMap;

    ThermalDataReader thermalReader;

    if (!thermalReader.loadFromCSV(
            "thermal_grid.csv",
            thermalMap))
    {
        std::cout
            << "Error: Failed to load thermal data.\n";

        return 1;
    }

    std::cout
        << "Thermal map loaded successfully.\n";

    std::cout
        << "Number of thermal nodes: "
        << thermalMap.size()
        << "\n\n";


    // ========================================
    // 2. Create Environment
    // ========================================

    Environment environment;

    environment.setThermalMap(
        thermalMap
    );


    // ========================================
    // 3. Add Obstacles
    // ========================================

    environment.addObstacle(
        Obstacle(
            Point3D(1, 1, 0),
            Point3D(1, 3, 0)
        )
    );


    // ========================================
    // 4. Build Graph
    // ========================================

    std::cout << "Building navigation graph...\n";

    Graph graph;

    graph.buildFromEnvironment(
        environment
    );

    std::cout
        << "Graph built successfully.\n\n";


    // ========================================
    // 5. Define Start and Goal
    // ========================================

    Point3D start(1.088332, 11.078080, 5.003098);

Point3D goal(16.088333, 11.078080, 25.003098);

    std::cout
        << "Start: "
        << start
        << "\n";

    std::cout
        << "Goal:  "
        << goal
        << "\n\n";


    // ========================================
    // 6. Create A* Planner
    // ========================================

    AStar planner;


    // ========================================
    // 7. Use Thermal Cost Function
    // ========================================

    planner.setCostFunction(
        std::make_unique<ThermalCost>(
            5.0,   // Thermal weight
            40.0   // Safe temperature
        )
    );


    // ========================================
    // 8. Find Thermal-Aware Path
    // ========================================

    std::cout
        << "Searching for thermal-aware path...\n";

    std::vector<GraphNode*> path =
        planner.findPath(
            graph,
            start,
            goal
        );


    // ========================================
    // 9. Check Result
    // ========================================

    if (path.empty())
    {
        std::cout
            << "\nNo valid path found.\n";

        return 1;
    }


    // ========================================
    // 10. Display Path
    // ========================================

    std::cout
        << "\nPath found successfully!\n\n";

    std::cout
        << "Path nodes: "
        << path.size()
        << "\n\n";

    std::cout
        << "----------------------------------------\n";

    for (std::size_t i = 0;
         i < path.size();
         ++i)
    {
        GraphNode* node = path[i];

        std::cout
            << "Step "
            << i
            << " : "
            << node->getPosition()
            << " | Temperature = "
            << node->getTemperature()
            << " C\n";
    }

    std::cout
        << "----------------------------------------\n";

        // ========================================
    // 11. Export Path
    // ========================================

    if (CSVWriter::writePath(
            "data/path_result.csv",
            path))
    {
        std::cout
            << "\nPath exported successfully to:\n"
            << "data/path_result.csv\n";
    }
    else
    {
        std::cout
            << "\nError: Failed to export path.\n";
    }


    // ========================================
    // 12. Finish
    // ========================================

    std::cout
        << "\n========================================\n"
        << "             PROGRAM FINISHED\n"
        << "========================================\n";


    return 0;
}