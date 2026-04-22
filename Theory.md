The approach we decided to use for this project:

Global lower bound idea. The easiest idea we could come up with was treating solving as a search tree. In which we let N = number of reachable cube states
at the current move and b = branching factor. If a search tree has branching factor at most b, then to distinguish among N possible states,
the solution depth must satisfy: d >= [logb(N)] 

The practical approach is to use an admissible heuristic lower bound for each scrambled cube that we are attempting to solve.
More specifically, this means we will count how many non-center facelets are not on their correct face. We know that one
quarter-turn can move at most 20 non-centered facelets which brings us to the lower bound:

h(s) = [misplaced non-centered facelets /20]

This allows us to compare h(s) lower bound estimate to L(s) solution length found by our solving algorithm.
If L(s) is much larger than h(s), our solver is far from ideal and if it is smaller our estimate is closer to ideal.
This fulfills the project objective of finding an efficient algorithm that comes as close as reasonably possible to the 
theoretical lower bound.
