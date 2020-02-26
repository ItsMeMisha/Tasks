#include <stdio.h>
#include <algorithm>

typedef unsigned int ui_t;
typedef long long lli;

const ui_t Size = 1 << 24; //2^24

ui_t _cur = 0;
ui_t a = 0;
ui_t b = 0;

ui_t nextRand(const ui_t& a, const ui_t& b);
void AddSeg(lli* arr, const ui_t& m);
ui_t SumSeg(lli* arr); 
void PrefixSums(lli* replArr);

void InitLR(ui_t& l, ui_t& r);

int main () {

    lli* array = new lli [Size] {0};
    
    ui_t m = 0;
    ui_t q = 0;
    scanf ("%u %u", &m, &q);

    scanf ("%u %u", &a, &b);

    AddSeg (array, m);
    PrefixSums(array);

    ui_t totalSum = 0;

    for (ui_t i = 0; i < q; ++i)
        totalSum += SumSeg(array);       

    printf ("%u", totalSum);

    delete[] array;
    return 0;

}

ui_t nextRand() {

    _cur = _cur * a + b;
    return (_cur >> 8);
}

void AddSeg(lli* arr, const ui_t& m) {

    for (int i = 0; i < m; ++i) {

        ui_t add = nextRand();
        ui_t l = 0;
        ui_t r = 0;
        InitLR (l, r);
    
        arr[l] += add; 

        if (r < Size - 1)
            arr[r+1] -= add;

    }

    return;

}

void PrefixSums(lli* replArr){

    lli add = replArr[0]; 

    for (int i = 1; i < Size; ++i) {

        add += replArr[i];
        replArr[i] = add;
        replArr[i] += replArr[i-1];
        
    }

    return;

}

ui_t SumSeg(lli* arr) {

    ui_t Sum = 0;

    ui_t l = 0;
    ui_t r = 0;
    InitLR (l, r);

    Sum = arr[r];
    
    if (l > 0)
        Sum -= arr[l-1];

    return Sum;

}

void InitLR(ui_t& l, ui_t& r) {

    l = nextRand();
    r = nextRand();
    if (l > r) std::swap(l, r);

    return;

}

