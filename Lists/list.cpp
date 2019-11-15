#include "List.h"

int main () {

    List testList = {};

    ListConstruct (&testList);
    
    InsertFirst (100, &testList);
    InsertAfter (200, 1, &testList);
    InsertBefore (300, 1, &testList);
    InsertLast (400, &testList);

    Delete (1, &testList);

    ListDump (&testList, "main");

    ListDestruct (&testList);

    return 0;

}
