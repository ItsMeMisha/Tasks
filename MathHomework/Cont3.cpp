#include <stdio.h>

double sec (int n);

int main () {

    FILE* input = fopen ("input.txt", "r");
    int n = 0;
    fscanf (input, "%d", &n);
    fclose (input);

    double res = sec (n);

    FILE* output = fopen ("output.txt", "w");
    fprintf (output, "%.3f", res);
    fclose (output);

    return 0;

}

double sec (int n) {

   if (n < 1)
        return 1;

    return 1 + 1 / sec (n - 1);

} 

