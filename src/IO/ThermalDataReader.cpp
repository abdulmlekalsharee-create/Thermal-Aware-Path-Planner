#include "ThermalDataReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

ThermalDataReader::ThermalDataReader()
{
}

ThermalDataReader::~ThermalDataReader()
{
}

bool ThermalDataReader::loadFromCSV(
    const std::string& filename,
    ThermalMap& thermalMap
) const
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open thermal data file: "
                  << filename << std::endl;

        return false;
    }

    std::string line;

    // Skip CSV header
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line); // to use ','
        std::string value;

        try
        {
            // Node (column 1) - read to advance the stream;
            // not needed after this, since ThermalNode is
            // identified by position, not by SolidWorks node ID.
            std::getline(ss, value, ',');
            int nodeNumber = std::stoi(value);
            (void)nodeNumber;

            // X in meters (column 2)
            std::getline(ss, value, ',');
            double x = std::stod(value);

            // Y in meters (column 3)
            std::getline(ss, value, ',');
            double y = std::stod(value);

            // Z in meters (column 4)
            std::getline(ss, value, ',');
            double z = std::stod(value);

            // Temperature in Kelvin (column 5) -> convert to
            // Celsius, since ThermalCost's safeTemperature is
            // expressed in Celsius (e.g. 40.0 in main.cpp).
            std::getline(ss, value, ',');
            double temperatureK = std::stod(value);
            double temperatureC = temperatureK - 273.15;

            // Remaining columns (Temperature_C, X_mm, Y_mm, Z_mm)
            // are not read; the rest of the line is simply
            // discarded when the stringstream goes out of scope.

            // Create position
            Point3D position(x, y, z);

            // Create ThermalNode
            ThermalNode thermalNode(
                position,
                temperatureC
            );

            // Add to ThermalMap
            thermalMap.addNode(thermalNode);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error reading CSV line: "
                      << line << std::endl;

            return false;
        }
    }

    file.close();

    std::cout << "Thermal data loaded successfully."
              << std::endl;

    std::cout << "Number of thermal nodes: "
              << thermalMap.size()
              << std::endl;

    return true;
}