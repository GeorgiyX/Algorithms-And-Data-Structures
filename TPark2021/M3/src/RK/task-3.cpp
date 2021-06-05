#ifndef GRAPHS_GRAPHINTERFACE_H
#define GRAPHS_GRAPHINTERFACE_H

#include <vector>
#include <functional>

struct IGraph {
    virtual ~IGraph() = default;

    /** Добавление ребра от from к to. */
    virtual void addEdge(int from, int to) = 0;

    virtual int verticesCount() const = 0;

    /** Возвращает список смежных вершин */
    virtual std::vector<int> getNextVertices(int vertex) const = 0;

    /** Возвращает список "родительских" вершин */
    virtual std::vector<int> getPrevVertices(int vertex) const = 0;

    void dfs_aux(std::vector<bool> &visited, int vertex, const std::function<void(int)> &callback ) const {
        visited[vertex] = true;
        callback(vertex);
        auto childs = getNextVertices(vertex);
        for (auto child : childs) {
            if (!visited[child]) {
                dfs_aux(visited, child, callback);
            }
        }
    };


    void dfs(const std::function<void(int)> &callback) const {
        std::vector<bool> visited(verticesCount(), false);
        dfs_aux(visited, 0, callback);
    }
};

/**
 *   Граф G является эйлеровым тогда и только тогда, когда G – связный и все
 *   его вершины имеют четную степень. */
bool is_eulerian_graph(const IGraph &graph) {
    for (int i = 0; i < graph.verticesCount(); ++i) {
        // Проверка степени вершины
        if (((graph.getNextVertices(i).size() + graph.getPrevVertices(i).size())) % 2 != 0) {
            return false;
        }
    }
    size_t visitedCount = 0;
    graph.dfs([&visitedCount](int){visitedCount++;});
    return graph.verticesCount() != visitedCount;
};

int main() {

}

#endif //GRAPHS_GRAPHINTERFACE_H
