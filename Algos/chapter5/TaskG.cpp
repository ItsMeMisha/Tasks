#include <iostream>
#include <stdio.h>
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

//====================================================================

void readCabels(std::vector<int>& data, const int num);
double countEffect(const std::vector<int>& cabels, const int num, const int distance);

int main() {

    int num = 0;
    int distance = 0;
    std::cin >> num >> distance;
    std::vector<int> cabels;
    readCabels(cabels, num);
    double result = countEffect(cabels, num, distance);

    (result < 0) ? printf ("impossible") : printf("%.7f", result);
    return 0;
}

double countEffect(const std::vector<int>& cabels, const int num, const int distance) {

    double result = -1;
    int max = distance + (num + 1)*5;
    Matrix<bool> dynamic(max + 1, num + 1);

    dynamic[0][10] = true;
    for (int i = 0; i < num; i++) {
        for (int j = num - 1; j >= 0; j--) {
            for (int k = 0; k + cabels[i] <= max; k++) {
                dynamic[j + 1][k + cabels[i]] = dynamic[j + 1][k + cabels[i]] | dynamic[j][k];
            }
        }
    }
 
    for (int i = 0; i <= num; i++) {
        for (int j = 0; j <= max - distance; j++) {
            if (!dynamic[i][j + distance]) continue;
            if (j > (i + 1) * 5) continue;
            result = std::max(result, double(j) / (i + 1));
        }
    }

    return result;
}

void readCabels(std::vector<int>& data, const int num) {
    
    int buf = 0;
    for (int i = 0; i < num; ++i){
        std::cin >> buf;
        data.push_back(buf);
    }
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

