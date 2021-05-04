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
 * При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.Ф
 *
 * Формат выходных данных:
 * Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в
 * зависимости от того, встречается ли данное слово в нашем множестве.
 */

const size_t HASHER_CONSTANT = 777;
const size_t HASH_TABLE_DEFAULT_SIZE = 32;
const size_t TABLE_RESIZE_MULTIPLIER = 2;
const size_t TABLE_FILL_IN_RATE_NUMERATOR = 2;
const size_t TABLE_FILL_IN_RATE_DENOMINATOR = 3;

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
class HashMap {
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
    };

    H _hasher;
    std::vector<TableCell> _table;
    size_t _keysCount;
    size_t _delCount;

    bool hasByHash(size_t hash);

    void reHash(bool isMustGrow);

    void tryReHash();

    size_t probe(size_t hash, size_t iteration);

public:
    explicit HashMap(const H &hasher = H());

    ~HashMap();

    HashMap(const HashMap &) = delete;

    HashMap &operator=(const HashMap &) = delete;

    bool has(const T &element);

    bool del(const T &element);

    bool add(const T &element);

};

template<class T, class H>
bool HashMap<T, H>::add(const T &element) {
    size_t index = 0, iteration = 0, absoluteHash = _hasher(element);
    int firstDelPosition = -1;

    while (true) {
        index = probe(absoluteHash, iteration++);
        if (firstDelPosition == -1 && _table[index].status == DEL) { firstDelPosition = index; }
        if (_table[index].status == FREE) {
            index = firstDelPosition != -1 ? firstDelPosition : index;
            _table[index].data = element;
            _table[index].hash = absoluteHash;
            _table[index].status = FULL;
            _keysCount++;
}           tryReHash();
            return true;
        }
        if (_table[index].status == FULL && _table[index].data == element) {
            return false;
        }
    }
}

template<class T, class H>
HashMap<T, H>::HashMap(const H &hasher) : _hasher(hasher), _table(HASH_TABLE_DEFAULT_SIZE),
                                          _keysCount(0), _delCount(0) {}

template<class T, class H>
size_t HashMap<T, H>::probe(size_t hash, size_t iteration) {
    return (hash + (iteration + iteration * iteration) / 2) % _table.size();;
}

template<class T, class H>
void HashMap<T, H>::tryReHash() {
    if ((_keysCount + _delCount) * TABLE_FILL_IN_RATE_DENOMINATOR < _table.size() * TABLE_FILL_IN_RATE_NUMERATOR) { return; }
   !_delCount ? reHash(true) : reHash(false);  // Если 2/3 таблицы - только ключи, то растем.
}
