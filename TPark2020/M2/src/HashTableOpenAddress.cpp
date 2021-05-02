#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>

/// Задача 1_2
/// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
/// Хранимые строки непустые и состоят из строчных латинских букв.
/// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
/// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
/// когда коэффициент заполнения таблицы достигает 3/4.
/// Структура данных должна поддерживать операции добавления строки в множество,
/// удаления строки из множества и проверки принадлежности данной строки множеству.
///
/// Для разрешения коллизий используйте двойное хеширование.

struct Hasher {
    size_t operator()(const std::string &data) const;
};

struct HasherOther {
    size_t operator()(const std::string &data) const;
};

size_t HasherOther::operator()(const std::string &data) const {
    size_t hash = 0;
    int a = 167;
    const char *str = data.c_str();
    while (*str != 0) {
        hash = hash * a + *str;
        ++str;
    }
    return hash;
}

size_t Hasher::operator()(const std::string &data) const {
    size_t hash = 0;
    int a = 137;
    const char *str = data.c_str();
    while (*str != 0) {
        hash = hash * a + *str;
        ++str;
    }
    return hash;
}

template<class T, class H, class H2>
class HashTable {
public:
    explicit HashTable(const H &hasher = H(), const H2 &probeHasher = H2(), size_t initSize = 8);

    HashTable(const HashTable &table) = delete;

    HashTable &operator=(const HashTable &table) = delete;

    ~HashTable() = default;

    bool has(const T &key) const;

    bool add(const T &key);

    bool remove(const T &key);

private:
    enum CellStatus {
        FREE,
        NOT_FREE,
        DELETED,
        REHASH_IT
    };

    size_t getProbeDoubleHash(size_t hash, size_t doubleHash, size_t i) const;

    void tryRehash();

    void reHash(size_t iterateFor);

    int getKeyIndex(const T &key) const;

    std::vector<T> _table;
    std::vector<CellStatus> _cellsStatus;
    const H &_hasher;
    const H2 &_probeHasher;
    size_t _keyCnt;
    size_t _delCnt;
};

template<class T, class H, class H2>
HashTable<T, H, H2>::HashTable(const H &hasher, const H2 &probeHasher, size_t initSize) : _table(initSize),
                                                                                          _cellsStatus(initSize,
                                                                                                       FREE),
                                                                                          _hasher(hasher),
                                                                                          _probeHasher(probeHasher),
                                                                                          _keyCnt(0), _delCnt(0) {
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::add(const T &key) {
    // Вычисляем и сохраняем хэши.
    auto hashFirst = _hasher(key) % _table.size();
    auto hashSecond = (2 * _probeHasher(key) + 1) % _table.size();
    // Проверяем, можем ли вставлять и ищем первую del ячейку:
    int i = 0, index, delCell = -1;
    do {
        index = getProbeDoubleHash(hashFirst, hashSecond, i);
        if (_cellsStatus[index] == DELETED && delCell == -1) { delCell = index; }
        ++i;
    } while (_cellsStatus[index] != FREE && (_cellsStatus[index] == DELETED || _table[index] != key) &&
             i < _table.size());
    // Если был найден дубликат или не найдено ни одной free/dell ячейки:
    if (_table[index] == key || (_cellsStatus[index] != FREE && delCell < 0)) {
        return false;
    }
    index = delCell > 0 ? delCell : index;
    _table[index] = key;
    _cellsStatus[index] = NOT_FREE;
    _keyCnt++;
    tryRehash();
    return true;
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::has(const T &key) const {
    return getKeyIndex(key) >= 0;
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::remove(const T &key) {
    int index = getKeyIndex(key);
    if (index < 0) { return false; }
    _cellsStatus[index] = DELETED;
    _delCnt++;
    _keyCnt--;
    return true;
}

template<class T, class H, class H2>
int HashTable<T, H, H2>::getKeyIndex(const T &key) const {
    auto hash_first = _hasher(key) % _table.size();
    auto hash_second = (2 * _probeHasher(key) + 1) % _table.size();
    int i = 0, index;
    do {
        index = getProbeDoubleHash(hash_first, hash_second, i);
        ++i;
    } while (_cellsStatus[index] != FREE && (_cellsStatus[index] == DELETED || _table[index] != key) &&
             i < _table.size());
    return _table[index] == key ? index : -1;
}

template<class T, class H, class H2>
size_t HashTable<T, H, H2>::getProbeDoubleHash(size_t hash, size_t doubleHash, size_t i) const {
    return (hash + doubleHash * i) % _table.size();
}

template<class T, class H, class H2>
void HashTable<T, H, H2>::tryRehash() {
    if (_keyCnt * 4 > _table.size() * 3) {
        _table.resize(_table.size() * 2);
        _cellsStatus.resize(_cellsStatus.size() * 2);
        reHash(_cellsStatus.size() / 2);
        _delCnt = 0;
    } else if (2 * _delCnt > _table.size()) {
        reHash(_cellsStatus.size());
        _delCnt = 0;
    }
}

template<class T, class H, class H2>
void HashTable<T, H, H2>::reHash(size_t iterateFor) {
    // Подчищаем таблицу статутов, отмечаем "старые" позиции как "REHASH_IT"
    for (auto &cell : _cellsStatus) { cell = cell == NOT_FREE ? REHASH_IT : FREE; }
    for (int i = 0; i < iterateFor; ++i) {
        if (_cellsStatus[i] != REHASH_IT) { continue; }

        auto hashFirst = _hasher(_table[i]) % _table.size();
        auto hashSecond = (2 * _probeHasher(_table[i]) + 1) % _table.size();
        int index = getProbeDoubleHash(hashFirst, hashSecond, 0);
        switch (_cellsStatus[index]) {
            case DELETED:
            case FREE:
                _table[index] = _table[i];
                _cellsStatus[index] = NOT_FREE;
                _cellsStatus[i] = FREE;
                break;
            case REHASH_IT:
                std::swap(_table[i], _table[index]);
                _cellsStatus[index] = NOT_FREE;
                --i; // Должны перехешировать свапнутый элемент..
                break;
            case NOT_FREE: // Коллизия
                for (int j = 1; _cellsStatus[index] != FREE; ++j) {
                    index = getProbeDoubleHash(hashFirst, hashSecond, j);
                }
                _table[index] = _table[i];
                _cellsStatus[index] = NOT_FREE;
                break;
        }
    }
}

void testCase(std::istream &in, std::ostream &out) {
    char operation = 0;
    std::string data;
    HashTable<std::string, Hasher, HasherOther> hashTable;
    while (in >> operation >> data) {
        switch (operation) {
            case '+':
                out << (hashTable.add(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                out << (hashTable.remove(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                out << (hashTable.has(data) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
                break;
        }
    }
}

void testHashMap() {
    {
        std::stringstream in, out;
        in << "+ hello\n? hello\n- hello\n? hello\n+ hello\n+ hello\n";
        testCase(in, out);
        assert(out.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\n");
        std::cout << "OK - 1" << std::endl;
    }
    {
        std::stringstream in, out;
        in
            << "+ qwert\n+ yuiop\n+ asdf\n+ ghjkl\n+ zxcvb\n+ mnbvcx\n+ zxcmnbx\n+ azsxcd\n+ vfgdcs\n+ vfiadgwsfss\n"; // 10
        in
            << "? qwert\n? yuiop\n? asdf\n? ghjkl\n? zxcvb\n? mnbvcx\n? zxcmnbx\n? azsxcd\n? vfgdcs\n? vfiadgwsfss\n"; // 10
        in
            << "- qwert\n- yuiop\n- asdf\n- ghjkl\n- zxcvb\n- mnbvcx\n- zxcmnbx\n- azsxcd\n- vfgdcs\n- vfiadgwsfss\n"; // 10
        in
            << "? qwert\n? yuiop\n? asdf\n? ghjkl\n? zxcvb\n? mnbvcx\n? zxcmnbx\n? azsxcd\n? vfgdcs\n? vfiadgwsfss\n"; // 10
        testCase(in, out);
        assert(out.str() ==
               "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n");
        std::cout << "OK - 2" << std::endl;
    }
};

void testHashMapIO() {
    testCase(std::cin, std::cout);
};

int main() {
    testHashMap();
    return 0;
}
