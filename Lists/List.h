#ifndef __LIST__
    
    #define __LIST__

#include <stdio.h>

#ifdef __DEBUG

#define ASSERTLST( lst )    \
    if (!ListOk (lst)){     \
        ListDump (lst);     \
        abort ();           \
    }
    
#endif

typedef Element_t int;
const Element_t Poison = {};
const int FirstMaxSize = 101;

enum Error {

    Allright = 0,
    WrongPositionReference = 1

};

/*! This struct is realization of list data structure
*
*   data - an array of data which type is Element_t\n
*   next - an array of 'pointers' to the next element 
*   prev - an array of 'pointers' to the previous element
*   freeHead - head of freePos
*   freePos - kind of list of free positions of the data
*   head - number of the element in data which is the head of list
*   tail - number of the element in data which is the tail of list
*   maxSize - max size of list
*   curSize - current number of elements is list
*   sorted - flag is true if list is sorted
*   errcode - code of error
*
*/

struct List {

    Element_t* data;
    int* next;
    int* prev;
    int freeHead;
    int* freePos;
    int head;
    int tail;
    int maxSize;
    int curSize;
    bool sorted;
    Error errcode

};


bool ListOk                 (List* lst);
void ListConstruct          (List* lst);
void ListDestruct           (List* lst);
bool PosOk         (int pos, List* lst);

int  GetFreePos             (List* lst);
bool DeleteOneFreePos       (List* lst);
bool AddFreePos    (int pos, List* lst);

bool InsertFirst           (Element_t elem, List* lst);
bool InsertLast            (Element_t elem, List* lst);
bool InsertAfter  (Element_t elem, int pos, List* lst);
bool InsertBefore (Element_t elem, int pos, List* lst);

bool DeleteFirst            (List* lst);
bool DeleteLast             (List* lst);
bool Delete        (int pos, List* lst);
bool DeleteAfter   (int pos, List* lst);
bool DeleteBefore  (int pos, List* lst);

void SortList               (List* lst);
void ListDump               (List* lst);

Element_t FindElementByPosition (int pos, List* lst);
int FindPosOfElement (Element_t elem, List* lst, int* compare (Element_t, Element_t) = nullptr); 

bool ListOk (List* lst) {}

/*! This is a constructor for List-type lists
*
*   @param lst - pointer to the list
*
*/

void ListConstruct (List* lst) {

    assert (lst);
    lst -> data = (Element_t*) calloc (FirstMaxSize, sizeof (Element_t*));
    lst -> next = (int*) calloc (FirstMaxSize, sizeof (int));
    lst -> prev = (int*) calloc (FirstMaxSize, sizeof (int));
    lst -> freeHead = 1;
    lst -> freePos = (int*) calloc (FirstMaxSize, sizeof (int));
    
    for (int i = 1; i < FirstMaxSize - 1; ++i)
        freePos[i] = i + 1;

    freePos[FirstMaxSize -1] = -1;
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
    free (lst -> freePos);

    lst -> freeHead = 0;
    lst -> head = 0;
    lst -> tail = 0;
    lst -> maxSize = 0;
    lsr -> curSize = 0;
    lst -> errcode = Allright;

    return;

}

bool PosOk (int pos, List* lst) {

    ASSERTLST (lst);

    if ((pos < 0) && (pos > lst -> maxSize)) {
        lst -> errcode = WrongPositionReference;
        return false;
    }
//TODO!!!!IT IS THE WRONG PosOK!!!!!!!!!!!!!!!
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

    if (lst -> freeHead <= 0)
        return 0;

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
    lst -> freeHead = nextFree;

    return true;

}

/*! This function adds new free position from to list of free positions
*
*   @param pos - position that should be added
*   @param lst - pointer to the list
*
*/

bool AddFreePos (int pos, List* lst) {

    ASSERTLST (lst);
    
    if (!PosOk (pos, lst))
        return false;
    
    lst -> freePos[pos] = lst -> freeHead;
    lst -> freeHead = pos;
        
    return true;

}

/*! This function add new element to the begin of the list
*
*   @param elem - inserting element
*   @param lst - pointer to the list
*
*/

bool InsertFirst (Element_t elem, List* lst) {

    ASSERTLST (lst);

    int NewElemPos = GetFreePos (lst);
    
    if (NewElemPos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;

    lst -> data[NewElemPos] = elem;
 
    lst -> prev[lst -> head] = NewElemPos;
    lst -> next[NewElemPos] = lst -> head;
    lst -> head = NewElemPos;

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

bool InsertLast (Element_t elem, List* lst) {

    ASSERTLST (lst);

    int NewElemPos = GetFreePos (lst);
    
    if (NewElemPos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;

    lst -> data[NewElemPos] = elem;

    lst -> next[lst -> tail] = NewElemPos;
    lst -> prev[NewElemPos] = lst -> tail;
    lst -> tail = NewElemPos;

    if (NewElemPos != lst -> curSize - 1)
        lsr -> sorted = false;

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

bool InsertAfter (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;
   
    if (pos == lst -> tail)
        return InsertLast (elem, lst);

    if (pos == 0)
        return InsertFirst (elem, lst);

//TODO maybe i should forbid to insert after 0?

    int NewElemPos = GetFreePos (lst);
    
    if (NewElemPos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;

    lst -> data[NewElemPos] = elem;

    lst -> next[NewElemPos] = next[pos];
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

bool InsertBefore (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

    if (pos == lst -> head)
        return InsertFirst (elem, lst);

    int NewElemPos = GetFreePos (lst);

    if (NewElemPos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;

    lst -> data[NewElemPos] = elem;

    lst -> next[NewElemPos] = pos;
    lst -> prev[NewElemPos] = prev[pos];

    lst -> next[lst -> prev[pos]] = NewElemPos;
    lst -> prev[pos] = NewElemPos;

    lst -> sorted = false;

    lst -> curSize++;
  
    return true;

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
    
    int nextElem = lst -> next[lst -> head];
    lst -> prev[nextElem] = -1;
    lst -> next[lst -> head] = -1;
    lst -> head = nextElem;

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

bool Delete (int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

    if (pos == lst -> head)
        return DeleteFirst (lst);

    if (pos == lst -> tail)
        return DeleteLast (lst);

    lst -> next[prev[pos]] = next[pos];
    lst -> prev[next[pos]] = prev[pos];
    lst -> data[pos] = Poison;
    next[pos] = 0;
    prev[pos] = 0;

    if (!AddFreePos (pos, lst))
        return false;

    return true;

}

/*! This function deletes the element after the element on the pos position in the list
*
*   @param pos - position
*   @param lst - pointer to the list
*
*/


bool DeleteAfter (int pos, List* lst) {

    ASSERTLST (lst);

    return Delete (next[pos], lst);

}

/*! This function deletes the element before the element on the pos position in the list
*
*   @param pos - position
*   @param lst - pointer to the list
*
*/

bool DeleteBefore (int pos, List* lst) {

    ASSERTLST (lst);

    return Delete (prev[pos], lst);

}

#endif
