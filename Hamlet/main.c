#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <ctype.h>

#define DEBUG

#ifdef DEBUG

    #define ASSERT( cond ) {                               \
                                                           \
        if (!(cond)) {                                     \
                                                           \
            printf ("Assert failed: %s, file %s, line %d", \
            #cond, __FILE__, __LINE__);                    \
                                                           \
            abort ();                                      \
                                                           \
        }                                                  \
                                                           \
                                                           \
    }

#else

    #define Assert( cond ) ;

#endif


void SwapLines(struct Line* str1, struct Line* str2);

char ToLow (char letter);

int NumOfCharsInFile (FILE* file);

void SplitStr (struct Line* ArrOfPtr, char* String, const int NumOfChars);

int CompGreaterStraightStr (struct Line strA, struct Line strB);

int CompGreaterReverseStr (struct Line strA, struct Line strB);

int NumOfSymbol (char* string, const int NumChars, const char symbol);

int PrintStrInFile (FILE* file, struct Line* strings, int NumberOfStrings);

void QsortLines (struct Line* Array, int Left, int Right, int (*Sorting)(struct Line, struct Line));

struct Line {

   char* Start;

   char* End;

};

/*! This program sorts lines of Shakespear's "Hamlet" in alphabet order,
*   sorts lines for rhyme dictionary and shows that it hasn't broken the tragedy
*/

int main () {

	FILE* Poem = fopen ("ShakespeareIn.txt", "r");

	ASSERT (Poem != NULL);

	const int NumOfChars = NumOfCharsInFile (Poem);

	char* AllLines = (char*) calloc (NumOfChars+1, sizeof (char) );

	fread (AllLines, sizeof (char), NumOfChars, Poem);

	const int NumOfLines = NumOfSymbol (AllLines, NumOfChars, '\n') + NumOfSymbol (AllLines, NumOfChars, '\0');

	struct Line *ArrStr = (struct Line*) calloc (NumOfLines+1, sizeof (struct Line) ); // Array with pointers to strings

	SplitStr (ArrStr, AllLines, NumOfChars);

	struct Line *Originally = (struct Line*) calloc (NumOfLines+1, sizeof (struct Line) ); // Array with pointers to strings;

	SplitStr (Originally, AllLines, NumOfChars);

	QsortLines (ArrStr, 0, NumOfLines-1, CompGreaterStraightStr);

    FILE* NewPoem = fopen ("Shakespeare From A to B.txt", "w");

	ASSERT (NewPoem != NULL);

	PrintStrInFile (NewPoem, ArrStr, NumOfLines);

    fclose (NewPoem);

    QsortLines (ArrStr, 0, NumOfLines-1, CompGreaterReverseStr);

    FILE* Rhymes = fopen ("ShakespeareRhyme.txt", "w");

    assert (Rhymes != NULL);

    PrintStrInFile (Rhymes, ArrStr, NumOfLines);

    fclose (Rhymes);

    FILE* Original = fopen ("ShakespeareOriginal.txt", "w");

    ASSERT (Original != NULL);

    PrintStrInFile (Original, Originally, NumOfLines);

    fclose (Original);


	free (ArrStr);
	free (AllLines);

	return 0;
}

/*! This function swaps to lines (strings)
*
* @param str1, str2 - addresses of strings (lines) that should be swaped
*
*/

void SwapLines(struct Line* str1, struct Line* str2) {

    ASSERT (str1 != NULL);
    ASSERT (str2 != NULL);

    struct Line buf = *str1;
    *str1 = *str2;
    *str2 = buf;

}

/*! This function turns uppercase letter to lowercase one
*
* @param[in] letter - turning letter
*
* @return letter as lowercase
*/

char ToLow (char letter) {

    if (letter <= 'Z' && letter >= 'A') {

        letter -= 'A';
        letter += 'a';

    }

    return letter;

}

/*! This function counts number of chars in file
*
* @param[in] file
*
* @return NumOfChars - number of chars
*
*/

int NumOfCharsInFile (FILE* file) {

    ASSERT (file != NULL);

    fseek (file, 0, SEEK_END);

	const int NumOfChars = ftell (file);

	fseek (file, 0, SEEK_SET);

    return NumOfChars;

}

/*! This function splits one string into several (finding '\n' or '\0');
*
* @param[out] ArrOfPtr - array of pointers to different strings
* @param[in] String - one big string
* @param[in] NumOfChars - number of chars, that should be splited
*
*/

void SplitStr (struct Line* ArrOfPtr, char* String, const int NumOfChars){

    ASSERT (ArrOfPtr != NULL);
    ASSERT (String != NULL);

    ArrOfPtr -> Start = String;

    ++ArrOfPtr;

    for (int i = 0; i <= NumOfChars; ++i, ++String) {

        if (*String == '\n' || *String == '\0') {

            *String = '\0';

            (ArrOfPtr - 1) -> End = String;

            ArrOfPtr -> Start= String + 1;

            ++ArrOfPtr;

        }

    }

}

/*! This function compares two strings according to the alphabet
*
* @param[in] strA - first string
* @param[in] strB - second string
*
* @return a value > 0 if first string more than second
* @return a value < 0 if second string more than first
* @return a value = 0 if strings are equal
*
*/

int CompGreaterStraightStr (struct Line strA, struct Line strB) {

    ASSERT (strA.Start != NULL);
    ASSERT (strB.Start != NULL);

    char A = ' ';
    char B = ' ';

    while (*strA.Start != '\0' && *strB.Start != '\0') {

        while ( (!isalpha(*strA.Start)) && (*strA.Start != '\0') )
            ++strA.Start;

        while ( (!isalpha(*strB.Start)) && (*strB.Start != '\0') )
            ++strB.Start;

        A = ToLow (*strA.Start);

        B = ToLow (*strB.Start);

        if ( (A - B) != 0)
            return (int)(A - B);

        else {

            ++strA.Start;
            ++strB.Start;

        }

    }

    return 0;

}

/*! This function compares two strings according to the alphabet in reverse order
*
* @param[in] strA - first string
* @param[in] strB - second string
*
* @return a value > 0 if first string more than second
* @return a value < 0 if second string more than first
* @return a value = 0 if strings are equal
*
*/

int CompGreaterReverseStr (struct Line strA, struct Line strB) {

    ASSERT (strA.Start != NULL);
    ASSERT (strB.Start != NULL);
    ASSERT (strA.End != NULL);
    ASSERT (strB.End != NULL);

    char letterA = ' ';
    char letterB = ' ';

    while (strA.End != strA.Start && strB.End != strB.Start) {

        while ( (!isalpha(*strA.End)) && (strA.End != strA.Start) )
            --strA.End;

        while ( (!isalpha(*strB.End)) && (strB.End != strB.Start) )
            --strB.End;

        letterA = ToLow (*strA.End);

        letterB = ToLow (*strB.End);

        if ( (letterA - letterB) != 0)
            return (int)(letterA - letterB);

        else if ( (strA.End != strA.Start) && (strB.End != strB.Start) ) {

            --strA.End;
            --strB.End;

        }

    }

    return 0;

}

/*! This function counts concrete symbols in string
*
* @param strings
* @param NumChars - number of chars in the string
* @param symbol - concrete symbol
*
* @return NumOfSymbols - number of concrete symbols
*
*/

int NumOfSymbol (char* strings, const int NumChars, const char symbol) {

	ASSERT (strings != NULL);

	int NumOfSymbols = 0;

	for (int i = 0; i <= NumChars; ++i, ++strings) {

	 if (*strings == symbol)
        ++NumOfSymbols;

	}

    return NumOfSymbols;

}

/*! This function records array of strings in file
*
* @param file
* @param strings - array of strings
* @param NumberOfStrings
*
*/

int PrintStrInFile(FILE* file, struct Line* strings, int NumberOfStrings) {

	ASSERT (file != NULL);
	ASSERT (strings != NULL);

	for (int n = 0; n <= NumberOfStrings; ++n)  {

        if (*((strings + n) -> Start) != '\0' && *((strings +n) -> Start) != '\n' && *((strings +n) -> Start) != '\r')
            fprintf (file, "%s\n", (strings + n) -> Start);

    }

	return 0;

}

/*! This function does quick sorting of lines
*
* @param[in] Array - array of lines
* @param[in] Left, Right - borders of sorting
* @param[in] Compare - comparing function that returns a value > 0 if the first line is greater than the second one,
*            a value < 0, if the second line is greater than the first one, and a value = 0 if lines are equal
*
*/

void QsortLines (struct Line* Array, int Left, int Right, int (*Compare)(struct Line, struct Line) ) {

    ASSERT (Array != NULL);
    ASSERT (Left >= 0);

    if (Right - Left < 0)
        return;

        int Last = Left;

        int Mid = (Right + Left)/2;

        SwapLines ( (Array + Left), (Array + Mid) );

        for (int i = Left; i <= Right; ++i) {

            if (Compare ( *(Array + Left), *(Array + i) ) > 0) {

                ++Last;

               SwapLines ( (Array + Last), (Array + i) );

            }

        }

        SwapLines ( (Array + Last), (Array + Left) );

        QsortLines (Array, Left, Last -  1, Compare);
        QsortLines (Array, Last + 1, Right, Compare);

}
