#include <stdio.h>
#include <cstdlib>
#include <algorithm>

void CountPoints (int n, long long* lefrs, long long* rights, int m, long long* x, int* count);
int BinSearchLeftEnds (int lb, int rb, long long* arr, long long x); //returns right border for x
int BinSearchRightEnds (int lb, int rb, long long* arr, long long x);
void ReadSegs (int n, long long* lefts, long long* rights);
void ReadPoints (int m, long long* x, int* count);
void PrintArr (int* arr, int n);
int Compare (const void* num1, const void* num2);
 
int main () {

    int n = 0;
    int m = 0;

    scanf ("%d %d", &n, &m);
    long long* lefts = new long long[n];
    long long* rights = new long long[n];
    long long* x = new long long[m];
    int* count = new int[m];

    ReadSegs (n, lefts, rights);
    ReadPoints (m, x, count);

    CountPoints (n, lefts, rights, m, x, count);
    PrintArr (count, m);

    delete[] lefts;
    delete[] rights;
    delete[] x;
    delete[] count;

    return 0;

}

void CountPoints (int n, long long* lefts, long long* rights, int m, long long* x, int* count) {

    std::qsort (lefts, n, sizeof (long long), Compare);
    std::qsort (rights, n, sizeof (long long), Compare);

    int lborders = 0;
    int rborders = 0;

    for (int i = 0; i < m; ++i) {

        lborders = BinSearchLeftEnds (0, n-1, lefts, x[i]);
        rborders = BinSearchRightEnds (0, n-1, rights, x[i]);

        count[i] = lborders - rborders;

    }

    return;

}

int BinSearchLeftEnds (int lb, int rb, long long* arr, long long x) {

    if (lb == rb)
        if (arr[rb] <= x)
            return (rb + 1);

        else return rb;

    if ((arr[(lb + rb)/2] <= x) && (x < arr[(lb+rb)/2+1]))
        return ((lb+rb)/2 + 1);

    else

        if (x < arr[(lb + rb)/2])
            return BinSearchLeftEnds (lb, (lb + rb)/2, arr, x);

        else return BinSearchLeftEnds ((lb + rb)/2 + 1, rb, arr, x);

}

int BinSearchRightEnds (int lb, int rb, long long* arr, long long x) {

    if (lb == rb)

        if (arr[rb] < x)
            return (rb + 1);

        else return rb;

    if ((arr[(lb + rb)/2] < x) && (x <= arr[(lb+rb)/2+1]))
        return ((lb+rb)/2 + 1);

    else

        if (x > arr[(lb + rb)/2 + 1])
            return BinSearchRightEnds ((lb + rb)/2 + 1, rb, arr, x);

        else return BinSearchRightEnds (lb, (lb + rb)/2, arr, x);

}

void ReadSegs (int n, long long* lefts, long long* rights) {

    long long a = 0;
    long long b = 0;

    for (int i = 0; i < n; ++i) {

        scanf ("%lld %lld", &a, &b);
        lefts[i]  = std::min (a, b);
        rights[i] = std::max (a, b);

    }

    return;

}

void ReadPoints (int m, long long* x, int* count) {

    for (int i = 0; i < m; i++) {

        scanf ("%lld", x+i);
        count[i] = 0;

    }

    return;

}

void PrintArr (int* arr, int n) {

    for (int i = 0; i < n; ++i)
        printf ("%d ", arr[i]);

    return;

}

int Compare (const void* num1, const void* num2) {

    return ( *(long long*) num1 - *(long long*) num2);

}
