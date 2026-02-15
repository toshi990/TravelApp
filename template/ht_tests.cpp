#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "hashTable.h"
#include <vector>
#include <string>
#include <algorithm>

/*TEST_CASE("Default constructor creates empty table", "[HashTable]") {
    HashTable<int, int> table;
    REQUIRE(table.empty());
    REQUIRE(table.size() == 0);
    REQUIRE(table.begin() == table.end());
}

TEST_CASE("Insert and size", "[HashTable]") {
    HashTable<int, std::string> table;

    auto result1 = table.insert(1, "one");
    REQUIRE(result1.second == true);
    REQUIRE(table.size() == 1);

    auto result2 = table.insert(2, "two");
    REQUIRE(result2.second == true);
    REQUIRE(table.size() == 2);

    auto result3 = table.insert(1, "uno");
    REQUIRE(result3.second == false);
    REQUIRE(table.size() == 2);
}

TEST_CASE("Find and contains", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");

    auto it1 = table.find(1);
    REQUIRE(it1 != table.end());
    REQUIRE(it1->first == 1);
    REQUIRE(it1->second == "one");

    REQUIRE(table.contains(2));
    REQUIRE_FALSE(table.contains(3));

    const auto& const_table = table;
    auto cit = const_table.find(2);
    REQUIRE(cit != const_table.end());
}

TEST_CASE("At method", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "value");

    REQUIRE(table.at(1) == "value");
    REQUIRE_THROWS_AS(table.at(999), std::invalid_argument);

    table.at(1) = "newvalue";
    REQUIRE(table.at(1) == "newvalue");
}

TEST_CASE("Operator[]", "[HashTable]") {
    HashTable<int, std::string> table;

    table[1] = "first";
    REQUIRE(table[1] == "first");
    REQUIRE(table.size() == 1);

    std::string& val = table[2];
    REQUIRE(table.size() == 2);
    val = "second";
    REQUIRE(table[2] == "second");
}

TEST_CASE("Erase by key", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "a");
    table.insert(2, "b");
    table.insert(3, "c");

    REQUIRE(table.erase(2));
    REQUIRE(table.size() == 2);
    REQUIRE_FALSE(table.contains(2));

    REQUIRE_FALSE(table.erase(999));
    REQUIRE(table.size() == 2);
}

TEST_CASE("Erase by iterator", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "a");
    table.insert(2, "b");

    auto it = table.find(1);
    REQUIRE(it != table.end());

    auto next_it = table.erase(it);
    REQUIRE(table.size() == 1);
    REQUIRE(next_it != table.end());

    next_it = table.erase(table.end());
    REQUIRE(next_it == table.end());
}

TEST_CASE("Erase by const iterator", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "a");

    const auto& const_table = table;
    auto cit = const_table.find(1);
    REQUIRE(cit != const_table.end());

    auto next_it = table.erase(cit);
    REQUIRE(table.empty());
}

TEST_CASE("Clear", "[HashTable]") {
    HashTable<int, std::string> table;

    for (int i = 0; i < 5; ++i) {
        table.insert(i, std::to_string(i));
    }

    REQUIRE(table.size() == 5);
    table.clear();
    REQUIRE(table.empty());
    REQUIRE(table.size() == 0);
}

TEST_CASE("Copy constructor", "[HashTable]") {
    HashTable<int, std::string> table1;
    table1.insert(1, "one");
    table1.insert(2, "two");

    HashTable<int, std::string> table2 = table1;

    REQUIRE(table2.size() == 2);
    REQUIRE(table2.at(1) == "one");
    REQUIRE(table2.at(2) == "two");
}

TEST_CASE("Move constructor", "[HashTable]") {
    HashTable<int, std::string> table1;
    table1.insert(1, "one");

    HashTable<int, std::string> table2 = std::move(table1);

    REQUIRE(table2.size() == 1);
    REQUIRE(table2.at(1) == "one");
    REQUIRE(table1.empty());
}

TEST_CASE("Copy assignment", "[HashTable]") {
    HashTable<int, std::string> table1;
    table1.insert(1, "one");

    HashTable<int, std::string> table2;
    table2.insert(2, "two");

    table2 = table1;

    REQUIRE(table2.size() == 1);
    REQUIRE(table2.at(1) == "one");
}

TEST_CASE("Move assignment", "[HashTable]") {
    HashTable<int, std::string> table1;
    table1.insert(1, "one");

    HashTable<int, std::string> table2;
    table2 = std::move(table1);

    REQUIRE(table2.size() == 1);
    REQUIRE(table2.at(1) == "one");
    REQUIRE(table1.empty());
}

TEST_CASE("Swap", "[HashTable]") {
    HashTable<int, std::string> table1;
    table1.insert(1, "a");

    HashTable<int, std::string> table2;
    table2.insert(2, "b");

    table1.swap(table2);

    REQUIRE(table1.size() == 1);
    REQUIRE(table1.at(2) == "b");
    REQUIRE(table2.size() == 1);
    REQUIRE(table2.at(1) == "a");
}

TEST_CASE("Iterator operations", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "a");
    table.insert(2, "b");
    table.insert(3, "c");

    auto it = table.begin();
    REQUIRE(it->first == 1);

    ++it;
    REQUIRE(it->first == 2);

    --it;
    REQUIRE(it->first == 1);

    it++;
    REQUIRE(it->first == 2);

    it--;
    REQUIRE(it->first == 1);

    auto it2 = table.begin();
    REQUIRE(it == it2);
    ++it2;
    REQUIRE(it != it2);
}

TEST_CASE("Range-based for loop", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "a");
    table.insert(2, "b");

    int count = 0;
    for (const auto& pair : table) {
        count++;
        REQUIRE(pair.second == (pair.first == 1 ? "a" : "b"));
    }
    REQUIRE(count == 2);
}

TEST_CASE("Count and contains", "[HashTable]") {
    HashTable<int, std::string> table;
    table.insert(1, "one");

    REQUIRE(table.count(1) == 1);
    REQUIRE(table.count(999) == 0);
    REQUIRE(table.contains(1));
    REQUIRE_FALSE(table.contains(999));
}

TEST_CASE("Equality operators", "[HashTable]") {
    HashTable<int, std::string> table1;
    table1.insert(1, "a");
    table1.insert(2, "b");

    HashTable<int, std::string> table2;
    table2.insert(1, "a");
    table2.insert(2, "b");

    REQUIRE(table1 == table2);
    REQUIRE_FALSE(table1 != table2);

    table2.insert(1, "c");
    REQUIRE(table1 != table2);
    REQUIRE_FALSE(table1 == table2);
}

TEST_CASE("Version system", "[HashTable]") {
    HashTable<int, std::string> table;

    table.insert(1, "v1");
    REQUIRE(table.at(1) == "v1");

    table.insert(1, "v2");
    REQUIRE(table.at(1) == "v2");

    REQUIRE(table.eraseSpecificVersion(1, "v2"));
    REQUIRE(table.at(1) == "v1");

    REQUIRE(table.eraseSpecificVersion(1, "v1"));
    REQUIRE_FALSE(table.contains(1));
}

TEST_CASE("Rehashing", "[HashTable]") {
    HashTable<int, std::string> table(2);

    for (int i = 0; i < 10; ++i) {
        table.insert(i, std::to_string(i));
    }

    REQUIRE(table.size() == 10);
    for (int i = 0; i < 10; ++i) {
        REQUIRE(table.contains(i));
    }
}*/