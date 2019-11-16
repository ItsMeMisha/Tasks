#include <stdio.h>

long f (size_t n);

int main () {

    FILE* input = fopen ("input.txt", "r");
    
    size_t num = 0;
    fscanf (input, "%d", &num);
    fclose (input);

    long res = f (num);

    FILE* output = fopen ("output.txt", "w");
    fprintf (output, "%ld", res);

    fclose (output);

    return 0;

}

long f (size_t n) {

   if (n == 1)
        return 14;

   else if (n == 0)         
        return 3;

    else if (n == 2)
        return 94; 
    
   else return (14 * f (n-1) - 51 * f (n-2) + 54 * f (n-3));
}
