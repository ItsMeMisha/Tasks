#include <stdio.h>
#include <string.h>
#include <math.h>

const char len[] = "len";
const char rot[] = "rot";
const char X = 'X';
const char Y = 'Y';
const char Z = 'Z';

double Len (double vect[3]);
void LenChange (double vect[3], double newlen);
void mul (double vect[3], double rotMatr[3][3]);
void NewRot (double vect[3], char axis, double angle);

int main() {

    FILE* input = fopen ("input.txt", "r");

    double vector[3] = {0, 1, 0};

    char coord = 0;
    double newlen = 0;

    double angle = 0;
    
    char StrBuf[4] = "";
    int checkEof = fscanf (input, "%s ", StrBuf);

    while (checkEof != EOF) {
        if (strcmp (StrBuf, len) == 0) {

            fscanf (input, "%lg", &newlen);

            LenChange (vector, newlen);

        }

        else if (strcmp (StrBuf, rot) == 0) {

            fscanf (input, "%c", &coord);
            fscanf (input, "%lg", &angle);

            NewRot (vector, coord, angle);

            }
  
        checkEof = fscanf (input, "%s ", StrBuf);

    }

    fclose (input);

    FILE* output = fopen ("output.txt", "w");
    
    fprintf (output, "%.1f %.1f %.1f", vector[0], vector[1], vector[2]);

    fclose (output);

    return 0;

}

double Len (double vect[3]) {

    double sqrlen = 0;

    for (int i = 0; i < 3; ++i)
        sqrlen += vect[i]*vect[i];

    return sqrt (sqrlen);

}

void LenChange (double vect[3], double newlen) {

    double curlen = Len (vect);
    double newlencoef = (curlen + newlen) / curlen;

    for (int i = 0; i < 3; ++i)
        vect[i] *= newlencoef;

    return;

}

void mul (double vect[3], double rotMatr[3][3]) {

    double newvect[3] = {};
    
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            newvect[i] += vect[j] * rotMatr[i][j];

    for (int i = 0; i < 3; ++i)
        vect[i] = newvect[i];

    return;

}

void NewRot (double vect[3], char axis, double angle) {

    double rotMatrix[3][3] = {};

    switch (axis) {

    case 'X': {
        rotMatrix[0][0] = 1;
        rotMatrix[1][1] = cos (angle);
        rotMatrix[1][2] = -sin (angle);
        rotMatrix[2][1] = sin (angle);
        rotMatrix[2][2] = cos (angle);
        break; 
        }

    case 'Y': {
        rotMatrix[0][0] = cos (angle);
        rotMatrix[0][2] = sin (angle);
        rotMatrix[1][1] = 1;
        rotMatrix[2][0] = -sin (angle);
        rotMatrix[2][2] = cos (angle);
        break;
        }

    case 'Z': {
        rotMatrix[0][0] = cos (angle);
        rotMatrix[0][1] = -sin (angle);
        rotMatrix[1][0] = sin (angle);
        rotMatrix[1][1] = cos (angle);
        rotMatrix[2][2] = 1;
        break;
        }

    }

    mul (vect, rotMatrix);

    return;

}
