#include "cube.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void testInitialSolved() {
    Cube cube;
    require(cube.isSolved(), "New cube should start solved");
}

void testMoveThenInverse() {
    std::vector<std::string> moves = {
        "U", "D", "L", "R", "F", "B",
        "U2", "R2", "F2",
        "L'", "D'", "B'"
    };

    for (const auto& move : moves) {
        Cube cube;
        cube.move(move);
        cube.move(inverseMove(move));
        require(cube.isSolved(), "Move/inverse failed for " + move);
    }
}

void testFourTurnsRestore() {
    std::vector<std::string> moves = {"U", "D", "L", "R", "F", "B"};

    for (const auto& move : moves) {
        Cube cube;
        for (int i = 0; i < 4; ++i) {
            cube.move(move);
        }
        require(cube.isSolved(), "Four turns failed for " + move);
    }
}

void testSequenceThenInverse() {
    Cube cube;
    std::vector<std::string> sequence = {"R", "U", "R'", "U'", "F", "R", "F'"};

    cube.applyAlgorithm(sequence);
    cube.applyAlgorithm(inverseAlgorithm(sequence));

    require(cube.isSolved(), "Sequence/inverse failed");
}

void testScrambleThenInverse() {
    Cube cube;
    std::vector<std::string> scr = cube.scramble(25, 123);
    require(!cube.isSolved(), "Scrambled cube should not still be solved");

    cube.applyAlgorithm(inverseAlgorithm(scr));
    require(cube.isSolved(), "Scramble/inverse failed");
}

int main() {
    try {
        testInitialSolved();
        testMoveThenInverse();
        testFourTurnsRestore();
        testSequenceThenInverse();
        testScrambleThenInverse();

        std::cout << "All tests passed.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << '\n';
        return 1;
    }
}