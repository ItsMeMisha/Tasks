#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <iostream>

template <typename T>
class SparseTable {

    struct Matrix {

        Matrix(const int size);
        std::vector<std::vector<T>> data;
        std::vector<T>& operator[](const int indx);
        const int size;
        const int height;

    };

    const int size;
    const T& (*func)(const T&, const T&);
    Matrix values;
    void preCount(const int line = 1);
    
public:

    SparseTable(std::vector<T> data, const T& (*func)(const T&, const T&));
    T getFunctionValue(const int left, const int right);

};

struct Element {

    int data;
    int indx;

    const bool operator< (const Element& secOp) const {
        return (data < secOp.data);
    }
};

//====================================================================

void readElems(const int num, std::vector<Element>& data, std::ifstream& input);
void doCommands(const int numOfCommands, SparseTable<Element>& table, std::ifstream& input, std::ofstream& output);

int main() {

    std::ifstream input ("index-max.in", std::ifstream::in);
    int num = 0;
    input >> num;
    std::vector<Element> data;
    readElems(num, data, input);

    SparseTable<Element> table (data, std::max);
    
    std::ofstream output ("index-max.out", std::ofstream::out);
    int numOfCommands = 0;
    input >> numOfCommands;
    doCommands(numOfCommands, table, input, output);

    input.close();
    output.close();

    return 0;
}

void readElems(const int num, std::vector<Element>& data, std::ifstream& input) {

    int nextNum = 0;

    for (int i = 0; i < num; ++i) {

        input >> nextNum; 
        data.push_back({nextNum, i});

    }

}

void doCommands(const int numOfCommands, SparseTable<Element>& table, std::ifstream& input, std::ofstream& output){

    int left = 0;   
    int right = 0;

    for (int i = 0; i < numOfCommands; ++i) {

        input >> left >> right;
        output << (table.getFunctionValue(left - 1, right - 1)).indx + 1<< '\n';

    }

}
//====================================================================

template <typename T>
SparseTable<T>::Matrix::Matrix(const int size) : size(size), height(log2(size)+1) {

    data.resize(height);
    data.shrink_to_fit();
    for (auto& line : data) {

        line.resize(size);
        line.shrink_to_fit();

    }

}

template <typename T>
std::vector<T>& SparseTable<T>::Matrix::operator[](const int indx) {
    return data[indx];
}

template <typename T>
SparseTable<T>::SparseTable(std::vector<T> data, const T& (*func)(const T&, const T&)) : func(func), 
                                                                                  values(data.size()),
                                                                                  size(data.size()) {

    values[0] = data;
    preCount();

}

template <typename T>
void SparseTable<T>::preCount(const int line) {

    const int length = 1 << line;
    for (int i = 0; i < size - (length - 1); ++i)
        values[line][i] = func(values[line - 1][i], values[line - 1][i + length / 2]);

    if (line == values.height)
        return;

    preCount(line + 1);
}

template <typename T>
T SparseTable<T>::getFunctionValue(const int left, const int right) {

    int line = log2(right - left + 1);
    int length = 1 << line;

    return func(values[line][left], values[line][right - length + 1]);

}

