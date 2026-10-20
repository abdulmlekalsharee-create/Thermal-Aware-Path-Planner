#ifndef CSVREADER_H
#define CSVREADER_H

#include "../Thermal/ThermalMap.h"
#include <string>

class CSVReader
{
public:
    static ThermalMap readThermalMap(
        const std::string& filename
    );
};

#endif