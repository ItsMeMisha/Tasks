#ifndef __LIST__
    
    #define __LIST__

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
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
    NullNodePtr            = 3,
    NullFreePosPtr         = 4,
    RefPosNotFilled        = 5
    
};

struct List_node {

    Element_t data;
    int next;
    int prev;

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

    List_node* node;
    List_node* freePos;
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

int PrevCompare (const void* node1, const void* node2);
//int FindLogPosOfElement   (const Element_t elem, List* lst, int compare (const void*, const void*) = IntCompare); 

bool ListOk (List* lst) {

    if (lst == nullptr) 
        return false;

    if (lst -> errcode != Allright)
        return false;

    if (lst -> node == nullptr) {

        lst -> errcode = NullNodePtr;
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
    lst -> node = (List_node*) calloc (FirstMaxSize, sizeof (List_node));
    lst -> freePos = lst -> node;
    lst -> freeHead = 1;
 
    for (int i = 1; i < FirstMaxSize - 1; ++i) {

        lst -> freePos[i].next = i + 1;
        lst -> freePos[i].prev = -1;

    }

    lst -> freePos[FirstMaxSize - 1].next = 0;
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

    free (lst -> node);

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

    if (lst -> node[pos].prev <= PosIsFree) {

        lst -> errcode = RefPosNotFilled;
        return false;

    }

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

    int nextFree = lst -> freePos[lst -> freeHead].next;

    lst -> freePos[lst -> freeHead].next = -1;
    lst -> node[lst -> freeHead].prev = 0;
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
 
    lst -> freePos[pos].next = lst -> freeHead;
    lst -> node[pos].prev = PosIsFree;
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

    lst -> node[NewElemPos].data = elem;
 
    lst -> node[lst -> head].prev = NewElemPos;
    lst -> node[NewElemPos].next = lst -> head;
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
 
    lst -> node[NewElemPos].data = elem;

    lst -> node[lst -> tail].next = NewElemPos;
    lst -> node[NewElemPos].prev = lst -> tail;
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
   
    lst -> node[NewElemPos].data = elem;

    lst -> node[NewElemPos].next = lst -> node[pos].next;
    lst -> node[NewElemPos].prev = pos;
 
    lst -> node[lst -> node[pos].next].prev = NewElemPos;
    lst -> node[pos].next = NewElemPos;

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

    return InsertAfter (elem, lst -> node[pos].prev, lst);

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

    lst -> node[lst -> head].data = Poison;
    lst -> curSize--;
    int head = lst -> head;
    
    if (!CheckDeleteLast (lst)) {
        
        int nextElem = lst -> node[lst -> head].next;
        lst -> node[nextElem].prev = -1;
        lst -> node[head].next = -1;
        lst -> head = nextElem;

    }

    if (!AddFreePos (head, lst))
        return false;
 
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

    lst -> node[lst -> tail].data = Poison;
    lst -> curSize--;
    int tail = lst -> tail;

    if (!CheckDeleteLast (lst)) {
    
        int prevElem = lst -> node[lst -> tail].prev;

        lst -> node[prevElem].next = -1;
        lst -> node[tail].prev = -1;
        lst -> tail = prevElem;

    }

    if (!AddFreePos (tail, lst))
        return false;

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

    lst -> node[lst -> node[pos].prev].next = lst -> node[pos].next;
    lst -> node[lst -> node[pos].next].prev = lst -> node[pos].prev;
    lst -> node[pos].data = Poison;
    lst -> node[pos].next = 0;
    lst -> node[pos].prev = PosIsFree;

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

    return Delete (lst -> node[pos].next, lst);

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

    return Delete (lst -> node[pos].prev, lst);

}

void SortList (List* lst) {

    ASSERTLST (lst);

    if (lst -> curSize <= 1) {

        lst -> sorted = true;
        return;

    }

    for (int i = 0; i < lst -> maxSize; ++i)
        lst -> node[i].prev = lst -> maxSize + 1;

    lst -> node[lst -> head].prev = 1;

    int curNext = lst -> node[lst -> head].next;

    for (int i = 2; i <= lst -> curSize; ++i) {

        lst -> node[curNext].prev = i;
        curNext = lst -> node[curNext].next;

    }

    qsort (lst -> node + 1, lst -> maxSize - 1, sizeof (List_node), PrevCompare); 

    for (int i = 1; i < lst -> maxSize; ++i) {

        lst -> node[i].next = i + 1;
        lst -> node[i].prev = i - 1;

    }

    lst -> node[lst -> curSize].next = -1;

    lst -> head = 1;
    lst -> tail = lst -> curSize;
    lst -> freeHead = lst -> tail + 1;
    lst -> freePos[lst -> maxSize - 1].next = -1;

    for (int i = lst -> curSize + 1; i < lst -> maxSize; ++i)
        lst -> freePos[i].prev = -1;

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

        fprintf (file, "\t%x[color =", lst -> node + i);
        if (lst -> node[i].prev == PosIsFree)
            fprintf (file, "green, ");
        else
            fprintf (file, "red, ");

        fprintf (file, "shape = record, label = \"{%d | {%d | %d} | %d }\"]\n", lst -> node[i].prev, i, lst -> node[i].data, lst -> node[i].next);

    }

    for (int i = 1; i < lst -> maxSize; ++i) 
        fprintf (file, "%x -> ", lst -> node + i);

    fprintf (file, "%x\n", lst -> node + lst -> maxSize);

    fprintf (file, "edge[color=black];");

    int cur = lst -> head;

    for (int i = 1; i < lst -> curSize; ++i) {

        fprintf (file, "%x -> ", lst -> node + cur);
        cur = lst -> node[cur].next;

    }

    fprintf (file, "%x\n", lst -> node + lst -> tail);
 
    cur = lst -> freeHead; 

    for (int i = 1; i < lst -> maxSize - lst -> curSize; ++i) {

        fprintf (file, "%x -> ", lst -> node + cur);
        cur = lst -> freePos[cur].next;

    }

    fprintf (file, "%x\n", lst -> node + cur);

  
    fprintf (file, "}\n");

    return; 

}

void ErrDecode (List* lst) {

    switch (lst -> errcode) {    

    case Allright: printf ("Allright \n"); break;
    case WrongPositionReference: printf ("Wrong Position Reference \n"); break;
    case NoFreePosLeft: printf ("No free positions left \n"); break;
    case NullNodePtr: printf ("lst -> node is nullptr \n"); break;
    case NullFreePosPtr: printf ("lst -> freePos is nullptr \n"); break;
    case RefPosNotFilled: printf ("Referenced position is not filled \n"); break;

    default: printf ("Unexpected error \n"); break;

    }

}

int PrevCompare (const void* node1, const void* node2) {

    assert (node1);
    assert (node2);

    return (((List_node*) node1) -> prev - ((List_node*) node2) -> prev);

}

/*int FindLogPosOfElement (Element_t elem, List* lst, int compare (const void*, const void*)) {

    ASSERTLST (lst);

    for (int i = 1; i > 0; i = lst -> next[i])
        if (compare (&elem, lst -> node + i) == 0)
            return i;

    return 0;    

}*/

#endif
