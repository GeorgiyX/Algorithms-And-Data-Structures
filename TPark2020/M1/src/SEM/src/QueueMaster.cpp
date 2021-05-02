#include <assert.h>
#include <iostream>
#include <sstream>

using std::cin;

template<class T>
class Queue {
public:
    Queue();
    Queue( const Queue& q) = delete;
    Queue& operator=( const Queue& q) = delete;
    ~Queue();

    // Положить элемент в очередь
    void Enqueue( const T& data );
    // Извлечь элемент из очереди
    T Dequeue();
    // Проверить на пустоту
    bool IsEmpty() const { return tail == nullptr; }

private:
    struct Node {
        T Data;
        Node* Next;
        Node() : Next( nullptr ) {}
    };
    Node* head;
    Node* tail;


};

template<class T>
Queue<T>::Queue() :
        head( nullptr ),
        tail( nullptr )
{
    head = nullptr;
}

template<class T>
Queue<T>::~Queue()
{
    while( !IsEmpty() )
        Dequeue();
}

template<class T>
void Queue<T>::Enqueue( const T& data )
{
    Node* newNode = new Node();
    newNode->Data = data;

    if( IsEmpty() ) {
        head = newNode;
    } else {
        tail->Next = newNode;
    }
    tail = newNode;
}

template<class T>
T Queue<T>::Dequeue()
{
    assert( !IsEmpty() );

    Node* tmpNode = head;
    T tmpData = head->Data;
    head = head->Next;
    if( head == nullptr )
        tail = nullptr;
    delete tmpNode;
    return tmpData;
}

void run( std::istream& input, std::ostream& output )
{
    Queue<int> q;
    int n = 0;
    input >> n;
    bool result = true;
    for( int i = 0; i < n && result; i++ ) {
        int command = 0;
        int data = 0;
        input >> command >> data;
        switch( command ) {
            case 2:
                if( q.IsEmpty() )
                    result = result && data == -1;
                else
                    result = result && data == q.Dequeue();
                break;
            case 3:
                q.Enqueue( data );
                break;
            default:
                assert( false );
        }
    }

    output << (result ? "YES" : "NO");
}

void testQueue()
{
    {
        Queue<bool> q;
        assert( q.IsEmpty() );
        for( int i = 0; i < 10; i++ )
            q.Enqueue( i % 2 == 0 );
        assert( !q.IsEmpty() );
        for( int i = 0; i < 10; i++ )
            assert( q.Dequeue() == (i % 2 == 0) );
    }
    { // 1й тест из условия
        std::stringstream input;
        std::stringstream output;
        input << "3 3 44 3 50 2 44";
        run( input, output );
        assert( output.str() == "YES" );
    }
    { // 3й тест из условия
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 2 66";
        run( input, output );
        assert( output.str() == "NO" );
    }
}

int main()
{
//	run( std::cin, std::cout );
    testQueue();
    return 0;
}