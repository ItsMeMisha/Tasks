#include <stdio.h>
#include <unordered_map>

int* CreatenReadIntArr (int num);
void CreateKeys (std::unordered_map<int, int>* countMap, int* arr, int num);
void CountElems (std::unordered_map<int, int>* countMap, int* arr, int num);

int main () {

    int n = 0;
    scanf ("%d", &n);
    int* arr1 = CreatenReadIntArr (n);

    int m = 0;
    scanf ("%d", &m);
    int* arr2 = CreatenReadIntArr (m);

    std::unordered_map<int, int> countMap;
    CreateKeys (&countMap, arr2, m);
    CountElems (&countMap, arr1, n);

    for (int i = 0; i < m; i++)
        printf ("%d ", countMap.at (arr2[i]));

    delete[] arr1;
    delete[] arr2;
    
    return 0;

}

int* CreatenReadIntArr (int num) {

    int* arr = new int[num];

    for (int i = 0; i < num; i++)
        scanf ("%d", arr+i);

    return arr;

}

void CreateKeys (std::unordered_map<int, int>* countMap, int* arr, int num) {

    for (int i = 0; i < num; i++)
        countMap->insert (std::pair<int, int>(arr[i], 0));

    return;

}

void CountElems (std::unordered_map<int, int>* countMap, int* arr, int num) {

    for (int i = 0; i < num; i++)
        if (countMap->find (arr[i]) != countMap->end ())
            countMap->at(arr[i])++;

    return;

}

