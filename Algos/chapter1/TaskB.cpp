#include <stdio.h>
#include <string.h>

const int maxResLen = 10000;
const int maxCount = 100;

int main () {

    FILE* file = fopen ("number.in", "r");
    


    fclose (file);

    char* result = new char[maxResLen];
    file = fopen ("number.out", "w");
    fprintf ("%s", result);
    delete[] result;

    return 0;

}
