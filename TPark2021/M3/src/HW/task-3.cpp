#include <functional>
#include <iostream>
#include <cassert>
#include <sstream>
#include <set>

/**
 * Требуется отыскать самый выгодный маршрут между городами.
 * Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
 *
 * Формат входных данных:
 * Первая строка содержит число N – количество городов.
 * Вторая строка содержит число M - количество дорог.
 * Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
 * Все указанные дороги двусторонние. Между любыми двумя городами может быть больше одной дороги.
 * Последняя строка содержит маршрут (откуда и куда нужно доехать).
 *
 * Формат выходных данных:
 * Вывести длину самого выгодного маршрута.
 */

class RoadListGraph {
public:
    struct Road {
        Road(size_t _to, size_t _distance) : to(_to), distance(_distance) {}
        size_t to;
        size_t distance;
    };

    explicit RoadListGraph(size_t verticesCount);

    ~RoadListGraph() = default;

    void addEdge(size_t from, size_t to, size_t time);

    size_t verticesCount() const;

    std::vector<Road> getNextVertices(size_t vertex) const;

    std::vector<Road> getPrevVertices(size_t vertex) const;

    /**
     * Алгоритм Дейкстры.
     * @return кратчайшее расстояние между from и to
     * */
    size_t getDistance(size_t from, size_t to);

private:
    /**
     * Проверяет, сократит ли newRoad из from дисатнцию до newRoad.to. Если, да то обновляем
     * новое расстояние.
     * @return true если найден более короткий маршрут, иначе false.
     */
    bool relax(Road newRoad, size_t from, std::vector<size_t> &distances);

    std::vector<std::vector<Road>> _graph;
    size_t _verticesCount;
};

/** Вспомогательный компаратор для поиска в std::set только по road.to */
bool operator<(const RoadListGraph::Road &road, size_t to) {
    return road.to < to;
}

/** Вспомогательный компаратор для поиска в std::set только по road.to */
bool operator<(size_t to, const RoadListGraph::Road &road) {
    return to < road.to;
}

/** Компаратор для элементов std::set */
bool operator<(const RoadListGraph::Road &lhs, const RoadListGraph::Road &rhs) {
    return lhs.distance < rhs.distance;
}

RoadListGraph::RoadListGraph(size_t verticesCount) : _graph(verticesCount, std::vector<Road>()),
                                                     _verticesCount(verticesCount) {

}

void RoadListGraph::addEdge(size_t from, size_t to, size_t time) {
    assert(from < _graph.size() && to < _graph.size());
    _graph[from].emplace_back(to, time);
    _graph[to].emplace_back(from, time);
}

size_t RoadListGraph::verticesCount() const {
    return _verticesCount;
}

std::vector<RoadListGraph::Road> RoadListGraph::getNextVertices(size_t vertex) const {
    assert(vertex < _graph.size());
    return _graph[vertex];
}

std::vector<RoadListGraph::Road> RoadListGraph::getPrevVertices(size_t vertex) const {
    assert(vertex < _graph.size());
    std::vector<Road> result;
    for (int parent = 0; parent < verticesCount(); parent++) {
        for (auto child : _graph[parent]) {
            if (child.to == vertex) { result.emplace_back(parent, child.distance); }
        }
    }
    return result;
}

size_t RoadListGraph::getDistance(size_t from, size_t to) {
    std::vector<size_t> distances(verticesCount(), std::numeric_limits<size_t>::max());
    std::set<Road, std::less<>> unvisited;
    distances[from] = 0;
    unvisited.emplace(from, 0);
    while (!unvisited.empty()) {
        /* Извлекаем город, дистанция между from и current известна точно: */
        auto current = unvisited.begin()->to;
        unvisited.erase(unvisited.begin());
        for (auto road : getNextVertices(current)) {
            if (distances[road.to] == std::numeric_limits<size_t>::max()) {
                distances[road.to] = distances[current] + road.distance;
                unvisited.emplace(road.to, distances[road.to]);
            } else if (relax(road, current, distances)) {
                /* Меняем "приоритет" города в unvisited: */
                unvisited.erase(unvisited.find(road.to));
                unvisited.emplace(road.to, distances[road.to]);
            }
        }
    }
    return distances[to];
}

bool RoadListGraph::relax(Road newRoad, size_t from, std::vector<size_t> &distances) {
    if (distances[newRoad.to] > distances[from] + newRoad.distance) {
        distances[newRoad.to] = distances[from] + newRoad.distance;
        return true;
    }
    return false;
}

void testCase(std::istream &in, std::ostream &out) {
    size_t citiesCount = 0, roadsCount = 0;
    size_t from = 0, to = 0, distance = 0;
    in >> citiesCount >> roadsCount;
    RoadListGraph graph(citiesCount);
    for (size_t i = 0; i < roadsCount; ++i) {
        in >> from >> to >> distance;
        graph.addEdge(from, to, distance);
    }
    in >> from >> to;
    out << graph.getDistance(from, to);
}

void test() {
    {
        std::stringstream in, out;
        in << "6\n 9\n 0 3 1\n 0 4 2\n 1 2 7\n 1 3 2\n 1 4 3\n 1 5 3\n 2 5 3\n 3 4 4\n 3 5 6\n 0 2";
        testCase(in, out);
        assert(out.str() == "9");
        std::cout << "OK - 1" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "9\n 16\n 0 2 2\n 0 3 6\n 0 1 9\n 2 3 3\n 3 1 2\n 1 6 4\n 3 6 7\n 2 4 1\n "
              "4 3 1\n 3 5 9\n 4 7 6\n 5 7 5\n 6 5 1\n 5 8 1\n 7 8 5\n 6 8 5\n 0 8";
        testCase(in, out);
        assert(out.str() == "12");
        std::cout << "OK - 2" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "9\n 18\n 0 2 2\n 0 3 6\n 0 1 9\n 2 3 3\n 3 1 2\n 1 6 4\n 3 6 7\n 2 4 1\n "
              "4 3 1\n 3 5 9\n 4 7 6\n 5 7 5\n 6 5 1\n 5 8 2\n 5 8 1\n 5 8 4\n 7 8 5\n 6 8 5\n 0 8";
        testCase(in, out);
        assert(out.str() == "12");
        std::cout << "OK - 3" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "9\n 16\n 0 2 2\n 0 3 6\n 0 1 9\n 2 3 3\n 3 1 2\n 1 6 4\n 3 6 7\n 2 4 1\n "
              "4 3 1\n 3 5 9\n 4 7 6\n 5 7 5\n 6 5 1\n 5 8 1\n 7 8 5\n 6 8 5\n 0 6";
        testCase(in, out);
        assert(out.str() == "10");
        std::cout << "OK - 4" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "11\n 18\n 0 2 2\n 0 3 6\n 0 1 9\n 2 3 3\n 3 1 2\n 1 6 4\n 3 6 7\n 2 4 1\n "
              "4 3 1\n 4 4 1\n 3 5 9\n 4 7 6\n 5 7 5\n 6 5 1\n 5 8 1\n 7 8 5\n 6 8 5\n 9 10 1\n 9 10";
        testCase(in, out);
        assert(out.str() == "1");
        std::cout << "OK - 6" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "11\n 18\n 0 2 2\n 0 3 6\n 0 1 9\n 2 3 3\n 3 1 2\n 1 6 4\n 3 6 7\n 2 4 1\n "
              "4 3 1\n 4 4 1\n 3 5 9\n 4 7 6\n 5 7 5\n 6 5 1\n 5 8 1\n 7 8 5\n 6 8 5\n 9 10 1\n 0 10";
        testCase(in, out);
        assert(out.str() == std::to_string(std::numeric_limits<size_t>::max()));
        std::cout << "OK - 7" << std::endl;
    }
}

int main() {
    testCase(std::cin, std::cout);
}
