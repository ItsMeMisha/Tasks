#include <iostream>
#include <vector>
#include <utility>

template <typename T>
class Treap {

public:

    Treap(const T& defElem) : defaultElem(defElem) {};
    ~Treap();

    void insert(const T& elem);
    T findNext(const T& elem);
    void erase(const T& elem);
    bool empty();

private:

    struct Node{
        
        Node(const T& data) : data(data), prior(rand () % 300000) {};

        const T data;
        const int prior;
        Node* left = nullptr;
        Node* right = nullptr;

        std::pair<Node*, Node*> split(const T& key);
        void clear();
        T findNext(const T& elem, const T& defaultElem);
        void erase(const T& elem, Node* parent);

    };

    int size = 0;
    Node* root;
    const T defaultElem;
    Node* merge(Node* leftNode, Node* rightNode);

};

void doCommands (int numberOfCommands);

int main() {

    int numOfCommands = 0;
    std::cin >> numOfCommands;
    doCommands (numOfCommands);

    return 0;
}

void doCommands (int numberOfCommands) {

    Treap<int> tree (-1);
    std::vector<int> results;
    char command = 0;
    int param = 0;
    char prevCom = 0;
    int prevRes = 0;
    for (int i = 0; i < numberOfCommands; ++i) {
        std::cin >> command;

        if (command == '+') {
            
            std::cin >> param;
            if (prevCom == '?')
                tree.insert((param + prevRes) % 1000000000);
            else tree.insert(param);

        }

        else if (command == '?') {

            std::cin >> param;
            int next = tree.findNext (param);
            prevRes = next;
            results.push_back(next);

        }
        prevCom = command;
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
            delete root;

        } else root->erase(elem);

    --size;
}

template <typename T>
void Treap<T>::Node::erase(const T& elem, Node* parent) {

    if (data == elem) {

        parent = merge (left, right);
        delete this;
        return;

    } else if (data < elem && right != nullptr)
        right->erase(elem, this);

    else if (left != nullptr)
        left->erase(elem, this);

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
        return leftNode;

    }

    rightNode->left = merge(leftNode, rightNode->left);
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
