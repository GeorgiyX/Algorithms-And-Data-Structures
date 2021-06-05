#include <functional>
#include <iostream>
#include <cassert>
#include <sstream>
#include <queue>

/**
 * Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
 * Найдите количество различных кратчайших путей между заданными вершинами.
 * Требования: сложность O(V+E).
 * Формат ввода:
 * v: кол-во вершин (макс. 50000),
 * n: кол-во ребер (макс. 200000),
 * n пар реберных вершин,
 * пара вершин u, w для запроса.
 * Формат вывода:
 * Количество кратчайших путей от u к w.
 */

struct IGraph {
    virtual ~IGraph() = default;

    /** Добавление ребра от from к to. */
    virtual void addEdge(size_t from, size_t to) = 0;

    virtual size_t verticesCount() const = 0;

    /** Возвращает список смежных вершин */
    virtual std::vector<size_t> getNextVertices(size_t vertex) const = 0;

    /** Возвращает список "родительских" вершин */
    virtual std::vector<size_t> getPrevVertices(size_t vertex) const = 0;

};

class ListGraph : public IGraph{
public:

    explicit ListGraph(size_t verticesCount);

    explicit ListGraph(const IGraph &rhs);

    ~ListGraph() override = default;

    void addEdge(size_t from, size_t to) override;

    size_t verticesCount() const override;

    std::vector<size_t> getNextVertices(size_t vertex) const override;

    std::vector<size_t> getPrevVertices(size_t vertex) const override;

    size_t getShortestPathCount(size_t from, size_t to) const;

private:
    std::vector<std::vector<size_t>> _graph;
    size_t _verticesCount;
};


ListGraph::ListGraph(size_t verticesCount) : _graph(verticesCount, std::vector<size_t>()),
                                             _verticesCount(verticesCount) {

}

void ListGraph::addEdge(size_t from, size_t to) {
    assert(from < _graph.size() && to < _graph.size());
    _graph[from].push_back(to);
    _graph[to].push_back(from);
}

size_t ListGraph::verticesCount() const {
    return _verticesCount;
}

std::vector<size_t> ListGraph::getNextVertices(size_t vertex) const {
    assert(vertex < _graph.size());
    return _graph[vertex];
}

std::vector<size_t> ListGraph::getPrevVertices(size_t vertex) const {
    assert(vertex < _graph.size());
    std::vector<size_t> result;
    for (int parent = 0; parent < verticesCount(); parent++) {
        for (auto child : _graph[parent]) {
            if (child == vertex) { result.push_back(parent); }
        }
    }
    return result;
}

ListGraph::ListGraph(const IGraph &rhs) : _graph(rhs.verticesCount(), std::vector<size_t>()),
                                          _verticesCount(rhs.verticesCount()) {
    for (int i = 0; i < rhs.verticesCount(); ++i) {
        _graph[i] = rhs.getNextVertices(i);
    }
}

size_t ListGraph::getShortestPathCount(size_t from, size_t to) const {
    std::vector<size_t> distance(verticesCount(), std::numeric_limits<size_t>::max());
    std::vector<size_t> pathCount(verticesCount(), 0);
    std::queue<size_t> queue;
    queue.push(from);
    pathCount[from] = 1;
    distance[from] = 0;
    while(!queue.empty()) {
        from = queue.front();
        queue.pop();
        for (auto child : getNextVertices(from)) {
            if (distance[child] > distance[from] + 1) {
                /* Найден более короткий путь, все все предыдущие пути не учитываем: */
                pathCount[child] = pathCount[from];
                distance[child] = distance[from] + 1;
                queue.push(child);
            } else if (distance[child] == distance[from] + 1) {
                /* Найден еще один короткий путь, посчитаем его вместе с остальными: */
                pathCount[child] = pathCount[child] + pathCount[from];
            }
        }
    }
    return pathCount[to];
}

void testCase(std::istream &in, std::ostream &out) {
    size_t vertexCount = 0, edgesCount = 0;
    size_t from = 0, to = 0;
    in >> vertexCount >> edgesCount;
    ListGraph graph(vertexCount);
    for (size_t i = 0; i < edgesCount; ++i) {
        in >> from >> to;
        graph.addEdge(from, to);

    }
    in >> from >> to;
    out << graph.getShortestPathCount(from, to);
}

void test() {
    {
        std::stringstream in, out;
        in << "4\n5\n0 1\n0 2\n1 2\n1 3\n2 3\n0 3";
        testCase(in, out);
        assert(out.str() == "2");
        std::cout << "OK - 1" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "7\n9\n0 1\n0 6\n6 5\n1 5\n1 2\n5 2\n5 4\n4 3\n2 3\n0 4";
        testCase(in, out);
        assert(out.str() == "2");
        std::cout << "OK - 2" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "7\n9\n0 1\n0 6\n6 5\n1 5\n1 2\n5 2\n5 4\n4 3\n2 3\n0 3";
        testCase(in, out);
        assert(out.str() == "1");
        std::cout << "OK - 3" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "4\n4\n0 2\n2 1\n3 0\n3 1\n0 1";
        testCase(in, out);
        assert(out.str() == "2");
        std::cout << "OK - 4" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "7\n8\n0 1\n0 2\n2 3\n1 3\n3 4\n3 5\n4 6\n5 6\n0 6";
        testCase(in, out);
        assert(out.str() == "4");
        std::cout << "OK - 5" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "7\n10\n0 1\n0 2\n2 3\n1 3\n3 4\n3 5\n4 6\n5 6\n2 4\n1 4\n0 6";
        testCase(in, out);
        assert(out.str() == "2");
        std::cout << "OK - 6" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "7\n8\n0 1\n0 2\n2 3\n1 3\n3 4\n3 5\n4 6\n5 6\n0 4";
        testCase(in, out);
        assert(out.str() == "2");
        std::cout << "OK - 7" << std::endl;
    }
}

int main() {
    testCase(std::cin, std::cout);
}
