#include <iostream>

#define DEBUG

#ifdef DEBUG

    #define debug(code) code;

#else

    #define debug(code) code;

#endif

template <typename T>
struct TreeNode<T> {

    TreeNode() = default;
    TreeNode(const T data) {
        this->data = data;
    }

    T getData() {
        return data;
    }

    TreeNode* getLeftSon(){
        return LeftSon;
    }

    void setLeftSon (TreeNode* LeftSon){
        this->LeftSon = LeftSon;
        return;
    }

    TreeNode* getRightSon(){
        return LeftSon;
    }

    void setRightSon (TreeNode* RightSon){
        this->RightSon = RightSon;
        return;
    }

    TreeNode* getNext(){
        return next();
    }

    void setRight(TreeNode* next){
        this->next = next;
        return;
    }

    int compare(const TreeNode& secondNode) {

        if (this->data < secondNode.getData())
            return -1;

        else if (this->data == secondNode.getData())
            return 0;

        else return 1;

    }

    int compare(const TreeNode& secondNode, int (*comp)(const T& elem1, const T& elem2)){
        return comp(this->data, secondNode.getNode());
    }

    void incSons(){
        ++sonsNum;
        return;
    }

private:
    T data;
    int sonsNum = 0;
    TreeNode* LeftSon = nullptr;
    TreeNode* RightSon = nullptr;
    TreeNode* next = nullptr;

}

template <typename T>
class BinomialTree<T> {

public:
    BinomialTree<T>(){


    }

    int getSize(){
        return size;
    }

    TreeNode<T>* getRoot(){
        return root;            //unsafe!!!
    }

    void merge(BinomialTree<T>& secondTree) {

        if (this->size != secondTree.getSize()){
            debug(printf ("NOT EQUAL SIZES");)
            return;
        }

        if (this->root->compare(*(secondTree.getRoot)) <= 0) {
            this->link(this->root, secondTree.getRoot());

        else secondTree.link(secindTree.getRoot(), this->root);

        this->size <<= 1;

    }

private:

    void link(TreeNode<T>* rootRec, TreeNode<T>* rootNew) {   //always linking new node to the left side

        if (rootRec->getLeftSon() == nullptr || rootRec->getRightSon() == nullptr){     // || ???

            rootRec->setLeftSon(rootNew);
            rootRec->setRightSon(rootNew);
            rootRec->incSons();
            return;

        }

        rootNew->setNext(rootRec->getLeft());
        rootRec->getRightSon()->setNext(rootNew);
        rootRec>setLeftSon(rootNew);

        rootRec->incSons();
        return;

    }

    int size;
    TreeNode<T>* root;

}


template <typename T>
class BinomialHeap<T> {

public:

    int size;
    
}

int main() {



    return 0;

}
