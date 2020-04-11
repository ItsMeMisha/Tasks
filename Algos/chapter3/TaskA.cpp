#include <iostream>

#include <stdio.h>

#include <vector>
#include <utility>

template <typename T>
class BTree {

public:

    BTree() : root(new Node) {};
    ~BTree();

    void insert(const T& elem);
    T findNext(const T& elem);
    void erase(const T& elem);
    bool empty();

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
    };

    static const int maxT = 1 << 7; //128
    int size = 0;

public:

    Node* root;

    void dump(Node* node, int lvl = 0) {

        printf ("\nlvl: %d\nsize: %d\n\tnode: %p\n\tsize of node: %d\n\tsize of data: %d\n\tsize of children: %d\n", lvl, size, node, node->size, node->data.size(), node->children.size());

        if (!(node->isLeaf()))
            for (int i = 0; i < node->children.size(); ++i)
                dump (node->children[i], lvl + 1);

        return;
    }

private:

    void deleteFromNode(Node* node, int indx);
    void movFromNext(Node* parent, int childIndx);
    void movFromPrev(Node* parent, int childIndx);
    void eraseFromNode(Node* node, const int indx, Node* parent, const int childIndx);
    void fixNode(Node* node, Node* parent, int childIndx);
    void eraseFromSubtree(const T& elem, Node* node, Node* parent, int childIndx);

    Node* mergeSons(Node* parent, int midPos);
    //void eraseFromNode(Node* node, const int indx, Node* parent, const int childIndx);  
    //void eraseFromLeaf(Node* node, const int indx, Node* parent, const int childIndx);  
    std::pair<Node*, int> findElem(const T& elem);    
    //std::pair<Node*, int> findNode(const T& elem);  //returns ptr to PARENTNODE and INDEX of CHILD
                                                     //which contains ELEM


};

//================================================

int main() {

    BTree<long long> tree;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {

            tree.insert(j);

        }

    for (int i = 0; i < 8; i++)
        for (int j = i; j < 8; j++) {
        
            tree.erase(j);
            printf ("\n%d.%d:", i, j);
            tree.dump(tree.root);

        }

    tree.insert(2);
    printf ("%d\n", tree.findNext(3));

    return 0;
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
typename BTree<T>::Node* BTree<T>::Node::split(Node* parent, const int& posInParent) {

    if (parent == nullptr) {

        parent = new Node;
        parent->children[0] = this;

    }

    const int midElemPos = this->size / 2;
    T midElem = this->data[midElemPos];
    Node* rightPart = new Node (*this, midElemPos + 1, this->size);
    this->data.erase(this->data.begin()+midElemPos, this->data.end());

    if (!this->isLeaf())
        this->children.erase(this->children.begin()+midElemPos + 1, this->children.end());
   
    if (parent->size <= 0) {

        parent->data.push_back(midElem);
        parent->children.push_back(rightPart);

    } else {

        parent->data.insert(parent->data.begin()+posInParent, midElem);          //check positions!!!
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

//-----------------------------------------------------------

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

            else return curNode->data[i-1];

        curNode = curNode->children[i];

    } while (curNode != nullptr);

}

template <typename T>
std::pair<typename BTree<T>::Node*, int> BTree<T>::findElem (const T& elem) {

    Node* curNode = root;
    int i = 0;

    do {

        for (i = 0; i < curNode->size && curNode->data[i] < elem; ++i);

        if (curNode->data[i] == elem)
            return std::pair<Node*, int> (curNode, i);

        if (curNode->isLeaf())
            return std::pair<Node*, int> (nullptr, 0);

        curNode = curNode->children[i];

    } while (curNode != nullptr);

}

/*template <typename T>
std::pair<typename BTree<T>::Node*,int> BTree<T>::findNode (const T& elem) {

    Node* parentNode = nullptr;
    int childIndex = 0;
    Node* curNode = root;
    int i = 0;

    do {

        for (i = 0; i < curNode->size && curNode->data[i] < elem; ++i);

        if (curNode->data[i] == elem)
            return std::pair<Node*, int> (parentNode, childIndex);

        if (curNode->isLeaf())
            return std::pair<Node*, int> (nullptr, -1);

        parentNode = curNode;
        childIndex = i;
        curNode = curNode->children[i];

    } while (curNode != nullptr);

}

template <typename T>
void BTree<T>::eraseFromLeaf(Node* node, const int indx, Node* parent, const int childIndx){

    if (node->isLeaf()) {
        if (node->size > maxT - 1 || node == root){

            node->size--;
            node->data.erase(node->data.begin()+indx);
            node->children.pop_back();
            return;
        
        } else {

            Node* nextNode = parent->children[childIndx + 1];
            Node* prevNode = parent->children[childIndx - 1];

            if (childIndx != parent->size && nextNode->size > maxT - 1) {
                
                node->data.push_back(parent->data[childIndx]);
                parent->data[childIndx] = nextNode->data[0];
                nextNode->data.erase(nextNode->data.begin());
                nextNode->children.pop_back();
                nextNode-size--;

            } else if (childIndx != 0 && prevNode->size > maxT - 1) {

                node->data.insert(node->data.begin(), parent->data[childIndx]);
                parent->data[childIndx] = prevNode->data[prevNode->size-1];
                prevNode->data.pop_back();
                prevNode->children.pop_back();
                prevNode-size--;

            } else if (childIndx != parent->size) {

                Node* merged = mergeLeafs(parent, childIndx);

                merged->data.erase(merged->data.begin() + indx);
                merged->children.pop_back();
                merged->size--;

                delete node;
                delete nextNode;
                
            } else {

                Node* merged = mergeSons(parent, childIndx - 1);

                merged->data.erase(merged->data.begin() + indx + prevNode->size);
                merged->children.pop_back();
                merged->size--;

                delete node;
                delete prevNode;

            }
        }
    }
    return;
}

template <typename T>
void BTree<T>::eraseFromNode(Node* node, const int indx, Node* parent, const int childIndx){

    if (node->isLeaf()) {

        eraseFromLeaf(node, indx, parent, childIndx);
        return;

    } else {

        Node* curNode = node->children[indx];
        Node* prevNode = node;
        for (curNode; !curNode->isLeaf(); prevNode = curNode, curNode = curNode->children[curNode->size]);
        node->data[indx] = curNode->data[curNode->size-1];
        eraseFromLeaf(curNode, curNode->size-1, prevNode, prevNode->size);

    }
    return;
}

template <typename T>
void BTree<T>::erase(const T& elem) {

    if (empty())
        return;

    std::pair<Node*, int> ParentNChild = findNode (elem);
    if (ParentNChild.first == nullptr && ParentNChild.second == -1)
        return;

    Node* node = nullptr;

    if (ParentNChild.first == nullptr)
        node = root;
    else
        node = ParentNChild.first->children[ParentNChild.second];

    int indx = 0;
    for (indx; indx < node -> size && node->data[indx] != elem; ++indx);

    if (node->data[indx] != elem)
        return;
    
    eraseFromNode(node, indx, ParentNChild.first, ParentNChild.second);
    size--;
    return;
}*/

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
    parent->data[childIndx] = nextNode->data[0];
    nextNode->data.erase(nextNode->data.begin());
    nextNode->children.pop_back();
    nextNode-size--;

}

template <typename T>
void BTree<T>::movFromPrev(Node* parent, int childIndx) {

    Node* node = parent->children[childIndx];
    Node* prevNode = parent->children[childIndx-1];

    node->data.insert(node->data.begin(), parent->data[childIndx]);
    parent->data[childIndx] = prevNode->data[prevNode->size-1];
    prevNode->data.pop_back();
    prevNode->children.pop_back();
    prevNode-size--;

}


template <typename T>
void BTree<T>::eraseFromNode(Node* node, const int indx, Node* parent, const int childIndx) {

    if (node->isLeaf()) {

        eraseFromLeaf(node, indx, parent, childIndx);
        return;

    } else {

        Node* curNode = node->children[indx];
        Node* prevNode = node;
        for (curNode; !curNode->isLeaf(); prevNode = curNode, curNode = curNode->children[curNode->size]);
        node->data[indx] = curNode->data[curNode->size-1];
        eraseFromLeaf(curNode, curNode->size-1, prevNode, prevNode->size);

    }
    return;

}

template <typename T>
void BTree<T>::fixNode(Node* node, Node* parent, int childIndx) {

    Node* nextNode = parent->children[childIndx + 1];
    Node* prevNode = parent->children[childIndx - 1];

    if (childIndx != parent->size && nextNode->size > maxT - 1) {
                
        movFromNext(parent, childIndx);

    } else if (childIndx != 0 && prevNode->size > maxT - 1) {

        movFromPrev(parent, childIndx);

    } else if (childIndx != parent->size) {

        Node* merged = mergeSons(parent, childIndx);

        delete node;
        delete nextNode;
                
    } else {

        Node* merged = mergeSons(parent, childIndx - 1);

        delete node;
        delete prevNode;
    }

}

template <typename T>
void BTree<T>::eraseFromSubtree(const T& elem, Node* node, Node* parent, int childIndx) {

    int i = 0;
    for (i = 0; i < node->size && node->data[i] < elem; ++i);

    if (i == node->size || node->data[i] > elem) eraseFromSubtree(elem, node->children[i], node, i); 

    else if (node->data[i] == elem && !node->isLeaf()) {

        if (node->children[i+1] != nullptr && node->children[i+1]->data[0] == elem)
            eraseFromSubtree(elem, node->children[i+1], node, i+1);
        
        else { 

            Node* curNode = node->children[i+1];
            for (curNode; !curNode->isLeaf(); curNode = curNode->children[0]);
            node->data[i] = curNode->data[0];
            eraseFromSubtree(elem, node->children[i+1], node, i+1);
        }
        
    } else if (node->isLeaf()) {

        //DELETE
        if (node->size > maxT - 1 || node == root){

            deleteFromNode(node, i);
            return;
        
        } else {

            Node* nextNode = parent->children[childIndx + 1];
            Node* prevNode = parent->children[childIndx - 1];

            if (childIndx != parent->size && nextNode->size > maxT - 1) {
                
                movFromNext(parent, childIndx);
                deleteFromNode(node, i);                

            } else if (childIndx != 0 && prevNode->size > maxT - 1) {

                movFromPrev(parent, childIndx);
                deleteFromNode(node, i+1);

            } else if (childIndx != parent->size) {

                Node* merged = mergeSons(parent, childIndx);
                deleteFromNode(merged, i);

                delete node;
                delete nextNode;
                
            } else {

                Node* merged = mergeSons(parent, childIndx - 1);
                deleteFromNode(merged, i+prevNode->size);

                delete node;
                delete prevNode;
            }
        }
        return;
    } 

    if (node->size < maxT - 1) {

        //FIXTREE
        fixNode(node, parent, childIndx);

    }
}

template <typename T>
void BTree<T>::erase(const T& elem) {

    eraseFromSubtree(elem, root, nullptr, 0);

}

template <typename T>
bool BTree<T>::empty() {
    
    return (this->size <= 0);

}
