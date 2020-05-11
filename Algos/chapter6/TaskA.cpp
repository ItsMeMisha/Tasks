#include <iostream>
#include <vector>
#include <queue>

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;

    const int sizeX = 0;
    const int sizeY = 0;

};

struct Cell {

    int x = 0;
    int y = 0;
    bool horse = false;
    
    const bool withinBorders(const Matrix<bool>& desk){
        return ((x < desk.sizeX && x >= 0) && (y < desk.sizeY && y >= 0));
    }

    const Cell operator+(const Cell& secOp) const {
        Cell res = {x + secOp.x, y + secOp.y};
        return res;
    }
};

//====================================================================

const std::vector<Cell> shift = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

int countMaxHorses(const int sizeX, const int sizeY);
int bfsForDesk(Matrix<bool>& desk, Cell coord);

int main() {

    int sizeX = 0, sizeY = 0;
    std::cin >> sizeX >> sizeY;
    if (sizeY > sizeX)
        std::swap(sizeX, sizeY);
    
    if ((sizeX == 1) || (sizeY == 1) || (sizeX == 2 && sizeY == 2))
        std::cout << sizeX * sizeY;
    else
        std::cout << countMaxHorses(sizeX, sizeY);

    return 0;
}

int bfsForDesk(Matrix<bool>& desk, Cell coord){

    std::queue<Cell> queue;
    queue.push(coord);
    desk[coord.y][coord.x] = true;
    Cell curCell;
    int res = 1;

    while (!queue.empty()) {
        Cell nextCell;
        curCell = queue.front();
        for (auto sh : shift){

            nextCell = curCell + sh;
            nextCell.horse = !curCell.horse;
            if (nextCell.withinBorders(desk) && !desk[nextCell.y][nextCell.x]){

                desk[nextCell.y][nextCell.x] = true;
                if (nextCell.horse)
                    ++res;
                queue.push(nextCell);
            }
        }
        queue.pop();
   }

    return res;
}

int countMaxHorses(const int sizeX, const int sizeY) {

    int result = 0;
    Matrix<bool> desk(sizeX, sizeY);

    for (int i = 0; i < sizeX; ++i)
        for (int j = 0; j < sizeY; ++j)
            if (!desk[j][i])
                result += bfsForDesk(desk, {i, j, true});

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

