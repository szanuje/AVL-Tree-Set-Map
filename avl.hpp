#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <type_traits>
#include <iterator>
#include <utility>
#include <cstddef>
#include <stack>
using namespace std;

template<typename T>
class AVL {

public:

    struct Node {
        T value;
        int height;
        Node* left;
        Node* right;

    public:
        friend ostream& operator<< (ostream& os, const Node* s) { return os << s->value; }
    };

    class Iterator {

    public:
        stack<AVL<T>::Node*> s;
        AVL<T>::Node *n;

        Iterator(const AVL<T> &t) {
            typename AVL<T>::Node *tmp = t.getRoot();
            while(tmp != nullptr) {
                s.push(tmp);
                tmp = tmp->left;
            }
        }

        Iterator() : n(nullptr) {}

        Iterator(typename AVL<T>::Node *node) : n(node) {}

        Iterator& operator++() {

            if(s.empty()) {
                n = nullptr;
                return *this;
            }
            typename AVL<T>::Node *top = s.top();
            s.pop();
            if(top->right != nullptr) {
                typename AVL<T>::Node *current = top->right;
                while(current != nullptr) {
                    s.push(current);
                    current = current->left;
                }
            }
            n = top;
            return *this;
        }

        const Iterator operator++(int) {
            Iterator it(*this);
            ++(*this);
            return it;
        }

        bool operator==(const Iterator &it) const { return it.n == this->n; }
        bool operator!=(const Iterator &it) const { return it.n != this->n; }
        const T &operator*() const { return n->value; }
        operator bool() const { return this->n != nullptr; }
    };

private:

    Node * root;
    int treeSize = 0;
    Node * lastAdded;
    vector<Node*> nodes;

    friend void swap(AVL<T>& first, AVL<T>& second)
    {
        using std::swap;
        swap(first.root, second.root);
        second.root = nullptr;
    }

    Node *newNode(T val);
    int getHeight(const Node *tree);
    Node *rightRotate(Node *y);
    Node *leftRotate(Node *x);
    int getBalance(Node *b);
    Node *insertRecursive(Node *node, T thisval);
    void preorderRecursive(const Node *n);
    Node *minValNode(Node *minValNode);
    Node *removeRecursive(Node *n, T thisval);
    void inorderRecursive(const Node * n);

public:

    Node *getRoot() const { return root; }
    Iterator begin() const { return ++Iterator(*this); }
    Iterator end() const { return Iterator(); }
    AVL() : root(nullptr) {}
    ~AVL() {}
    AVL(const AVL<T> &source);
    AVL<T>& operator=(const AVL<T> &source);
    AVL(AVL<T> &&source) noexcept;
    AVL<T>& operator=(AVL<T> &&source);
    void preorder();
    std::pair<typename AVL<T>::Iterator, bool> insert(T val);
    bool remove(T val);
    int getTreeSize() const { return treeSize; }
    void setTreeSize(int size) { this->treeSize = size; }
    bool isPresent(const T &val);
    void copy(Node * &current, Node * source);
    Iterator find(T val);
    Node* findNode(T val);

    void inorder();
    void inorder_traverse_nonrecursive(Node *node);
    Node* getLastAdded() { return lastAdded; }
};

template<typename T>
AVL<T>::AVL(const AVL<T> &source) : root(nullptr) {
    //cout << "cc-tor" << endl;
    if(source.root != nullptr) {
        copy(this->root, source.root);
        this->treeSize = source.treeSize;
    }
}

template <typename T>
AVL<T>& AVL<T>::operator=(AVL<T> const &source) {

    //    cout << "ca-op" << endl;
    AVL temp(source);
    swap(*this, temp);
    this->treeSize = source.treeSize;

    return *this;
}

template <typename T>
AVL<T>::AVL(AVL<T> &&source) noexcept : root(nullptr) {
    swap(*this, source);
    this->treeSize = source.treeSize;

}

template <typename T>
AVL<T>& AVL<T>::operator=(AVL<T> &&source) {

    //    cout << " ma-op" << endl;
    swap(*this, source);
    this->treeSize = source.treeSize;

    return *this;
}

template<typename T>
void AVL<T>::copy(Node * &current, Node * source) {
    if(source == nullptr) current = nullptr;
    else {
        current = new Node;
        current->value = source->value;
        copy(current->left, source->left);
        copy(current->right, source->right);
    }
}

template <typename T>
typename AVL<T>::Iterator AVL<T>::find(T val) {
    typename AVL<T>::Node *n = root;
    while(n->value != val) {
        if(val < n->value) n = n->left;
        else n = n->right;

        if(n == nullptr) return Iterator(n);
    }
    return Iterator(n);
}

template <typename T>
typename AVL<T>::Node* AVL<T>::findNode(T val) {
    typename AVL<T>::Node *n = root;
    while(n->value != val) {
        if(val < n->value) n = n->left;
        else n = n->right;

        if(n == nullptr) return n;
    }
    return n;
}

template<typename T>
typename AVL<T>::Node * AVL<T>::insertRecursive(Node *node, T thisval) {
    if(node == nullptr) {
        return newNode(thisval);
    }

    if(thisval < node->value) {
        node->left = insertRecursive(node->left, thisval);
    }
    else {
        node->right = insertRecursive(node->right, thisval);
    }

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    if(balance > 1 && thisval < node->left->value) {
        return rightRotate(node);
    }
    if(balance < -1 && thisval > node->right->value) {
        return leftRotate(node);
    }
    if(balance > 1 && thisval > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if(balance < -1 && thisval < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

template<typename T>
std::pair<typename AVL<T>::Iterator, bool> AVL<T>::insert(T val) {
    treeSize++;
    //    if(isPresent(val)) {
    //        return make_pair(Iterator(find(val)), false);
    //    }

    root = insertRecursive(root, val);
    //    cout << "New node = " << root->value << endl;
    return make_pair(Iterator(lastAdded), true);
}

template<typename T>
void AVL<T>::preorderRecursive(const Node *n) {
    if(n != nullptr) {
        cout << n->value << ", ";
        preorderRecursive(n->left);
        preorderRecursive(n->right);
    }
}

template<typename T>
void AVL<T>::preorder() {
    cout << "Preorder traversal: ";
    preorderRecursive(root);
    cout << endl;
}

template <typename T>
void AVL<T>::inorderRecursive(const Node * n) {
    if(n == nullptr) return;

    inorderRecursive(n->left);
    cout << n->value << ", ";
    inorderRecursive(n->right);
}

template <typename T>
void AVL<T>::inorder() {
    cout << "In order traversal: ";
    inorderRecursive(root);
    cout << endl;
}

template<typename T>
typename AVL<T>::Node * AVL<T>::removeRecursive(Node *node, T thisval) {
    if(node == nullptr) {
        return node;
    }

    if(thisval < node->value) {
        node->left = removeRecursive(node->left, thisval);
    }
    else if(thisval > node->value) {
        node->right = removeRecursive(node->right, thisval);
    }
    else {
        if(node->left == nullptr || node->right == nullptr) {
            Node *temp = node->left ? node->left : node->right;

            if(temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }

            free(temp);
        }
        else {
            Node *temp = minValNode(node->right);
            node->value = temp->value;
            node->right = removeRecursive(node->right, temp->value);
        }
    }

    if(node == nullptr) {
        return node;
    }

    node->height = max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);

    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

template<typename T>
bool AVL<T>::remove(T val) {
    //    cout << "Removed node = " << val << endl;
    treeSize--;
    root = removeRecursive(root, val);
    if(root == nullptr) return false;
    else return true;
}

template<typename T>
int AVL<T>::getHeight(const Node *tree) {
    if(tree == nullptr) {
        return 0;
    } else {
        return tree->height;
    }
}

template<typename T>
int AVL<T>::getBalance(Node *tree) {
    if(tree == nullptr) {
        return 0;
    } else {
        return getHeight(tree->left) - getHeight(tree->right);
    }
}

template<typename T>
typename AVL<T>::Node * AVL<T>::newNode(T val) {
    Node * node = new Node;
    node->value = val;
    node->height = 1;
    node->left = nullptr;
    node->right = nullptr;
    lastAdded = node;
    nodes.push_back(node);
    return node;
}

template<typename T>
typename AVL<T>::Node * AVL<T>::rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return x;
}

template<typename T>
typename AVL<T>::Node * AVL<T>::leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    x->right = T2;
    y->left = x;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

template<typename T>
typename AVL<T>::Node * AVL<T>::minValNode(Node *minValNode) {
    Node *current = minValNode;

    while(current->left != nullptr) {
        current = current->left;
    }
    return current;
}

template<typename T>
bool AVL<T>::isPresent(const T &val) {
    Node * temp = root;
    while(temp != nullptr) {
        if(val < temp->value) temp = temp->left;
        else if(val > temp->value) temp = temp->right;
        else return true;
    }
    return false;
}

template <typename T>
void AVL<T>::inorder_traverse_nonrecursive(Node *node) {
    stack<Node*> stack;
    do {
        while (nullptr != node) {
            stack.push(node);
            node = node->left;
        }
        do {
            Node *top = stack.top();
            stack.pop();
            cout << top->value << ", ";
            if (nullptr != top->right) {
                node = top->right;
                break;
            }
        }
        while (!stack.empty());
    }
    while (!stack.empty());
}
#endif // AVL_H
