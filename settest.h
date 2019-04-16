#ifndef SETTEST_H
#define SETTEST_H
//
#include <iostream>
#include <string>
#include "set.hpp"

using namespace std;

template <typename Iter>
void printDebugInfo(const std::pair<Iter, bool> &insert_info) { std::cout << (insert_info.second ? "Dodano " + std::to_string(*insert_info.first) : "Nie dodano ;/") << std::endl; }

template <typename Iter>
void printDebugInfo(const Iter &find_info) { std::cout << (find_info ? "Znaleziono " + std::to_string(*find_info) : "Nie znaleziono ;/") << std::endl; }

template <typename T>
void printSetSizeAndElems(Set<T> &s)
{
    std::cout << "Size: " << s.getSize() << std::endl;
    for(auto &e : s) std::cout << e << ", ";
    std::cout << std::endl;
}

template <typename T>
void printSetSizeAndElems(const Set<T> &s)
{
    std::cout << "Size: " << s.getSize() << std::endl;
    for(const auto &e : s) std::cout << e << ", ";
    std::cout << std::endl;
}

void setTest();

#endif // SETTEST_H
