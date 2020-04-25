#include <iostream>
#include <vector>

template <typename T>
struct Matrix {

    Matrix(const int size);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const int size = 0;

};


template <typename T>
struct Matrix3D {

public:
    Matrix3D (const int& size);
    Matrix<T>& operator[](const int indx);

private:
    std::vector<Matrix<T>> data;
    const int size = 0;

};

template <typename T>
class BIT_3D_SUM {

public:
    BIT_3D_SUM(const Matrix3D<T>& data);
    BIT_3D_SUM(const int size);

    T countSum(int x1, int y1, int z1, const int x2, const int y2, const int z2);
    void changeElem(const T& value, const int x, const int y, const int z);

private:

    Matrix3D<T> data;
    const int size = 0;

    int funcAND(const int& value);
    int funcOR(const int& value);
    T countPrefixSum(const int x, const int y, const int z);
};

//==========================================================

void doCommands(const short n);

int main() {
    
    short n = 0;
    std::cin >> n;
    doCommands(n);

    return 0;
}

void doCommands(const short n){

    BIT_3D_SUM<long long> sky(n);

    short command = 0;
    short x1 = 0, x2 = 0;
    short y1 = 0, y2 = 0;
    short z1 = 0, z2 = 0;
    int k = 0;

    std::cin >> command;
    while (command != 3) {

        if (command == 1) {

            std::cin >> x1 >> y1 >> z1 >> k;
            sky.changeElem(k, x1, y1, z1);

        } else if (command == 2) {

            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            std::cout << sky.countSum(x1, y1, z1, x2, y2, z2) << '\n';

        }

        std::cin >> command;
    }
}

//==========================================================

template <typename T>
Matrix<T>::Matrix(const int size) : size(size) {

    data.resize(size);
    data.shrink_to_fit();
    for (auto& line : data) {

        line.resize(size);
        line.shrink_to_fit();

    }

}

template <typename T>
std::vector<T>& Matrix<T>::operator[](const int indx) {
    return data[indx];
}

template <typename T>
Matrix3D<T>::Matrix3D(const int& size) : size(size) {

    for (int i = 0; i < size; i++)
        data.push_back(size);
    data.shrink_to_fit();
}

template <typename T>
Matrix<T>&  Matrix3D<T>::operator[] (const int indx) {
    return data[indx];
}

template <typename T>
BIT_3D_SUM<T>::BIT_3D_SUM(const Matrix3D<T>& data) : size(data.size()) {

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k) {

                this->data[i][j][k] = 0;
                for (int l = funcAND(i); l < i; l++)
                    for (int m = funcAND(j); m < j; m++)
                        for (int n = funcAND(k); n < k; n++)
                            this->data[i][j][k] += data[l][m][n];
            }
}

template <typename T>
BIT_3D_SUM<T>::BIT_3D_SUM(const int size) : size(size), data(size) {}

template <typename T>
int BIT_3D_SUM<T>::funcAND(const int& value){
    return value&(value + 1);
}

template <typename T>
int BIT_3D_SUM<T>::funcOR(const int& value){
    return value|(value + 1);
}

template <typename T>
T BIT_3D_SUM<T>::countPrefixSum(const int x, const int y, const int z) {

    T result = {};
    for (int i = x; i >= 0; i = funcAND(i) - 1)
        for (int j = y; j >= 0; j = funcAND(j) - 1)
            for (int k = z; k >= 0; k = funcAND(k) - 1)
                result += data[i][j][k];

    return result;
}

template <typename T>
T BIT_3D_SUM<T>::countSum(int x1, int y1, int z1, const int x2, const int y2, const int z2) {

    if (x1 == 0 && y1 == 0 && z1 == 0)
        return countPrefixSum(x2, y2, z2);

    x1 -= 1; y1 -= 1; z1 -= 1;

    T result = countPrefixSum(x2, y2, z2) - countPrefixSum(x2, y2, z1) - countPrefixSum(x2, y1, z2) - countPrefixSum(x1, y2, z2);
    result += countPrefixSum(x1, y1, z2) + countPrefixSum(x1, y2, z1) + countPrefixSum(x2, y1, z1);
    result -= countPrefixSum(x1, y1, z1); 
    return result;

}

template <typename T>
void BIT_3D_SUM<T>::changeElem(const T& value, const int x, const int y, const int z) {

    for (int i = x; i < size; i = funcOR(i))
        for (int j = y; j < size; j = funcOR(j))
            for (int k = z; k < size; k = funcOR(k))
                data[i][j][k] += value;
}

