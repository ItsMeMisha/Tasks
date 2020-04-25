#include <iostream>

#include <stdio.h>

#include <vector>
#include <utility>

template <typename T>
class BTree {

public:

    BTree() : root(new Node) {};
    BTree(const T& defElem) : root(new Node), defaultElem(defElem) {};
    ~BTree();

    void insert(const T& elem);
    T findNext(const T& elem);
    void erase(const T& elem);
    T findKMax(int k);
    bool empty();

private:

    struct Node {

        Node() {

            children.push_back(nullptr);
            subTreeSize.push_back(0);

        }

        Node(const Node& sourceNode, const int& first, const int& last) :

            size (last - first),
            children (sourceNode.children.begin() + first, sourceNode.children.begin() + last + 1),
            data (sourceNode.data.begin() + first, sourceNode.data.begin() + last),
            subTreeSize (sourceNode.subTreeSize.begin() + first, sourceNode.subTreeSize.begin() + last + 1)

        {};

        std::vector<T> data;
        std::vector<Node*> children;
        std::vector<int> subTreeSize;
        int size = 0;

        bool isLeaf() const;
        Node* split(Node* parent = nullptr, const int& posInParent = 0);
        void insert(const T& elem);
        void destroyChildren();
        int countSubTreeSize();
        T findKMax(int k);

    };

    static const int maxT = 1 << 7; //128
    int size = 0;
    Node* root;
    const T defaultElem = {};

    void deleteFromNode(Node* node, int indx);
    void movFromNext(Node* parent, int childIndx);
    void movFromPrev(Node* parent, int childIndx);
    void fixNode(Node* node, Node* parent, int childIndx);
    void eraseFromSubtree(const T& elem, Node* node, Node* parent, int childIndx);
    void eraseLeftFromLeftLeaf(Node* node, Node* parent, int childIndx = 0);

    Node* mergeSons(Node* parent, int midPos);

};

//================================================

void doCommands(int numOfCommands);

int main() {

    int numOfCommands = 0;
    std::cin >> numOfCommands;
    doCommands(numOfCommands);
    
    return 0;

}

void doCommands(int numOfCommands) {

    BTree<int> tree (-1);
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

//================================================
template <typename T>
void BTree<T>::Node::destroyChildren() {

    if (isLeaf())
        return;

    for (int i = 0; i <= size; ++i){
 
        children[i]->destroyChildren();
        delete children[i];
    }

    return;
}

template <typename T>
bool BTree<T>::Node::isLeaf() const {

        return (this->children[0] == nullptr);
}

template <typename T>
int BTree<T>::Node::countSubTreeSize() {

    if (isLeaf())
        return size;

    int subSize = subTreeSize[0];

    for (int i = 1; i <= size; ++i)
        subSize += subTreeSize[i] + 1;

    return subSize;
}

template <typename T>
typename BTree<T>::Node* BTree<T>::Node::split(Node* parent, const int& posInParent) {

    if (parent == nullptr) {

        parent = new Node;
        parent->children[0] = this;

    }

    const int midElemPos = this->size / 2;
    T midElem = this->data[midElemPos];
    Node* rightPart = new Node (*this, midElemPos + 1, this->size);
    this->data.erase(this->data.begin()+midElemPos, this->data.end());
    this->children.erase(this->children.begin()+midElemPos + 1, this->children.end());
    if (parent->size <= 0) {

        parent->data.push_back(midElem);
        parent->children.push_back(rightPart);
        parent->subTreeSize.push_back(0);

    } else {

        parent->data.insert(parent->data.begin()+posInParent, midElem);
        parent->children.insert(parent->children.begin()+posInParent + 1, rightPart);

    }
    parent->subTreeSize[posInParent] = countSubTreeSize();
    parent->subTreeSize[posInParent + 1] = rightPart->countSubTreeSize();
    ++(parent->size);
    this->size /= 2;

    return parent;

}

template <typename T>
void BTree<T>::Node::insert(const T& elem) {

    int pos = 0;
    while ((pos < this->size) && (this->data[pos] < elem)) ++pos;

    if (!this->isLeaf()) {

        if (this->children[pos]->size >= maxT*2 - 1)
            this->children[pos]->split(this, pos);

        if (pos < this->size && this->data[pos] < elem)
            ++pos;

        this->subTreeSize[pos]++;
        this->children[pos]->insert(elem);

    } else {

        if (pos == this->size)
            this->data.push_back(elem);

        else this->data.insert(this->data.begin()+pos, elem);

        this->children.push_back(nullptr);
        this->subTreeSize.push_back(0);
        ++(this->size);

    }

    return;
}

template <typename T>
T BTree<T>::Node::findKMax(int k) {

    if (isLeaf()) 
        return data[size - k];

    int numOfElems = subTreeSize[size];

    if (numOfElems >= k)
        return children[size]->findKMax(k);

    int i = size - 1;
    for (int i; numOfElems < k; --i) {

        numOfElems++;
        if (numOfElems == k)
            return data[i];

        numOfElems += subTreeSize[i];

    }

    return children[i]->findKMax(k - (numOfElems - subTreeSize[i]));

}

//----------------------------------------------------------

template <typename T>
T BTree<T>::findKMax(int k) {

    if (empty() || k > size)
        return defaultElem;

    return root->findKMax(k);

}

template <typename T>
typename BTree<T>::Node* BTree<T>::mergeSons(Node* parent, int midPos) {

    if (midPos < 0 || midPos >= parent->size)
        return nullptr;

    Node* nodeA = parent->children[midPos];
    Node* nodeB = parent->children[midPos + 1];

    Node* mergedNodes = new Node(*nodeA, 0, nodeA->size - 1);
    
    mergedNodes->data.push_back(parent->data[midPos]);
    mergedNodes->data.insert (mergedNodes->data.end(), nodeB->data.begin(), nodeB->data.end());
    mergedNodes->children.insert (mergedNodes->children.end(), nodeB->children.begin(), nodeB->children.end());
    mergedNodes->size += nodeB->size + 1;
    
    parent->data.erase(parent->data.begin() + midPos);
    parent->children.erase(parent->children.begin() + midPos + 1);
    parent->subTreeSize.erase(parent->subTreeSize.begin() + midPos + 1);

    parent->children[midPos] = mergedNodes;
    parent->subTreeSize[midPos] = mergedNodes->countSubTreeSize(); 
    parent->size--;

    return mergedNodes;
}

template <typename T>
BTree<T>::~BTree() {

    root->destroyChildren();
    delete root;

}

template <typename T>
void BTree<T>::insert(const T& elem) {

    ++size;
    if (empty()) { 

        root->data.push_back(elem);
        root->size++;
        root->children.push_back(nullptr);
        return;
    }

    if (root->size >= maxT*2 - 1)
        root = root->split();

    root->insert(elem);
    return;
}

template <typename T>
T BTree<T>::findNext(const T& elem) {

    Node* curNode = root;
    int i = 0;

    do {

        for (i = 0; i < curNode->size && curNode->data[i] < elem; ++i);

        if (i < curNode->size && curNode->data[i] == elem)
            return elem;

        if (curNode->isLeaf())
            if (i < curNode->size)
                return curNode->data[i];

            else return defaultElem;

        curNode = curNode->children[i];

    } while (curNode != nullptr);

}


template <typename T>
void BTree<T>::deleteFromNode(Node* node, int indx) {

    node->size--;
    node->data.erase(node->data.begin()+indx);
    node->children.pop_back();
    node->subTreeSize.pop_back();
}

template <typename T>
void BTree<T>::movFromNext(Node* parent, int childIndx) {

    Node* node = parent->children[childIndx];
    Node* nextNode = parent->children[childIndx+1];

    node->data.push_back(parent->data[childIndx]);
    node->children.push_back(nextNode->children[0]);
    node->subTreeSize.push_back(nextNode->subTreeSize[0]);
    node->size++;

    parent->data[childIndx] = nextNode->data[0];
    nextNode->data.erase(nextNode->data.begin());
    nextNode->children.erase(nextNode->children.begin());
    nextNode->subTreeSize.erase(nextNode->subTreeSize.begin());
    nextNode->size--;

    parent->subTreeSize[childIndx] = node->countSubTreeSize();
    parent->subTreeSize[childIndx+1] = nextNode->countSubTreeSize();

}

template <typename T>
void BTree<T>::movFromPrev(Node* parent, int childIndx) {

    Node* node = parent->children[childIndx];
    Node* prevNode = parent->children[childIndx-1];

    node->data.insert(node->data.begin(), parent->data[childIndx]);
    node->size++;
    node->children.insert(node->children.begin(), prevNode->children[prevNode->size]);
    node->subTreeSize.insert(node->subTreeSize.begin(), prevNode->subTreeSize[prevNode->size]);

    parent->data[childIndx] = prevNode->data[prevNode->size-1];
    prevNode->data.pop_back();

    prevNode->children.pop_back();
    prevNode->subTreeSize.pop_back();
    prevNode->size--;

    parent->subTreeSize[childIndx] = node->countSubTreeSize();
    parent->subTreeSize[childIndx-1] = prevNode->countSubTreeSize();

}


template <typename T>
void BTree<T>::fixNode(Node* node, Node* parent, int childIndx) {

    if (node == root || node == nullptr)
        return;

    Node* nextNode = nullptr;
    if (childIndx < parent->size)
            nextNode = parent->children[childIndx + 1];

    Node* prevNode = nullptr;
    if (childIndx > 0)
        prevNode = parent->children[childIndx - 1];


    if (childIndx != parent->size && nextNode->size > maxT - 1) {
               
        movFromNext(parent, childIndx);

    } else if (childIndx != 0 && prevNode->size > maxT - 1) {

        movFromPrev(parent, childIndx);

    } else if (childIndx != parent->size) {

        mergeSons(parent, childIndx);

        delete node;
        delete nextNode;
                
    } else {

        mergeSons(parent, childIndx - 1);

        delete node;
        delete prevNode;
    }
}

template <typename T>
void BTree<T>::eraseFromSubtree(const T& elem, Node* node, Node* parent, int childIndx) {

    if (node == nullptr)
        return;

    int i = 0;
    for (i = 0; i < node->size && node->data[i] < elem; ++i);

    if (i == node->size || node->data[i] > elem) {
 
        if (!node->isLeaf())
            eraseFromSubtree(elem, node->children[i], node, i); 
        else return;

    }

    else if (node->data[i] == elem && !node->isLeaf()) {

        if (node->children[i+1] != nullptr && node->children[i+1]->data[0] == elem)
            eraseFromSubtree(elem, node->children[i+1], node, i+1);
        
        else { 

            Node* curNode = node->children[i+1];
            for (curNode; !curNode->isLeaf(); curNode = curNode->children[0]);
            node->data[i] = curNode->data[0];
            curNode->data[0] = elem;
            eraseLeftFromLeftLeaf(node->children[i+1], node, i+1);
//            eraseFromSubtree(elem, node->children[i+1], node, i+1);
        }
        
    } else if (node->isLeaf()) {

        //DELETE
        if (node->size > maxT - 1 || node == root){

            deleteFromNode(node, i);

            if (node != root)
                parent->subTreeSize[childIndx]--;

            size--;
            return;
        
        } else {

            deleteFromNode(node, i);
            parent->subTreeSize[childIndx]--;
            fixNode(node, parent, childIndx);
            
        }
        size--;
        return;
    } 

    if (node->size < maxT - 1) {

        //FIXTREE
        fixNode(node, parent, childIndx);

        if (node == root && node->size == 0) {

            root = node->children[0];
            delete node;

        }

    }
}

template <typename T>
void BTree<T>::eraseLeftFromLeftLeaf(Node* node, Node* parent, int childIndx){

    if (node->isLeaf()) {

        if (node->size > maxT - 1 || node == root){

            deleteFromNode(node, 0);
            size--;
            return;
        
        } else {

            deleteFromNode(node, 0);
            fixNode(node, parent, childIndx);
            
        }
        size--;
        return;

    } else {

        eraseLeftFromLeftLeaf(node->children[0], node);

        if (node->size < maxT - 1)
            fixNode(node, parent, childIndx);

    }
}

template <typename T>
void BTree<T>::erase(const T& elem) {

    if (empty())
        return;

    eraseFromSubtree(elem, root, nullptr, 0);

}

template <typename T>
bool BTree<T>::empty() {

    return (this->size <= 0);

}
