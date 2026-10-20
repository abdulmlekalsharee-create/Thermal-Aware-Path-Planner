#ifndef THERMALDATAREADER_H
#define THERMALDATAREADER_H

#include "../Thermal/ThermalMap.h"
#include <string>

class ThermalDataReader
{
public:
    ThermalDataReader();
    ~ThermalDataReader();

    bool loadFromCSV(
        const std::string& filename,
        ThermalMap& thermalMap
    ) const; // const is for the object noy ThermalMap
};

#endif