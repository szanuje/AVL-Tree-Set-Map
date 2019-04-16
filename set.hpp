#ifndef SET_H
#define SET_H
#include "avl.hpp"

template <typename T>
class Set {

public:

    AVL<T> tree;

    void inorder();
    void preorder();
    typename AVL<T>::Node* findVal(T val);
    typename AVL<T>::Iterator find(T val);
    bool isPresent(const T &val);
    std::pair<typename AVL<T>::Iterator, bool> insert(T val);
    bool remove(T val);
    int getSize() const;

    typename AVL<T>::Iterator begin() const;
    typename AVL<T>::Iterator end() const;

};

template <typename T>
typename AVL<T>::Iterator Set<T>::begin() const {
    return tree.begin();
}

template <typename T>
typename AVL<T>::Iterator Set<T>::end() const {
    return tree.end();
}

template <typename T>
typename AVL<T>::Node* Set<T>::findVal(T val) {
    return tree.findRecursive(val);
}

template <typename T>
void Set<T>::inorder() {
    return tree.inorder();
}

template <typename T>
typename AVL<T>::Iterator Set<T>::find(T val) {
    return tree.find(val);
}

template <typename T>
void Set<T>::preorder() {
    return tree.preorder();
}

template <typename T>
bool Set<T>::isPresent(const T &val) {
    return tree.isPresent(val);
}

template <typename T>
std::pair<typename AVL<T>::Iterator, bool> Set<T>::insert(T val) {
    if(tree.isPresent(val)) return make_pair(typename AVL<T>::Iterator(tree.find(val)), false);
    else return tree.insert(val);
}

template <typename T>
bool Set<T>::remove(T val) {
    return tree.remove(val);
}

template <typename T>
int Set<T>::getSize() const {
    return tree.getTreeSize();
}

#endif // SET_H
