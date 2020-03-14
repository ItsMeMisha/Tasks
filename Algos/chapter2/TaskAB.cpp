#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

#include <stdio.h>

typedef long long ll;

template <typename T>
class BinaryHeap {

public:

    void insert (const T& elem) {

        size++;
        data.push_back(elem);
        LiftUpInserted();

    }

    T getMin () {

        if (empty()) {

            T smth = {};
            return smth;

        }

        return (data[0]);

    }

    void extractMin () {

        if (empty())
            return;

        data[0] = data[size - 1];
        data.pop_back();

        --size;
        if (!empty())
            heapify();

        return;
    }

    bool empty() {

        return (size <= 0);

    }

private:

    ll size = 0;
    std::vector<T> data;

    void heapify(ll nodeIndx = 0) {

        ll left = nodeIndx*2 + 1;
        ll right = left + 1;
        ll minIndx = 0;

        if (left >= size)
            return;

        if (right >= size)
            minIndx = left;

        else {

            minIndx = (data[left] < data[right]) ? left : right;

            if (data[minIndx] >= data[nodeIndx])
                return;

        }

        if (data[minIndx] < data[nodeIndx])
            std::swap(data[minIndx], data[nodeIndx]);

        heapify(minIndx);
        return;

    }

    void LiftUp(ll nodeIndx) {

        if (nodeIndx < 1)
            return;

        if (data[(nodeIndx-1)/2] > data[nodeIndx]) {

            std::swap (data[(nodeIndx -1)/2], data[nodeIndx]);
            LiftUp ((nodeIndx-1)/2);

        }

        return;

    }

    void LiftUpInserted() {

        LiftUp(size-1);
        return;

    }

};

struct Command {

    std::string name;
    ll arg;

};

void GetInput(std::vector<Command>& stream, const ll& num);
void PrintResult(std::vector<Command>& stream);

int main() {

    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    std::vector<Command> stream;
    ll num = 0;
    std::cin >> num;
    GetInput (stream, num);
    PrintResult (stream);

    return 0;

}

void GetInput(std::vector<Command>& stream, const ll& num){

    std::string buf;
    ll arg = 0;
    
    BinaryHeap<ll> heap;

    for (ll i = 0; i < num; ++i){

        std::cin >> buf;

        if (buf.compare ("removeMin") == 0) {

            if (heap.empty()) {

                stream.push_back({"insert", 0});
                heap.insert(0);
            }

            stream.push_back({buf, 0});
            heap.extractMin();
            continue;
        }

        std::cin >> arg;

        if (buf.compare ("insert") == 0) {

            heap.insert (arg);

        } else

        if (buf.compare ("getMin") == 0) {

            while ((heap.getMin() < arg) && !heap.empty()) {

                heap.extractMin();
                stream.push_back({"removeMin", 0});

                }

            if (heap.empty() || heap.getMin() > arg) {

                stream.push_back({"insert", arg});
                heap.insert(arg);

            }   
                
        }

        stream.push_back({buf, arg});

    }    

    return;
}

void PrintResult(std::vector<Command>& stream){

    std::cout << stream.size() << '\n';

    for (ll i = 0; i < stream.size(); ++i) {

        std::cout << stream[i].name;
        if (stream[i].name.compare("removeMin") != 0)
            std::cout << ' ' << stream[i].arg;

        std::cout << '\n';

    }

    return;
}


