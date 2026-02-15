#ifndef TRAVEL_APP_HASHTABLE_H
#define TRAVEL_APP_HASHTABLE_H
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <utility>
#include <functional>
#include <vector>
#include <type_traits>

template <typename K, typename V>
class HashTable {
private:
    struct VersionNode {
        V data;
        VersionNode* nextV;
        VersionNode(const V& val) : data(val), nextV(nullptr) {}
        VersionNode(V&& val) : data(std::move(val)), nextV(nullptr) {}

        ~VersionNode() {
            if (nextV) delete nextV;
        }
    };

    struct Node {
        std::pair<const K, V> kv;
        VersionNode* versionsHead;
        Node* nextInBucket;
        Node* nextGlobal;
        Node* prevGlobal;

        Node(const K& newKey, const V& newVer)
            : kv(std::piecewise_construct,
                 std::forward_as_tuple(newKey),
                 std::forward_as_tuple(newVer)),
              versionsHead(nullptr),
              nextInBucket(nullptr), nextGlobal(nullptr), prevGlobal(nullptr) {
            versionsHead = new VersionNode(std::move(newVer));
            const_cast<V&>(kv.second) = std::move(versionsHead->data);
        }

        Node(const K& newKey, V&& newVer)
    : kv(std::piecewise_construct, std::forward_as_tuple(newKey), std::forward_as_tuple()),
      versionsHead(nullptr), nextInBucket(nullptr), nextGlobal(nullptr), prevGlobal(nullptr)
        {
            versionsHead = new VersionNode(std::move(newVer));
            std::swap(const_cast<V&>(kv.second), versionsHead->data);
        }

        ~Node() {
            if (versionsHead) delete versionsHead;
        }

        void addVersion(const V& val) {
            VersionNode* newNode = new VersionNode(val);
            newNode->nextV = versionsHead;
            versionsHead = newNode;
            const_cast<V&>(kv.second) = val;
        }

        void addVersion(V&& val) {
            VersionNode* newNode = new VersionNode(std::move(val));
            newNode->nextV = versionsHead;
            versionsHead = newNode;
            std::swap(const_cast<V&>(kv.second), newNode->data);
        }

    };

    size_t cap;
    size_t tableSize;
    Node** table;
    Node* head;
    Node* tail;

public:
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    class ConstIterator;
    class Iterator {
    public:
        Node* curr;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const K, V>;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = std::ptrdiff_t;

        Iterator(Node* node = nullptr) : curr(node) {}
        Iterator(const ConstIterator& other) : curr(other.curr) {}

        Iterator& operator++() {
            if (curr) curr = curr->nextGlobal;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--() {
            if (curr) curr = curr->prevGlobal;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        reference operator*() const {
            return curr->kv;
        }

        pointer operator->() const {
            return &(curr->kv);
        }

        bool operator==(const Iterator& other) const {
            return curr == other.curr;
        }

        bool operator!=(const Iterator& other) const {
            return curr != other.curr;
        }

        bool operator==(const ConstIterator& other) const {
            return curr == other.curr;
        }

        bool operator!=(const ConstIterator& other) const {
            return curr != other.curr;
        }
    };

    class ConstIterator {
    public:
        const Node* curr;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const K, V>;
        using pointer = const value_type*;
        using reference = const value_type;
        using difference_type = std::ptrdiff_t;

        ConstIterator(const Node* node = nullptr) : curr(node) {}
        ConstIterator(const Iterator& it) : curr(it.curr) {}

        ConstIterator& operator++() {
            if (curr) curr = curr->nextGlobal;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        ConstIterator& operator--() {
            if (curr) curr = curr->prevGlobal;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator tmp = *this;
            --(*this);
            return tmp;
        }

        reference operator*() const {
            return curr->kv;
        }

        pointer operator->() const {
            return &(curr->kv);
        }

        bool operator==(const ConstIterator& other) const {
            return curr == other.curr;
        }

        bool operator!=(const ConstIterator& other) const {
            return curr != other.curr;
        }

        bool operator==(const Iterator& other) const {
            return curr == other.curr;
        }

        bool operator!=(const Iterator& other) const {
            return curr != other.curr;
        }
    };

private:
    size_t getIndex(const K& key) const {
        return std::hash<K>{}(key) % cap;
    }

    Node* findNode(const K& key) const {
        size_t idx = getIndex(key);
        Node* curr = table[idx];
        while (curr != nullptr) {
            if (curr->kv.first == key) {
                return curr;
            }
            curr = curr->nextInBucket;
        }
        return nullptr;
    }

    void insertIntoGlobalChain(Node* newNode) {
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->nextGlobal = newNode;
            newNode->prevGlobal = tail;
            tail = newNode;
        }
    }

    void removeFromGlobalChain(Node* node) {
        if (node->prevGlobal) node->prevGlobal->nextGlobal = node->nextGlobal;
        if (node->nextGlobal) node->nextGlobal->prevGlobal = node->prevGlobal;
        if (node == head) head = node->nextGlobal;
        if (node == tail) tail = node->prevGlobal;

        node->nextGlobal = nullptr;
        node->prevGlobal = nullptr;
    }

    void rehash(size_t newCap) {
        Node** newTable = new Node*[newCap]();

        std::vector<Node*> nodes;
        Node* curr = head;
        while (curr != nullptr) {
            nodes.push_back(curr);
            curr = curr->nextGlobal;
        }

        for (Node* node : nodes) {
            node->nextInBucket = nullptr;
        }

        for (size_t i = 0; i < cap; ++i) {
            table[i] = nullptr;
        }

        for (Node* node : nodes) {
            size_t newIdx = std::hash<K>{}(node->kv.first) % newCap;

            node->nextInBucket = newTable[newIdx];
            newTable[newIdx] = node;
        }

        delete[] table;
        table = newTable;
        cap = newCap;
    }

    void construct(size_t initCap) {
        cap = (initCap == 0) ? 1 : initCap;
        table = new Node*[cap]();
        tableSize = 0;
        head = nullptr;
        tail = nullptr;
    }

public:
    HashTable() {
        construct(16);
    }

    explicit HashTable(size_t initCap) {
        construct(initCap);
    }

    HashTable(const HashTable& other) {
        construct(other.cap);

        Node* curr = other.head;
        while (curr != nullptr) {
            if (curr->versionsHead != nullptr) {
                insert(curr->kv.first, curr->versionsHead->data);
                VersionNode* vCurr = curr->versionsHead->nextV;
                while (vCurr != nullptr) {
                    Node* node = findNode(curr->kv.first);
                    if (node) {
                        node->addVersion(vCurr->data);
                    }
                    vCurr = vCurr->nextV;
                }
            }
            curr = curr->nextGlobal;
        }
    }

    HashTable(HashTable&& other) noexcept
        : cap(other.cap), tableSize(other.tableSize), table(other.table),
          head(other.head), tail(other.tail) {
        other.cap = 0;
        other.tableSize = 0;
        other.table = nullptr;
        other.head = nullptr;
        other.tail = nullptr;
    }

    HashTable(std::initializer_list<std::pair<K, V>> list) {
        construct(list.size() > 16 ? list.size() : 16);
        for (auto& item : list) {
            insert(item.first, std::move(item.second));
        }
    }

    ~HashTable() {
        clear();
        delete[] table;
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            HashTable temp(other);
            swap(temp);
        }
        return *this;
    }

    HashTable& operator=(HashTable&& other) noexcept {
        if (this != &other) {
            clear();
            delete[] table;

            cap = other.cap;
            tableSize = other.tableSize;
            table = other.table;
            head = other.head;
            tail = other.tail;

            other.cap = 0;
            other.tableSize = 0;
            other.table = nullptr;
            other.head = nullptr;
            other.tail = nullptr;
        }
        return *this;
    }

    void swap(HashTable& other) noexcept {
        std::swap(cap, other.cap);
        std::swap(tableSize, other.tableSize);
        std::swap(table, other.table);
        std::swap(head, other.head);
        std::swap(tail, other.tail);
    }

    size_t size() const {
        return tableSize;
    }

    bool empty() const {
        return tableSize == 0;
    }

    void clear() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* nextGlobal = curr->nextGlobal;
            delete curr;
            curr = nextGlobal;
        }

        for (size_t i = 0; i < cap; ++i) {
            table[i] = nullptr;
        }

        head = tail = nullptr;
        tableSize = 0;
    }

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    ConstIterator begin() const {
        return ConstIterator(head);
    }

    ConstIterator end() const {
        return ConstIterator(nullptr);
    }

    ConstIterator cbegin() const {
        return ConstIterator(head);
    }

    ConstIterator cend() const {
        return ConstIterator(nullptr);
    }

    Iterator find(const K& key) {
        return Iterator(findNode(key));
    }

    ConstIterator find(const K& key) const {
        return ConstIterator(findNode(key));
    }

    template <typename... Args>
    std::pair<Iterator, bool> emplace(const K& key, Args&&... args) {
        if (tableSize * 2 >= cap) {
            rehash(cap * 2);
        }

        Node* existing = findNode(key);
        if (existing != nullptr) {
            existing->addVersion(V(std::forward<Args>(args)...));
            return std::make_pair(Iterator(existing), false);
        }

        V value(std::forward<Args>(args)...);
        Node* newNode = new Node(key, std::move(value));
        size_t idx = getIndex(key);

        newNode->nextInBucket = table[idx];
        table[idx] = newNode;
        insertIntoGlobalChain(newNode);

        tableSize++;
        return std::make_pair(Iterator(newNode), true);
    }

    std::pair<Iterator, bool> insert(const K& key, const V& value) {
        if (tableSize * 2 >= cap) {
            rehash(cap * 2);
        }

        Node* existing = findNode(key);
        if (existing != nullptr) {
            existing->addVersion(value);
            return std::make_pair(Iterator(existing), false);
        }

        Node* newNode = new Node(key, value);
        size_t idx = getIndex(key);

        newNode->nextInBucket = table[idx];
        table[idx] = newNode;

        insertIntoGlobalChain(newNode);

        tableSize++;
        return std::make_pair(Iterator(newNode), true);
    }

    std::pair<Iterator, bool> insert(const K& key, V&& value) {
        if (tableSize * 2 >= cap) {
            rehash(cap * 2);
        }

        Node* existing = findNode(key);
        if (existing != nullptr) {
            existing->addVersion(std::move(value));
            return std::make_pair(Iterator(existing), false);
        }

        Node* newNode = new Node(key, std::move(value));
        size_t idx = getIndex(key);

        newNode->nextInBucket = table[idx];
        table[idx] = newNode;

        insertIntoGlobalChain(newNode);

        tableSize++;
        return std::make_pair(Iterator(newNode), true);
    }

    void insert(std::initializer_list<std::pair<K, V>> list) {
        for (auto& item : list) {
            insert(item.first, std::move(item.second));
        }
    }

    bool erase(const K& key) {
        size_t idx = getIndex(key);
        Node* curr = table[idx];
        Node* prevInBucket = nullptr;

        while (curr != nullptr) {
            if (curr->kv.first == key) {
                if (prevInBucket == nullptr) {
                    table[idx] = curr->nextInBucket;
                } else {
                    prevInBucket->nextInBucket = curr->nextInBucket;
                }

                removeFromGlobalChain(curr);

                delete curr;
                tableSize--;
                return true;
            }
            prevInBucket = curr;
            curr = curr->nextInBucket;
        }
        return false;
    }

    Iterator erase(Iterator pos) {
        if (pos == end()) return end();

        Node* node = pos.curr;
        Iterator nextIt = pos;
        ++nextIt;

        erase(node->kv.first);
        return nextIt;
    }

    Iterator erase(ConstIterator pos) {
        if (pos == end()) return end();

        const Node* node = pos.curr;
        ConstIterator nextIt = pos;
        ++nextIt;

        erase(node->kv.first);
        return nextIt;
    }

    V* findSpecificVersion(const K& key, std::function<bool(const V&)> predicate) {
        Node* node = findNode(key);
        if (!node) return nullptr;
        if (predicate(node->kv.second)) {
            return &(const_cast<V&>(node->kv.second));
        }
        VersionNode* currV = node->versionsHead;
        while (currV) {
            if (currV->data && predicate(currV->data)) {
                return &(currV->data);
            }
            currV = currV->nextV;
        }
        return nullptr;
    }

    bool eraseSpecificVersion(const K& key, const V& value) {
        Node* node = findNode(key);
        if (node == nullptr) return false;

        VersionNode* curr = node->versionsHead;
        VersionNode* prev = nullptr;

        while (curr != nullptr) {
            if (curr->data == value) {
                if (prev == nullptr) {
                    node->versionsHead = curr->nextV;
                    if (node->versionsHead) {
                        const_cast<V&>(node->kv.second) = node->versionsHead->data;
                    }
                } else {
                    prev->nextV = curr->nextV;
                }

                curr->nextV = nullptr;
                delete curr;

                if (node->versionsHead == nullptr) {
                    return erase(key);
                }
                return true;
            }
            prev = curr;
            curr = curr->nextV;
        }
        return false;
    }

    bool eraseSpecificVersion(const K& key, void* ptr) {
        Node* node = findNode(key);
        if (!node) return false;

        if (node->kv.second.get() == ptr) {
            const_cast<V&>(node->kv.second).reset();

            VersionNode* currV = node->versionsHead;
            VersionNode* prevV = nullptr;

            while (currV) {
                if (currV->data == nullptr) {
                    if (prevV) prevV->nextV = currV->nextV;
                    else node->versionsHead = currV->nextV;
                    currV->nextV = nullptr;
                    delete currV;
                    break;
                }
                prevV = currV;
                currV = currV->nextV;
            }
            if (node->versionsHead) {
                const_cast<V&>(node->kv.second) = std::move(node->versionsHead->data);
            }
            else {
                erase(key);
            }
            return true;
        }

        VersionNode* currV = node->versionsHead;
        VersionNode* prevV = nullptr;

        while (currV) {
            if (currV->data.get() == ptr) {
                if (prevV) prevV->nextV = currV->nextV;
                else node->versionsHead = currV->nextV;
                currV->nextV = nullptr;
                delete currV;
                return true;
            }
            prevV = currV;
            currV = currV->nextV;
        }
        return false;
    }

    Iterator erase(Iterator first, Iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return first;
    }

    void erase(std::initializer_list<K> list) {
        for (const auto& key : list) {
            erase(key);
        }
    }

    V& at(const K& key) {
        Node* node = findNode(key);
        if (node == nullptr) {
            throw std::invalid_argument("Key not found");
        }
        return node->kv.second;
    }

    const V& at(const K& key) const {
        const Node* node = findNode(key);
        if (node == nullptr) {
            throw std::invalid_argument("Key not found");
        }
        return node->kv.second;
    }

    V& operator[](const K& key) {
        Node* node = findNode(key);
        if (node != nullptr) {
            return node->kv.second;
        }

        V defaultVal{};
        auto result = insert(key, std::move(defaultVal));
        return result.first->second;
    }

    size_t count(const K& key) const {
        return (find(key) != end()) ? 1 : 0;
    }

    bool contains(const K& key) const {
        return find(key) != end();
    }

    bool operator==(const HashTable& other) const {
        if (tableSize != other.tableSize) return false;

        ConstIterator it1 = begin();
        ConstIterator it2 = other.begin();

        while (it1 != end() && it2 != other.end()) {
            if (it1->first != it2->first) return false;
            if (it1->second != it2->second) return false;
            ++it1;
            ++it2;
        }

        return it1 == end() && it2 == other.end();
    }

    bool operator!=(const HashTable& other) const {
        return !(*this == other);
    }

    static_assert(std::is_convertible_v<typename Iterator::iterator_category, std::bidirectional_iterator_tag>,
            "HashTable::Iterator must be at least a Bidirectional Iterator");

    static_assert(std::is_same_v<typename Iterator::value_type, std::pair<const K, V>>,
        "Iterator::value_type must be std::pair<const K, V>");

    static_assert(std::is_convertible_v<typename ConstIterator::iterator_category, std::bidirectional_iterator_tag>,
        "HashTable::ConstIterator must be at least a Bidirectional Iterator");

    static_assert(std::is_same_v<typename ConstIterator::value_type, std::pair<const K, V>>,
        "ConstIterator::value_type must be std::pair<const K, V>");

    static_assert(std::is_const_v<std::remove_reference_t<typename ConstIterator::reference>>,
        "ConstIterator reference type must be const");

    static_assert(std::bidirectional_iterator<Iterator>, "HashTable::Iterator must be at least a Bidirectional Iterator");
    static_assert(std::bidirectional_iterator<ConstIterator>, "HashTable::ConstIterator must be at least a Bidirectional Iterator");
};
#endif // TRAVEL_APP_HASHTABLE_H