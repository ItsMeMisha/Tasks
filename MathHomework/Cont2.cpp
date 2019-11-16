#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

long long nim (long long n);

int main () {

    FILE* input = fopen ("input.txt", "r");

    long long num = 0;

    fscanf (input, "%lld", &num);
    
    fclose (input);

    long long res = nim (num);

    FILE* output = fopen ("output.txt", "w");
    fprintf (output, "%lld", res);
    fclose (output);

    return 0;

}

long long nim (long long n) {

    if (n%5 == 0)
        return 4;
    else
        return (n%5 - 1);

}
