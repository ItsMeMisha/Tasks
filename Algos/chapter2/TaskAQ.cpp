#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

#include <stdio.h>

typedef long long ll;

template <typename T>
class QuickHeap {

public: 

    QuickHeap() {

        pivots.push_back(0);

    }

    T getMin(){

        if (empty()) {
            
            T smth = {0};
            return smth;

        }

        makeLastPivotFirstInData();
 
        return data.front();
    }

    void extractMin(){

        if (empty()) 
            return;
printf ("\nbefore extr\n");
dump();

        makeLastPivotFirstInData();

        pivots.pop_back();

        for (int i = 0; i < pivots.size(); ++i)
            pivots[i]--;

        data.pop_front(); 
        --size;
printf ("\nafter extr\n");
dump();

        return;
    }

    void insert(const T& elem){
printf ("\nbefore insert\n");
dump();
        ++size;
        ll curPivPtr = 0;
        (pivots[curPivPtr])++;

        if (pivots.size() == 1 || empty()) {

            data.push_front(elem);
printf ("\nafter insert\n");
dump();

            return;
    
        }

        
        if (this->size > 2)
            data.push_back(data[pivots[curPivPtr + 1]+1]);

        else
            data.push_back(data[pivots[curPivPtr + 1]]);

        ll pivotsEnd = pivots.size()-1;

        do {

            ++curPivPtr;

            if (elem < data[pivots[curPivPtr]]) {

                ll insertPos = pivots[curPivPtr];
                pivots[curPivPtr]++;

                if (curPivPtr == pivotsEnd)
                    data[insertPos] = data[0];
                else
                    data[insertPos] = data[pivots[curPivPtr + 1]+1]; 
                               
            }

        } while ((curPivPtr != pivotsEnd) && (elem < data[pivots[curPivPtr]]));

        if ((curPivPtr == pivotsEnd) && (elem < data[pivots[pivotsEnd]]))
            data.push_front(elem);

        else
            data[pivots[curPivPtr] + 1] = elem;
printf ("\nafter insert\n");
dump();


        return;
    }

    bool empty() {

        return (size <= 0);

    }

    void dump() {

        printf ("\nsize:%lld\n", size);
        ll j = 0;
        for (j; j < size; ++j)
            printf ("dump %lld:%lld\n", j, data[j]);

        return;

    }

private:

    ll size = 0;
    std::deque<T> data;
    std::deque<ll> pivots;

    struct Mediana{

        T data;
        ll pos;

        bool operator< (const Mediana& secElem) const {
            return (this->data < secElem.data);
        }

    };

    ll findPivotInFive (const ll& begin, const ll& end){

        std::vector<Mediana> bufVector;
        Mediana elem;
        for (ll i = begin; i <= end; ++i) {

            elem.pos = i;
            elem.data = data[i];
            bufVector.push_back(elem);

        }

        std::sort(bufVector.begin(), bufVector.end());
    
        return bufVector[bufVector.size()/2].pos; 

    }

    ll findPivot (const ll& begin, const ll& end){

        std::vector<Mediana> medians;

        //find medians;
        Mediana nextMedian;
        ll curBegin = begin;
        ll curLast = std::min(begin + 4, end);

        for (curBegin; curBegin <= end; curBegin += 5, curLast = std::min(curLast+5, end)) {

            nextMedian.pos = findPivotInFive(curBegin, curLast);
            nextMedian.data = data[nextMedian.pos];
            medians.push_back(nextMedian);

        }

        //find median of medians
        std::sort(medians.begin(), medians.end());

        return medians[medians.size()/2].pos;

    }

    void partition (const ll& begin, const ll& end){

        ll pivot = findPivot (begin, end);
        ll left = begin;
        ll right = end;

        while (left < right) {

            while ((data[left] < data[pivot]) && (left < pivot))
                ++left;

            while ((data[right] >= data[pivot]) && (right > pivot))
                --right;

            if (left == pivot)
                pivot = right;

            else if (right == pivot)
                pivot = left;

            std::swap (data[left], data[right]);

        }
    
        pivots.push_back(pivot);

        return;
    }

    void heapify (const ll begin, ll end){

        while (end >= begin) {

            partition(begin, end);
            end = this->pivots[pivots.size()-1] - 1;

        }

        return;

    }

    void sort (const ll& begin, const ll& end){

        heapify (begin, end);
        return;
    };

    void makeLastPivotFirstInData() {

        if (this->empty())
            return;

        ll lastPivot = this->pivots[pivots.size()-1];

        if (lastPivot != 0)
            this->sort (0, lastPivot - 1);

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
    
    QuickHeap<ll> heap;

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


