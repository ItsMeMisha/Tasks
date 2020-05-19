#include <iostream>
#include <vector>
#include <deque>

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;

    const int sizeX = 0;
    const int sizeY = 0;

};

//====================================================================

struct InitData {

    int numOfLines = 0;
    int numOfColoumns = 0;
    int startY = 0;
    int startX = 0;
    int maxLeft = 0;
    int maxRight = 0;
};

struct Position {

    Position(const int& posX, const int& posY, const int& leftLeft, const int& rightLeft) :
        posX(posX), posY(posY), leftLeft(leftLeft), rightLeft(rightLeft) {};

    int posX = 0;
    int posY = 0;
    int leftLeft = 0;
    int rightLeft = 0;

};

void readInitData(InitData& data);
void readMaze(Matrix<bool>& maze);
int bfs(const InitData& data, const Matrix<bool>& maze);

int main() {

    InitData data = {};
    readInitData(data);
    Matrix<bool> maze (data.numOfColoumns, data.numOfLines);
    readMaze(maze);
    std::cout << bfs (data, maze);

    return 0;
}

void readInitData(InitData& data){

    int numOfLines = 0, numOfColoumns = 0, startY = 0, startX = 0, maxLeft = 0, maxRight = 0;
    std::cin >> numOfLines >> numOfColoumns >> startY >> startX >> maxLeft >> maxRight;
    data = {numOfLines, numOfColoumns, startY - 1, startX - 1, maxLeft, maxRight};
}

void readMaze(Matrix<bool>& maze) {

    char pos = 0;
    for (int i = 0; i < maze.sizeY; ++i)
        for (int j = 0; j < maze.sizeX; ++j) {
            std::cin >> pos;
            if (pos == '.')
                maze[i][j] = true;
            else maze[i][j] = false;
        }
}

int bfs(const InitData& data, const Matrix<bool>& maze) {

    int result = 1;
    
    Matrix<bool> visited (maze.sizeX, maze.sizeY);

    std::deque<Position> cells;
    Position curPos = {data.startX, data.startY, data.maxLeft, data.maxRight};
    visited[curPos.posY][curPos.posX] = true;
    cells.push_back(curPos);

    while (!cells.empty()) {
        curPos = cells.front();
        cells.pop_front();
        
        if (curPos.posY > 0 && !visited[curPos.posY - 1][curPos.posX] && maze[curPos.posY - 1][curPos.posX]) {
            visited[curPos.posY - 1][curPos.posX] = true;
            ++result;
            cells.emplace_front(curPos.posX, curPos.posY - 1, curPos.leftLeft, curPos.rightLeft);
        }

        if (curPos.posY < maze.sizeY - 1 && !visited[curPos.posY + 1][curPos.posX] && maze[curPos.posY + 1][curPos.posX])  {

            visited[curPos.posY + 1][curPos.posX] = true;
            ++result;
            cells.emplace_front(curPos.posX, curPos.posY + 1, curPos.leftLeft, curPos.rightLeft);
        }

        if (curPos.posX > 0 && !visited[curPos.posY][curPos.posX - 1] && maze[curPos.posY][curPos.posX - 1]) {

            if (curPos.leftLeft > 0) {
                visited[curPos.posY][curPos.posX - 1] = true;
                ++result;
                cells.emplace_back(curPos.posX - 1, curPos.posY, curPos.leftLeft - 1, curPos.rightLeft);
            }
        }

        if (curPos.posX < maze.sizeX - 1 && !visited[curPos.posY][curPos.posX + 1] && maze[curPos.posY][curPos.posX + 1]) {
            if (curPos.rightLeft > 0) {
                visited[curPos.posY][curPos.posX + 1] = true;
                ++result;
                cells.emplace_back(curPos.posX + 1, curPos.posY, curPos.leftLeft, curPos.rightLeft - 1);
            }
        }
    }

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



