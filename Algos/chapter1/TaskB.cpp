#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

const int maxResLen = 10000;
const int maxCount = 100;
const int unknown = 0;

void ReadPieces (std::ifstream& file, std::vector<std::string>& pieces, int& num);
void SortPieces (std::vector<std::string>& pieces, const int& num);
bool PiecesComp (const std::string pieceA, const std::string pieceB);

int main () {

    std::vector<std::string> pieces;
    int num = unknown;

    std::ifstream InPut;
    InPut.open("number.in");

    ReadPieces (InPut, pieces, num);

    InPut.close();

    SortPieces (pieces, num);
    
    std::ofstream OutPut;
    OutPut.open ("number.out");

    for (int i = 0; i < num; ++i)
        OutPut << pieces[i];

    OutPut.close();

    return 0;

}

void ReadPieces (std::ifstream& file, std::vector<std::string>& pieces, int& num) {

    std::string buf;
    file >> buf;

     while (!file.eof()) {

        pieces.push_back(buf);
        num++;
        file >> buf;

    } 

    return;

}
 
void SortPieces (std::vector<std::string>& pieces, const int& num) {

    std::sort (pieces.begin(), pieces.end(), PiecesComp);

    return;

}

bool PiecesComp (const std::string pieceA, const std::string pieceB) {

    std::string FullPiece1 = pieceA + pieceB;
    std::string FullPiece2 = pieceB + pieceA;

    return (FullPiece2.compare(FullPiece1) < 0);

}

