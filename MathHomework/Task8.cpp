#include <stdio.h>
#include <malloc.h>
#include <assert.h>

int** MatrRead (FILE* file, const int size, int** Matr);
void MatrPrint (FILE* file, const int size, int** Matr);
int** SubMatrix (const int subsize, const int LineShift, const int ColShift, int** BigMatr);
int** NewMatr (const int size);
void FreeMatr (const int size, int** Matr);
int** SumMatr (const int size, int** Matr1, int** Matr2, int** MatrRes);
int** SubtrMatr (const int size, int** Matr1, int** Matr2, int** MatrRes);
int** ShtrMulInNew (const int size, int** Matr1, int** Matr2);
void MatrCpy (const int size, int** MatrTarget, int** MatrSource);

int main () {

    FILE* input = fopen ("input.txt", "r");
    
    int size = 0;

    fscanf (input, "%d", &size);

    int** Matr1 = NewMatr (size);
    int** Matr2 = NewMatr (size);

    Matr1 = MatrRead (input, size, Matr1);
    Matr2 = MatrRead (input, size, Matr2);

    fclose (input); 

    int** MatrRes = ShtrMulInNew (size, Matr1, Matr2);

    FreeMatr (size, Matr1);
    FreeMatr (size, Matr2);

    FILE* output = fopen ("output.txt", "w");
    
    MatrPrint (output, size, MatrRes);

    fclose (output);

    FreeMatr (size, MatrRes);

    return 0;

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

int** SubMatrix (const int subsize, const int LineShift, const int ColShift, int** BigMatr) {

    int** SubM = (int**) calloc (subsize, sizeof (int*));
    
    for (int i = 0; i < subsize; ++i)
        SubM[i] = &(BigMatr[LineShift + i][ColShift]);

    return SubM;

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

int** SumMatr (const int size, int** Matr1, int** Matr2, int** MatrRes) {

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            MatrRes[i][j] = Matr1[i][j] + Matr2[i][j];

    return MatrRes;

}

int** SubtrMatr (const int size, int** Matr1, int** Matr2, int** MatrRes) {

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            MatrRes[i][j] = Matr1[i][j] - Matr2[i][j];

    return MatrRes;

}

void MatrCpy (const int size, int** MatrTarget, int** MatrSource) {

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            MatrTarget[i][j] = MatrSource[i][j];
    return;

}

int** ShtrMulInNew (const int size, int** Matr1, int** Matr2) {

    int** MatrRes = NewMatr (size);

    if (size > 1) {

        const int SmallSize = size / 2;

        int** HelpMatr1 = NewMatr (SmallSize);
        int** HelpMatr2 = NewMatr (SmallSize);
        int** HelpMatr3 = NewMatr (SmallSize);
        
        int** A11 = SubMatrix (SmallSize, 0, 0, Matr1); 
        int** A12 = SubMatrix (SmallSize, 0, SmallSize, Matr1);
        int** A21 = SubMatrix (SmallSize, SmallSize, 0, Matr1);
        int** A22 = SubMatrix (SmallSize, SmallSize, SmallSize, Matr1);

        int** B11 = SubMatrix (SmallSize, 0, 0, Matr2);
        int** B12 = SubMatrix (SmallSize, 0, SmallSize, Matr2);
        int** B21 = SubMatrix (SmallSize, SmallSize, 0, Matr2);
        int** B22 = SubMatrix (SmallSize, SmallSize, SmallSize, Matr2);

        int** C11 = SubMatrix (SmallSize, 0, 0, MatrRes);
        int** C12 = SubMatrix (SmallSize, 0, SmallSize, MatrRes);
        int** C21 = SubMatrix (SmallSize, SmallSize, 0, MatrRes);
        int** C22 = SubMatrix (SmallSize, SmallSize, SmallSize, MatrRes);

        int** P1 = ShtrMulInNew (SmallSize, SumMatr (SmallSize, A11, A22, HelpMatr1), SumMatr (SmallSize, B11, B22, HelpMatr2));
        int** P2 = ShtrMulInNew (SmallSize, SumMatr (SmallSize, A21, A22, HelpMatr1), B11);
        int** P3 = ShtrMulInNew (SmallSize, A11, SubtrMatr (SmallSize, B12, B22, HelpMatr1));
        int** P4 = ShtrMulInNew (SmallSize, A22, SubtrMatr (SmallSize, B21, B11, HelpMatr1));
        int** P5 = ShtrMulInNew (SmallSize, SumMatr (SmallSize, A11, A12, HelpMatr1), B22);
        int** P6 = ShtrMulInNew (SmallSize, SubtrMatr (SmallSize, A21, A11, HelpMatr1), SumMatr (SmallSize, B11, B12, HelpMatr2));
        int** P7 = ShtrMulInNew (SmallSize, SubtrMatr (SmallSize, A12, A22, HelpMatr1), SumMatr (SmallSize, B21, B22, HelpMatr2));

        MatrCpy (SmallSize, C11, SubtrMatr (SmallSize, SumMatr (SmallSize, P1, P4, HelpMatr1), SubtrMatr (SmallSize, P5, P7, HelpMatr2), HelpMatr3));
        MatrCpy (SmallSize, C12, SumMatr (SmallSize, P3, P5, HelpMatr1));
        MatrCpy (SmallSize, C21, SumMatr (SmallSize, P2, P4, HelpMatr1));
        MatrCpy (SmallSize, C22, SumMatr (SmallSize, SubtrMatr (SmallSize, P1, P2, HelpMatr1), SumMatr (SmallSize, P3, P6, HelpMatr2), HelpMatr3));

        FreeMatr (SmallSize, P1); FreeMatr (SmallSize, P2); FreeMatr (SmallSize, P3); FreeMatr (SmallSize, P4);
        FreeMatr (SmallSize, P5); FreeMatr (SmallSize, P6); FreeMatr (SmallSize, P7);

        free (A11); free (A12); free (A21); free (A22);
        free (B11); free (B12); free (B21); free (B22);
        free (C11); free (C12); free (C21); free (C22);

        FreeMatr (SmallSize, HelpMatr1);
        FreeMatr (SmallSize, HelpMatr2);
        FreeMatr (SmallSize, HelpMatr3);

    }

    else
        **MatrRes = (**Matr1) * (**Matr2);

    return MatrRes;

}
