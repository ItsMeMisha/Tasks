#include <iostream>
#include <vector>
#include <cmath>

template <typename T>
class SegmentTree_KthZero{

public:

    SegmentTree_KthZero(const std::vector<T>& data);
    int findKthZeroOnSegment(const int left, const int right, int indxK);
    void updateElem(int indx, T newValue);


private:

    std::vector<int> tree;
    int size = 0;
    const int treeSize = 0;
    int dataZero = 0;

    void buildTree(const int rootIndx, const int left, const int right, const std::vector<T>& data);

    int getNumOfZeroesOnPrefix(int right);
    int findKthZero(const int curVertex, const int left, const int right, const int indxK);
};

//================================================

void getArray(std::vector<int>& data);
void doCommands(std::vector<int>& data);

int main() {

    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    std::vector<int> data;
    getArray(data);
    doCommands(data);

    return 0;
}

void getArray(std::vector<int>& data) {

    int size = 0;
    std::cin >> size;
    int elem = 0;

    for (int i = 0; i < size; ++i) {

        std::cin >> elem;
        data.push_back(elem);
    }

}

void doCommands(std::vector<int>& data) {

    int numOfCommands = 0;
    std::cin >> numOfCommands;

    SegmentTree_KthZero<int> tree(data);

    char command = 0;
    int left = 0;
    int right = 0;
    int param = 0;

    for (int i = 0; i < numOfCommands; ++i) {

        std::cin >> command;

        if (command == 'u') {

            std::cin >> left >> param;
            tree.updateElem(left - 1, param);

        } else if (command == 's') {

            std::cin >> left >> right >> param;
            std::cout << tree.findKthZeroOnSegment(left - 1, right - 1, param) + 1 << '\n';
        }
    }
}

//================================================

template <typename T>
SegmentTree_KthZero<T>::SegmentTree_KthZero(const std::vector<T>& data) : treeSize(data.size()*4) {

    int height = log2(data.size());
    height += (1 << height < data.size()) ? 1 : 0;

    size = 1 << height;
    std::vector<T> newData = data;
    
    for (int i = data.size(); i < size; ++i)
        newData.push_back(1);

    tree.resize(treeSize);
    tree.shrink_to_fit();
    buildTree(0, 0, size - 1, newData);

}

template <typename T>
void SegmentTree_KthZero<T>::buildTree(const int rootIndx, const int left, const int right, const std::vector<T>& data) {

    if (left > right)
        return;
    
    if (left == right) {

        tree[rootIndx] = (data[left] == 0) ? 1 : 0;

        if (left == 0)
            dataZero = rootIndx;

        return;
    }

    int mid = (left + right) / 2;
    buildTree(rootIndx*2 + 1, left, mid, data);
    buildTree(rootIndx*2 + 2, mid + 1, right, data);

    tree[rootIndx] = 0;

    if (rootIndx*2 + 1 < treeSize)
        tree[rootIndx] += tree[rootIndx*2 + 1];

    if (rootIndx*2 + 2 < treeSize)
        tree[rootIndx] += tree[rootIndx*2 + 2];    
}

template <typename T>
int SegmentTree_KthZero<T>::findKthZeroOnSegment(const int left, const int right, int indxK) {

    if (left != 0)
        indxK += getNumOfZeroesOnPrefix(left - 1);

    if (indxK > getNumOfZeroesOnPrefix(right))
        return -2;

    return findKthZero(0, 0, size - 1, indxK);
}

template <typename T>
int SegmentTree_KthZero<T>::getNumOfZeroesOnPrefix(int right) {

    if (right < 0)
        return 0;

    right += dataZero;
    int left = dataZero;

    int delta = 0;
    while (right != left) {

        if (right % 2 == 1 && right < dataZero + size - 1)
            delta += tree[right + 1];

        left -= 1; left /= 2;
        right -= 1; right /= 2;

    }

    return tree[left] - delta;
}

template <typename T>
int SegmentTree_KthZero<T>::findKthZero(const int curVertex, const int left, const int right, const int indxK){

    if (left == right)
        return left;

    if (indxK > tree[curVertex])
        return -2;
   
    int mid = (left + right) / 2;

    if (tree[curVertex*2 + 1] >= indxK)
        return findKthZero(curVertex*2 + 1, left, mid, indxK);

    else return findKthZero(curVertex*2 + 2, mid + 1, right, indxK - tree[curVertex*2 + 1]);
}

template <typename T>
void SegmentTree_KthZero<T>::updateElem(int indx, T newValue) {

    indx += dataZero;

    if (newValue == 0 && tree[indx] == 1)
        return;

    if (newValue != 0 && tree[indx] != 1)
        return;

    tree[indx] = (newValue == 0) ? 1 : 0;
    while (indx != 0) {

        indx -= 1;
        indx /= 2;
        tree[indx] += (newValue == 0) ? 1 : (-1);
    }
}

