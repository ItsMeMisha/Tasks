#ifndef __LIST__
    
    #define __LIST__

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define _DEBUG //!!!!!!

#ifdef _DEBUG

#define ASSERTLST( lst )                  \
    if (!ListOk (lst)){                   \
        ListDump (lst, __FUNCTION__);     \
        assert (0);                       \
    }
    
#else

#define ASSERTLST( lst ) 

#endif

typedef int Element_t;
const Element_t Poison = {};
const int FirstMaxSize = 101;
const int PosIsFree = -1;

enum Error {

    Allright               = 0,
    WrongPositionReference = 1,
    NoFreePosLeft          = 2,
    NullDataPtr            = 3,
    NullNextPtr            = 4,
    NullPrevPtr            = 5,
    NullFreePosPtr         = 6
    
};

/*! This struct is realization of list data structure
*
*   data - an array of data which type is Element_t \n
*   next - an array of 'pointers' to the next element \n 
*   prev - an array of 'pointers' to the previous element \n
*   freeHead - head of freePos \n
*   freePos - kind of list of free positions of the data \n
*   head - number of the element in data which is the head of list \n
*   tail - number of the element in data which is the tail of list \n
*   maxSize - max size of list \n
*   curSize - current number of elements is list \n
*   sorted - flag is true if list is sorted \n
*   errcode - code of error \n
*
*/

struct List {

    Element_t* data;
    int* next;
    int* prev;
    int* freePos;
    int freeHead;
    int head;
    int tail;
    int maxSize;
    int curSize;
    bool sorted;
    Error errcode;

};


bool ListOk                                 (List* lst);
void ListConstruct                          (List* lst);
void ListDestruct                           (List* lst);
bool PosOk                   (const int pos, List* lst);
bool PosIsFilled             (const int pos, List* lst);
bool NewNodePossible         (const int pos, List* lst);

int  GetFreePos                             (List* lst);
bool DeleteOneFreePos                       (List* lst);
bool AddFreePos              (const int pos, List* lst);

bool InsertFirst   (const Element_t elem,                List* lst);
bool InsertLast    (const Element_t elem,                List* lst);
bool InsertAfter   (const Element_t elem, const int pos, List* lst);
bool InsertBefore  (const Element_t elem, const int pos, List* lst);

bool CheckDeleteLast                        (List* lst);
bool DeleteFirst                            (List* lst);
bool DeleteLast                             (List* lst);
bool Delete                  (const int pos, List* lst);
bool DeleteAfter             (const int pos, List* lst);
bool DeleteBefore            (const int pos, List* lst);

void SortList                               (List* lst);
void ListDump                               (List* lst, const char* funcName);
void DrawList      (FILE* file,              List* lst);
void ErrDecode                              (List* lst);

int IntCompare (const void* numBigger, const void* numLess);
int FindLogPosOfElement   (const Element_t elem, List* lst, int compare (const void*, const void*) = IntCompare); 

bool ListOk (List* lst) {

    if (lst == nullptr) 
        return false;

    if (lst -> errcode != Allright)
        return false;

    if (lst -> data == nullptr) {

        lst -> errcode = NullDataPtr;
        return false;

    }

     if (lst -> next == nullptr) {

        lst -> errcode = NullNextPtr;
        return false;

    }

    if (lst -> prev == nullptr) {

        lst -> errcode = NullPrevPtr;
        return false;

    }

    if (lst -> freePos == nullptr) {

        lst -> errcode = NullFreePosPtr;
        return false;

    }

    return true;

}

/*! This is a constructor for List-type lists
*
*   @param lst - pointer to the list
*
*/

void ListConstruct (List* lst) {

    assert (lst);
    lst -> data = (Element_t*) calloc (FirstMaxSize, sizeof (Element_t));
    lst -> next = (int*) calloc (FirstMaxSize, sizeof (int));
    lst -> freePos = lst -> next;
    lst -> prev = (int*) calloc (FirstMaxSize, sizeof (int));
    lst -> freeHead = 1;
 
    for (int i = 1; i < FirstMaxSize - 1; ++i) {

        lst -> freePos[i] = i + 1;
        lst -> prev[i] = -1;

    }

    lst -> freePos[FirstMaxSize - 1] = 0;
    lst -> head = 0;
    lst -> tail = 0;
    lst -> maxSize = FirstMaxSize;
    lst -> curSize = 0;
    lst -> sorted = true;
    lst -> errcode = Allright;

    return;

}

/*! This is a destructor for the List-type lists
*
*   @param lst - pointer to the list
*
*/

void ListDestruct (List* lst) {

    ASSERTLST (lst);

    free (lst -> data);
    free (lst -> next);
    free (lst -> prev);

    lst -> freeHead = 0;
    lst -> head = 0;
    lst -> tail = 0;
    lst -> maxSize = 0;
    lst -> curSize = 0;
    lst -> errcode = Allright;

    return;

}

bool PosOk (const int pos, List* lst) {

    ASSERTLST (lst);

    if ((pos < 0) && (pos > lst -> maxSize)) {
        lst -> errcode = WrongPositionReference;
        return false;
    }

    return true;    

}

bool PosIsFilled (const int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

    if (lst -> prev[pos] <= PosIsFree)
        return false;

    return true;

}

bool NewNodePossible (const int pos, List* lst) {

    if (pos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;
    
    return true;

}

/*! This function returns next free position in the list and DOES NOT delete it
*
*   @param lst - pointer to the list
*
*   @return next free position
*
*/

int GetFreePos (List* lst) {

    ASSERTLST (lst);

    if (lst -> freeHead <= 0) {

        lst -> errcode = NoFreePosLeft;
        return 0;

    }

    return lst -> freeHead; 

}

/*! This function deletes next free position from the list of free positions
*
*   @param lst - pointer to the list
*
*/

bool DeleteOneFreePos (List* lst) {

    ASSERTLST (lst);

    if (lst -> freeHead <= 0) 
        return false;

    int nextFree = lst -> freePos[lst -> freeHead];

    lst -> freePos[lst -> freeHead] = -1;
    lst -> prev[lst -> freeHead] = 0;
    lst -> freeHead = nextFree;

    return true;

}

/*! This function adds new free position from to list of free positions
*
*   @param pos - position that should be added
*   @param lst - pointer to the list
*
*/

bool AddFreePos (const int pos, List* lst) {

    ASSERTLST (lst);
    
    if (!PosOk (pos, lst))
        return false;
    
    lst -> freePos[pos] = lst -> freeHead;
    lst -> prev[pos] = PosIsFree;
    lst -> freeHead = pos;
        
    return true;

}

/*! This function add new element to the begin of the list
*
*   @param elem - inserting element
*   @param lst - pointer to the list
*
*/

bool InsertFirst (const Element_t elem, List* lst) {

    ASSERTLST (lst);

    int NewElemPos = GetFreePos (lst);
    
    if (!NewNodePossible (NewElemPos, lst))
        return false;

    lst -> data[NewElemPos] = elem;
 
    lst -> prev[lst -> head] = NewElemPos;
    lst -> next[NewElemPos] = lst -> head;
    lst -> head = NewElemPos;

    if (lst -> tail <= 0)
        lst -> tail = lst -> head;

    lst -> sorted = false;

    lst -> curSize++;

    return true;

}

/*! This function add new element to the end of the list
*
*   @param elem - inserting element
*   @param lst - pointer to the list
*
*/

bool InsertLast (const Element_t elem, List* lst) {

    ASSERTLST (lst);

    int NewElemPos = GetFreePos (lst);
 
    if (!NewNodePossible (NewElemPos, lst))
        return false;
 
    lst -> data[NewElemPos] = elem;

    lst -> next[lst -> tail] = NewElemPos;
    lst -> prev[NewElemPos] = lst -> tail;
    lst -> tail = NewElemPos;

    if (lst -> head <= 0)
        lst -> head = lst -> tail;

    if (NewElemPos != lst -> curSize)
        lst -> sorted = false;

    lst -> curSize++;

    return true;
 
}

/*! This function add new element after the element on pos positon
*
*   @param elem - inserting element
*   @param pos - position
*   @param lst - pointer to the list
*
*/

bool InsertAfter (const Element_t elem, const int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosIsFilled (pos, lst))
        return false;
   
    if (pos == lst -> tail)
        return InsertLast (elem, lst);

    if (pos == 0)
        return InsertFirst (elem, lst);

//TODO maybe i should forbid to insert after 0?

    if (!PosIsFilled (pos, lst))
        return false;

    int NewElemPos = GetFreePos (lst);
 
    if (!NewNodePossible (NewElemPos, lst))
        return false;
   
    lst -> data[NewElemPos] = elem;

    lst -> next[NewElemPos] = lst -> next[pos];
    lst -> prev[NewElemPos] = pos;
 
    lst -> prev[lst -> next[pos]] = NewElemPos;
    lst -> next[pos] = NewElemPos;

    lst -> sorted = false;

    lst -> curSize++;

    return true;

}

/*! This function add new element before the element on pos positon
*
*   @param elem - inserting element
*   @param pos - position
*   @param lst - pointer to the list
*
*/

bool InsertBefore (const Element_t elem, const int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosIsFilled (pos, lst))
        return false;

    if (pos == lst -> head)
        return InsertFirst (elem, lst);

    return InsertAfter (elem, lst -> prev[pos], lst);

}

bool CheckDeleteLast (List* lst) {

    ASSERTLST (lst);

    if (lst -> tail == lst -> head) {

        lst -> tail = 0;
        lst -> head = 0;

        lst -> sorted = true;
        return true;

    }   

    return false;
 
}

/*! This function deletes the first element in the list
*
*   @param - lst - pointer to the list
*
*/

bool DeleteFirst (List* lst) {

    ASSERTLST (lst);

    if (!AddFreePos (lst -> head, lst))
        return false;

    lst -> data[lst -> head] = Poison;
    lst -> curSize--;
    
    if (CheckDeleteLast (lst))
        return true;
   
    int nextElem = lst -> next[lst -> head];
    lst -> prev[nextElem] = -1;
    lst -> next[lst -> head] = -1;

    lst -> head = nextElem;

    lst -> sorted = false;

    return true;

}

/*! This function deletes the last element in the list
*
*   @param lst - pointer to the list
*
*/

bool DeleteLast (List* lst) {

    ASSERTLST (lst);

    if (!AddFreePos (lst -> tail, lst))
        return false;

    lst -> data[lst -> tail] = Poison;
    lst -> curSize--;

    if (CheckDeleteLast (lst))
        return true;
    
    int prevElem = lst -> prev[lst -> tail];
    lst -> next[prevElem] = -1;
    lst -> prev[lst -> tail] = -1;
    lst -> tail = prevElem;

    return true;

}

/*! This function deletes the element on the pos position in the list
*
*   @param pos - position
*   @param lst - pointer to the list
*
*/

bool Delete (const int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosIsFilled (pos, lst))
        return false;

    if (pos == lst -> head)
        return DeleteFirst (lst);

    if (pos == lst -> tail)
        return DeleteLast (lst);

    lst -> next[lst -> prev[pos]] = lst -> next[pos];
    lst -> prev[lst -> next[pos]] = lst -> prev[pos];
    lst -> data[pos] = Poison;
    lst -> next[pos] = 0;
    lst -> prev[pos] = PosIsFree;

    if (!AddFreePos (pos, lst))
        return false;

    lst -> curSize--;

    return true;

}

/*! This function deletes the element after the element on the pos position in the list
*
*   @param pos - position
*   @param lst - pointer to the list
*
*/


bool DeleteAfter (const int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosIsFilled (pos, lst))
        return false;

    return Delete (lst -> next[pos], lst);

}

/*! This function deletes the element before the element on the pos position in the list
*
*   @param pos - position
*   @param lst - pointer to the list
*
*/

bool DeleteBefore (const int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosIsFilled (pos, lst))
        return false;

    return Delete (lst -> prev[pos], lst);

}

void SortList (List* lst) {

    ASSERTLST (lst);

    if (lst -> curSize <= 1) {

        lst -> sorted = true;
        return;

    }

    memset (lst -> prev, lst -> maxSize + 1, lst -> maxSize * sizeof (int));

    lst -> prev[lst -> head] = 1;

    int curNext = lst -> next[lst -> head];

    for (int i = 2; i <= lst -> curSize; ++i) {

        lst -> prev[curNext] = i;
        curNext = lst -> next[curNext];

    }

    qsort (lst -> prev + 1, lst -> maxSize, sizeof (int), IntCompare); 

    for (int i = 1; i < lst -> maxSize; ++i) {

        lst -> next[i] = i + 1;
        lst -> prev[i] = i - 1;

    }

    lst -> next[lst -> curSize] = -1;

    lst -> head = 1;
    lst -> tail = lst -> curSize;
    lst -> freeHead = lst -> tail + 1;
    lst -> freePos[lst -> maxSize - 1] = -1;

    memset (lst -> prev + lst -> curSize + 1, -1, (lst -> maxSize - lst -> curSize) * sizeof (int));

    lst -> sorted = true;

    return;

}

void ListDump (List* lst, const char* funcName) {

//...
    FILE* fileout = fopen ("list.dot", "w");

    printf ("In function %s\n", funcName);

    if (lst == nullptr) {

        printf ("List is nullptr\n");
        return;

    }

    printf ("List [%p]\n", lst);
    

    if (lst -> errcode == Allright)
        printf ("OK\n");
    else 
        printf ("ERROR\n");

    ErrDecode (lst);         
    
    DrawList (fileout, lst);
    fclose (fileout);

    return;

}

void DrawList (FILE* file, List* lst) {

    fprintf (file, "digraph\n{\n"); 
    fprintf (file, "rankdir=LR;\n");
    fprintf (file, "edge[color=white];\n");

    for (int i = 1; i < lst -> maxSize; ++i) {

        fprintf (file, "\t%x[color =", lst -> data + i);
        if (lst -> prev[i] == PosIsFree)
            fprintf (file, "green, ");
        else
            fprintf (file, "red, ");

        fprintf (file, "shape = record, label = \"{%d | {%d | %d} | %d }\"]\n", lst -> prev[i], i, lst -> data[i], lst -> next[i]);

    }

    for (int i = 1; i < lst -> maxSize; ++i) 
        fprintf (file, "%x -> ", lst -> data + i);

    fprintf (file, "%x\n", lst -> data + lst -> maxSize);

    fprintf (file, "edge[color=black];");

    int cur = lst -> head;

    for (int i = 1; i < lst -> curSize; ++i) {

        fprintf (file, "%x -> ", lst -> data + cur);
        cur = lst -> next[cur];

    }

    fprintf (file, "%x\n", lst -> data + lst -> tail);
 
    cur = lst -> freeHead; 

    for (int i = 1; i < lst -> maxSize - lst -> curSize; ++i) {

        fprintf (file, "%x -> ", lst -> data + cur);
        cur = lst -> freePos[cur];

    }

    fprintf (file, "%x\n", lst -> data + cur);

  
    fprintf (file, "}\n");

    return; 

}

void ErrDecode (List* lst) {

    switch (lst -> errcode) {    

    case Allright: printf ("Allright \n"); break;
    case WrongPositionReference: printf ("Wrong Position Reference \n"); break;
    case NoFreePosLeft: printf ("No free positions left \n"); break;
    case NullDataPtr: printf ("lst -> data is nullptr \n"); break;
    case NullNextPtr: printf ("lst -> next is nullptr \n"); break;
    case NullPrevPtr: printf ("lst -> prev is nullptr \n"); break;
    case NullFreePosPtr: printf ("lst -> freePos is nullptr \n"); break;

    default: printf ("Unexpected error \n"); break;

    }

}

int IntCompare (const void* numBigger, const void* numLess) {

    assert (numBigger);
    assert (numLess);

    return (*((int*) numBigger) - *((int*) numLess));

}

int FindLogPosOfElement (Element_t elem, List* lst, int compare (const void*, const void*)) {

    ASSERTLST (lst);

    for (int i = 1; i > 0; i = lst -> next[i])
        if (compare (&elem, lst -> data + i) == 0)
            return i;

    return 0;    

}

#endif
