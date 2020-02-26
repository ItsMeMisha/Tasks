#include <stdio.h>

struct req_t {

    int arr1;
    int arr2;

};

void findMinMax (int** arrsA, int** arrsB, int numA, int numB, int size, req_t* reqs, int reqNum);
int BinSearchForTwoArr (int* arrA, int* arrB, int left, int right);
void ReadArrs (int** arrs, int num, int size);
void ReadRequests (req_t* reqs, int num);
void DestrArr (int** arrs, int num);

int main() {

    int numA = 0;
    int numB = 0;
    int size = 0;
    scanf ("%d %d %d", &numA, &numB, &size);

    int** arrsA = new int*[numA];
    int** arrsB = new int*[numB];

    ReadArrs (arrsA, numA, size);
    ReadArrs (arrsB, numB, size);

    int reqNum = 0;
    scanf ("%d", &reqNum);

    req_t* reqs = new req_t[reqNum];

    ReadRequests (reqs, reqNum);

    findMinMax (arrsA, arrsB, numA, numB, size, reqs, reqNum);
    
    DestrArr (arrsA, numA);
    delete[] arrsA;
    DestrArr (arrsB, numB);
    delete[] arrsB;

    delete[] reqs;

    return 0;

}

void findMinMax (int** arrsA, int** arrsB, int numA, int numB, int size, req_t* reqs, int reqNum) {

    int res = 0;

    for (int i = 0; i < reqNum; ++i) {

        res = BinSearchForTwoArr (arrsA[reqs[i].arr1-1], arrsB[reqs[i].arr2-1], 0, size-1);
        printf ("%d\n", res);

    }

    return;

}

int BinSearchForTwoArr (int* arrA, int* arrB, int left, int right) {

    if (right - left <= 1) {

        int maxL = (arrA[left] < arrB[left]) ? arrB[left] : arrA[left];
        int maxR = (arrA[right] < arrB[right]) ? arrB[right] : arrA[right];

        return ((maxL < maxR) ? left : right) + 1;

    }

    int mid = (left + right) / 2;

    if (arrA[mid] == arrB[mid])
        return mid + 1;

    if (arrA[mid] > arrB[mid])
        return BinSearchForTwoArr (arrA, arrB, left, mid);

    else return BinSearchForTwoArr (arrA, arrB, mid, right);

}

void ReadArrs (int** arrs, int num, int size) {

    for (int i = 0; i < num; ++i) {

        arrs[i] = new int[size];

        for (int j = 0; j < size; ++j)
            scanf ("%d", &arrs[i][j]);

    }

    return;
}

void ReadRequests (req_t* reqs, int num) {

    for (int i = 0; i < num; ++i)
        scanf ("%d %d", &(reqs[i].arr1), &(reqs[i].arr2));

    return;

}


void DestrArr (int** arrs, int num) {

    for (int i = 0; i < num; ++i)
        delete[] arrs[i];

}

