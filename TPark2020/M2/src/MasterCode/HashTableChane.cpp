#include <iostream>
#include <vector>
#include <assert.h>
#include <string>

using std::vector;
using std::cin;
using std::cout;
using std::string;

// Элемент цепочки в хеш-таблице.
template<class T>
struct HashTableNode {
    T Key;
    HashTableNode<T>* Next;

    HashTableNode() : Next( nullptr ) {}
    HashTableNode( const T& key, HashTableNode* next ) : Key( key ), Next( next ) {}
};

// Хеш-таблица.
template<class T, class H>
class HashTable {
public:
    HashTable( int initialSize = 8, const H& hasher = H() );
    HashTable( const HashTable& ) = delete;
    HashTable& operator = ( const HashTable* ) = delete;
    ~HashTable();
    // 1. делаете максимум table.size() проб, если сделали - элемента нет
    // 2. при пробировании
    //		Del - идёте дальше
    //		пусто - элемента нет
    //		key - или ваш (return true) или идёте дальше
    bool Has( const T& key ) const;
    // 1. делаете максимум table.size() проб, если сделали - assert( встречали del ) и пишем в del
    // 2. при пробировании
    //		Del - запоминаем позицию первой встреченной del-ячейки и идёте дальше
    //		пусто - если ранее встретили del, пишем в del, иначе пишем в пусто
    //		key - или ваш (return false) или идёте дальше
    bool Add( const T& key );
    // 1. делаете максимум table.size() проб, если сделали - элемента нет
    // 2. при пробировании
    //		Del - идёте дальше
    //		пусто - элемента нет
    //		key - или ваш (заменяете на Del и return true) или идёте дальше
    bool Delete( const T& key );

private:
    H hasher;
    vector<HashTableNode<T>*> table;
    unsigned int keysCount;

    void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable( int initialSize, const H& _hasher ) :
    hasher( _hasher ),
    table( initialSize, nullptr ),
    keysCount(0)
{
}

template<class T, class H>
HashTable<T, H>::~HashTable()
{
    for( unsigned int i = 0; i < table.size(); i++ ) {
        HashTableNode<T>* node = table[i];
        while( node != nullptr ) {
            HashTableNode<T>* nextNode = node->Next;
            delete node;
            node = nextNode;
        }
    }
}

template<class T, class H>
bool HashTable<T, H>::Has( const T& key ) const
{
    unsigned int hash = hasher( key ) % table.size();
    HashTableNode<T>* node = table[hash];
    while( node != nullptr && node->Key != key )
        node = node->Next;

    return node != nullptr;
}

template<class T, class H>
bool HashTable<T, H>::Add( const T& key )
{
    if( keysCount + 1 > 3 * table.size() )
        growTable();

    unsigned int hash = hasher( key ) % table.size();
    HashTableNode<T>* node = table[hash];
    while( node != nullptr && node->Key != key )
        node = node->Next;

    if( node != nullptr )
        return false;

    table[hash] = new HashTableNode<T>( key, table[hash] );
    keysCount++;
    return true;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
    vector<HashTableNode<T>*> newTable( table.size() * 2, nullptr );
    for( unsigned int i = 0; i < table.size(); i++ ) {
        HashTableNode<T>* node = table[i];
        while( node != nullptr ) {
            HashTableNode<T>* nextNode = node->Next;
            int newHash = hasher( node->Key ) % newTable.size();
            node->Next = newTable[newHash];
            newTable[newHash] = node;
            node = nextNode;
        }
    }
    table = newTable;
}

template<class T, class H>
bool HashTable<T, H>::Delete( const T& key )
{
    unsigned int hash = hasher( key ) % table.size();

    HashTableNode<T>* prevNode = nullptr;
    HashTableNode<T>* node = table[hash];
    while( node != nullptr && node->Key != key ) {
        prevNode = node;
        node = node->Next;
    }

    if( node == nullptr )
        return false;

    if( prevNode == nullptr )
        table[hash] = node->Next;
    else
        prevNode->Next = node->Next;

    delete node;
    keysCount--;
    return true;
}

struct StringHasher {
    unsigned int operator()( const string& key ) const {
        unsigned int hash = 0;
        for( int i = 0; i < key.size(); i++ ) {
            hash = hash * 137 + key[i] ;
        }
        return hash;
    }
};

int main()
{
    HashTable<string, StringHasher> table;
    char operation = 0;
    string word;

    while( cin >> operation >> word ) {
        if( operation == '+' )
            cout << (table.Add( word ) ? "OK" : "FAIL") << "\n";
        else if( operation == '-' )
            cout << (table.Delete( word ) ? "OK" : "FAIL") << "\n";
        else if( operation == '?' )
            cout << (table.Has( word ) ? "OK" : "FAIL") << "\n";
        else
            assert( false );
    }
    return 0;
}