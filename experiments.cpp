#include "cube.h"
#include "solver.h"

#include <chrono>
#include <iostream>
#include <vector>

void printMoves(const std::vector<std::string>& moves) {
    for (const auto& m : moves) {
        std::cout << m << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> scrambleLengths = {3, 4, 5, 6};
    int trialsPerDepth = 3;

    for (int depth : scrambleLengths) {
        std::cout << "=============================\n";
        std::cout << "Scramble length group: " << depth << "\n";
        std::cout << "=============================\n";

        for (int trial = 0; trial < trialsPerDepth; ++trial) {
            Cube cube;
            unsigned int seed = 100 + depth * 10 + trial;

            std::vector<std::string> scramble = cube.scramble(depth, seed);

            std::cout << "\nTrial " << (trial + 1) << "\n";
            std::cout << "Scramble: ";
            printMoves(scramble);

            int lb = lowerBoundMisplacedFacelets(cube);

            auto startTime = std::chrono::steady_clock::now();
            SolveResult result = solveIDAStar(cube, depth + 2);
            auto endTime = std::chrono::steady_clock::now();

            auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                endTime - startTime
            ).count();

            std::cout << "Lower bound h(s): " << lb << "\n";
            std::cout << "Solved: " << (result.solved ? "true" : "false") << "\n";
            std::cout << "Nodes expanded: " << result.nodesExpanded << "\n";
            std::cout << "Search bound reached: " << result.finalBound << "\n";
            std::cout << "Time (ms): " << elapsedMs << "\n";

            if (result.solved) {
                std::cout << "Solution length: " << result.moves.size() << "\n";
                std::cout << "Solution moves: ";
                printMoves(result.moves);

                Cube check = cube;
                check.applyAlgorithm(result.moves);
                std::cout << "Actually solved after applying solution: "
                          << (check.isSolved() ? "true" : "false") << "\n";

                if (lb > 0) {
                    double ratio = static_cast<double>(result.moves.size()) / lb;
                    std::cout << "Solution / lower-bound ratio: " << ratio << "\n";
                }
            }

            std::cout << '\n';
        }
    }

    return 0;
}
