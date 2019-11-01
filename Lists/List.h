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


bool ListOk (List* lst);
void ListConstruct (List* lst);
void ListDestruct (List* lst);
bool PosOk (int pos, List* lst);

bool Insert (Element_t elem, int pos, List* lst);
bool InsertAfter (Element_t elem, int pos, List* lst);
bool InsertBefore (Element_t elem, int pos, List* lst);

bool Delete (int pos, List* lst);
bool DeleteAfter (int pos, List* lst);
bool DeleteBefore (int pos, List* lst);

void SortList (List* lst);
Element_t FindElementByPosition (int pos, List* lst);
int FindPosOfElement (Element_t elem, List* lst, int* compare (Element_t, Element_t) = nullptr); 
void ListDump (List* lst);

bool ListOk (List* lst) {}

void ListConstruct (List* lst) {

    assert (lst);
    lst -> data = (Element_t*) calloc (FirstMaxSize, sizeof (Element_t*));
    lst -> next = (int*) calloc (FirstMaxSize, sizeof (int));
    lst -> prev = (int*) calloc (FirstMaxSize, sizeof (int));
    lst -> freeHead = 1;
    lst -> freePos = (int*) calloc (FirstMaxSize, sizeof (int));
    
    for (int i = 1; i < FirstMaxSize; ++i)
        freePos[i] = i + 1;

    freePos[FirstMaxSize] = -1;
    lst -> head = 0;
    lst -> tail = 0;
    lst -> maxSize = FirstMaxSize;
    lst -> curSize = 0;
    lst -> errcode = Allright;

    return;

}

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

    if ((pos < 0) && (pos > lst -> maxSize)) {
        lst -> errcode = WrongPositionReference;
        return false;
    }

    return true;    

}

bool Insert (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;
    
}

bool InsertAfter (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

}

bool InsertBefore (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

}

#endif
