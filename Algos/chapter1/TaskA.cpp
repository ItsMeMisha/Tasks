#include <stdio.h>
//#include

typedef unsigned long long ull;

void ReadArr (FILE* file, int* arr, int num);
int* MergeSort (int* arr, int num, ull* invers);
int* CopyArr (int* arrSource, int* arrRec, int num);

int main () {

    int num = 0;

    FILE* file = fopen ("inverse.in", "r");
    fscanf (file, "%d", &num);
    int* arr = new int[num];
    ReadArr (file, arr, num);
    fclose (file);

    ull invers = 0;
    MergeSort (arr, num, &invers);

    file = fopen ("inverse.out", "w");
    fprintf (file, "%llu", invers);
    fclose (file);

    return 0;

}

void ReadArr (FILE* file, int* arr, int num) {

    for (int i = 0; i < num; ++i)
        fscanf (file, "%d", arr+i);

    return;

}

int* MergeSort (int* arr, int num, ull* invers) {

    if (num == 1)
        return arr;

    int numL = num/2;
    int* left = arr;
    
    int numR = num/2 + num % 2;
    int* right = arr + numL;

    MergeSort (left, numL, invers);
    MergeSort (right, numR, invers);

    int i = 0;  //cur left
    int j = 0;  //cur right
    int k = 0;  //cur main

    int* arrBuf = new int[num];

    while (i < numL || j < numR) {

        if (i >= numL) {

            arrBuf[k] = right[j];
            k++; j++;

        } else if (j >= numR) {

            arrBuf[k] = left[i];
            k++; i++;

        } else if (left[i] > right[j]) {

            arrBuf[k] = right[j];
            k++; j++; (*invers) += j + numL - k;

        } else {

            arrBuf[k] = left[i];
            k++; i++;

        }
    }

    arr = CopyArr (arrBuf, arr, num);

    delete[] arrBuf;

    return arr;

}

int* CopyArr (int* arrSource, int* arrRec, int num) {

    for (int i = 0; i < num; ++i)
        arrRec[i] = arrSource[i];

    return arrRec;

}

