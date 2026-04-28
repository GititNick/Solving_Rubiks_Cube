# Experimental Results

## 1. Experimental Setup

To evaluate our implementation, we tested the baseline IDA\* solver on several short random scrambles. For each test case, we recorded:

- the scramble sequence
- the heuristic lower bound `h(s)`
- whether the solver found a valid solution
- the number of nodes expanded
- the search bound at which the solution was found
- the total runtime
- the solution length

After a solution was returned, we verified correctness by applying the solution sequence to the scrambled cube and checking that the cube returned to the solved state.

## 2. Trial Data

### Scramble Length Group: 5

| Trial | Scramble | Lower Bound | Solved | Nodes Expanded | Time (ms) | Solution Length |
|---|---|---:|---|---:|---:|---:|
| 1 | `B2 D' B' F' R'` | 2 | true | 1614 | 1671 | 5 |
| 2 | `F D2 F R2 F2` | 2 | true | 3767 | 3672 | 5 |
| 3 | `B F' U D2 U'` | 2 | true | 9 | 16 | 3 |

### Scramble Length Group: 6

| Trial | Scramble | Lower Bound | Solved | Nodes Expanded | Time (ms) | Solution Length |
|---|---|---:|---|---:|---:|---:|
| 1 | `B' D2 L2 D' R F` | 2 | true | 37393 | 36378 | 6 |
| 2 | `B L R U F' R2` | 2 | true | 25509 | 24649 | 6 |
| 3 | `D2 F B2 F R2 B2` | 2 | true | 4631 | 4676 | 5 |

## 3. Average Results

### Average for 5-Move Scrambles

- Average runtime: **1.79 seconds**
- Average nodes expanded: **1796.67**
- Average solution length: **4.33 moves**

### Average for 6-Move Scrambles

- Average runtime: **21.90 seconds**
- Average nodes expanded: **22511.00**
- Average solution length: **5.67 moves**

### Overall Average Across All 6 Trials

- Average runtime: **11.84 seconds**
- Average nodes expanded: **12153.83**
- Average solution length: **5.00 moves**

## 4. Interpretation of Results

Our experimental results showed that the current IDA\* baseline solver was able to correctly solve all tested short scrambles, but the runtime increased significantly as scramble depth increased. 
For the three 5-move scramble trials, the observed solve times were 1671 ms, 3672 ms, and 16 ms, giving an average runtime of approximately 1.79 seconds. For the three 6-move scramble trials, 
the solve times were 36378 ms, 24649 ms, and 4676 ms, giving an average runtime of approximately 21.90 seconds. Across all six trials combined, the overall average runtime was about 11.84 seconds per solve. 
These results suggest that while the method is effective on short scrambles, its computational cost grows rapidly as problem difficulty increases. This trend is consistent with the exponential nature of Rubik’s
cube search and also highlights a limitation of our current heuristic, which provides a valid lower bound but is too weak to guide the search efficiently on harder states.

## 5. What the Lower Bound Results Show

In every test case shown above, the heuristic lower bound remained at `2`, even though the actual solution lengths ranged from `3` to `6`. This tells us that the heuristic is admissible but not very informative.

A stronger heuristic should produce values that more closely reflect the true remaining difficulty of the cube state. Because our lower bound often stayed small even for harder cases, 
the solver was forced to expand many additional states before finding a solution.

This is one of the most important conclusions from the experiments:  
the search method works, but the current heuristic is too weak to scale efficiently.

## 6. Growth in Search Cost

One of the clearest patterns in the data is the sharp growth in search cost from scramble length 5 to scramble length 6.

- The average runtime increased from **1.79 seconds** to **21.90 seconds**
- The average number of expanded states increased from **1796.67** to **22511.00**

This means that increasing scramble difficulty by only one move caused a dramatic increase in computational effort. This behavior is consistent with the exponential growth expected in large search spaces.

## 7. Important Observation About Scramble Length

The scramble length was not always equal to the final solution length. For example, one 5-move scramble was solved in 3 moves, and one 6-move scramble was solved in 5 moves.

This happened because the scramble sequence itself was not necessarily optimal. A scramble may contain moves that partially cancel, or the cube may have a shorter alternative path back to the solved state.

This is important because it shows that scramble length alone does not determine the true difficulty of a cube state.

## 8. Limitations of the Current Results

These results should be interpreted carefully.

- The tests were performed only on short scrambles.
- The number of trials was small.
- Runtime depends on the current hardware and implementation.
- The heuristic is a simple baseline lower bound and does not reflect full cube structure.

Because of these limitations, the measured runtimes should not be treated as general solve times for arbitrary Rubik’s cube states. Instead, they should be viewed 
as evidence of the behavior of our current baseline implementation.

## 9. Conclusion

The experiments show that our solver is correct on short scrambles and that IDA\* is capable of solving the cube when guided by even a simple admissible heuristic. 
However, the results also show that runtime and node expansions grow quickly as scramble depth increases. This confirms that Rubik’s cube solving remains computationally 
difficult even with informed search, and it supports our conclusion that stronger heuristics are needed to bring performance closer to the theoretical lower bound.
