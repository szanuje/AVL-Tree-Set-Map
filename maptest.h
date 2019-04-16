#ifndef MAPTEST_H
#define MAPTEST_H

#include <iostream>
#include <string>
#include "map.hpp"

using namespace std;

template <typename Iter>
void printDebugInfo(const std::pair<Iter, bool> &insert_info) { std::cout << (insert_info.second ? "Dodano " + *insert_info.first : "Nie dodano ;/") << std::endl; }

template <typename Iter>
void printDebugInfo(const Iter &find_info) { std::cout << (find_info ? "Znaleziono " + *find_info : "Nie znaleziono ;/") << std::endl; }

template <typename K, typename V>
void printSetSizeAndElems(Map<K, V> &m)
{
    std::cout << "Size: " << m.size() << std::endl;
    for(auto &e : m) std::cout << e << ", ";
    std::cout << std::endl;
}

template <typename K, typename V>
void printSetSizeAndElems(const Map<K, V> &m)
{
    std::cout << "Size: " << m.size() << std::endl;
    for(const auto &e : m) std::cout << e << ", ";
    std::cout << std::endl;
}

void mapTest();

#endif // MAPTEST_H
