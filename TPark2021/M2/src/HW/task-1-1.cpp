#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>

/** Задание 1_1: "Хеш-таблица"
* Реализуйте структуру данных типа “множество строк” на основе динамической
* хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
 * при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество,
 * удаления строки из множества и проверки принадлежности данной строки множеству.
 *
 * 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
 * g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
 *
 * Требования: В таблице запрещено хранение указателей на описатель элемента.
 *
 * Формат входных данных
 * Каждая строка входных данных задает одну операцию над множеством. Запись операции
 * состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
 *
 * Тип операции – один из трех символов:
 * + означает добавление данной строки в множество;
 * - означает удаление строки из множества;
 * ? означает проверку принадлежности данной строки множеству.
 * При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве.
 * При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
 *
 * Формат выходных данных:
 * Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в
 * зависимости от того, встречается ли данное слово в нашем множестве.
 */

const size_t HASHER_CONSTANT = 777;
const size_t TABLE_DEFAULT_SIZE = 8;
const size_t TABLE_RESIZE_MULTIPLIER = 2;
const size_t TABLE_FILL_IN_RATE_NUMERATOR = 3;
const size_t TABLE_FILL_IN_RATE_DENOMINATOR = 4;

struct Hasher {
    size_t operator()(const std::string &string) const {
        size_t hash = 0;
        for (auto ch : string) {
            hash *= HASHER_CONSTANT;
            hash += ch;
        }
        return hash;
    }
};

template<class T, class H>
class HashSet {
    enum CellStatus {
        DEL,
        FREE,
        FULL
    };

    /**
     * @brief В таблице помимо данных храним служебную информацию,
     * необходимую для работы СД.
     */
    struct TableCell {
        T data;
        size_t hash;
        CellStatus status;
        bool reHashFlag;  // перехешированные и не перехешированные ячейки отличаются значением этого поля
    };

    H _hasher;
    bool _currentReHashFlag;  // текущее значение флага не перехешированных ячеек
    std::vector<TableCell> _table;
    size_t _keysCount;
    size_t _delCount;

    void reHash(bool isMustGrow);
    void tryReHash();
    size_t probe(size_t hash, size_t iteration);

public:
    explicit HashSet(const H &hasher = H());
    HashSet(const HashSet &) = delete;
    HashSet(HashSet &&) = delete;
    HashSet &operator=(const HashSet &) = delete;
    HashSet &operator=(HashSet &&) = delete;

    bool has(const T &element);
    bool del(const T &element);
    bool add(const T &element);
};

template<class T, class H>
HashSet<T, H>::HashSet(const H &hasher) : _hasher(hasher), _currentReHashFlag(true),
                                          _table(TABLE_DEFAULT_SIZE, {T(), 0, FREE, _currentReHashFlag}),
                                          _keysCount(0), _delCount(0) {}

template<class T, class H>
size_t HashSet<T, H>::probe(size_t hash, size_t iteration) {
    return (hash + (iteration + iteration * iteration) / 2) % _table.size();;
}

template<class T, class H>
void HashSet<T, H>::tryReHash() {
    if ((_keysCount + _delCount) * TABLE_FILL_IN_RATE_DENOMINATOR < _table.size() * TABLE_FILL_IN_RATE_NUMERATOR) { return; }
    !_delCount ? reHash(true) : reHash(false);  // Если 3/4 таблицы - только ключи, то растем.
}

template<class T, class H>
bool HashSet<T, H>::add(const T &element) {
    size_t index = 0, iteration = 0, absoluteHash = _hasher(element);
    int firstDelPosition = -1;

    while (true) {
        assert(iteration < _table.size());
        index = probe(absoluteHash, iteration++);
        if (firstDelPosition == -1 && _table[index].status == DEL) { firstDelPosition = index; }
        if (_table[index].status == FULL && _table[index].data == element) { return false; }
        if (_table[index].status == FREE) {
            if (firstDelPosition != -1) {
                _delCount--;
                index = firstDelPosition;
            }
            _table[index].data = element;
            _table[index].hash = absoluteHash;
            _table[index].status = FULL;
            _table[index].reHashFlag = _currentReHashFlag;
            _keysCount++;
            tryReHash();
            return true;
        }
    }
}

template<class T, class H>
bool HashSet<T, H>::del(const T &element) {
    size_t index = 0, iteration = 0, absoluteHash = _hasher(element);

    while (true) {
        assert(iteration < _table.size());
        index = probe(absoluteHash, iteration++);
        if (_table[index].status == FULL && _table[index].data == element) {
            _table[index].status = DEL;
            _delCount++;
            _keysCount--;
            return true;
        }
        if (_table[index].status == FREE) {
            return false;
        }
    }
}

template<class T, class H>
bool HashSet<T, H>::has(const T &element) {
    size_t index = 0, iteration = 0, absoluteHash = _hasher(element);

    while (true) {
        assert(iteration < _table.size());
        index = probe(absoluteHash, iteration++);
        if (_table[index].status == FULL && _table[index].data == element) { return true; }
        if (_table[index].status == FREE) { return false; }
    }
}

template<class T, class H>
void HashSet<T, H>::reHash(bool isMustGrow) {
    if (isMustGrow) {_table.resize(_table.size() * TABLE_RESIZE_MULTIPLIER,
                                   {T(), 0, FREE, _currentReHashFlag});
    }
    size_t index = 0, pendingElements = _keysCount, elementNewIndex = 0, elementProbeIteration = 0;

    while (pendingElements || _delCount) {
        assert(index < _table.size());
        /* Если в ячейке есть данные, и они не были перехешированы: */
        if (_table[index].status == FULL && _table[index].reHashFlag == _currentReHashFlag) {
            elementProbeIteration = 0;
            /* Пробируемся, пока нам попадаются перехешированные ячейки */
            do { elementNewIndex = probe(_table[index].hash, elementProbeIteration++);
            } while (_table[elementNewIndex].status == FULL
                     && _table[elementNewIndex].reHashFlag == !_currentReHashFlag);

            if (_table[elementNewIndex].status == FULL) {
                std::swap(_table[index].data, _table[elementNewIndex].data);
                std::swap(_table[index].hash, _table[elementNewIndex].hash);
                _table[elementNewIndex].reHashFlag = !_currentReHashFlag;
            } else {
                if (_table[elementNewIndex].status == DEL) {_delCount--;}
                _table[elementNewIndex].data = _table[index].data;
                _table[elementNewIndex].hash = _table[index].hash;
                _table[elementNewIndex].status = FULL;
                _table[elementNewIndex].reHashFlag = !_currentReHashFlag;
                _table[index].status = FREE;
                index++;
            }
            pendingElements--;
        } else {
            if (_table[index].status == DEL) {
                _table[index].status = FREE;
                _delCount--;
            }
            index++;
        }
    }
    _currentReHashFlag = !_currentReHashFlag;
}

void testCase(std::istream &in, std::ostream &out) {
    char operation = 0;
    std::string data;
    HashSet<std::string, Hasher> hashSet;
    while (in >> operation >> data) {
        switch (operation) {
            case '+':
                out << (hashSet.add(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                out << (hashSet.del(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                out << (hashSet.has(data) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
                break;
        }
    }
}

void testHashSet() {
    {
        std::stringstream in, out;
        in << "+ hello\n? hello\n- hello\n? hello\n+ hello\n+ hello\n";
        testCase(in, out);
        assert(out.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\n");
        std::cout << "OK - 1" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "+ hello1\n+ hello2\n+ hello3\n+ hello4\n+ hello5\n+ hello6\n+ hello7\n+ hello8\n+ hello9\n+ hello10\n";
        testCase(in, out);
        assert(out.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n");
        std::cout << "OK - 2" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "+ qwert\n+ yuiop\n+ asdf\n+ ghjkl\n+ zxcvb\n+ mnbvcx\n+ zxcmnbx\n+ azsxcd\n+ vfgdcs\n+ vfiadgwsfss\n"; // 10
        in << "? qwert\n? yuiop\n? asdf\n? ghjkl\n? zxcvb\n? mnbvcx\n? zxcmnbx\n? azsxcd\n? vfgdcs\n? vfiadgwsfss\n"; // 10
        in << "- qwert\n- yuiop\n- asdf\n- ghjkl\n- zxcvb\n- mnbvcx\n- zxcmnbx\n- azsxcd\n- vfgdcs\n- vfiadgwsfss\n"; // 10
        in << "? qwert\n? yuiop\n? asdf\n? ghjkl\n? zxcvb\n? mnbvcx\n? zxcmnbx\n? azsxcd\n? vfgdcs\n? vfiadgwsfss\n"; // 10
        testCase(in, out);
        assert(out.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n"
                            "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\n"
                            "FAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n");
        std::cout << "OK - 3" << std::endl;
    }
    {
        std::stringstream in, out;
        in << "+ hello1\n+ hello2\n+ hello3\n+ hello4\n+ hello5\n+ hello6\n+ hello7\n+ hello8\n+ hello9\n+ hello10\n"
              "+ hello11\n";
        in << "- hello1\n- hello2\n- hello3\n- hello4\n- hello5\n- hello6\n- hello7\n- hello8\n- hello9\n- hello10\n"
              "- hello11\n";
        in << "+ 1\n? 1\n? 2\n";
        testCase(in, out);
        assert(out.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n"
                            "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\n");
        std::cout << "OK - 4" << std::endl;
    }
}

int main() {
    testCase(std::cin, std::cout);
    return 0;
}