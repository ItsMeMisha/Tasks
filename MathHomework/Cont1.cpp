#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

const int MaxStrLen = 512;

char* minus (char* num1, char* num2);
char* sum (char* num1, char* num2, bool* ext);
char* Karatsuba (char* num1, char* num2);

int main () {

    FILE* input = fopen ("input.txt", "r");

    char num1[MaxStrLen] = "";
    char num2[MaxStrLen] = "";

    fscanf (input, "%s ", num1);
    fscanf (input, "%s", num2);
    
    fclose (input);

    char* res = Karatsuba (num1, num2);

    FILE* output = fopen ("output.txt", "w");
    fprintf (output, "%s", res);
    free (res);
    fclose (output);

    return 0;

}

//acx^2 + x((a+b)(c+d) - (ab + cd)) + bd, x - sdvig

char* sum (char* num1, char* num2, bool* ext) {

    int len1 = strlen (num1);
    int len2 = strlen (num2);
    char* res = (char*) calloc (MaxStrLen, sizeof (char));
    int maxlen = 0;
    int bonus = 0;

    if (len1 > len2)
        maxlen = len1;
    else maxlen = len2;

    for (int i = 0; i < maxlen; ++i) {
    
        int c1 = 0;
        if (len1 - i > 0)
            c1 = num1[len1 - i -1] - '0';
        
        int c2 = 0;
        if (len2 - i > 0)
            c2 = num2[len2 - i - 1] - '0';

        int s = c1 + c2 + bonus;
        res[maxlen - i] = '0' + (s % 10);
        bonus = s / 10;

    }

    if (bonus) {
        res[0] = '0' + bonus;
        *ext = true;
        return res;
    }
    else {
    
        *ext = false;
        return res+1;

    }

}

char* minus (char* num1, char* num2, bool* ext) {

    int len1 = strlen (num1);
    int len2 = strlen (num2);
    char* res = (char*) calloc (MaxStrLen, sizeof (char));
    int maxlen = 0;
    int bonus = 0;

    if (len1 > len2)
        maxlen = len1;
    else maxlen = len2;

    for (int i = 0; i < maxlen; ++i) {
    
        int c1 = 0;
        if (len1 - i > 0)
            c1 = num1[len1 - i -1] - '0';
        
        int c2 = 0;
        if (len2 - i > 0)
            c2 = num2[len2 - i - 1] - '0';

        int s = c1 - c2 - bonus;
        res[maxlen - i] = '0' + (s % 10);
        bonus = s / 10;

    }

    if (bonus) {
        res[0] = '0' + bonus;
        *ext = true;
        return res;
    }
    else {
    
        *ext = false;
        return res+1;

    }

}

char* flip
char* Karatsuba (char* num1, char* num2) {

    int len1 = strlen (num1);
    int len2 = strlen (num2);
    char* res = (char*) calloc (MaxStrLen, sizeof (char));;

    if (!((len1 == 1) && (len2 == 1))) {

        int shift = (len1 + len2) / 4;

        if (shift == 0)
            shift = 1;

        char a[MaxStrLen] = "";
        char b[MaxStrLen] = "";
        char c[MaxStrLen] = "";
        char d[MaxStrLen] = "";

        if (len1 > shift) {

            strncpy (a, num1, len1 - shift);
            strncpy (b, num1 + len1 - shift, shift);

        }

        else  {
    
            strncpy (b, num1, len1);
            a[0] = '0';
            a[1] = '\0';

        }

        if (len2 > shift) {

            strncpy (c, num2, len2 - shift);
            strncpy (d, num2 + len2 - shift, shift);

        }

        else {

            strncpy (d, num2, len2);
            c[0] = '0';
            c[1] = '\0';

        }

        char* ac = Karatsuba (a, c);
        char* bc = Karatsuba (b, c);
        char* ab = sum (a, b);
        char* cd = sum (c, d);
        char* adbc = minus (minus (Karatsuba (ab, cd), ab), cd);
        bool ext  = false;
        char* sumadbc = sum (ad, bc, &ext);
    
        sprintf (res, "%s%s%s", ac, adbc, bd);

        free (ac); free (adbc);
        free (bc); free (cd); free (ab);

        if (ext)
            free (sumadbc);
        else free (sumadbc - 1);
        
    }

    else {

        char n1 = num1[0] - '0';
        char n2 = num2[0] - '0';
        
        n1 *= n2;
        if (n1 > 9) {

            res[0] = '0' + (n1 / 10);
            res[1] = '0' + (n1 % 10);
            res[2] = '\0';

        }
        
        else {

            res[0] = '0' + n1;
            res[1] = '\0';
        }
    }

    return res;
}
