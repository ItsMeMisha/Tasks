#ifndef __LIST__
    
    #define __LIST__

    #define ListConstruct( lst ) ListConstructor (lst, #lst)

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

const int StrSize = 256;
const Element_t Poison = {};
const int FirstMaxSize = 101;
const int PosIsFree = -1;

enum Error {

    Allright               = 0,
    WrongPositionReference = 1,
    NoFreePosLeft          = 2,
    NullNodePtr            = 3,
    NullFreePosPtr         = 4,
    RefPosNotFilled        = 5,
    LostConnection         = 6
    
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
    
    char name[StrSize];
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
bool ConnectionOk                           (List* lst);

void ListConstructor                        (List* lst);
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
bool ListInfoDraw  (FILE* file,              List* lst, const char* funcName);
void DrawNext      (FILE* file,              List* lst);
void DrawPrev      (FILE* file,              List* lst);
void DrawList      (FILE* file,              List* lst, const char* funcName);
void ErrDecode     (FILE* file,              List* lst);

int PrevCompare (const void* node1, const void* node2);
int FindPhysPosByLogPos   (const int logPos, List* lst); 

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

    if (!ConnectionOk (lst)) {
        
        lst -> errcode = LostConnection;
        return false;

    }

    return true;

}

bool ConnectionOk (List* lst) {

    if (lst -> curSize > 1) {

        for (int cur = lst -> head; cur != lst -> node[lst -> tail].prev; cur = lst -> node[cur].next)
            if (cur != lst -> node[lst -> node[cur].next].prev)
                return false;

         for (int cur = lst -> tail; cur != lst -> node[lst -> head].next; cur = lst -> node[cur].prev)
            if (cur != lst -> node[lst -> node[cur].prev].next)
                return false;   

    }

    return true;

}

/*! This is a constructor for List-type lists
*
*   @param lst - pointer to the list
*
*/

void ListConstructor (List* lst, char Name[StrSize]) {

    assert (lst);
    
    strncpy (lst -> name, Name, StrSize);
    lst -> node = (List_node*) calloc (FirstMaxSize, sizeof (List_node));
    lst -> freePos = lst -> node;
    lst -> freeHead = 1;
 
    for (int i = 1; i < FirstMaxSize - 1; ++i) {

        lst -> freePos[i].next = i + 1;
        lst -> freePos[i].prev = -1;

    }

    lst -> freePos[FirstMaxSize - 1].next = 0;
    lst -> freePos[FirstMaxSize - 1].prev = -1;
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

    if (lst -> node[pos].prev == PosIsFree) {

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

    if (lst -> freeHead <= 0) {

        lst -> errcode = NoFreePosLeft; 
        return false;

    }

    int nextFree = lst -> freePos[lst -> freeHead].next;

    lst -> freePos[lst -> freeHead].next = -1;
    lst -> node[lst -> freeHead].prev = -1;
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
    lst -> node[NewElemPos].prev = 0;
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

    lst -> sorted = false;

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
        assert (fileout);
   
    DrawList (fileout, lst, funcName);
    fclose (fileout);

    return;

}

bool ListInfoDraw  (FILE* file, List* lst, const char* funcName) {

    fprintf (file, "\t function [label = \"In function %s\"]\n", funcName);

    if (lst == nullptr) {

        fprintf (file, "\t LstNull [label=\"List is nullptr\"]\n");
        fprintf (file, "function -> LstNull\n");
        return false;

    }

    if (lst -> name == nullptr) {
        
        fprintf (file, "\t NoName [label=\"List has no name\"]\n");
        fprintf (file, "function -> NoName\n");
        return false;

    }
    
    if (*(lst -> name) == '&')    
        fprintf (file, "\t List [label=\"{List %s | %p}\"]\n", lst -> name + 1, lst);
    else
        fprintf (file, "\t List [label=\"{List %s | %p}\"]\n", lst -> name, lst);  

    if (lst -> errcode == Allright) 
        fprintf (file, "\t error [label =\"OK\"]\n");

    else 
        fprintf (file, "\t error [label=\"ERROR\"]\n");

    ErrDecode (file, lst);         

    fprintf (file, "function -> List -> error -> code\n");

    return true;
    
}

void DrawNext (FILE* file, List* lst) {

    fprintf (file, "edge[color=red] \n");

    int cur = lst -> head;

    for (int i = 1; i < lst -> curSize; ++i) {

        fprintf (file, "%x:<n> -> ", lst -> node + cur);
        cur = lst -> node[cur].next;

    }

    fprintf (file, "%x:<n>\n", lst -> node + lst -> tail);

    fprintf (file, "edge[color=green] \n");
 
    cur = lst -> freeHead; 

    for (int i = 1; i < lst -> maxSize - lst -> curSize - 1; ++i) {

        fprintf (file, "%x:<n> -> ", lst -> node + cur);
        cur = lst -> freePos[cur].next;

    }

    fprintf (file, "%x:<n>\n", lst -> node + cur);

    return;
 
}

void DrawPrev (FILE* file, List* lst) {

    fprintf (file, "edge[color=blue] \n");

    int cur = lst -> tail;

    for (int i = 1; i < lst -> curSize; ++i) {

        fprintf (file, "%x:<p> -> ", lst -> node + cur);
        cur = lst -> node[cur].prev;

    }

    fprintf (file, "%x:<p>\n", lst -> node + lst -> head);

    return;

}

void DrawList (FILE* file, List* lst, const char* funcName) {

    fprintf (file, "digraph\n{\n"); 
    fprintf (file, "rankdir=LR;\n");
    fprintf (file, "edge[color=white];\n");
    fprintf (file, "node[shape=record];\n");

    if (!ListInfoDraw (file, lst, funcName)) {

        fprintf (file, "}");
        return;

    }

    for (int i = 1; i < lst -> maxSize; ++i) {

        fprintf (file, "\t%x[style=filled, color =", lst -> node + i);

        if (lst -> head == i)
            fprintf (file, "red, fillcolor=orange, ");

        else if (lst -> tail == i)
            fprintf (file, "red, fillcolor=orange, ");

        else if (lst -> node[i].prev == PosIsFree)
            fprintf (file, "black, fillcolor=green, ");
        else
            fprintf (file, "black, fillcolor=red, ");

        fprintf (file, "label = \"{{<p> prev\n %d | {index\n %d | data\n %d} |<n> next\n %d }}\"]\n", lst -> node[i].prev, i, lst -> node[i].data, lst -> node[i].next);

    }

    for (int i = 1; i < lst -> maxSize - 1; ++i) 
        fprintf (file, "%x -> ", lst -> node + i);

    fprintf (file, "%x\n", lst -> node + lst -> maxSize - 1);

    DrawNext (file, lst);
    DrawPrev (file, lst);
 
    fprintf (file, "}\n");

    return; 

}

void ErrDecode (FILE* file, List* lst) {

    fprintf (file, "code [label=\"");

    switch (lst -> errcode) {    

    case Allright: fprintf (file, "Allright");                                 break;
    case WrongPositionReference: fprintf (file, "Wrong Position Reference");   break;
    case NoFreePosLeft: fprintf (file, "No free positions left");              break;
    case NullNodePtr: fprintf (file, "lst -> node is nullptr");                break;
    case NullFreePosPtr: fprintf (file, "lst -> freePos is nullptr");          break;
    case RefPosNotFilled: fprintf (file, "Referenced position is not filled"); break;
    case LostConnection: fprintf (file, "Lost Connection");                    break;

    default: printf ("Unexpected error"); break;

    }

    fprintf (file, "\"]\n");

    return;

}

int PrevCompare (const void* node1, const void* node2) {

    assert (node1);
    assert (node2);

    return (((List_node*) node1) -> prev - ((List_node*) node2) -> prev);

}

int FindPhysPosByLogPos (const int logPos, List* lst) {

    ASSERTLST (lst);

    if (logPos > lst -> curSize)
        return 0;


    if (lst -> sorted)
        return logPos;

    int cur = lst -> head;

    for (int i = 1; i <= lst -> curSize; ++i)
        cur = lst -> node[cur].next;

    return cur;    

}

#endif
