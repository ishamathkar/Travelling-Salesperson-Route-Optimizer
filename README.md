# Travelling Salesperson Route Optimizer

## Module Mapping
**Module 5: Backtracking and Branch and Bound**

## Problem Statement
Create a console-based program that finds a minimum-cost tour that visits every city exactly once and returns to the starting city.

## Description
The Travelling Salesperson Problem (TSP) is an optimization problem in which a salesperson must visit every city exactly once and return to the starting city while minimizing the total travel cost.

This program uses the **Branch and Bound** technique to solve TSP for small numbers of cities. It explores possible partial tours and calculates a lower bound for each branch. A branch is discarded when its estimated cost cannot improve the best complete tour found so far.

The program also uses a **Nearest Neighbor heuristic** to obtain an initial feasible tour whenever possible. The cost of this tour is used as the initial upper bound, which helps the Branch and Bound algorithm prune more branches.

Since TSP is computationally difficult and its search space grows rapidly with the number of cities, the program is intended for small city counts.

## Objective
- Apply the Branch and Bound technique to an optimization problem.
- Find the minimum-cost Hamiltonian tour.
- Maintain an upper bound using the best complete tour found.
- Calculate lower bounds for partial tours.
- Prune branches that cannot produce a better solution.
- Track the number of expanded and pruned nodes.
- Compare the Branch and Bound search with the growth of brute-force permutations.

## Expected Input
The program accepts:
- Number of cities
- Name of each city
- Cost matrix representing the travel cost between cities
- Starting city

A cost of `-1` can be used to represent no direct edge between two cities.

## Expected Output
The program displays:
- Initial feasible tour, when found
- Initial upper bound
- Best tour found
- Minimum total cost
- Number of nodes expanded
- Number of nodes pruned
- Brute-force comparison using `(n-1)!`
- A message if no feasible tour exists

## Methodology

### 1. Validate Input
The program checks the number of cities, starting city, and cost matrix. Invalid costs are rejected, and diagonal entries are treated as invalid because a city cannot travel to itself.

### 2. Calculate Minimum Edges
For every city, the program finds its first and second minimum outgoing edge costs. These values are used to calculate the lower bound required by Branch and Bound.

### 3. Find Initial Feasible Tour
The program uses the **Nearest Neighbor heuristic** to find an initial feasible tour. If a tour is found, its cost becomes the initial upper bound.

### 4. Initialize Branch and Bound
A lower bound is calculated for the root node. The starting city is marked as visited, and the search begins with an empty partial travel cost.

### 5. Expand Partial Tours
The algorithm recursively explores unvisited cities and extends the current partial route. For every selected edge, the path cost and lower bound are updated.

### 6. Prune Unpromising Branches
If the current path cost plus its lower bound is not better than the best complete tour already found, that branch is discarded.

### 7. Complete the Tour
When all cities have been visited, the program checks whether the last city can return to the starting city. If the resulting tour has a lower cost, it becomes the new best solution.

### 8. Display Search Statistics
The program records the number of nodes expanded and pruned. It also calculates `(n-1)!` to demonstrate how quickly the brute-force search space grows as the number of cities increases.

## Algorithm Used
**Branch and Bound**

The lower bound is calculated using the first and second minimum outgoing edges of each city. This allows the algorithm to identify branches that cannot produce a better solution and avoid unnecessary exploration.

### Supporting Technique
**Nearest Neighbor Heuristic**

It is used only to obtain an initial feasible solution and upper bound. The final solution is determined by Branch and Bound.

## Complexity
The Travelling Salesperson Problem has factorial growth in the number of possible tours.

For a fixed starting city, brute-force search can require up to:

`(n-1)!`

possible permutations.

Branch and Bound can significantly reduce the number of explored nodes by pruning unpromising branches. However, its worst-case time complexity remains **O(n!)**.

The space complexity is approximately **O(n²)** due to the cost matrix, with additional **O(n)** space for paths, visited arrays, and recursion.

## Features
- User-defined city names
- User-defined travel costs
- Configurable starting city
- Support for missing edges using `-1`
- Initial solution using Nearest Neighbor
- Lower-bound calculation
- Branch pruning
- Best tour and minimum cost
- Expanded and pruned node statistics
- Brute-force comparison
- Handling of cases where no feasible tour exists

## Limitations
- The program is intended for small numbers of cities.
- The search space grows factorially as the number of cities increases.
- A feasible initial tour may not always be found by the Nearest Neighbor heuristic.
- The program uses a fixed maximum of 20 cities.
