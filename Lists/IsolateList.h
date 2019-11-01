#ifndef __ISOLATELIST__
    
    #define __ISOLATELIST__

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

enum Error {
    
    Allright = 0,
    WrongPositionReference = 1

};

struct ListElement {

    ListElement* prev;
    Element_t data;
    ListElement* next;

};

struct List {

    ListElement* Head;
    int Size;
    ListElement* Tail;
    Error errcode

};


bool ListOk (List* lst);
void ListConstruct (List* lst);
void ListDestruct (List* lst);
bool PosOk (int pos, List* lst);
bool AddConnection (ListElement* Left, ListElement* Mid, ListElement* Right);

void InsertFirst (Element_t* NewElem, List* lst);
void InsertLast (Element_t* NewElem, List* lst);
bool InsertAfter (Element_t elem, int pos, List* lst);
bool InsertBefore (Element_t elem, int pos, List* lst);

void DeleteFirst (List* lst);
void DeleteLast (List* lst);
bool Delete (int pos, List* lst);
bool DeleteAfter (int pos, List* lst);
bool DeleteBefore (int pos, List* lst);

int IntCompare (const int NumBigger, const int NumLess);
Element_t FindElementByPosition (int pos, List* lst);
int FindPosOfElement (Element_t elem, List* lst, int* compare (const Element_t, const Element_t) = IntCompare); 
void ListDump (List* lst);

bool ListOk (List* lst) {



}

void ListConstruct (List* lst) {

    assert (lst);
    lst -> Head = nullptr;
    lst -> Tail = nullptr;
    lst -> Size = 0;
    lst -> errcode = Allright;

    return;

}

void ListDestruct (List* lst) {

    ASSERTLST (lst);

    while (lst -> Size)
        Delete (lst -> Size, lst);

    lst -> Head = nullptr;
    lst -> Tail = nullptr;
    lst -> errcode = Allright;

    return;

}

bool PosOk (int pos, List* lst) {

    if ((pos < 0) && (pos > lst -> Size)) {
        lst -> errcode = WorngPositionReference;
        return false;
    }

    return true;    

}

void AddConnection (ListElement* Left, ListElement* Mid, ListElement* Right) {

    assert (Left);
    assert (Mid);
    assert (Right);

    Left -> next = Mid;
    Mid -> prev = Left;
    Mid -> next = Right;
    Right -> prev = Mid;

    return;

}

void InsertFirst (Element_t* NewElem, List* lst) {

    ASSERTLST (lst);
    
    ListElement* FirstElem = lst -> head;
    NewElem -> next = FirstElem;
    NewElem -> prev = nullptr;
    FirstElem -> prev = NewElem;
    lst -> head = NewElem;

    return;
   
}

void InsertLast (Element_t* NewElem, List* lst) {

    ASSERTLST (lst);
        
    ListElement* LastElem = lst -> Tail;
    NewElem -> prev = LastElem;
    NewElem -> next = nullptr; 
    LastElem -> next = NewElem;
    lst -> Tail = NewElem;

    return;

}

bool InsertAfter (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos ,lst))
        return false;

    ListElement* NewElem = (ListElement*) calloc (1, sizeof (ListElement));
    NewElement -> data = elem;
    
    ListElement* curElem = nullptr;
    ListElement* nextElem = lst -> Head;

    if (pos == lst -> Size)
        InsertLast (NewElem, lst);

    else if (pos == 0) 
        InsertFirst (NewElem, lst);

    else {    

        for (int i = 0; i < pos; ++i) {

            curElem = nextElem;
            nextElem = lst -> next;

        }

        AddConnection (curElem, NewElem, nextElem);

    }
    
    lst -> Size++;

    return true;

}

bool InsertBefore (Element_t elem, int pos, List* lst) {

    ASSERTLST (lst);

    bool InsertOk = true;

    if (pos == 0)
        InsertOk = InsertAfter (elem, pos, lst)
    else 
        InsertOk = InsertAfter (elem, pos - 1, lst);

    return InsertOk;

}

void DeleteFirst (List* lst) {

    ASSERTLST (lst);

    ListElement* FirstElem = lst -> head;
    lst -> head = FirstElem -> next;
    free (FirstElem);

    return;

}

void DeleteLast (List* lst) {

    ASSERTLST (lst);

    ListElement* LastElem = lst -> head;
    lst -> tail = LastElem -> prev;
    free (LastElem);

}

bool Delete (int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst))
        return false;

    if (pos == 0)
        return false;

    if (pos == 1)
        DeleteFirst (lst);

    else if (pos == lst -> Size)
        DeleteLast (lst);

    else {
 
         for (int i = 0; i < pos; ++i) {

            curElem = nextElem;
            nextElem = lst -> next;

        }

        AddConnection (curElem ->  prev, nextElem, nextElem -> next);
        free (curElem);
     
   }

}

bool DeleteAfter (int pos, List* lst) {

    ASSERTLST (lst);

    if (pos >= lst -> Size)
        return false;

    if (!Delete (pos + 1, lst))
        return false;
    
    return true;

}

bool DeleteBefore (int pos, List* lst) {

    ASSERTLST (lst);

    if (pos < 2)
        return false;

    if (!Delete (pos - 1, lst))
        return false;

    return true;

}

Element_t FindElementByPosition (int pos, List* lst) {

    ASSERTLST (lst);

    if (!PosOk (pos, lst)) 
        return Poison;
    
    ListElement* CurElem = lst -> head;

    for (int i = 1; i < pos; ++i)
        CurElem = CurElem -> next;

    return CurElem -> data;

}

int IntCompare (const int NumBigger, const int NumLess) {

    if (NumBigger == NumLess)
        return 0;

    if (NumBigger < NumLess)
        return -10;

    return 10;

}

int FindPosOfElement (Element_t elem, List* lst, int* compare (Element_t, Element_t) = IntCompare) {

    ASSERTLST (lst);

    ListElement* CurElem = lst -> head;

    int pos = 1;

    while ((compare (CurElem -> data, elem) != 0) && (CurElem != lst -> tail)) {

        CurElem = CurElem -> next;
        pos++;

    }

    if ((compare (CurElem -> data, elem) != 0) && (CurElem == lst -> tail))
        return -1;

    else return pos;

} 

#endif
