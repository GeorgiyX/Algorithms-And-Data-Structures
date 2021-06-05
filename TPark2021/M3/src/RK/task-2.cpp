#include <vector>
#include <stack>
#include <iostream>

void dfs_aux(std::vector<std::vector<bool>> &graph, size_t vertexCount, size_t &vertexVisitedCount,
             std::vector<bool> &visited, size_t vertex) {
    visited[vertex] = true;
    vertexVisitedCount++;
    for (int i = 0; i < vertexCount; i++) {
        if (graph[vertex][i] && !visited[i]) {
            dfs_aux(graph, vertexCount, vertexVisitedCount, visited, i);
        }
    }
}
/**
 * Если граф является деревом если он связный и V == E+1;
 */
int main() {
    size_t vertexCount = 0, edgesCount = 0, from = 0, to = 0, vertexVisitedCount = 0;
    std::cin >> vertexCount >> edgesCount;
    std::vector<std::vector<bool>> graph(vertexCount, std::vector<bool>(vertexCount, false));
    std::vector<bool> visited(vertexCount, false);
    for (size_t i = 0; i < edgesCount; ++i) {
        std::cin >> from >> to;
        graph[from][to] = true;
    }

    dfs_aux(graph, vertexCount, vertexVisitedCount, visited, 0);

    if (vertexCount != edgesCount + 1 || vertexVisitedCount != vertexCount) {
        std::cout << 0;
    } else {
        std::cout << 1;
    }
}