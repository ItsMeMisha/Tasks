#include <stdio.h>
#include <malloc.h>

int** KhirgMatr (const int size, int** Matr);
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

    int** MatrRes = KhirgMatr (size, Matr);

    FreeMatr (size, Matr);

    FILE* output = fopen ("output.txt", "w");

    fprintf (output, "%d\n", size);    
    MatrPrint (output, size, MatrRes);

    fclose (output);

    FreeMatr (size, MatrRes);
 
    return 0;

}

int** KhirgMatr (const int size, int** Matr) {

    int** Khirg = NewMatr (size);

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {

            if (Matr[i][j] > 0) {

                Khirg[i][i]++;
                Khirg[i][j] = -1;

            } 

    }
 
    return Khirg;

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


