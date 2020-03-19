#include <iostream>
#include <vector>

template <typename T>
class BTree {

public:

    BTree() : root(new Node);
    BTree(int newT) : maxT(newT), root(new Node) {};

    ~BTree();

    void insert(const T& elem);
    T findNext(const T& elem);
    void erase(const T& elem);
    bool empty();

private:

    struct Node {

        Node() = default;

        Node(const Node& sourceNode, const int& first, const int& last) :

            size (last - first + 1),
            data (sourceNode.data, sourceNode.data.begin() + first, sourceNode.data.begin() + last),
            children (sourceNode.children, sourceNode.children.begin() + first, sourceNode.children.begin() + last + 1)

        {};

        std::vector<T> data;
        std::vector<Node*> children;
        int size = 0;

        bool isLeaf();
        Node* split(Node* parent = nullptr, const int& posInParent = 0);
        void insert(const T& elem);

    };

    Node* root;
    int maxT = 1 << 7; //128
    int size = 0;

    

};

int main() {


    return 0;
}


//============================================

template <typename T>
bool BTree::Node::isLeaf() {

        return (this->children == nullptr);

}

template <typename T>
Node* BTree::Node::split(Node* parent, const int& posInParent) {

    if (parent == nullptr) {

        parent = new Node;
        parent->children.push_back(this);

    }

    const int midElemPos = this->size / 2;
    T midElem = this->data[midElemPos];
    Node* rightPart = new Node (*this, midElemPos + 1, this->size - 1);
    this->data.erase(this->data.begin()+midElemPos, this->data.end());

    if (!this->isLeaf())
        this->children.erase(this->children.begin()+midElemPos + 1, this->children.end());
    
    parent->data.insert(midElem, parent->data.begin()+posInParent);          //check positions!!!
    parent->children.insert(rightPart, parent->children.begin()+posInParent + 1);
    ++(parent->size);

    return parent;

}

template (typename T>
void BTree::Node::insert(const T& elem) {

    int pos = 0;
    while ((pos < this->size) && (this->data[pos] < elem)) ++pos;

    if (!this->isLeaf()) {

        if (this->children[pos]->size >= maxT*2 - 1)
            this->children[pos]->split(this, pos);

        if (this->data[pos] < elem)
            ++pos;

        this->children[pos].insert(elem);

    } else {

        this->data.insert(elem, this->data.begin()+pos);
        ++(this->size);

    }

    return;
}

template <typename T>
BTree::~BTree() {


}

template <typename T>
void BTree::insert(const T& elem) {

    ++size;

    if (root->size >= maxT*2 - 1)
        root = root->split();

    root.insert(elem);

    return;
}

template <typename T>
T BTree::findNext(const T& elem) {

}

template <typename T>
void BTree::erase(const T& elem) {


    return;
}

template <typename T>
bool BTree::empty() {
    
    return (this->size <= 0);

}
