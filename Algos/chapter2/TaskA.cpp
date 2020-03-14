#include <iostream>
#include <vector>
#include <string>

#define DEBUG

#ifdef DEBUG

    #define debug(code) code;

#else

    #define debug(code) code;

#endif

template <typename T>
struct TreeNode {

    TreeNode() = default;
    TreeNode(const T data) {
        this->data = data;
    }

    T getData() const { 
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
        return next;
    }

    void setNext(TreeNode* next){
        this->next = next;
        return;
    }

    const int compare(const TreeNode& secondNode) const {

        if (this->getData() < secondNode.getData())
            return -1;

        else if (this->getData() == secondNode.getData())
            return 0;

        else return 1;

    }

    int compare(const TreeNode& secondNode, int (*comp)(const T& elem1, const T& elem2)) const {
        return comp(this->data, secondNode.getNode());
    }

    short getSonsNum(){
        return sonsNum;
    }

    void incSons(){
        ++sonsNum;
        return;
    }

private:
    T data;
    short sonsNum = 0;
    TreeNode* LeftSon = nullptr;
    TreeNode* RightSon = nullptr;
    TreeNode* next = nullptr;

};

template <typename T>
class BinomialTree {

public:
    BinomialTree(TreeNode<T>* root){

        this->root = root;
        size = sizeFromRoot(root);

    }

    int getSize(){
        return this->size;
    }

    TreeNode<T>* getRoot(){
        return this->root;            //unsafe!!!
    }

    void merge(BinomialTree<T>& secondTree) {

        printf ("%p\t%p\n", this, &secondTree);

        if (this->size != secondTree.getSize()){
            debug(printf ("NOT EQUAL SIZES");)
            return;
        }

        if (this->root->compare(*(secondTree.getRoot())) <= 0)
            this->link(this->root, secondTree.getRoot());

        else secondTree.link(secondTree.getRoot(), this->root);

        this->size <<= 1;
        ++Deg;

    }

    short getDeg (){
        return Deg;
    }

private:

    void link(TreeNode<T>* rootRec, TreeNode<T>* rootNew) {   //always linking new node to the left side

        if (rootRec->getLeftSon() == nullptr || rootRec->getRightSon() == nullptr){     // || ???

            rootRec->setLeftSon(rootNew);
            rootRec->setRightSon(rootNew);
            rootRec->incSons();
            return;

        }

        rootNew->setNext(rootRec->getLeftSon());
        rootRec->getRightSon()->setNext(rootNew);
        rootRec->setLeftSon(rootNew);

        rootRec->incSons();
        return;

    }

    int sizeFromRoot(TreeNode<T>* root) {

        return 1 << root->getSonsNum();

    }

    int size;
    short Deg = 0;
    TreeNode<T>* root;

};


template <typename T>
class BinomialHeap {

public:

    BinomialHeap() {

        for (int i = 0; i < vectSize; ++i)
            trees.push_back(nullptr);

    }

    void insert(T elem){

        ++size;

        TreeNode<T>* newRoot = new TreeNode<T>(elem);
        BinomialTree<T>* newTree = new BinomialTree<T>(newRoot);

        addTreeToVect (*newTree, trees);

        definedMin = false;

        return;
    }

    T getMin(){

        T min;

        if (this->empty())
            return min;

        if (definedMin) {
            min = trees[minId]->getRoot()->getData();
            return min;
        }


        int i = 0;
        while ((trees[i] == nullptr) && i < vectSize)
            ++i;
            
        min = trees[i]->getRoot()->getData();
        short curId = i;

        for (i; i < vectSize; ++i)

            if (trees[i] != nullptr)
                if (trees[i]->getRoot()->getData() < min) {

                    min = trees[i]->getRoot()->getData();
                    curId = i;

                }

        minId = curId;
        definedMin = true;
        return min;

    }

    void extractMin(){

        if (this->empty())
            return;

        getMin();

        removeRoot (minId);
        --size;

        return;
    }

    bool empty(){

        if (size <= 0)
            return true;
    
        return false;

    }

    ~BinomialHeap(){ 

        for (int i = 0; i < size; ++i)
            extractMin(); 

    }

private:

    int size = 0;
    short vectSize = 32;
    short minId;
    bool  definedMin = true;
    std::vector<BinomialTree<T>*> trees;

    void addTreeToVect (BinomialTree<T>& newTree, std::vector<BinomialTree<T>*>& newTrees) {

        int vecSize = newTrees.size();
        
        int i = 0;
        for (i = 0; (i < vecSize) && (newTrees[i] != nullptr); ++i)
            newTree.merge(*(newTrees[i]));

        if (i < vectSize)
            newTrees[i] = &newTree;

        else newTrees.push_back(&newTree);

        return;
    }

    void addVectOfTrees (std::vector<BinomialTree<T>*>& newTrees) {

        for (int i = newTrees.size()-1; i >= 0; --i) {

//            if (newTrees[i] != nullptr)
                addTreeToVect (*(newTrees[i]), this->trees);

        }
        
        return;
    }

    void removeRoot (short idInTrees) {

        std::vector<BinomialTree<T>*> bufTrees (vectSize, nullptr);

        TreeNode<T>* cur = trees[idInTrees]->getRoot()->getLeftSon();
        TreeNode<T>* removing = trees[idInTrees]->getRoot();
        delete removing;

        if (cur == nullptr)
            return;

        for (int i = trees[idInTrees]->getDeg()-1; i >= 0; --i) {

            bufTrees[i] = new BinomialTree<T> (cur);
            cur = cur->getNext();

        }

        addVectOfTrees (bufTrees);

        for (int i = trees[idInTrees]->getDeg()-1; i >= 0; --i)
            delete bufTrees[i];

        return;
    }
    
};

void GetInput(std::vector<std::string>& stream, const int& num, std::vector<int>& args, int& resNum);
void PrintResult(std::vector<std::string>& stream, std::vector<int>& args, const int& num);

int main() {

    std::vector<std::string> stream;
    std::vector<int> args;
    int num = 0;
    std::cin >> num;
    int resNum = num;
    GetInput (stream, num, args, resNum);
    PrintResult (stream, args, resNum);

    return 0;

}

void GetInput(std::vector<std::string>& stream, const int& num, std::vector<int>& args, int& resNum){

    std::string buf;
    int arg;
    
    BinomialHeap<int> heap;

    for (int i = 0; i < num; ++i){

        std::cin >> buf;

        if (buf.compare ("removeMin") == 0) { //name of cmd is REMOVE!!!!!!!!!!

            stream.push_back(buf);
            args.push_back(0);
            heap.extractMin();
            continue;
        }

        std::cin >> arg;

        if (buf.compare ("insert") == 0) {

            heap.insert (arg);

        } else

        if (buf.compare ("getMin") == 0) {

            if (heap.empty()) {

                stream.push_back("insert");
                args.push_back(arg);
                heap.insert(arg);
                ++resNum;
                stream.push_back(buf);
                args.push_back(arg);

                continue;
            }

            int curMin = heap.getMin();
            while (curMin < arg) {

                heap.extractMin();

                stream.push_back("removeMin");
                args.push_back(0);
                curMin = heap.getMin();
                ++resNum;

            }

            if (curMin != arg) {

                stream.push_back("insert");
                args.push_back(arg);
                heap.insert(arg);
                ++resNum;

            }   
                
        }

        stream.push_back(buf);
        args.push_back(arg);

    }    

    return;
}

void PrintResult(std::vector<std::string>& stream, std::vector<int>& args, const int& num){

    std::cout << num << '\n';

    for (int i = 0; i < num; ++i) {

        std::cout << stream[i];
        if (stream[i].compare("removeMin") != 0)
            std::cout << ' ' << args[i];

        std::cout << '\n';

    }

    return;
}


