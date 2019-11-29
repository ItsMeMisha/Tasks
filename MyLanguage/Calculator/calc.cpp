#include <stdio.h>
#include <malloc.h>

const char* CurChar = "";

int GetG (const char* str);
int GetE ();
int GetT ();
int GetP ();
int GetN ();

int main () {

    char* str = nullptr;

    scanf ("%ms", &str);

    printf ("%s = %d\n", str, GetG (str));

    free (str);

    return 0;

}

int GetG (const char* str) {

    CurChar = str;
        
    int val = GetE ();

    if (*CurChar != '\0') {

        printf ("Error: invalid syntax near %.10s\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    return val;

}

int GetE () {

    int val = GetT ();

    while (*CurChar == '+' || *CurChar == '-') {

        char op = *CurChar;

        CurChar++;

        int val2 = GetT ();

        if (op == '+')
            val += val2;
        else
            val -= val2;

    }

    return val;

}

int GetT () {

    int val = GetP ();

    while (*CurChar == '*' || *CurChar == '/') {

        char op = *CurChar;

        CurChar++;

        int val2 = GetP ();

        if (op == '*')
            val *= val2;
        else
            val /= val2;

    }

    return val;


}

int GetP () {

    if (*CurChar == '(') {

        CurChar++;
        int val = GetE ();

        if (*CurChar != ')') {

           printf ("Error: invalid syntax near %.10s: expected ')'\n", CurChar);
           printf ("                           ^\n");
           return 0;

        }

        CurChar++;
        return val;

    }

    return GetN ();

}

int GetN () {

    int val = 0;

    if ('0' <= *CurChar && *CurChar <= '9') {

        val = *CurChar - '0';
        CurChar++;

    }

    else {

        printf ("Error: invalid syntax near %.10s: expected number\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    while ('0' <= *CurChar && *CurChar <= '9') {

        val = val * 10 + *CurChar - '0';
        CurChar++;

    }

    return val;

}

