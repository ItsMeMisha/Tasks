#ifndef __LIST__
    
    #define __LIST__

#include <stdio.h>

#ifdef _DEBUG

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

    Allright               = 0,
    WrongPositionReference = 1,
    NoFreePosLeft          = 2,
    NullDataPtr            = 3,
    NullNextPtr            = 4,
    NullPrevPtr            = 5,
    NullFreePosPtr         = 6
    
};

struct ListElem {

    Element_t data;
    int next;
    int prev;
    int freePos;
 
}

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

    ListElem* nodes;
    int freeHead;
    int head;
    int tail;
    int maxSize;
    int curSize;
    bool sorted;
    Error errcode

};


bool ListOk                                 (List* lst);
void ListConstruct                          (List* lst);
void ListDestruct                           (List* lst);
bool PosOk                         (int pos, List* lst);
bool PosIsFilled                   (int pos, List* lst);

int  GetFreePos                             (List* lst);
bool DeleteOneFreePos                       (List* lst);
bool AddFreePos                    (int pos, List* lst);

bool InsertFirst   (Element_t elem,          List* lst);
bool InsertLast    (Element_t elem,          List* lst);
bool InsertAfter   (Element_t elem, int pos, List* lst);
bool InsertBefore  (Element_t elem, int pos, List* lst);

bool DeleteFirst                            (List* lst);
bool DeleteLast                             (List* lst);
bool Delete                        (int pos, List* lst);
bool DeleteAfter                   (int pos, List* lst);
bool DeleteBefore                  (int pos, List* lst);

void SortList                               (List* lst);
void ListDump                               (List* lst);
void DrawList      (FILE* file,              List* lst);

Element_t FindElementByPosition    (int pos, List* lst);
int FindPosOfElement (Element_t elem, List* lst, int* compare (Element_t, Element_t) = nullptr); 

bool ListOk (List* lst) {

    if (lst == nullptr) {
        
        printf ("Error! lst is nullptr\n");
        return false;

    }

    if (lst -> errcode != Allright)
        return false;

    if (lst -> data == nullptr) {

        lst -> errcode = NullDataPtr;
        return false;

    }

    if (lst -> next == nullptr) {

        lst -> errcode = NullNextPtr;
        return false;

    {

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
    lst -> nodes = (ListElem*) calloc (FirstMaxSize, sizeof (ListElem));
    lst -> freeHead = 1;
    
    for (int i = 1; i < FirstMaxSize - 1; ++i)
        lst -> nodes[i].freePos= i + 1;

    lst -> nodes[FirstMaxSize - 1].freePos = 0;
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

    free (lst -> nodes);

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

    return true;    

}

bool PosIsFilled (int pos, List* lst) {

    ASSERTLST (lst);

    if (lst -> nodes[pos].freePos >= 0)
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

    int nextFree = lst -> nodes[lst -> freeHead].freePos;

    lst -> nodes[lst -> freeHead].freePos = -1;
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
    
    lst -> nodes[pos].freePos = lst -> freeHead;
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
 
    lst -> nodes[lst -> head].prev = NewElemPos;
    lst -> nodes[NewElemPos].next = lst -> head;
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

    lst -> nodes[NewElemPos].data = elem;

    lst -> nodes[lst -> tail].next = NewElemPos;
    lst -> nodes[NewElemPos].prev = lst -> tail;
    lst -> tail = NewElemPos;

    if (NewElemPos != lst -> curSize)
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

    if (!PosIsFilled (pos, lst))
        return false;

    int NewElemPos = GetFreePos (lst);
    
    if (NewElemPos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;

    lst -> nodes[NewElemPos].data = elem;

    lst -> nodes[NewElemPos].next = lst -> nodes[pos].next;
    lst -> nodes[NewElemPos].prev = pos;
 
    lst -> nodes[lst -> nodes[pos].next].prev = NewElemPos;
    lst -> nodes[pos].next = NewElemPos;

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

    if (!PosIsFilled (pos, lst))
        return false;

    int NewElemPos = GetFreePos (lst);

    if (NewElemPos == 0)
        return false;

    if (!DeleteOneFreePos (lst))
        return false;

    lst -> data[NewElemPos] = elem;

    lst -> nodes[NewElemPos].next = pos;
    lst -> nodes[NewElemPos].prev = lst -> nodes[pos].prev;

    lst -> nodes[lst -> nodes[pos].prev].next = NewElemPos;
    lst -> nodes[pos].prev = NewElemPos;

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

    lst -> nodes[lst -> head].data = Poison;
    
    int nextElem = lst -> nodes[lst -> head].next;
    lst -> nodes[nextElem].prev = -1;
    lst -> nodes[lst -> head].next = -1;
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

    lst -> nodes[lst -> tail].data = Poison;
    
    int prevElem = lst -> nodes[lst -> tail].prev;
    lst -> nodes[prevElem].next = -1;
    lst -> nodes[lst -> tail].prev = -1;
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

    if (!PosIsFilled (pos, lst))
        return false;

    if (pos == lst -> head)
        return DeleteFirst (lst);

    if (pos == lst -> tail)
        return DeleteLast (lst);

    lst -> nodes[lst -> nodes[pos].prev].next = lst -> nodes[pos].next;
    lst -> nodes[lst -> nodes[pos].next].prev = lst -> nodes[pos].prev;
    lst -> nodes[pos].data = Poison;
    lst -> nodes[pos].next = 0;
    lst -> nodes[pos].prev = 0;

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

    if (!PosOk (pos, lst))
        return false;

    if (!PosIsFilled (pos, lst))
        return false;

    return Delete (lst -> nodes[pos].next, lst);

}

/*! This function deletes the element before the element on the pos position in the list
*
*   @param pos - position
*   @param lst - pointer to the list
*
*/

bool DeleteBefore (int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

    if (!PosIsFilled (pos, lst))
        return false;

    return Delete (lst -> nodes[pos].prev, lst);

}

void SortList (List* lst) {

    ASSERTLST (lst);

    if (lst -> curSize = 0) {

        lst -> sorted = true;
        return;

    }

    int* PosBuf = (int*) calloc (lst -> curSize, sizeof (int));    
    PosBuf[1] = lst -> head;

    int curNext = lst -> nodes[lst -> head].next;
//TODO Need to be carefull with tail elem
    for (int i = 2; i <= lst -> curSize; ++i) {

        PosBuf[i] = curNext;
        curNext = lst -> nodes[curNext].next;

    }

    PosBuf[lst -> curSize] = lst -> tail;

    ListElem elemBuf = {};

    for (int i = 1; i <= lst -> curSize; ++i) {

        elemBuf = lst -> nodes[i];
        lst -> nodes[i] = lst -> nodes[PosBuf[i]];
        lst -> nodes[PosBuf[i]] = elemBuf;

    }

    lst -> head = 1;
    lst -> tail = lst -> curSize;

    lst -> sorted = true;

    free (PosBuf);

    return;

}

void ListDump (List* lst);
void DrawList (FILE* file, List* lst) {

    fprintf (file, "digraph\n{\n"); 

    for (int i = 1; i <= lst -> maxSize; ++i) 
        fprintf (file, "\t%p[label = %d]\n", lst -> nodes + i, lst -> nodes[i].data);

    int cur = lst -> head; 

    for (int i = 1; i < lst -> curSize; ++i) {

        fprintf (file, "%p -> ", lst -> nodes + cur);
        cur = lst -> nodes[cur].next;

    }

    fprintf (file, "%p\n", lst -> nodes + cur);

    int cur = lst -> freeHead; 

    for (int i = 1; i < lst -> maxSize - lst -> curSize; ++i) {

        fprintf (file, "%p -> ", lst -> nodes + cur);
        cur = lst -> nodes[cur].freePos;

    }

    fprintf (file, "%p\n", lst -> nodes + cur);

  
    fprintf (file, "}\n");

    return; 

}

Element_t FindElementByPosition (int pos, List* lst);
int FindPosOfElement (Element_t elem, List* lst, int* compare (Element_t, Element_t) = nullptr); 


#endif
