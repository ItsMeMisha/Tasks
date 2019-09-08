#include <stdio.h>
#include <assert.h>
#include <malloc.h>

/*! This function writes one string in standard output
*
* @param[in] str - string that should be writed
*
* @return 0 in case of successful execution
*
*/

int Myputs (const char* str);

/*! This function reads one string from file with length not more than num
*
* @param[in] num - maximum number of characters in string
* @param[in] file - file from where string should be readed
* @param[out] str - the pointer where tha string should be saved
*
* @return str in case of successful execution
*
*/

char* Myfgets (char* str, int num, FILE* file);

/*! This function turns int-type number into string
*
* @param[in] num - int-type number
* @param[in] base - number system base
* @param[out] str - string where the num will be writed
*
* @return pointer to str
*
*/

char* Myitoa (int num, char* str, int base);

/*! This function turns a number from string into int-type
*
* @param[in] str - string from where the number will be readed
*
* @return num - number from string
*
*/

int Myatoi (char* str);

/*! This program tests functions Myputs, Myfgets, Myitoa and Myatoi */

int main () {

    const int MaxChars = 256;
    char* str = (char*) calloc (MaxChars, sizeof(char));

    FILE* f = fopen ("test.txt", "r");

    int i = 1;

    while (*Myfgets (str, MaxChars, f) != EOF) {

        printf ("Test %d\n", i);

        printf ("Myputs: ");

        Myputs (str);

        int TestNum = Myatoi (str);

        printf ("Myatoi: %d\n", TestNum);

        Myitoa (TestNum, str, 10);

        printf ("Myiota: %s\n ", str);

        printf ("\n");

        ++i;

    }

    fclose (f);

    return 0;
}

int Myputs (const char* str) {

    assert (str != NULL);

    for (str; *str != EOF && *str != '\0' && *str != '\n'; ++str){

        assert (putc (*str, stdout) != EOF);

    }

    assert (putc ('\n', stdout) != EOF);

    return 0;
}

char* Myfgets (char* str, const int num, FILE* file) {

    assert (str != NULL);
    assert (num > 0);
    assert (file != NULL);

    char* str0 = str;

    for (int i = 0; i < num && (*str = fgetc(file)) != EOF && *str != '\n' && *str != '\0'; ++i, ++str);

    return str0;
}

char* Myitoa (int num, char* str, int base = 10) {

    assert (str != NULL);
    assert (base > 1);

    if (num == 0) {
        *str = '0';
        *(str + 1) = '\0';
        return str;
    }

    int length = 0;
    const int Zero = '0';
    const int A = 'A';
    const int Fletter = 10;
    int mod = 0;

    if (num < 0) {

        *str = '-';
        ++str;
        ++length;
        num *= (-1);
    }


    while (num != 0) {

        mod = num%base;
        num /= base;

        if (mod >= 0 && mod <= 9) {
            *str = mod + Zero;
            ++length;
            ++str;
        }
        else {
            *str = mod - Fletter + A;
            ++length;
            ++str;
        }

    }

    str -= length--;

    if (*str == '-') {
        ++str;
        --length;
    }


    char buffer = '0';

    for (int i = 0; i <= length/2; ++i) {
        buffer = *(str+i);
        *(str+i) = *(str+length - i);
        *(str+length - i) = buffer;
    }

    *(str+length + 1) = '\0';

    return str;
}

int Myatoi(char* str) {

    assert (str != NULL);

    int num = 0;

    const char Zero = '0';

    int i = 0;

    int sign = 1;

    while (*str == ' ') {
        ++str;

    }

    if (*str == '-') {

        sign = -1;
        ++str;

    }

    while (*str >= '0' && *str <= '9') {

        num *= 10;
        num += (*str - Zero);
        ++str;

    }

    return num*sign;
}
