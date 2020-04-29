#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const int sizeX = 0;
    const int sizeY = 0;

};

struct Item {

    int weight = 0;
    int cost = 0;
};

//====================================================================

void getItems(std::vector<Item>& items, const int numOfItems);
int getMaxNumOfItemsAndPath(const std::vector<Item>& items, Matrix<int>& dynamicMatr, std::vector<int>& path);
void initDynamicMatr(const std::vector<Item>& items, Matrix<int>& dynamicMatr);

int main() {

    int numOfItems = 0;
    int maxWeight = 0;
    std::cin >> numOfItems >> maxWeight;
    std::vector<Item> items;
    getItems(items, numOfItems);
    Matrix<int> dynamicMatr(maxWeight + 1, numOfItems + 1);
    std::vector<int> path;

    std::cout << getMaxNumOfItemsAndPath(items, dynamicMatr, path) << '\n';
    for (int i = path.size() - 1; i >= 0; --i)
        std::cout << path[i] << ' ';

    return 0;
}

void getItems(std::vector<Item>& items, const int numOfItems) {

    int weight = 0;
    for (int i = 0; i < numOfItems; ++i) {

        std::cin >> weight;
        items.push_back({weight, 0});
    }

    int cost = 0;
    for (auto& i : items) {

        std::cin >> cost;
        i.cost = cost;
    }
}

int getMaxNumOfItemsAndPath(const std::vector<Item>& items, Matrix<int>& dynamicMatr, std::vector<int>& path){

    initDynamicMatr(items, dynamicMatr);
    int itemIndx = dynamicMatr.sizeY - 1;
    int weight = dynamicMatr.sizeX - 1;
    int result = 0;

    while (dynamicMatr[itemIndx][weight] > 0) {
        if (dynamicMatr[itemIndx][weight] == dynamicMatr[itemIndx - 1][weight])
            --itemIndx;

        else {

            path.push_back(itemIndx);
            ++result; weight -= items[itemIndx-1].weight; --itemIndx;
        }
    }

    return result;
}

void initDynamicMatr(const std::vector<Item>& items, Matrix<int>& dynamicMatr) {

    for (int i = 1; i < dynamicMatr.sizeY; ++i)
        for (int w = 1; w < dynamicMatr.sizeX; ++w)
            if (w >= items[i-1].weight)
                dynamicMatr[i][w] = std::max(dynamicMatr[i - 1][w], dynamicMatr[i-1][w - items[i-1].weight] + items[i-1].cost); 
            else dynamicMatr[i][w] = dynamicMatr[i - 1][w];
}

//====================================================================

template <typename T>
Matrix<T>::Matrix(const int sizeX, const int sizeY) : sizeX(sizeX), sizeY(sizeY) {

    data.resize(sizeY);
    data.shrink_to_fit();
    for (auto& line : data) {

        line.resize(sizeX);
        line.shrink_to_fit();

    }

}

template <typename T>
std::vector<T>& Matrix<T>::operator[](const int indx) {
    return data[indx];
}

