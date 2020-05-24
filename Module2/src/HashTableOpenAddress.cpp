#include <iostream>
#include <vector>

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




template<class T, class H, class H2>
class HashTable {
public:
    explicit HashTable(size_t initSize, const H &hasher = H(), const H2 &probeHasher = H2());

    HashTable(const HashTable &table) = delete;

    HashTable &operator=(const HashTable &table) = delete;

    ~HashTable();

    bool has(const T &key) const;

    bool add(const T &key);

    bool remove(const T &key);

private:
    enum CellStatus {
        FREE,
        NOT_FREE,
        DELETED
    };

    size_t getProbeDoubleHash(size_t hash, size_t doubleHash, size_t i) const;

    void tryRehash();

    int getIndexOfKey(const T &key) const;

    std::vector<T> _table;
    std::vector<CellStatus> _cellsStatus;
    const H &_hasher;
    const H &_probeHasher;
    size_t _keyCnt;
    size_t _delCnt;
};

template<class T, class H, class H2>
HashTable<T, H, H2>::HashTable(size_t initSize, const H &hasher, const H2 &probeHasher) : _table(initSize),
                                                                                          _cellsStatus(initSize,
                                                                                                       CellStatus::FREE),
                                                                                          _hasher(hasher),
                                                                                          _probeHasher(probeHasher),
                                                                                          _keyCnt(0), _delCnt(0) {
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::add(const T &key) {
    // Вычисляем и сохраняем хэши.
    auto hash_first = _hasher(key) % _table.size();
    auto hash_second = (2 * _probeHasher(key) + 1) % _table.size();
    // Проверяем, есть ли элемент в таблице и ищем первую del ячейку:
    int i = 0, index = 0, delCell = -1;
    while (i < _table.size() || (_cellsStatus[index] == DELETED || _table[index] != key) ||
           _cellsStatus[index] != FREE) {
        index = getProbeDoubleHash(hash_first, hash_second, i);
        if (_cellsStatus[index] == DELETED && delCell == -1) { delCell = index; }
        ++i;
    }
    // Если был найден дубликат или не найдено ни одной free/dell ячейки:
    if (_table[index] == key || (_cellsStatus[index] == NOT_FREE && delCell < 0)) {
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
    return getIndexOfKey(key) >= 0;
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::remove(const T &key) {
    int index = getIndexOfKey(key);
    if (index >= 0) {
        _cellsStatus[index] = DELETED;
        _delCnt++;
        _keyCnt--;
        return true;
    }
    return false;
}

template<class T, class H, class H2>
int HashTable<T, H, H2>::getIndexOfKey(const T &key) const {
    auto hash_first = _hasher(key) % _table.size();
    auto hash_second = (2 * _probeHasher(key) + 1) % _table.size();
    int i = 0, index = 0;
    while (i < _table.size() && (_cellsStatus[index] == DELETED || _table[index] != key) &&
           _cellsStatus[index] != FREE) {
        index = getProbeDoubleHash(hash_first, hash_second, i);
        ++i;
    }
    return _table[index] == key ? index : -1;
}

template<class T, class H, class H2>
size_t HashTable<T, H, H2>::getProbeDoubleHash(size_t hash, size_t doubleHash, size_t i) const {
    return hash + doubleHash * i;
}

template<class T, class H, class H2>
void HashTable<T, H, H2>::tryRehash() {
    if (_keyCnt / _table.size() > 0.75) {

    }
    if (_delCnt > 0.5 * _table.size()) {
    }
}

int main() {
    return 0;
}
