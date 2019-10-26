#include <stdio.h>
#include <malloc.h>

int** SubMatrix (const int subsize, const int LineShift, const int ColShift, int** BigMatr);
int** NewMatr (const int size);
void FreeMatr (const int size, int** Matr);
int** SumMatr (const int size, int** Matr1, int** Matr2, int** MatrRes);
int** SubtrMatr (const int size, int** Matr1, int** Matr2, int** MatrRes);
int** ShtrMulInNew (const int size, int** Matr1, int** Matr2);

int main () {

    FILE* input = fopen ("input.txt", "r");
     

    return 0;

}

int** SubMatrix (const int subsize, const int LineShift, const int ColShift, int** BigMatr) {

    int** SubM = BigMatr + LineShift;
    
    for (int i = 0; i < subsize; ++i)
        SubM[i] += ColShift;

    return SubM;

}

int** NewMatr (const int size) {

    int** Matr = (int**) calloc (size, sizeof (int*));

    for (int i = 0; i < size; ++i)
        Matr[i] = (int*) calloc (size, sizeof (int));

    return Matr;

}

void FreeMatr (const int size, int** Matr) {

    for (int i = 0; i < size; ++ i)
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

int** ShtrMulInNew (const int size, int** Matr1, int** Matr2) {

    int** MatrRes = NewMatr (size);

    if (size > 1) {

        const int SmallSize = size / 2;

        int** HelpMatr1 = NewMatr (SmallSize);
        int** HelpMatr2 = NewMatr (SmallSize);
        
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
        int** C22 = SubMatrix (SmallSize, SmallSize, 0, MatrRes);

        int** P1 = ShtrMulInNew (SmallSize, SumMatr (SmallSize, A11, A22, HelpMatr1), SumMatr (SmallSize, B11, B22, HelpMatr2);
        int** P2 = ShtrMulInNew (SmallSize, SumMatr (SmallSize, A21, A22, HelpMatr1), B11);
        int** P3 = ShtrMulInNew (SmallSize, A11, SubtrMatr (SmallSize, B12, B22, HelpMatr1));
        int** P4 = ShtrMulInNew (SmallSize, A22, SubtrMatr (SmallSize, B21, B11, HelpMatr1));
        int** P5 = ShtrMulInNew (SmallSize, SumMatr (SmallSize, A11, A12, HelpMatr1), B22);
        int** P6 = ShtrMulInNew (SmallSize, SubtrMatr (SmallSize, A21, A11, HelpMatr1), SumMatr (SmallSize, B11, B12, HelpMatr2);
        int** P7 = ShtrMulInNew (SmallSize, SubtrMatr (SmallSize, A12, A22, HelpMatr1), SumMatr (SmallSize, B21, B22, HelpMatr2);

        C11 = SubtrMatr (SmallSize, SumMatr (SmallSize, P1, P4, HelpMatr1), SumMatr (SmallSize, P5, P7, HelpMatr2), C11);
        C12 = SumMatr (SmallSize, P3, P5, C12);
        C21 = SumMatr (SmallSize, P2, P4, C21);
        C22 = SumMatr (SmallSize, SubtrMatr (SmallSize, P1, P2, HelpMatr1), SumMat (SmallSize, P3, P6, HelpMatr2), C22);

    }

    else
        **MatrRes = (**Matr1) * (**Matr2);

    FreeMatr (SmallSize, HelpMatr1);
    FreeMatr (SmallSize, HelpMatr2);

    return MatrRes;

}
