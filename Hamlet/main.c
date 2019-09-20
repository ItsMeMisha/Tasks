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
    }

#else

    #define ASSERT( cond ) ;

#endif

struct Line {

   char* Start;

   char* End;

};

char ToLow (char letter);

int NumOfCharsInFile (FILE* file);

void SplitStr (struct Line* ArrOfPtr, char* String, const int NumOfChars);

int CompGreaterStraightStr (const void* str1, const void* str2);

int CompGreaterReverseStr (const void* str1, const void* str2);

int NumOfSymbol (char* strings, const int NumChars, const char symbol);

int PrintStrToFile (FILE* file, struct Line* strings, int NumberOfStrings);

void AddLinesToFile (const char* FileName, struct Line* ArrLines, int NumOfLines);

char* FileToString (const char* FileName, int* NumOfChars);


#define TEST

#ifdef TEST

void TestToLow ();

void TestCompGreaterStraightStr ();

void TestCompGreaterReverseStr ();

void TestNumOfSymbol ();

int main () {

    TestToLow ();

    TestCompGreaterStraightStr ();

    TestCompGreaterReverseStr ();

    TestNumOfSymbol ();

    return 0;

}

void TestToLow () {

    printf ("\n\tTesting ToLow\n\n");

    const int TestNum = 8;

    char    inputChar[TestNum] = {'a', 'A', '"', '\0', '\n', 'Ф', 'B', 'z'};
    char expectedChar[TestNum] = {'a', 'a', '"', '\0', '\n', 'Ф', 'b', 'z'};

    char ResChar = -1;

    int flag = 1;

    for (int i = 0; i < TestNum; ++i) {

        ResChar = ToLow (inputChar[i]);

        if (ResChar == expectedChar[i])
            printf ("Successful test %d, get element ""%c"", expected ""%c""\n\n", i, ResChar, expectedChar[i]);

        else {

            printf ("FAILED test %d, get element ""%c"", expected ""%c""\n\n", i, ResChar, expectedChar[i]);

            flag = 0;

        }

    }

    if(!flag)
        printf ("FAILD TESTS");

    else
        printf ("\tSUCCESSFUL TESTS\n");


}

void TestCompGreaterStraightStr () {

    printf ("\n\tTesting CompGreaterStraightStr\n\n");

    const int TestNum = 5;

    Line Str1[TestNum] = {"1",     Str1[0].Start + 1,
                          "abc",   Str1[1].Start + 3,
                          "Abc",   Str1[2].Start + 3,
                          "",      Str1[3].Start,
                          "zbDzx", Str1[4].Start};

    Line Str2[TestNum] = {"123",  Str2[0].Start + 1,
                          "abcd", Str2[1].Start + 4,
                          "ABC",  Str2[2].Start + 3,
                          "",     Str2[3].Start,
                          "Dcseds",     Str2[4].Start};

    int expectedRes[TestNum] = {0, -(int)'d', 0, 0, (int)('z' - 'd')};

    int flag = 1;

    for (int i = 0; i < TestNum; ++i) {

        int Result = CompGreaterStraightStr ((void*)&Str1[i], (void*)&Str2[i]);

        if (Result == expectedRes[i])
            printf ("Successful test %d, get result ""%d"", expected ""%d""\n\n", i, Result, expectedRes[i]);

        else {

            printf ("FAILED test %d, get result ""%d"", expected ""%d""\n\n", i, Result, expectedRes[i]);

            flag = 0;

        }

    }

    if(!flag)
        printf ("FAILD TESTS");

    else
        printf ("\tSUCCESSFUL TESTS\n");

}

void TestCompGreaterReverseStr () {

    printf ("\n\tTesting CompGreaterReverseStr\n\n");

    const int TestNum = 5;

    Line Str1[TestNum] = {"",      Str1[0].Start,
                          "1",     Str1[1].Start + 1,
                          "1",     Str1[2].Start + 1,
                          "a",     Str1[3].Start + 1,
                          "zxDsq", Str1[4].Start + 5};

    Line Str2[TestNum] = {"",      Str2[0].Start,
                          "23262", Str2[1].Start + 5,
                          "1",     Str2[2].Start + 1,
                          "b",     Str2[3].Start + 1,
                          "Dswwq", Str2[4].Start + 5};

    int expectedRes[TestNum] = {0, 0, 0, (int)('a'-'b'), (int)('s'-'w')};

    int flag = 1;

    for (int i = 0; i < TestNum; ++i) {

        int Result = CompGreaterReverseStr ((void*)&Str1[i], (void*)&Str2[i]);

        if (Result == expectedRes[i])
            printf ("Successful test %d, get result ""%d"", expected ""%d""\n\n", i, Result, expectedRes[i]);

        else {

            printf ("FAILED test %d, get result ""%d"", expected ""%d""\n\n", i, Result, expectedRes[i]);

            flag = 0;

        }

    }

    if(!flag)
        printf ("FAILD TESTS");

    else
        printf ("\tSUCCESSFUL TESTS\n");

}

void TestNumOfSymbol () {

    printf ("\n\tTesting NumOfSymbol\n\n");

    const int TestNum = 5;

    char* TestStr= "adwwad01932#2$%AASWEFGGFF3214";

    const int NumOfChars = 29;

    char TestSymbol[TestNum] = {'a', 'w', '2', 'F', '%'};

    int expectedRes[TestNum] = { 2 ,  2 ,  3 ,  3 ,  1 };


    int flag = 1;

    for (int i = 0; i < TestNum; ++i) {

        int Result = NumOfSymbol (TestStr, NumOfChars, TestSymbol[i]);

        if (Result == expectedRes[i])
            printf ("Successful test %d, get result ""%d"", expected ""%d""\n\n", i, Result, expectedRes[i]);

        else {

            printf ("FAILED test %d, get result ""%d"", expected ""%d""\n\n", i, Result, expectedRes[i]);

            flag = 0;

        }

    }

    if(!flag)
        printf ("FAILD TESTS");

    else
        printf ("\tSUCCESSFUL TESTS\n");
}


#else

/*! This program sorts lines of Shakespear's "Hamlet" in alphabet order,
*   sorts lines for rhyme dictionary and shows that it hasn't broken the tragedy
*/

int main () {

	const char* NameOfSorted = "Shakespeare Sorted.txt";

	const char* NameOfFile = "ShakespeareIn.txt";

	int NumOfChars = 0;

	char* AllLines = FileToString (NameOfFile, &NumOfChars);

	int NumOfLines = NumOfSymbol (AllLines, NumOfChars, '\n') + NumOfSymbol (AllLines, NumOfChars, '\0');

	Line *ArrStr = (struct Line*) calloc (NumOfLines+1, sizeof (struct Line) ); // Array with pointers to strings

	SplitStr (ArrStr, AllLines, NumOfChars);

	Line *Originally = (struct Line*) calloc (NumOfLines+1, sizeof (struct Line) );

	SplitStr (Originally, AllLines, NumOfChars);



    qsort (ArrStr, NumOfLines, sizeof(Line), CompGreaterStraightStr);

    AddLinesToFile (NameOfSorted, ArrStr, NumOfLines);

    qsort (ArrStr, NumOfLines, sizeof(Line), CompGreaterReverseStr);

    AddLinesToFile (NameOfSorted, ArrStr, NumOfLines);

    AddLinesToFile (NameOfSorted, Originally, NumOfLines);

	free (ArrStr);
	free (AllLines);

	return 0;

}

#endif


/*! This function turns uppercase letter to lowercase one
*
* @param[in] letter - turning letter
*
* @return letter as lowercase
*/

char ToLow (char letter) {

    if (letter <= 'Z' && letter >= 'A') {

        letter += ('a' - 'A');

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

void SplitStr (Line* ArrOfPtr, char* String, const int NumOfChars){

    ASSERT (ArrOfPtr != NULL);
    ASSERT (String != NULL);

    ArrOfPtr -> Start = String;

    ++ArrOfPtr;

    for (int i = 0; i <= NumOfChars; ++i, ++String) {

        if (*String == '\n' || *String == '\0') {

            *String = '\0';

            (ArrOfPtr - 1) -> End = String;

            ArrOfPtr -> Start = String + 1;

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

int CompGreaterStraightStr (const void* str1, const void* str2) {

    ASSERT (str1 != NULL);
    ASSERT (str2 != NULL);

    Line strA = *(Line*) str1;
    Line strB = *(Line*) str2;

    char A = ' ';
    char B = ' ';

    do {

        while ( (!isalpha(*strA.Start)) && (strA.Start != strA.End) )
            ++strA.Start;

        while ( (!isalpha(*strB.Start)) && (strB.Start != strB.End) )
            ++strB.Start;

        A = ToLow (*strA.Start);

        B = ToLow (*strB.Start);


        if ((!isalpha(A)) && (!isalpha(B)))
            return 0;

        if (!isalpha(A))
            return -10;

        if (!isalpha(B))
            return 10;

        if ( (A - B) != 0)
            return (int)(A - B);

        else {

            if (strA.Start != strB.End)
              A = *(++strA.Start);

            if (strB.Start != strB.End)
              B = *(++strB.Start);

        }

    }

    while (strA.Start != strA.End && strB.Start != strB.End);

    if ( (A - B) != 0)
        return (int)(A - B);

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

int CompGreaterReverseStr (const void* str1, const void* str2) {

    ASSERT (str1 != NULL);
    ASSERT (str2 != NULL);

    Line strA = *(Line*) str1;
    Line strB = *(Line*) str2;

    char A = ' ';
    char B = ' ';

    do {

        while ( (!isalpha(*strA.End)) && (strA.End != strA.Start) )
            --strA.End;

        while ( (!isalpha(*strB.End)) && (strB.End != strB.Start) )
            --strB.End;

        A = ToLow (*strA.End);

        B = ToLow (*strB.End);


        if ((!isalpha(A)) && (!isalpha(B)))
            return 0;

        if (!isalpha(A))
            return -10;

        if (!isalpha(B))
            return 10;

        if ( (A - B) != 0)
            return (int)(A - B);

        else if ( (strA.End != strA.Start) && (strB.End != strB.Start) ) {

           A = *(--strA.End);
           B = *(--strB.End);

        }

    } while (strA.End != strA.Start && strB.End != strB.Start);

    if ( (A - B) != 0)
        return (int)(A - B);

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

int PrintStrToFile(FILE* file, Line* strings, int NumberOfStrings) {

	ASSERT (file != NULL);
	ASSERT (strings != NULL);

	for (int n = 0; n < NumberOfStrings; ++n)  {

        if (*((strings + n) -> Start) != '\0' && *((strings + n) -> Start) != '\n' && *((strings +n) -> Start) != '\r')
            fprintf (file, "%s\n", (strings + n) -> Start);

    }

    fprintf (file, "\n");

	return 0;

}

/*! This function adds lines into the end of file
*
* @param FileName - name of file
* @param ArrLines - an array of lines
* @param NumOfLines - number of lines in array
*
*/

void AddLinesToFile (const char* FileName, Line* ArrLines, int NumOfLines) {

    ASSERT (FileName != NULL);
    ASSERT (ArrLines != NULL);

    FILE* file = fopen (FileName, "a");

	ASSERT (file != NULL);

	PrintStrToFile (file, ArrLines, NumOfLines);

    fclose (file);

}

/*! This function records characters of file to one string
*
*   @param FileName - name of file
*   @param NumOfChars - address of the variable where records number of characters in file
*
*   @return AllLines - string of characters !!!AllLines needs to free!!!
*
*/

char* FileToString (const char* FileName, int* NumOfChars) {

    FILE* file = fopen (FileName, "r");

	ASSERT (file != NULL);

	*NumOfChars = NumOfCharsInFile (file);

	char* AllLines = (char*) calloc (*NumOfChars+1, sizeof (char) );

	fread (AllLines, sizeof (char), *NumOfChars, file);

	fclose (file);

    return AllLines;

}
