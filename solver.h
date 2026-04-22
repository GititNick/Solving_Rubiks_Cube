#ifndef SOLVER_H
#define SOLVER_H

#include "cube.h"
#include <string>
#include <vector>

struct SolveResult {
    bool solved = false;
    std::vector<std::string> moves;
    int nodesExpanded = 0;
    int lowerBound = 0;
    int finalBound = 0;
};

int lowerBoundMisplacedFacelets(const Cube& cube);
SolveResult solveIDAStar(const Cube& start, int maxDepth = 8);

#endif
