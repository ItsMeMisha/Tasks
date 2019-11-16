#include <stdio.h>
#include <malloc.h>


const char yes[] = "YES";
const char no[] = "NO";

int** KirgMatr (const int size, int** Matr);
const char* EPathCheck (const int size, int** Kirg);
int** MatrRead (FILE* file, const int size, int** Matr);
void MatrPrint (FILE* file, const int size, int** Matr);
int** NewMatr (const int size);
void FreeMatr (const int size, int** Matr);

int main () {

    FILE* input = fopen ("input.txt", "r");
    
    int size = 0;

    fscanf (input, "%d", &size);

    int** Matr = NewMatr (size);

    Matr = MatrRead (input, size, Matr);

    fclose (input); 

    int** MatrRes = KirgMatr (size, Matr);

    FreeMatr (size, Matr);

    FILE* output = fopen ("output.txt", "w");

    fprintf (output, "%s", EPathCheck (size, MatrRes));

    fclose (output);

    FreeMatr (size, MatrRes);
 
    return 0;

}

const char* EPathCheck (const int size, int** Kirg) {

    int oddDeg = 0;

    for (int i = 0; i < size; i++) {

        //if (Kirg[i][i] == 0)
          //  return no;

        if (Kirg[i][i] % 2 != 0)
            oddDeg++;

    }
    
    if (oddDeg > 0 && oddDeg != 2)
        return no;

    return yes;

}

int** KirgMatr (const int size, int** Matr) {

    int** Kirg = NewMatr (size);

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {

            if (Matr[i][j] > 0) {

                Kirg[i][i]++;
                if (i != j)
                    Kirg[i][j] = -1;

            } 

    }
 
    return Kirg;

}

int** MatrRead (FILE* file, const int size, int** Matr) {

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            fscanf (file, "%d", &Matr[i][j]);

    return Matr;

}

void MatrPrint (FILE* file, int size, int** Matr) {

    for (int i = 0; i < size; ++i) {

        for (int j = 0; j < size; ++j)
            fprintf (file, "%d ", Matr[i][j]);
        fprintf (file, "\n");

    }

    return;

}

int** NewMatr (const int size) {

    int** Matr = (int**) calloc (size, sizeof (int*));

    for (int i = 0; i < size; ++i) 
        Matr[i] = (int*) calloc (size, sizeof (int));

    return Matr;

}

void FreeMatr (const int size, int** Matr) {

    for (int i = 0; i < size; ++i) 
        free (Matr[i]);
    
    free (Matr);

    return;
    
}


