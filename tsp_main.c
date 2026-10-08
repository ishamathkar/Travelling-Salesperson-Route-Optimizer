#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 20
#define INF 1000000   

int n;                         
char cityNames[MAX][30];        
int cost[MAX][MAX];            
int visited[MAX];               
int curr_path[MAX + 1];         
int final_path[MAX + 1];        
long final_res;                
int firstMin[MAX], secondMin[MAX];  
int startCity;                  

long expandedNodes = 0;         
long prunedNodes = 0;           

int firstMinFunc(int i) {
    int minVal = INF;
    for (int j = 0; j < n; j++) {
        if (i != j && cost[i][j] != -1 && cost[i][j] < minVal)
            minVal = cost[i][j];
    }
    return minVal;
}

int secondMinFunc(int i) {
    int first = INF, second = INF;
    for (int j = 0; j < n; j++) {
        if (i == j || cost[i][j] == -1) continue;
        if (cost[i][j] <= first) {
            second = first;
            first = cost[i][j];
        }
        else if (cost[i][j] <= second) {
            second = cost[i][j];
        }
    }
    return second;
}


long nearestNeighborTour(int start, int path[]) {
    int localVisited[MAX] = {0};
    localVisited[start] = 1;
    path[0] = start;
    int curr = start;
    long totalCost = 0;

    for (int count = 1; count < n; count++) {
        int nextCity = -1;
        int minCost = INF;
        for (int j = 0; j < n; j++) {
            if (!localVisited[j] && cost[curr][j] != -1 && cost[curr][j] < minCost) {
                minCost = cost[curr][j];
                nextCity = j;
            }
        }
        if (nextCity == -1)
            return INF;  

        path[count] = nextCity;
        localVisited[nextCity] = 1;
        totalCost += minCost;
        curr = nextCity;
    }

    /* close the tour by returning to start */
    if (cost[curr][start] == -1)
        return INF;

    totalCost += cost[curr][start];
    path[n] = start;
    return totalCost;
}


void TSPBranchAndBound(int curr_bound, long curr_weight, int level) {
    expandedNodes++;

    if (level == n) {
        int lastCity = curr_path[level - 1];
        if (cost[lastCity][startCity] != -1) {
            long total = curr_weight + cost[lastCity][startCity];
            if (total < final_res) {
                for (int k = 0; k < n; k++)
                    final_path[k] = curr_path[k];
                final_path[n] = startCity;
                final_res = total;
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        int lastCity = curr_path[level - 1];

        if (cost[lastCity][i] != -1 && !visited[i]) {
            int temp = curr_bound;
            long newWeight = curr_weight + cost[lastCity][i];

            
            if (level == 1)
                curr_bound -= ((firstMin[lastCity] + firstMin[i]) / 2);
            else
                curr_bound -= ((secondMin[lastCity] + firstMin[i]) / 2);

            /* Step 5: prune if this branch cannot beat the incumbent */
            if (curr_bound + newWeight < final_res) {
                curr_path[level] = i;
                visited[i] = 1;
                TSPBranchAndBound(curr_bound, newWeight, level + 1);
            }
            else {
                prunedNodes++;
            }

            /* backtrack: undo bound change and unmark visited */
            curr_bound = temp;
            visited[i] = 0;
        }
    }
}


int validateInput() {
    if (n < 1 || n > MAX) {
        printf("Invalid number of cities.\n");
        return 0;
    }
    if (startCity < 0 || startCity >= n) {
        printf("Invalid starting city index.\n");
        return 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && cost[i][j] < -1) {
                printf("Invalid cost at (%d,%d): costs must be >= 0, or -1 for no edge.\n", i, j);
                return 0;
            }
        }
        cost[i][i] = -1;  /* a city cannot travel to itself */
    }
    return 1;
}


double factorial(int x) {
    double result = 1;
    for (int i = 2; i <= x; i++)
        result *= i;
    return result;
}

int main() {
    printf("===== Travelling Salesperson Route Optimizer (Branch & Bound) =====\n\n");

    printf("Enter number of cities: ");
    scanf("%d", &n);

    if (n < 1 || n > MAX) {
        printf("Number of cities must be between 1 and %d.\n", MAX);
        return 1;
    }

    printf("\nEnter city names:\n");
    for (int i = 0; i < n; i++) {
        printf("City %d: ", i + 1);
        scanf("%s", cityNames[i]);
    }

    printf("\nEnter the cost matrix (%d x %d).\n", n, n);
    printf("Use -1 to indicate no direct edge between two cities.\n");
    printf("Diagonal entries (i to i) will be ignored automatically.\n\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("Cost[%s][%s]: ", cityNames[i], cityNames[j]);
            scanf("%d", &cost[i][j]);
        }
    }

    printf("\nEnter starting city (1 to %d): ", n);
    int startInput;
    scanf("%d", &startInput);
    startCity = startInput - 1;

    if (!validateInput()) {
        printf("Invalid input. Program terminated.\n");
        return 1;
    }

    
    if (n == 1) {
        printf("\nOnly one city given. Trivial tour: %s -> %s, Cost = 0\n",
               cityNames[startCity], cityNames[startCity]);
        return 0;
    }

    
    for (int i = 0; i < n; i++) {
        firstMin[i] = firstMinFunc(i);
        secondMin[i] = secondMinFunc(i);
        if (secondMin[i] == INF)   /* city has only one valid neighbor, fallback */
            secondMin[i] = firstMin[i];
    }

    
    int nnPath[MAX + 1];
    long nnCost = nearestNeighborTour(startCity, nnPath);

    if (nnCost != INF) {
        final_res = nnCost;
        for (int k = 0; k <= n; k++)
            final_path[k] = nnPath[k];
        printf("\nInitial feasible tour found (nearest neighbor heuristic).\n");
        printf("Initial upper bound = %ld\n", final_res);
    }
    else {
        final_res = LONG_MAX;  /* no feasible tour found via heuristic, start unbounded */
        printf("\nNo feasible initial tour found via heuristic; starting unbounded search.\n");
    }

    
    for (int i = 0; i < n; i++)
        visited[i] = 0;
    visited[startCity] = 1;
    curr_path[0] = startCity;


    int rootBound = 0;
    for (int i = 0; i < n; i++)
        rootBound += (firstMin[i] + secondMin[i]);
    rootBound = (rootBound % 2 == 0) ? rootBound / 2 : rootBound / 2 + 1;

    
    TSPBranchAndBound(rootBound, 0, 1);

    
    printf("\n================ RESULT ================\n");
    if (final_res == LONG_MAX) {
        printf("No feasible tour exists for the given cost matrix.\n");
    }
    else {
        printf("Best Tour Found:\n  ");
        for (int k = 0; k <= n; k++) {
            printf("%s", cityNames[final_path[k]]);
            if (k != n) printf(" -> ");
        }
        printf("\nTotal Minimum Cost = %ld\n", final_res);
    }

    printf("\n------------ Search Statistics ------------\n");
    printf("Nodes expanded : %ld\n", expandedNodes);
    printf("Nodes pruned   : %ld\n", prunedNodes);

    double bruteForceCount = factorial(n - 1);
    printf("\nFor comparison: brute-force would explore up to (n-1)! = %.0f permutations.\n", bruteForceCount);
    printf("Branch and Bound explored only %ld nodes -- demonstrating the pruning benefit.\n", expandedNodes);
    printf("(Note: as n grows, (n-1)! grows extremely fast -- this is why TSP via B&B is only\n");
    printf(" practical for small city counts.)\n");

    return 0;
}