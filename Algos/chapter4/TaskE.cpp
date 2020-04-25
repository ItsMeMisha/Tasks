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
struct CompressedMatrix {




}

template <typename T>
class BIT_SUM {

public:
    BIT_SUM(const std::vector<T>& data);
    BIT_SUM(const int size);

    T countSum(int x1, int y1, int z1, const int x2, const int y2, const int z2);
    void changeElem(const T& value, const int x, const int y, const int z);

protected:

    std::vector<T> data;
    const int size = 0;

    int funcAND(const int& value);
    int funcOR(const int& value);
    T countPrefixSum(const int x, const int y, const int z);
};

template <typename T, typename container>
class BIT_of_BITs : private BIT_SUM{

public:
    BIT_of_BITs(const int size);

private:


};


//==========================================================

int 

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
BIT_SUM<T>::BIT_SUM(const std::vector<T>& data) : size(data.size()) {

    for (int i = 0; i < size; ++i) {
        
        this->data.push_back(0);
        for (int j = funcAND(i); j < i; ++j)
            this->data[i] += data[j];
    }
}

template <typename T>
BIT_SUM<T>::BIT_SUM(const int size) : size(size), data(size) {}

template <typename T>
int BIT_SUM<T>::funcAND(const int& value){
    return value&(value + 1);
}

template <typename T>
int BIT_SUM<T>::funcOR(const int& value){
    return value|(value + 1);
}

template <typename T>
T BIT_SUM<T>::countPrefixSum(const int right) {

    if (right < 0 || right >= size)
        return 0;

    T result = {};
    for (int i = right; i >= 0; i = funcAND(i) - 1)
        result += data[i];

    return result;
}

template <typename T>
T BIT_SUM<T>::countSum(int left, const int right) {

    if (left == 0)
        return countPrefixSum(right);

    left -= 1;
    return (countPrefixSum(right) - countPrefixSum(left));
}

template <typename T>
void BIT_SUM<T>::changeElem(const T& value, const int pos) {

    if (pos < 0 || pos >= size)
        return;

    for (int i = pos; i < size; i = funcOR(i))
        data[i] += value;
}


