#include <iostream>
#include <vector>
#include <utility>
#include <stdio.h>

template <typename T>
class Treap {

public:

    Treap(const T& defElem) : defaultElem(defElem) {};
    ~Treap();

    void insert(const T& elem);
    T findNext(const T& elem);
    void erase(const T& elem);
    bool empty();
    T findKMax(const int kMax);


private:

    struct Node{
        
        Node(const T& data) : data(data), prior(rand () % 300000) {};

        const T data;
        const int prior;
        Node* left = nullptr;
        Node* right = nullptr;
        int size = 1;

        std::pair<Node*, Node*> split(const T& key);
        void clear();
        T findNext(const T& elem, const T& defaultElem);
        void erase(const T& elem, Node* parent = nullptr, bool Left = false);
        T findKMax(const int kMax, const T& defaultElem);
        Node* merge(Node*, Node*);

    };

    int size = 0;
    Node* root;
    const T defaultElem;
    Node* merge(Node* leftNode, Node* rightNode);

};

void doCommands(int numOfCommands);

int main() {

    int numOfCommands = 0;
    std::cin >> numOfCommands;
    doCommands(numOfCommands);
    
    return 0;

}

void doCommands(int numOfCommands) {

    Treap<int> tree (-1);
    std::vector<int> results;
    short int command = 0;
    int param = 0;

    for (int i = 0; i < numOfCommands; ++i) {

        std::cin >> command;
        std::cin >> param;
        if (command == 1)
            tree.insert(param);

        else if (command == -1) 
            tree.erase(param);

        else if (command == 0) {

            int next = tree.findKMax (param);
            results.push_back(next);

        }
    }


    for (int i = 0; i < results.size(); i++)
        std::cout << results[i] << '\n';

}

//==========================================================

template <typename T>
Treap<T>::~Treap() {

    if (!empty())
        root->clear();

    delete root;
    root = nullptr;

}

template <typename T>
T Treap<T>::findKMax(const int kMax) {

    if (empty())
        return defaultElem;

    return root->findKMax(kMax, defaultElem);
}

template <typename T>
T Treap<T>::Node::findKMax(const int kMax, const T& defaultElem) {

    if (left != nullptr)

        if (kMax == size - left->size)
            return data;
        else if (kMax > size - left->size)
            return left->findKMax(kMax - size + left->size, defaultElem);

        else if (right != nullptr)
            return right->findKMax(kMax, defaultElem);

        else return defaultElem;

    else if (kMax == size)
            return data;

        else if (kMax < size && right != nullptr)
            return right->findKMax(kMax, defaultElem);

        else return defaultElem;

}

template <typename T>
void Treap<T>::insert(const T& elem) {

    Node* newNode = new Node(elem);
    if (empty())
        root = newNode;

    else {

        if (findNext(elem) == elem) {

            delete newNode;
            return;

        }

        std::pair<Node*, Node*> roots = root->split(elem);
        roots.first = merge(roots.first, newNode);
        root = merge(roots.first, roots.second);
    }

    ++size;

}

template <typename T>
T Treap<T>::findNext(const T& elem) {

    if (!empty())
        return root->findNext(elem, defaultElem);

    else return defaultElem;

}

template <typename T>
T Treap<T>::Node::findNext(const T& elem, const T& defaultElem) {

    if (data == elem)
        return elem;

    if (elem > data)
        if (right != nullptr)
            return right->findNext(elem, defaultElem);

        else return defaultElem;

    if (left != nullptr) {
        T found = left->findNext(elem, defaultElem);
        if (found == defaultElem)
            return data;

        else return found;

    } else return data;
        
}


template <typename T>
void Treap<T>::erase(const T& elem) {

    if (!empty() && (findNext(elem) != defaultElem)) 

        if (root->data == elem) {

            Node* oldRoot = root;
            root = merge(root->left, root->right);
            delete oldRoot;

        } else root->erase(elem);

    --size;
}

template <typename T>
void Treap<T>::Node::erase(const T& elem, Node* parent,  bool Left) {

    --size;
    if (data == elem) {

        if (Left)
            parent->left = merge (left, right);
        else parent->right = merge (left, right);

        delete this;
        return;

    } else if (data < elem && right != nullptr)
        right->erase(elem, this, false);

    else if (left != nullptr)
        left->erase(elem, this, true);

}

template <typename T>
bool Treap<T>::empty() {

    return (size <= 0);

}

template <typename T>
std::pair<typename Treap<T>::Node*, typename Treap<T>::Node*> Treap<T>::Node::split(const T& key) {

    std::pair<Node*, Node*> roots = {nullptr, nullptr};
    std::pair<Node*, Node*> bufRoots = {nullptr, nullptr};

    if (key > data) {

        roots.first = this;

        if (right != nullptr)
            bufRoots = right->split(key);

        right = bufRoots.first;
        roots.second = bufRoots.second;

    } else {
        
        roots.second = this;
        
        if (left != nullptr)
            bufRoots = left->split(key);

        left = bufRoots.second;
        roots.first = bufRoots.first;
    }

    this->size = 1;
    if (left != nullptr)
        this->size += left->size;
    
    if (right != nullptr)
        this->size += right->size;

    return roots;
}

template <typename T>
typename Treap<T>::Node* Treap<T>::merge(Node* leftNode, Node* rightNode) {

    if (leftNode == nullptr)
        return rightNode;

    if (rightNode == nullptr)
        return leftNode;

    if (leftNode->prior >= rightNode->prior) {

        leftNode->right = merge(leftNode->right, rightNode);
        leftNode->size = 1;
        if (leftNode->left != nullptr)
            leftNode->size += leftNode->left->size;
    
        if (leftNode->right != nullptr)
            leftNode->size += leftNode->right->size;

        return leftNode;

    }

    rightNode->left = merge(leftNode, rightNode->left);
    rightNode->size = 1;
    if (rightNode->left != nullptr)
        rightNode->size += rightNode->left->size;
    
    if (rightNode->right != nullptr)
        rightNode->size += rightNode->right->size;


    return rightNode;

}

template <typename T>
typename Treap<T>::Node* Treap<T>::Node::merge(Node* leftNode, Node* rightNode) {

    if (leftNode == nullptr)
        return rightNode;

    if (rightNode == nullptr)
        return leftNode;

    if (leftNode->prior >= rightNode->prior) {

        leftNode->right = merge(leftNode->right, rightNode);
        leftNode->size = 1;
        if (leftNode->left != nullptr)
            leftNode->size += leftNode->left->size;
    
        if (leftNode->right != nullptr)
            leftNode->size += leftNode->right->size;

        return leftNode;

    }

    rightNode->left = merge(leftNode, rightNode->left);
    rightNode->size = 1;
    if (rightNode->left != nullptr)
        rightNode->size += rightNode->left->size;
    
    if (rightNode->right != nullptr)
        rightNode->size += rightNode->right->size;


    return rightNode;

}


template <typename T>
void Treap<T>::Node::clear() {

    if (left != nullptr) {

        left->clear();
        delete left;
        left = nullptr;
    }

    if (right != nullptr) {

        right->clear();
        delete right;
        right = nullptr;

    }

}
