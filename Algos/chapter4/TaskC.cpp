#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

using ull = unsigned long long;

template <typename T>
class BIT_SUM {

public:
    BIT_SUM(const std::vector<T>& data);
    BIT_SUM(const int size);

    T countSum(int left, const int right);
    void changeElem(const T& value, const int pos);

private:

    std::vector<T> data;
    const int size = 0;

    int funcAND(const int& value);
    int funcOR(const int& value);
    T countPrefixSum(const int right);
};


//====================================================================

void readData(std::vector<int>& data);
void compressData(std::vector<int>& data);
ull countWeakness(const std::vector<int>& data);
void countNextLess(const std::vector<int>& data, std::vector<ull>& result);
void countPrevBigger(const std::vector<int>& data, std::vector<ull>& result);

int main() {

    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    std::vector<int> data;
    readData(data);
    compressData(data);
    std::cout << countWeakness(data);

    return 0;
}

void readData(std::vector<int>& data) {

    int number;
    std::cin >> number;
    int newElem = 0;

    for (int i = 0; i < number; ++i) {

        std::cin >> newElem;
        data.push_back(newElem);
    }
}

void compressData(std::vector<int>& data) {

    std::vector<int> sorted (data.begin(), data.end());
    std::sort(sorted.begin(), sorted.end());
 
    std::map<int, int> numbersMap;
    for (int i = sorted.size() - 1; i >= 0; --i)
        numbersMap[sorted[i]] = i;
 
    for (int i = 0; i < data.size(); ++i)
        data[i] = numbersMap[data[i]];    
}

ull countWeakness(const std::vector<int>& data) {

    std::vector<ull> less (data.size(), 0);
    countNextLess(data, less);
    
    std::vector<ull> bigger (data.size(), 0);
    countPrevBigger(data, bigger);

    ull weakness = 0;
    for (int i = 0; i < data.size(); ++i)
        weakness += less[i] * bigger[i];

    return weakness;
}

void countNextLess(const std::vector<int>& data, std::vector<ull>& result) {

    BIT_SUM<int> nextLess (data.size());
    for (int i = data.size() - 1; i >= 0; --i) {

        result[i] = nextLess.countSum(0, data[i] - 1);
        nextLess.changeElem(1, data[i]);
    }
}

void countPrevBigger(const std::vector<int>& data, std::vector<ull>& result) {

    int size = data.size();
    BIT_SUM<int> prevBigger (size);
    for (int i = 0; i < size; ++i) {

        result[i] = prevBigger.countSum(data[i] + 1, size - 1);
        prevBigger.changeElem(1, data[i]);
    }
}


//====================================================================

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


