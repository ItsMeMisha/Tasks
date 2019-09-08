#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! This function puts the bytes of an integer number in reverse order
*
* param[in] number - pointer to a number
*
*/

void ReverseBytesOne (int* number);

/*! This function puts the bytes of an integer number in reverse order
*
* param[in] number - pointer to a number
*
*/

void ReverseBytesTwo (int* number);

/*! This program checks functions ReverseBytesOne and ReverseBytesTwo */

int main() {

    int num = (12343142);

    printf ("%d %d %d %d\n", *((char*) &num), *((char*) &num + 1), *((char*) &num + 2), *((char*) &num + 3));

    ReverseBytesOne (&num);

    printf ("%d %d %d %d\n", *((char*) &num), *((char*) &num + 1), *((char*) &num + 2), *((char*) &num + 3));

    ReverseBytesTwo (&num);

    printf ("%d %d %d %d\n", *((char*) &num), *((char*) &num + 1), *((char*) &num + 2), *((char*) &num + 3));

    return 0;
}

void ReverseBytesOne (int* number) {

    assert (number != NULL);


    char buf = *((char*) number);

    *((char*) number) = *((char*) number + 3);

    *((char*) number + 3) = buf;


    buf = *((char*) number + 1);

    *((char*) number + 1) = *((char*) number + 2);

    *((char*) number + 2) = buf;

}

void ReverseBytesTwo (int* number) {

    assert (number != NULL);


    char* byte0 = (char*) number;

    char* byte1 = byte0 + 1;

    char* byte2 = byte1 + 1;

    char* byte3 = byte2 + 1;


    char buf = *byte0;

    *byte0 = *byte3;

    *byte3 = buf;


    buf = *byte1;

    *byte1 = *byte2;

    *byte2 = buf;

}
