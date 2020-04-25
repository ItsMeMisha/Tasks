#include <iostream>
#include <set>
#include <queue>

struct Car {

    int number;
    int freq = 0;

    const bool operator< (const Car& op) const {
        return this->freq < op.freq;
    }

    const bool operator== (const Car& op) const {
        return (number == op.number);
    }

};

int countActions(int numOfCars, int maxFloorNum, int numOfRequests);

int main() {

    int numOfCars = 0, maxFloorNum = 0, numOfRequests = 0;
    std::cin >> numOfCars >> maxFloorNum >> numOfRequests;
    std::cout << countActions(numOfCars, maxFloorNum, numOfRequests);

    return 0;
}

int countActions(int numOfCars, int maxFloorNum, int numOfRequests) {

    int numOfActions = 0;

    std::queue<int> requests;
    int* carsFreq = new int[numOfCars];
    int carNum = 0;

    for (int i = 0; i < numOfRequests; ++i) {

        std::cin >> carNum;
        carsFreq[carNum - 1]++;
        requests.push(carNum - 1);

    }

    std::set<Car> carsOnFloor;
    Car buf = {};
    for (int i = 0; i < numOfRequests; ++i) {

        int curCar = requests.front();
        requests.pop();
        buf.number = curCar;
        buf.freq = carsFreq[curCar];
        std::set<Car>::iterator thisCar = carsOnFloor.find(buf);
        if (thisCar == carsOnFloor.end()) {

            if (carsOnFloor.size() >= maxFloorNum) {

                carsOnFloor.erase(carsOnFloor.begin());
            }

            buf.freq--;
            carsOnFloor.insert(buf);
            numOfActions++;

        } else {

            carsOnFloor.erase(thisCar);
            buf.freq--;
            carsOnFloor.insert(buf);
        }

        carsFreq[curCar]--;

    }    

    delete[] carsFreq;
    return numOfActions;
}

