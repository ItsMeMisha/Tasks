#include <iostream>
#include <set>
#include <map>
#include <string>

class MapOfSet {

    std::map<int, std::set<long long>> dataForSets;
    std::map<long long, std::set<int>> dataForElems;

public:
    void add(long long elem, int setNum);
    void remove(long long elem, int setNum);
    void clear(int setNum);
    void listSet(int setNum);
    void listSetsOf(long long elem);
};

void doCommands(int k);

int main() {

    long long n = 0;
    int m = 0, k = 0;
    std::cin >> n >> m >> k;
    doCommands(k);

    return 0;
}

void doCommands(int k){

    MapOfSet set;
    std::string command;
    long long elem = 0;
    int setNum = 0;
    for (int i = 0; i < k; ++i) {
        
        std::cin >> command;
        if (command.compare("ADD") == 0) {
            
            std::cin >> elem;
            std::cin >> setNum;
            set.add(elem, setNum);
    
        } else if (command.compare("DELETE") == 0) {

            std::cin >> elem;
            std::cin >> setNum;
            set.remove(elem, setNum);

        } else if (command.compare("CLEAR") == 0) {

            std::cin >> setNum;
            set.clear(setNum);

        } else if (command.compare("LISTSETSOF") == 0) {

            std::cin >> elem;
            set.listSetsOf(elem);

        } else if (command.compare("LISTSET") == 0) {

            std::cin >> setNum;
            set.listSet(setNum);

        }
    }

}

void MapOfSet::add(long long elem, int setNum) {

    dataForSets[setNum].insert(elem);
    dataForElems[elem].insert(setNum);

}

void MapOfSet::remove(long long elem, int setNum) {

    dataForSets[setNum].erase(elem);
    dataForElems[elem].erase(setNum);

}

void MapOfSet::clear(int setNum) {

    for (auto elem: dataForSets[setNum])
        dataForElems[elem].erase(setNum);

    dataForSets[setNum].clear();

}

void MapOfSet::listSet(int setNum) {

    if (dataForSets[setNum].empty()) {

        std::cout << "-1" << '\n';
        return;

    }

    for (auto elem: dataForSets[setNum])
        std::cout << elem << ' ';

    std::cout << '\n';

}

void MapOfSet::listSetsOf(long long elem) {

    if (dataForElems[elem].empty()) {

        std::cout << "-1" << '\n';
        return;

    }

    for (auto setNum: dataForElems[elem])
        std::cout << setNum << ' ';

    std::cout << '\n';

}

