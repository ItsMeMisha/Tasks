#include <iostream>

#include <stdio.h>

#include <vector>
#include <utility>
#include <algorithm>

template <typename T>
class BTree {

public:

    BTree() : root(new Node) {};
    BTree(const T& defElem) : root(new Node), defaultElem(defElem) {};
    ~BTree();

    void insert(const T& elem);
    T findNext(const T& elem);
    void erase(const T& elem);
    bool empty();
    void dump() {
        dump(root);
    }

private:

    struct Node {

        Node() {

            children.push_back(nullptr);

        }

        Node(const Node& sourceNode, const int& first, const int& last) :

            size (last - first),
            children (sourceNode.children.begin() + first, sourceNode.children.begin() + last + 1),
            data (sourceNode.data.begin() + first, sourceNode.data.begin() + last)

        {};

        std::vector<T> data;
        std::vector<Node*> children;
        int size = 0;

        bool isLeaf() const;
        Node* split(Node* parent = nullptr, const int& posInParent = 0);
        void insert(const T& elem);
        void destroyChildren();

        void dump() {

            printf ("Node: %p; size: %d {", this, size);
    
            for (int i = 0; i < size && i < 10; ++i)
                printf ("%d ", data[i]);

//            printf ("\n");

            /*for (int i = 0; i <=size; ++i)
                printf ("%p ", children[i]);*/
    
            printf ("}\n");

            }

    };

    static const int maxT = 1 << 7; //128
    int size = 0;
    Node* root;
    const T defaultElem;

    void dump(Node* node, int lvl = 0) {

        if (node == nullptr)
            return;

        for (int i = 0; i < lvl; ++i) printf ("\t");
/*        printf ("Node: %p; size: %d {", node, node->size);

        for (int i = 0; i < node->size; ++i)
            printf ("%d ", node->data[i]);

        printf ("}\n");*/
        node->dump();
        for (int i = 0; i <= node->size; ++i)
            dump(node->children[i], lvl + 1);

    }

    void deleteFromNode(Node* node, int indx);
    void movFromNext(Node* parent, int childIndx);
    void movFromPrev(Node* parent, int childIndx);
    void fixNode(Node* node, Node* parent, int childIndx);
    void eraseFromSubtree(const T& elem, Node* node, Node* parent, int childIndx);
    void eraseLeftFromLeftLeaf(Node* node, Node* parent, int childIndx = 0);

    Node* mergeSons(Node* parent, int midPos);

};

//================================================

void Test(int num) {

    BTree<int> tree (-1);
    std::vector<int> results;
    char command = 0;
    int param = 0;
    char prevCom = 0;
    int prevRes = 0;
    int curMax = 2;

    for (int i = 0; i < num; ++i) {
        command = rand() % 2;
        param = rand() %1000000000;
        if (i % 100 == 0)
            printf ("%d: \n", i);

        if (command == 1) {

      //      printf ("+ %d\n", param);
            
            if (prevCom == 0) {

                tree.insert((param + prevRes) % 1000000000);

            } else {

                tree.insert(param);

            }

            if (param > curMax) curMax = param;

        }

        else if (command == 0) {

        //    printf ("? %d\n", param);

            if (param > curMax)
                param = curMax - 1;

            int next = tree.findNext (param);
            prevRes = next;
            results.push_back(next);

        }
        prevCom = command;
    }

//    for (int i = 0; i < results.size(); i++)
//        std::cout << results[i] << '\n';

//    tree.dump();

}

void doCommands (int numberOfCommands);

int main() {

/*    BTree<int> tree (-1);
    for (int i = 0; i < 64; ++i)
        for (int j = i; j < 64; ++j)
            tree.insert(j);


    for (int i = 0; i < 64; ++i)
        for (int j = i; j < 20; ++j) {

            printf ("I: %d J: %d; ", i, j);
            tree.checkLeftElem();
            tree.erase(j);

        }

//tree.dump();
    printf ("findNext(1): %d\n", tree.findNext(1)); */

//    int numOfCommands = 0;
//   std::cin >> numOfCommands;
//    doCommands (numOfCommands);

    Test(300000);

    return 0;
}

void doCommands (int numberOfCommands) {

    BTree<int> tree (-1);
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

//================================================



template <typename T>
void BTree<T>::Node::destroyChildren() {

    if (isLeaf())
        return;
//dump();

    for (int i = 0; i <= size; ++i){
 
//        printf ("%d: %p\n", i, children[i]);
       
        children[i]->destroyChildren();
        delete children[i];
    }
//    printf ("\t\tEnd\n");


    return;
}

template <typename T>
bool BTree<T>::Node::isLeaf() const {

        return (this->children[0] == nullptr);
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

    } else {

        parent->data.insert(parent->data.begin()+posInParent, midElem);
        parent->children.insert(parent->children.begin()+posInParent + 1, rightPart);

    }
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

        this->children[pos]->insert(elem);

    } else {

        if (pos == this->size)
            this->data.push_back(elem);

        else this->data.insert(this->data.begin()+pos, elem);

        this->children.push_back(nullptr);
        ++(this->size);

    }

    return;
}

//----------------------------------------------------------

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
    parent->children[midPos] = mergedNodes;
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
}

template <typename T>
void BTree<T>::movFromNext(Node* parent, int childIndx) {

    Node* node = parent->children[childIndx];
    Node* nextNode = parent->children[childIndx+1];

    node->data.push_back(parent->data[childIndx]);
    node->size++;
    node->children.push_back(nextNode->children[0]);

    parent->data[childIndx] = nextNode->data[0];
    nextNode->data.erase(nextNode->data.begin());
    nextNode->children.erase(nextNode->children.begin());
    nextNode->size--;

}

template <typename T>
void BTree<T>::movFromPrev(Node* parent, int childIndx) {

    Node* node = parent->children[childIndx];
    Node* prevNode = parent->children[childIndx-1];

    node->data.insert(node->data.begin(), parent->data[childIndx]);
    node->size++;
    node->children.insert(node->children.begin(), prevNode->children[prevNode->size]);


    parent->data[childIndx] = prevNode->data[prevNode->size-1];
    prevNode->data.pop_back();

    prevNode->children.pop_back();
    prevNode->size--;

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
        }
        
    } else if (node->isLeaf()) {

        //DELETE
        if (node->size > maxT - 1 || node == root){

            deleteFromNode(node, i);
            size--;
            return;
        
        } else {

            deleteFromNode(node, i);
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
