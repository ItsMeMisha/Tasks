#include <stdio.h>
#include <malloc.h>

int** MatrRead (FILE* file, const int size, int** Matr);
void MatrPrint (FILE* file, const int size, int** Matr);
int** NewMatr (const int size);
void FreeMatr (const int size, int** Matr);
int Components (const int size, int** Matr);
int** ConnectMatr (const int size, int** Matr);

int main () {

    FILE* input = fopen ("input.txt", "r");
    
    int size = 0;

    fscanf (input, "%d", &size);

    int** Matr = NewMatr (size);

    Matr = MatrRead (input, size, Matr);

    fclose (input); 

    int res = Components (size, Matr);

    FreeMatr (size, Matr);

    FILE* output = fopen ("output.txt", "w");
    
    fprintf (output, "%d", res);

    fclose (output);

    return 0;

}

int Components (const int size, int** Matr) {

    int res = 0;
    int available = size;

    int** Connect = ConnectMatr (size, Matr);

    bool* removed = (bool*) calloc (size, sizeof (bool));

    for (int i = 0; i < size; ++i)
        removed[i] = false;

    for (int i = 0; i < size && available > 0; ++i) {

        if (removed[i]) continue;
        res++;

        for (int j = 0; j < size; j++) {

            if (removed[j]) continue;
            
            if (Connect[i][j] > 0) {

                    removed[j] = true;
                    removed[i] = true;
                    available--;

            } 

        }

    }

    free (removed);
                 
    FreeMatr (size, Connect);

    return res;

}

int** ConnectMatr (const int size, int** Matr) {

    int** Connect = NewMatr (size);

    for (int k = 0; k < size; ++k)
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                Connect[i][j] = Connect[i][j] | (Matr[i][j] | (Matr[i][k] & Matr[k][j]));


    return Connect;

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

