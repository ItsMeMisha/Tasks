#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

void readData (vector<int>& data, const int numOfNumbers);
int countMinLen(vector<int>& nails);

int main() {

    int num = 0;
    std::cin >> num;
    vector<int> nails;
    readData(nails, num);
    std::cout << countMinLen(nails);

    return 0;
}

void readData (vector<int>& data, const int numOfNumbers) {

    int buf = 0;
    for (int i = 0; i < numOfNumbers; ++i) {
        std::cin >> buf;
        data.push_back(buf);
    }
}

int countMinLen(vector<int>& nails) {

    const int numOfNails = nails.size();
    std::sort(nails.begin(), nails.end());
    vector<int> dynamic(numOfNails);
    dynamic[0] = nails[1] - nails[0] + 1;
    dynamic[1] = dynamic[0] - 1;

    for (int i = 2; i < numOfNails; ++i)
        dynamic[i] = std::min(dynamic[i-1], dynamic[i-2]) + nails[i] - nails[i-1];

    return dynamic[numOfNails - 1]; 
}

