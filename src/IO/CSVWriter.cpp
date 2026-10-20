#include "CSVWriter.h"

#include <fstream>
#include <filesystem>

bool CSVWriter::writePath(
    const std::string& filename,
    const std::vector<GraphNode*>& path)
{
    // Create the destination folder (e.g. "data/") if it doesn't
    // exist yet. std::ofstream will NOT create missing directories
    // on its own, so without this, writing silently fails whenever
    // the project is checked out fresh and "data/" hasn't been
    // created by hand.
    std::filesystem::path outputPath(filename);

    if (outputPath.has_parent_path())
    {
        std::filesystem::create_directories(
            outputPath.parent_path()
        );
    }

    std::ofstream file(filename);

    if (!file.is_open())
        return false;

    file << "x,y,z,temperature\n";

    for (const GraphNode* node : path)
    {
        file
            << node->getPosition().getX() << "," // nod is a pointer
            << node->getPosition().getY() << ","
            << node->getPosition().getZ() << ","
            << node->getTemperature()
            << "\n";
    }

    file.close();

    return true;
}