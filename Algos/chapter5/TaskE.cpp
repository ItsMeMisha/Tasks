#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    vector<vector<T>> data;
    vector<T>& operator[](const int indx);
    const vector<T>& operator[](const int indx) const;
    const int sizeX = 0;
    const int sizeY = 0;

};

//====================================================================

void getMatrix(Matrix<int>& matrix);
int getCostAndPath(const Matrix<int>& graph, vector<int>& path);
int initDynamic(Matrix<int>& dynamic, const Matrix<int>& graph, const int mask, const int vertex, Matrix<int>& path);
void findPath(vector<int>& path, const Matrix<int>& allPaths, int mask, const int pos);

int main() {

    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int size = 0;
    std::cin >> size;
    Matrix<int> graph(size, size);
    getMatrix(graph);

    vector<int> path;
    std::cout << getCostAndPath(graph, path) << '\n';
    
    for (int i = 0; i < path.size(); ++i)
        std::cout << path[i] + 1 << ' ';

    return 0;
}

void getMatrix(Matrix<int>& matrix){

    int buf = 0;
    for (int i = 0; i < matrix.sizeY; ++i)
        for (int j = 0; j < matrix.sizeX; ++j) {

            std::cin >> buf;
            if (i == j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = buf;
        }
}

int getCostAndPath(const Matrix<int>& graph, vector<int>& path) {
    
    const int maxMask = (1 << graph.sizeX) - 1;
    Matrix<int> dynamic(graph.sizeX, maxMask + 1);
    Matrix<int> allPaths(graph.sizeX, maxMask + 1);

    for (int i = 0; i <= maxMask; ++i)
        for (int j = 0; j < graph.sizeX; ++j)
            dynamic[i][j] = -1;

    for (int i = 0; i < graph.sizeX; ++i) {
        dynamic[1 << i][i] = 0;
        allPaths[1 << i][i] = i;
    }
            

    int min = initDynamic(dynamic, graph, maxMask, 0, allPaths);
    int minPos = 0;
    for (int k = 1; k < graph.sizeX; ++k) {

        int potentialMin = initDynamic(dynamic, graph, maxMask, k, allPaths);
        if (min > potentialMin) {
            min = potentialMin;
            minPos = k;
        }
    }
    findPath(path, allPaths, maxMask, minPos);
    
    return min;
}

int initDynamic(Matrix<int>& dynamic, const Matrix<int>& graph, const int mask, const int vertex, Matrix<int>& path){
    
    if (dynamic[mask][vertex] != -1)
        return dynamic[mask][vertex];

    int curMask = mask & ~(1 << vertex);
    for (int i = 0; i < graph.sizeX; ++i) {

        int vertexMask = 1 << i;

        if (curMask & vertexMask) {

            int potentialMin = initDynamic(dynamic, graph, curMask, i, path) + graph[i][vertex];
            if (dynamic[mask][vertex] < 0 || dynamic[mask][vertex] > potentialMin) {

                dynamic[mask][vertex] = potentialMin;
                path[mask][vertex] = i;
            }
        }
    }

    return dynamic[mask][vertex];
}    
void findPath(vector<int>& path, const Matrix<int>& allPaths, int mask, const int pos){

    int prevPos = allPaths[mask][pos];
    mask &= ~(1 << pos);
    if (pos != prevPos)
        findPath(path, allPaths, mask, prevPos);

    path.push_back(pos);
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
vector<T>& Matrix<T>::operator[](const int indx) {
    return data[indx];
}

template <typename T>
const vector<T>& Matrix<T>::operator[](const int indx) const {
    return data[indx];
}

