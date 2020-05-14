#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;
    void set(const T& value);

    const int sizeX = 0;
    const int sizeY = 0;

};

//====================================================================

using std::vector;
using std::pair;

struct InitialData {

    int numOfCities = 0;
    int numOfFlights = 0;
    int numOfNights = 0;
    int home = 0;
    int conference = 0;
};

using Graph = vector<vector<pair<int, int>>>;

void readInitial(InitialData& data);
void readGraph(Graph& cities, const InitialData& data);
int findMinWay(const Graph& cities, const InitialData& data);

int main() {

    InitialData data = {};
    readInitial(data);

    Graph cities(data.numOfCities);
    readGraph(cities, data);
    std::cout << findMinWay(cities, data);    

    return 0;
}

void readInitial(InitialData& data) {

    int numOfCities = 0, numOfFlights = 0, numOfNights = 0, home = 0, conference = 0;
    std::cin >> numOfCities >> numOfFlights >> numOfNights >> home >> conference;
    data = {numOfCities, numOfFlights, numOfNights, home - 1, conference - 1};
}

void readGraph(Graph& cities, const InitialData& data) {

    for (int i = 0; i < data.numOfFlights; ++i) {
        int vertex1 = 0, vertex2 = 0, cost = 0;
        std::cin >> vertex1 >> vertex2 >> cost;
        cities[vertex1 - 1].emplace_back (vertex2 - 1, cost);
    }
}

int findMinWay(const Graph& cities, const InitialData& data) {

    Matrix<int> dynamic(data.numOfNights + 1, data.numOfCities);
    dynamic.set(-1);
    dynamic[data.home][0] = 0;

    for (int i = 1; i <= data.numOfNights; ++i)
        for (int from = 0; from < data.numOfCities; ++from)
            for (auto to : cities[from])
                if ((dynamic[from][i-1] + to.second < dynamic[to.first][i] || dynamic[to.first][i] < 0) && dynamic[from][i-1] >= 0)
                    dynamic[to.first][i] = dynamic[from][i-1] + to.second;

    int result = dynamic[data.conference][data.numOfNights];
    for (int i = data.numOfNights - 1; i > 0; --i)
        if ((dynamic[data.conference][i] < result || result == -1) && dynamic[data.conference][i] > 0)
            result = dynamic[data.conference][i];

    return result;
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

template <typename T>
const std::vector<T>& Matrix<T>::operator[](const int indx) const {
    return data[indx];
}

template <typename T>
void Matrix<T>::set(const T& value) {

    for (auto& string : data)
        for (auto& elem : string)
            elem = value;
}
