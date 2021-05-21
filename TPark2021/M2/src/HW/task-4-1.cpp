#include <iostream>
#include <sstream>
#include <cassert>
#include <random>
#include <algorithm>
#include <stack>

/**
 * В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая,
 * то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо
 * начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по
 * росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
 * который заметил интересную особенность – все солдаты в части разного роста.
 *
 * Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого
 * приходящего солдата указывать, перед каким солдатом в строе он должен становится.
 * Требуемая скорость выполнения команды - O(log n).
 *
 * Формат входных данных:
 * Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000). В каждой следующей строке
 * содержится описание команды: число 1 и X если солдат приходит в строй (X – рост солдата,
 * натуральное число до 100 000 включительно) и число 2 и Y если солдата, стоящим в строе на месте Y надо
 * удалить из строя. Солдаты в строе нумеруются с нуля.
 *
 * Формат вывода:
 * На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен
 * встать этот солдат (все стоящие за ним двигаются назад).
 */

const size_t RANDOM_SEED = 777;
const size_t TEST_DATA_SIZE = 1000;

template<class T>
struct Compare {
    enum Comparison {
        LESS,
        EQUAL,
        MORE
    };

    Comparison operator()(const T &lhs, const T &rhs) {
        if (lhs < rhs) { return LESS; }
        if (lhs > rhs) { return MORE; }
        return EQUAL;
    };
};

template<class K, class D, class C = Compare<K>>
class AVLTree {
protected:
    struct Node {
        K key;
        D data;
        Node *left;
        Node *right;
        char height;
        size_t elementCount;

        Node(const K &k, const D &d) : key(k), data(d), left(nullptr), right(nullptr), height(1), elementCount(1) {}

        ~Node() {
            delete left;
            delete right;
        }

        Node(const Node &rhs) = delete;

        Node &operator=(const Node &rhs) = delete;

        Node(Node &&rhs) = delete;

        Node &operator=(Node &&rhs) = delete;
    };

public:
    explicit AVLTree(const C &compare = C());

    ~AVLTree() { delete _root; }

    AVLTree(const Node &rhs) = delete;

    AVLTree &operator=(const AVLTree &rhs) = delete;

    AVLTree(AVLTree &&rhs) = delete;

    AVLTree &operator=(AVLTree &&rhs) = delete;

    D *find(const K &key);

    void insert(const K &key, const D &data);

    void erase(const K &key);

    size_t size();

    bool isEmpty();

    void traverseInOrder(const std::function<void(const K &)> &callback);

protected:
    Node *findAUX(const K &key, Node *node);

    Node *insertAUX(const K &key, const D &data, Node *node);

    Node *eraseAUX(const K &key, Node *node);

    Node *deleteAUX(Node *node);

    Node *balance(Node *node);

    Node *findAndRemoveMin(Node *node, Node **minNode);

    void fixHeight(Node *node);

    void fixElementCount(Node *node);

    int balanceFactor(Node *node);

    char height(Node *node);

    Node *rotateLeft(Node *node);

    Node *rotateRight(Node *node);

    C _compare;
    Node *_root;
    size_t _itemCount;
};

template<class K, class D, class C>
size_t AVLTree<K, D, C>::size() {
    return _itemCount;
}

template<class K, class D, class C>
bool AVLTree<K, D, C>::isEmpty() {
    return !_itemCount;
}

template<class K, class D, class C>
D *AVLTree<K, D, C>::find(const K &key) {
    auto node = findAUX(key, _root);
    return node ? &node->data : nullptr;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::findAUX(const K &key, AVLTree::Node *node) {
    if (!node) { return nullptr; }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            return findAUX(key, node->left);
        case C::EQUAL:
            return node;
        case C::MORE:
            return findAUX(key, node->right);
        default:
            assert(false);
    }
}

template<class K, class D, class C>
AVLTree<K, D, C>::AVLTree(const C &compare) : _compare(compare), _root(nullptr), _itemCount(0) {}

template<class K, class D, class C>
void AVLTree<K, D, C>::insert(const K &key, const D &data) {
    _root = insertAUX(key, data, _root);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::insertAUX(const K &key, const D &data, AVLTree::Node *node) {
    if (!node) {
        _itemCount++;
        return new Node(key, data);
    }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            node->left = insertAUX(key, data, node->left);
            break;
        case C::EQUAL:
            break;
        case C::MORE:
            node->right = insertAUX(key, data, node->right);
            break;
    }
    return balance(node);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::eraseAUX(const K &key, AVLTree::Node *node) {
    if (!node) { return nullptr; }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            node->left = eraseAUX(key, node->left);
            break;
        case C::EQUAL: {
            return deleteAUX(node);
        }
        case C::MORE:
            node->right = eraseAUX(key, node->right);
            break;
    }
    return balance(node);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::findAndRemoveMin(AVLTree::Node *node, AVLTree::Node **minNode) {
    if (!node->left) {
        *minNode = node;
        return node->right;
    }
    node->left = findAndRemoveMin(node->left, minNode);
    return balance(node);
}

template<class K, class D, class C>
int AVLTree<K, D, C>::balanceFactor(Node *node) {
    return height(node->right) - height(node->left);
}

template<class K, class D, class C>
char AVLTree<K, D, C>::height(AVLTree::Node *node) {
    if (!node) { return 0; }
    return node->height;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::fixHeight(AVLTree::Node *node) {
    if (!node) { return; }
    node->height = std::max(node->left ? node->left->height : 0, node->right ? node->right->height : 0) + 1;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::erase(const K &key) {
    _root = eraseAUX(key, _root);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::balance(AVLTree::Node *node) {
    fixHeight(node);
    fixElementCount(node);
    auto balance = balanceFactor(node);
    if (balance == 2) {
        if (balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    } else if (balance == -2) {
        if (balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::rotateLeft(AVLTree::Node *node) {
    auto right = node->right;
    node->right = right->left;
    right->left = node;
    fixHeight(node);
    fixHeight(right);
    fixElementCount(node);
    fixElementCount(right);
    return right;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::rotateRight(AVLTree::Node *node) {
    auto left = node->left;
    node->left = left->right;
    left->right = node;
    fixHeight(node);
    fixHeight(left);
    fixElementCount(node);
    fixElementCount(left);
    return left;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::fixElementCount(AVLTree::Node *node) {
    if (!node) { return; }
    node->elementCount = ((node->left ? node->left->elementCount : 0)
                       + (node->right ? node->right->elementCount : 0) + 1);
}

template<class K, class D, class C>
void AVLTree<K, D, C>::traverseInOrder(const std::function<void(const K &)> &callback) {
    if (!_root) { return; }
    std::stack<Node *> stack;
    stack.push(_root);
    auto current = _root;
    while (current->left) {
        current = current->left;
        stack.push(current);
    }
    while (!stack.empty()) {
        current = stack.top();
        callback(current->key);
        stack.pop();

        if (!current->right) { continue; }
        current = current->right;
        stack.push(current);
        while (current->left) {
            current = current->left;
            stack.push(current);
        }
    }
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::deleteAUX(Node *node) {
    _itemCount--;
    auto left = node->left;
    auto right = node->right;
    node->left = nullptr;
    node->right = nullptr;
    delete node;

    if (!right) { return left; }
    Node *minNode = nullptr;
    auto treeWithoutMinNode = findAndRemoveMin(right, &minNode);
    minNode->right = treeWithoutMinNode;
    minNode->left = left;
    return balance(minNode);
}


template<class K, class D, class C = Compare<K>>
class SolderAVLTree : public AVLTree<K, D, C> {
public:
    using typename AVLTree<K, D, C>::Node;

    explicit SolderAVLTree(const C &compare = C());

    void eraseAtPosition(size_t position);

    size_t insertAndGetPos(const K &key, const D &data);

private:
    Node *insertWithPositionAUX(const K &key, const D &data, Node *node, size_t &position);
    Node *eraseAtPositionAUX(Node *node, size_t reminder);

    size_t elementCountWithoutRightTree(Node *node);

    using AVLTree<K, D, C>::eraseAUX;
    using AVLTree<K, D, C>::insertAUX;
    using AVLTree<K, D, C>::balance;
    using AVLTree<K, D, C>::deleteAUX;
    using AVLTree<K, D, C>::_root;
    using AVLTree<K, D, C>::_compare;
    using AVLTree<K, D, C>::_itemCount;
};


template<class K, class D, class C>
typename SolderAVLTree<K, D, C>::Node *
SolderAVLTree<K, D, C>::insertWithPositionAUX(const K &key, const D &data, Node *node, size_t &position) {
    if (!node) {
        _itemCount++;
        return new Node(key, data);
    }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            node->left = insertWithPositionAUX(key, data, node->left, position);
            break;
        case C::EQUAL:
            break;
        case C::MORE:
            position += elementCountWithoutRightTree(node);
            node->right = insertWithPositionAUX(key, data, node->right, position);
            break;
    }
    return balance(node);
}

template<class K, class D, class C>
size_t SolderAVLTree<K, D, C>::insertAndGetPos(const K &key, const D &data) {
    size_t pos = 0;
    _root = insertWithPositionAUX(key, data, _root, pos);
    return _itemCount - pos - 1;
}

template<class K, class D, class C>
SolderAVLTree<K, D, C>::SolderAVLTree(const C &compare) : AVLTree<K, D, C>(compare) {
}

template<class K, class D, class C>
void SolderAVLTree<K, D, C>::eraseAtPosition(size_t position) {
    assert(position <= _itemCount);
    position = _itemCount - position - 1;
    _root = eraseAtPositionAUX(_root, position + 1);
}

template<class K, class D, class C>
size_t SolderAVLTree<K, D, C>::elementCountWithoutRightTree(Node *node) {
    if (!node->left) { return 1; }
    return node->left->elementCount + 1;
}

template<class K, class D, class C>
typename SolderAVLTree<K, D, C>::Node *
SolderAVLTree<K, D, C>::eraseAtPositionAUX(Node *node, size_t reminder) {
    if (!node) { return nullptr; }
    auto currentNodePos = elementCountWithoutRightTree(node);
    if (currentNodePos < reminder) {
        reminder -= currentNodePos;
        node->right = eraseAtPositionAUX(node->right, reminder);
    } else if(currentNodePos > reminder) {
        node->left = eraseAtPositionAUX(node->left, reminder);
    } else {
        return deleteAUX(node);
    }
    return balance(node);
}

void testAVLTree() {
    std::vector<int> testData(TEST_DATA_SIZE);
    std::iota(testData.begin(), testData.end(), 0);
    std::shuffle(testData.begin(), testData.end(), std::mt19937(RANDOM_SEED));

    AVLTree<int, int> tree;
    for (int &data : testData) {
        tree.insert(data, data);
    }
    for (size_t i = 0; i < (testData.size() / 2); ++i) {
        tree.insert(testData[i], testData[i]);
    }

    assert(tree.size() == testData.size() && "Дубликаты ключей не должны добавляться");
    int prevKey = -1;
    tree.traverseInOrder([&prevKey](auto key) {
        if (prevKey != -1) { assert(prevKey < key && "Ключи должны быть упорядочены и уникальны"); }
        prevKey = key;
    });

    int *found = nullptr;
    for (auto key : testData) {
        found = tree.find(key);
        assert(*found == key && "Не найден ключ");
        tree.erase(key);
        assert(!tree.find(key) && "Ключ не удалился");
    }
    assert(tree.isEmpty() && "Должны были быть удалены все элементы");
}

void testSolderAVLTree() {
    std::vector<int> testData(TEST_DATA_SIZE);
    std::iota(testData.begin(), testData.end(), 0);
    std::shuffle(testData.begin(), testData.end(), std::mt19937(RANDOM_SEED));

    SolderAVLTree<int, int> tree;
    size_t insertedPos = 0;
    size_t traversePos = 0;
    for (auto data : testData) {
        insertedPos = tree.insertAndGetPos(data, data);
        traversePos = 0;
        tree.traverseInOrder([&](auto key) {
            if (key == data) {
                assert(traversePos == insertedPos && "Не правильный порядковый номер добавленного элемента");
            }
            traversePos++;
        });
    }

    std::vector<int> remainKeys(testData.size());
    std::iota(remainKeys.begin(), remainKeys.end(), 0);
    std::mt19937 gen(RANDOM_SEED + 4);
    int removedKey = -1;
    size_t indexToRemove = 0;
    /* Удаляем элемент по рандомному индексу и проверяем удалился ли он. */
    for (int maxIndex = remainKeys.size() - 1; maxIndex >= 0; maxIndex--) {
        std::uniform_int_distribution<size_t> distribution(0, maxIndex);
        indexToRemove = distribution(gen);
        removedKey = remainKeys[indexToRemove];
        remainKeys.erase(remainKeys.begin() + indexToRemove);

        tree.eraseAtPosition(indexToRemove);
        tree.traverseInOrder([&](auto key) {
            assert(key != removedKey && "Ключ должен был быть удален");
        });
    }
    assert(tree.isEmpty() && "Дерево должно опустеть");
}

void testCase(std::istream &in, std::ostream &out) {
    size_t commandCount = 0;
    in >> commandCount;

    SolderAVLTree<size_t, char> tree;
    int command = 0;
    size_t heightOrIndex = 0;
    for (size_t i = 0; i < commandCount; ++i) {
        in >> command >> heightOrIndex;
        switch (command) {
            case 1:
                out <<tree.insertAndGetPos(heightOrIndex, 0) << std::endl;
                break;
            case 2:
                tree.eraseAtPosition(heightOrIndex);
                break;
            default:
                assert(false && "Не известная команда");
        }
    }
}

void test() {
    {
        std::stringstream in, out;
        in << "5\n1 100\n1 200\n1 50\n2 1\n1 150";
        testCase(in, out);
        std::cout << "result:" << std::endl <<  out.str() << std::endl;
        assert(out.str() == "0\n0\n2\n1\n");
    }
    {
        std::stringstream in, out;
        in << "8\n1 100\n1 200\n1 50\n2 1\n1 150\n1 400\n2 0\n1 300";
        testCase(in, out);
        std::cout << "result:" << std::endl <<  out.str() << std::endl;
        assert(out.str() == "0\n0\n2\n1\n");
    }
}

int main() {
    /**
     * testAVLTree();
     * testSolderAVLTree();
     * test();
     * */
    testCase(std::cin, std::cout);
    return 0;
}
