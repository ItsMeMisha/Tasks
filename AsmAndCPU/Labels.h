#include <stdio.h>
#include <assert.h>
#include <string.h>

const int MaxLabels = 256;
const int MaxStrLen = 256;

#ifdef DEBUG

    #define ASSERT( cond ) assert (cond)

#else

    #define ASSERT( cond ) assert (cond)

#endif

struct label {

    char name[MaxStrLen];
    int place;

};

struct ManyLabels {

    label array[MaxLabels];
    int num;

};

/*! This function checks if label already exist
*
*   @return index of label if it exists and index = lblArr.num if it does not
*
*/

int LabelExist (ManyLabels lblArr, char* NewLabelName) {

    ASSERT (NewLabelName);
    
    int index = 0;

    for (index; index < lblArr.num; ++index) 
        if (strncmp (lblArr.array[index].name, NewLabelName, MaxStrLen) == 0)
            break;
    
    return index;

}

/*! This function adds new label with name newName and place newPlace to label's array
*
*   @param lblArr - array of labels
*   @param newName
*   @param newPlace
*
*   @return true if label was successfully added
*
*/

bool AddNewLabel (ManyLabels* lblArr, char* newName, int newPlace) {

    ASSERT (lblArr);
    ASSERT (newName);

    if (lblArr -> num < MaxLabels) {

        strncpy (lblArr -> array[lblArr -> num].name, newName, MaxStrLen);
        lblArr -> array[lblArr -> num].place = 0;
        ++(lblArr -> num);

        return true;

    }

    else {

        printf ("ERROR: too many unique labels\n");
        return false;

    }

}

#undef ASSERT
