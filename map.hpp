#ifndef MAP_H
#define MAP_H

#endif // MAP_H

#include "avl.hpp"

template <typename K, typename V>
class Map {

    class MapElem {
    public:
        std::pair<K,V> key_value;

        MapElem() = default;
        MapElem(const std::pair<K,V> &key_value);
        MapElem(const K &key, const V &value);
        bool operator==(const MapElem &elem) const;
        bool operator!=(const MapElem &elem) const;
        bool operator<(const MapElem &elem) const;
        bool operator>(const MapElem &elem) const;
    };

    AVL<MapElem> avl;

    class Iterator {
    private:

        stack<typename AVL<MapElem>::Node*> s;
        typename AVL<MapElem>::Node *n;

    public:
        Iterator();
        Iterator(const Map<K,V> &map);
        Iterator(typename AVL<MapElem>::Node *node);

        Iterator(const Iterator&) = default;
        //Iterator& operator*(const Iterator&) = default;
        Iterator(Iterator&&) = default;
        //Iterator& operator*(Iterator&&) = default;

        Iterator& operator++();
        const Iterator operator++(int);

        bool operator==(const Iterator &it) const;
        bool operator!=(const Iterator &it) const;
        V& operator*() const;
        V* operator->() const;
        operator bool() const;
    };

    class ConstIterator {
        ConstIterator();
        ConstIterator(const Map<K,V> &map);
        ConstIterator(typename AVL<MapElem>::Node *node);

    public:
        ConstIterator(const ConstIterator&) = default;
        ConstIterator& operator=(const ConstIterator&) = default;
        ConstIterator(ConstIterator&&) = default;
        ConstIterator& operator=(ConstIterator&&) = default;
        friend class Map<K,V>;

    private:
        stack<typename AVL<MapElem>::Node*> s;
        typename AVL<MapElem>::Node *n;
    public:
        bool operator==(const ConstIterator &it) const;
        bool operator!=(const ConstIterator &it) const;
        ConstIterator& operator++();
        ConstIterator& operator++(int);
        const V& operator*() const;
        const V* operator->() const;
        operator bool() const;
    };

public:

    friend void swap(Map<K, V>& first, Map<K, V>& second) noexcept
    {
        using std::swap;

        swap(first.avl, second.avl);
    }

    Map() = default;
    Map(const Map<K,V> &source);
    Map<K,V>& operator=(const Map<K,V> &source);
    Map(Map<K,V> &&source);
    Map<K,V>& operator=(Map<K, V>&&source);
    ~Map() = default;

    //zwraca ilosc elem
    std::size_t size() const;
    // dodaje klucz i wartosc, zwraca iterator do dodanej wartosci i true, lub iterator do istniejacej i false
    std::pair<Iterator, bool> insert(const K &key, const V &value);
    std::pair<Iterator, bool> insert(const std::pair<K,V> &key_value);
    // wyszukuje element o podanym kluczu - jezeli elem zostal znaleziony to zwraca constiterator
    ConstIterator find(const K &key) const;
    // wyszukuje element o podanym kluczu - jezeli znaleziono to zwraca iterator
    Iterator find(const K&key);
    typename AVL<MapElem>::Node* findNode(const K&key);
    // usuwa element o podanej wartosci
    bool remove(const K &key);
    // wyszukuje element o podanym kluczu, jesli znaleziony to to zwraca referencje
    V& operator[](const K &key);
    // zwraca constiterator na podany elem
    ConstIterator begin() const;
    //zwraca iterator na pierwszy element
    Iterator begin();
    // zwraca constiterator za ostatni element
    ConstIterator end() const;
    // zwraca iterator za ostatni elem
    Iterator end();

};

template <typename K, typename V>
Map<K,V>::Map(const Map<K,V> &source) {
    this->avl = source.avl;
}

template <typename K, typename V>
Map<K,V>& Map<K,V>::operator=(const Map<K,V> &source) {
    Map<K,V> temp(source);
    this->avl = temp.avl;
    return *this;
}

template <typename K, typename V>
Map<K,V>::Map(Map<K,V> &&source) {
    swap(*this, source);
}

template <typename K, typename V>
Map<K,V>& Map<K,V>::operator=(Map<K, V>&&source) {
    swap(*this, source);
    return *this;
}

template <typename K, typename V>
std::size_t Map<K,V>::size() const { return avl.getTreeSize(); }

template <typename K, typename V>
std::pair<typename Map<K,V>::Iterator, bool> Map<K,V>::insert(const K &key, const V &value) {
    MapElem element(key,value);
    if(avl.isPresent(element)) return make_pair(Iterator(avl.findNode(element)), false);
    else {
        avl.insert(element);
        return make_pair(Iterator(avl.getLastAdded()), true);
    }
}

template <typename K, typename V>
std::pair<typename Map<K,V>::Iterator, bool> Map<K,V>::insert(const std::pair<K,V> &key_value) {
    MapElem element(key_value.first,key_value.second);
    if(avl.isPresent(element)) return make_pair(Iterator(avl.findNode(element)), false);
    else {
        avl.insert(element);
        return make_pair(Iterator(avl.getLastAdded()), true);
    }
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator Map<K,V>::find(const K &key) const {
    typename AVL<MapElem>::Node *n = avl.getRoot();
    while(n->value.key_value.first != key) {
        if(key < n->value.key_value.first) n = n->left;
        else n = n->right;

        if(n == nullptr) return ConstIterator(n);
    }
    return ConstIterator(n);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K&key) {
    typename AVL<MapElem>::Node *n = avl.getRoot();
    while(n->value.key_value.first != key) {
        if(key < n->value.key_value.first) n = n->left;
        else n = n->right;

        if(n == nullptr) return Iterator(n);
    }
    return Iterator(n);

}

template <typename K, typename V>
typename AVL<typename Map<K,V>::MapElem>::Node* Map<K,V>::findNode(const K&key) {
    typename AVL<typename Map<K,V>::MapElem>::Node *n = Map<K,V>::avl.getRoot();
    while(n->value.key_value.first != key) {
        if(key < n->value.key_value.first) n = n->left;
        else n = n->right;

        if(n == nullptr) return n;
    }
    //cout << n->value.key_value.second;
    return n;
}

template <typename K, typename V>
bool Map<K,V>::remove(const K &key) {
    return Map<K,V>::avl.remove(findNode(key)->value);
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K &key) {
    typename AVL<MapElem>::Node* n = findNode(key);
    if(n != nullptr) return *find(key);
    else {
        avl.insert(MapElem(key, V{}));
        return *find(key);
    }
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator Map<K,V>::begin() const { return ++ConstIterator(*this); }

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin() { return ++Iterator(*this); }

template <typename K, typename V>
typename Map<K,V>::ConstIterator Map<K,V>::end() const { return ConstIterator(); }

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end() { return Iterator(); }

template <typename K, typename V>
Map<K,V>::MapElem::MapElem(const std::pair<K,V> &key_value) : key_value(key_value) {}

template <typename K, typename V>
Map<K,V>::MapElem::MapElem(const K &key, const V &value) { key_value = std::pair<K,V>(key, value); }

template <typename K, typename V>
bool Map<K,V>::MapElem::operator==(const MapElem &elem) const { return key_value.first == elem.key_value.first; }

template <typename K, typename V>
bool Map<K,V>::MapElem::operator!=(const MapElem &elem) const { return key_value.first != elem.key_value.first; }

template <typename K, typename V>
bool Map<K,V>::MapElem::operator<(const MapElem &elem) const { return key_value.first < elem.key_value.first; }

template <typename K, typename V>
bool Map<K,V>::MapElem::operator>(const MapElem &elem) const { return key_value.first > elem.key_value.first; }

template <typename K, typename V>
Map<K,V>::Iterator::Iterator() : n(nullptr) {}

template <typename K, typename V>
Map<K,V>::Iterator::Iterator(const Map<K,V> &map) {
    typename AVL<MapElem>::Node *tmp = map.avl.getRoot();
    while(tmp != nullptr) {
        s.push(tmp);
        tmp = tmp->left;
    }
}

template <typename K, typename V>
Map<K,V>::Iterator::Iterator(typename AVL<MapElem>::Node *node) : n(node) {}

template <typename K, typename V>
typename Map<K,V>::Iterator& Map<K,V>::Iterator::operator++() {

    if(s.empty()) {
        n = nullptr;
        return *this;
    }
    typename AVL<MapElem>::Node *top = s.top();
    s.pop();
    if(top->right != nullptr) {
        typename AVL<MapElem>::Node *current = top->right;
        while(current != nullptr) {
            s.push(current);
            current = current->left;
        }
    }
    n = top;
    return *this;
}

template <typename K, typename V>
const typename Map<K,V>::Iterator Map<K,V>::Iterator::operator++(int) {
    Iterator it(*this);
    ++(*this);
    return it;
}

template <typename K, typename V>
bool Map<K,V>::Iterator::operator==(const Iterator &it) const { return it.n == this->n; }

template <typename K, typename V>
bool Map<K,V>::Iterator::operator!=(const Iterator &it) const { return it.n != this->n; }

template <typename K, typename V>
V& Map<K,V>::Iterator::operator*() const { return n->value.key_value.second; }

template <typename K, typename V>
V* Map<K,V>::Iterator::operator->() const { return &n->value.key_value.second; }

template <typename K, typename V>
Map<K,V>::Iterator::operator bool() const { return this->n != nullptr; }

template <typename K, typename V>
Map<K,V>::ConstIterator::ConstIterator() : n(nullptr) {}

template <typename K, typename V>
Map<K,V>::ConstIterator::ConstIterator(const Map<K,V> &map) {
    typename AVL<MapElem>::Node *tmp = map.avl.getRoot();
    while(tmp != nullptr) {
        s.push(tmp);
        tmp = tmp->left;
    }
}

template <typename K, typename V>
Map<K,V>::ConstIterator::ConstIterator(typename AVL<MapElem>::Node *node) : n(node) {}

template <typename K, typename V>
bool Map<K,V>::ConstIterator::operator==(const ConstIterator &it) const { return it.n == this->n; }

template <typename K, typename V>
bool Map<K,V>::ConstIterator::operator!=(const ConstIterator &it) const { return it.n != this->n; }

template <typename K, typename V>
typename Map<K,V>::ConstIterator& Map<K,V>::ConstIterator::operator++() {
    if(s.empty()) {
        n = nullptr;
        return *this;
    }
    typename AVL<MapElem>::Node *top = s.top();
    s.pop();
    if(top->right != nullptr) {
        typename AVL<MapElem>::Node *current = top->right;
        while(current != nullptr) {
            s.push(current);
            current = current->left;
        }
    }
    n = top;
    return *this;
}

template <typename K, typename V>
typename Map<K,V>::ConstIterator& Map<K,V>::ConstIterator::operator++(int) {
    Iterator it(*this);
    ++(*this);
    return it;
}

template <typename K, typename V>
const V& Map<K,V>::ConstIterator::operator*() const { return n->value.key_value.second; }

template <typename K, typename V>
const V* Map<K,V>::ConstIterator::operator->() const { return &n->value.key_value.second; }

template <typename K, typename V>
Map<K,V>::ConstIterator::operator bool() const { return this->n != nullptr; }
