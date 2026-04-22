#include "solver.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

const std::vector<std::string> ALL_MOVES = {
    "U", "U'", "U2",
    "D", "D'", "D2",
    "L", "L'", "L2",
    "R", "R'", "R2",
    "F", "F'", "F2",
    "B", "B'", "B2"
};

std::string flattenState(const Cube& cube) {
    auto faces = cube.asFaces();
    std::string key;
    key.reserve(54);

    for (const auto& face : faces) {
        for (char c : face) {
            key.push_back(c);
        }
    }

    return key;
}

bool sameFace(const std::string& a, const std::string& b) {
    return !a.empty() && !b.empty() && a[0] == b[0];
}

int dfsIDA(
    const Cube& cube,
    int g,
    int bound,
    std::vector<std::string>& path,
    const std::string& lastMove,
    std::unordered_set<std::string>& onPath,
    int& nodesExpanded
) {
    int h = lowerBoundMisplacedFacelets(cube);
    int f = g + h;

    if (f > bound) {
        return f;
    }

    if (cube.isSolved()) {
        return -1; // FOUND
    }

    ++nodesExpanded;
    int minNextBound = std::numeric_limits<int>::max();

    for (const auto& move : ALL_MOVES) {
        if (sameFace(move, lastMove)) {
            continue;
        }

        Cube next = cube;
        next.move(move);

        std::string key = flattenState(next);
        if (onPath.count(key)) {
            continue;
        }

        onPath.insert(key);
        path.push_back(move);

        int t = dfsIDA(next, g + 1, bound, path, move, onPath, nodesExpanded);

        if (t == -1) {
            return -1;
        }

        minNextBound = std::min(minNextBound, t);

        path.pop_back();
        onPath.erase(key);
    }

    return minNextBound;
}

} // namespace

int lowerBoundMisplacedFacelets(const Cube& cube) {
    auto faces = cube.asFaces();
    int misplaced = 0;

    for (const auto& face : faces) {
        char goal = face[4]; // center sticker defines correct color for that face
        for (int i = 0; i < 9; ++i) {
            if (i == 4) {
                continue; // ignore center
            }
            if (face[i] != goal) {
                ++misplaced;
            }
        }
    }

    // One quarter-turn can move at most 20 non-center facelets.
    return (misplaced + 19) / 20;
}

SolveResult solveIDAStar(const Cube& start, int maxDepth) {
    SolveResult result;
    result.lowerBound = lowerBoundMisplacedFacelets(start);

    if (start.isSolved()) {
        result.solved = true;
        result.finalBound = 0;
        return result;
    }

    int bound = result.lowerBound;
    if (bound < 1) {
        bound = 1;
    }

    while (bound <= maxDepth) {
        std::vector<std::string> path;
        std::unordered_set<std::string> onPath;
        onPath.insert(flattenState(start));

        int nodesThisIteration = 0;

        int searchResult = dfsIDA(
            start,
            0,
            bound,
            path,
            "",
            onPath,
            nodesThisIteration
        );

        result.nodesExpanded += nodesThisIteration;

        if (searchResult == -1) {
            result.solved = true;
            result.moves = path;
            result.finalBound = bound;
            return result;
        }

        if (searchResult == std::numeric_limits<int>::max()) {
            break;
        }

        bound = searchResult;
    }

    result.solved = false;
    result.finalBound = bound;
    return result;
}
