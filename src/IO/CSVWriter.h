#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "../Graph/GraphNode.h"

#include <string>
#include <vector>

class CSVWriter
{
public:
    static bool writePath(
        const std::string& filename,
        const std::vector<GraphNode*>& path
    );
};

#endif