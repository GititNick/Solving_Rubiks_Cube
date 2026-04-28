## Theory and ALgorithm Analysis

## 1. The approach we decided to use for this project:

Global lower bound idea. The easiest idea we could come up with was treating solving as a search tree. In which we let N = number of reachable cube states
at the current move and b = branching factor. If a search tree has branching factor at most b, then to distinguish among N possible states,
the solution depth must satisfy: d >= [logb(N)] 

## 2. Why Brute Force Is Infeasible

A naive brute-force method would try all possible move sequences until it finds a solution. If the branching factor is denoted by `b` and the solution depth is `d`, then the time complexity of brute-force
search is:

`O(b^d)`

For Rubik’s cube, the branching factor is large because from most states there are many possible moves. As a result, brute-force search grows exponentially and becomes computationally infeasible very quickly.

Even though the maximum optimal solution length for the 3×3×3 cube is known to be 20 moves in the face-turn metric, that does not make brute-force practical. The issue is not only the length of the solution, 
but the enormous number of possible paths that must be explored before the correct path is found.

The practical approach is to use an admissible heuristic lower bound for each scrambled cube that we are attempting to solve.
More specifically, this means we will count how many non-center facelets are not on their correct face. We know that one quarter-turn can move at most 20 non-centered facelets which brings us to the lower bound:

h(s) = [misplaced non-centered facelets /20]

This allows us to compare h(s) lower bound estimate to L(s) solution length found by our solving algorithm.
If L(s) is much larger than h(s), our solver is far from ideal and if it is smaller our estimate is closer to ideal. This fulfills the project objective of finding an efficient algorithm that comes as close as reasonably 
possible to the theoretical lower bound.

## 3. Heuristic Meaning

A heuristic is an estimate of how far the current state is from the goal state. In this project, the goal state is the solved cube.

A good heuristic should:
- never overestimate the true remaining cost
- give meaningful guidance to the search
- be quick to compute

Our current heuristic is useful because it is safe and simple, but it does not capture enough structural information about the cube. Because of this, the search often still explores many unnecessary states.

## 4. Algorithm Used: IDA*

To solve the cube, we implemented a baseline version of **IDA\***, which stands for **Iterative Deepening A\***.

IDA\* combines:
- the heuristic-guided evaluation idea of A\*
- the low-memory behavior of depth-first search
- iterative deepening over increasing cost bounds

The algorithm uses the evaluation function:

`f(n) = g(n) + h(n)`

where:
- `g(n)` is the number of moves taken so far
- `h(n)` is the estimated number of moves remaining
- `f(n)` is the estimated total solution cost through that state

IDA\* begins with a search bound based on the heuristic value of the start state. It then performs a depth-first search, but only explores states whose `f(n)` value does not exceed the current bound. If no solution is found, the bound is increased and the search is repeated.

This makes IDA\* more memory-efficient than standard A\*, which is important for large combinatorial problems such as Rubik’s cube.

## 5. How Our Solver Works

Our solver follows these steps:

1. Represent the cube as a full cube state.
2. Simulate legal face turns, including regular, inverse, and double moves.
3. Compute the heuristic estimate for the scrambled state.
4. Use IDA\* to search for a valid solution sequence.
5. Increase the search bound until a solution is found.
6. Avoid unnecessary repetition by skipping immediate repeated face moves and preventing revisits to states already on the current search path.

This approach performs much better than brute force, but it still depends heavily on the quality of the heuristic.

## 6. Time Complexity of Our Method

The worst-case behavior of IDA\* is still exponential. In the absence of a strong heuristic, the algorithm may still have to examine a very large number of states. Therefore, the overall worst-case time complexity remains exponential in solution depth.

However, IDA\* can still be substantially more practical than brute force because the heuristic helps guide the search and prune unpromising paths.

In our implementation, the main limitation is that the heuristic is too weak. As a result, the solver remains effective mainly on short scrambles and begins to slow down quickly as scramble depth increases.

## 7. Why Our Method Is Not Yet Near the Ideal Lower Bound

The project objective asks for a method that comes as close as reasonably possible to the theoretical lower bound. Our implementation should be viewed as a **baseline informed search solver**, not a world-class near-optimal cube solver.

The gap between our observed performance and the ideal lower bound can be explained by two main factors:

1. **The heuristic is weak.**  
   It gives only a rough estimate of distance from the solved state and often assigns similarly small values to states with very different true difficulty.

2. **Rubik’s cube has a very large search space.**  
   Even with heuristic guidance, many states may still need to be explored.
