#include "CSVReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

ThermalMap CSVReader::readThermalMap(
    const std::string& filename)
{
    ThermalMap map;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr
            << "Error: Cannot open thermal map file: "
            << filename
            << std::endl;

        return map;
    }

    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream stream(line);

        double x;
        double y;
        double z;
        double temperature;

        // Read whitespace-separated values
        if (!(stream >> x >> y >> z >> temperature))
        {
            std::cerr
                << "Warning: Invalid line in thermal map: "
                << line
                << std::endl;

            continue;
        }

        map.addNode(
            ThermalNode(
                Point3D(x, y, z),
                temperature
            )
        );
    }

    file.close();

    return map;
}