# Rubik’s Cube Algorithms Project

## Team Members
- Nicholas Mruk
- Kofo Laditan

## Course
CSIT 441 Algorithms

## Project Overview
This project studies the Rubik’s cube as a state-space search problem. We analyze the computational complexity of solving the cube, discuss lower-bound reasoning, and implement an IDA* baseline solver guided by an admissible heuristic.

## Our Objectives:
  Solving the Rubik's cube can be a tricky and most of us will agree to this statement. However, how tricky is it actually? And how to solve it (algorithmically)? These are two questions that need to be answered in this project.
  In this project, we will have to analyze the complexity of solving the rubik's cube. In particular, we will have to determine the lower bound complexity for this problem and compare our solution to the ideal running time. 
  Our second objective will be to implement an efficient algorithm that, given any configuration of a rubik's cube, will determine the sequence of steps that need to be taken to solve the rubik's cube. (An efficient algorithm is one that comes as close as can reasonably be to the theoretical lower bound.) 
   Finally, we will design a testing strategy, run experiments and report on them so as to provide an experimental analysis of the algorithm. 

## Files
- `cube.h` / `cube.cpp` - cube representation and legal moves
- `solver.h` / `solver.cpp` - IDA* baseline solver
- `tests.cpp` - correctness tests
- `experiments.cpp` - runtime experiments
- `docs/THEORY.md` - complexity, lower bounds, heuristic, and algorithm discussion
- `docs/RESULTS.md` - experimental results and analysis

## How to Compile
```bash
g++ -std=c++17 cube.cpp solver.cpp experiments.cpp -o experiments
g++ -std=c++17 cube.cpp tests.cpp -o testcube
