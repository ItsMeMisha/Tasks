#ifndef STACK_T_H_INCLUDED
#define STACK_T_H_INCLUDED

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "Hash.h"

#define StackConstruct( stack ) StackConstructor (stack, #stack)

#ifndef _NODEBUG

#define ASSERTSTK( stack )                      \
                                                \
    if (!StackOk (stack)) {                     \
                                                \
        Dump (stack, "ASSERTAION FAILED!!!!!"); \
        assert(0);                              \
                                                \
    }

#else 

#define ASSERTSTK( stack )

#endif

#define Dump( name, comment )                                                   \
                                                                                \
    if (name)  StackDump (name, name -> Name, comment, __LINE__, __FUNCTION__); \
    else printf ("Stack is nullptr\n");


//#define _FullDefend 
//#define _CanaryDefend
//#define _HashDefend

#ifdef _FullDefend

    #define _CanaryDefend
    #define _HashDefend 

#endif

#ifdef _CanaryDefend
    
    const int canary = 2;
    #define CanaryCodeON( code ) code

#else 

    const int canary = 0;
    #define CanaryCodeON( code ) 

#endif

#ifdef _HashDefend

    #define HashCodeON( code ) code

#else

    #define HashCodeON( code )

#endif

enum ERRORS {
    OK = 0,
    LowSize,
    OverSize,
    NullDataPtr,
    WrongHashStruct,
    WrongHashData,
    WrongCanaryStructBegin,
    WrongCanaryStructEnd,
    WrongCanaryDataBegin,
    WrongCanaryDataEnd,
    EmptyPop
    };      

typedef int Element_t;

const int MinSize = 100;
const  Element_t Poison = {};
const int ResizeCoef = 2;
const int delta = 25;
CanaryCodeON (const unsigned CanaryDefault = 2863311;)
CanaryCodeON (const Element_t CanaryDataDefault = {};)

/*! @brief This structure is realization of stack of Element_t elements
*
*   @details MaxSize - current maximal size of stack \n
*   data - pointer to the begin of an array of elements \n
*   Size - current free element numbe r\n
*   HashStruct - Hash of struct without data's content \n
*   HashData - hash of data content \n
*   errcode - code of error \n
*   CanaryStructBegin - canary at the beginning of the struct \n
*   CanaryStructEnd - canary at the end of the struct \n
*   
*/

struct Stack_t {

    CanaryCodeON (unsigned CanaryStructBegin;)

    char Name[100];
    size_t MaxSize;

    CanaryCodeON (
    
        Element_t* CanaryDataBegin;
        Element_t* CanaryDataEnd;
    
    )

    Element_t* data;

    size_t Size;

    HashCodeON ( 

        unsigned* HashStruct;
        unsigned* HashData;

    )

    int errcode;

    CanaryCodeON (unsigned CanaryStructEnd;)

};

bool StackOk (Stack_t* stk);

bool ResizeStackUp (Stack_t* stk);

bool ResizeStackDown (Stack_t* stk);

void StackConstructor (Stack_t* stk, const char* name);

void StackDestruct (Stack_t* stk);

bool StackPush (Stack_t* stk, Element_t value);

void StackDump (Stack_t* stk, const char* name, const char* comment, int Line, const char* Function);

Element_t StackPop (Stack_t* stk);

void ErrCodeDecode (int errcode);

/*! This function checks if stack *stk is ok
*
*   @param[in] stk - address of stack that should be checked
*
*   @return true if stack is ok
*   @return false if stack is not ok
*/

bool StackOk (Stack_t* stk) {

    if (stk == nullptr) {

        return false;

    }

    if (stk -> Size < 0) {

        stk -> errcode = LowSize;
        return false;

    }

    if (stk -> Size > stk -> MaxSize) {

        stk -> errcode = OverSize;
        return false;

    }

    if (stk -> data == nullptr) {

        stk -> errcode = NullDataPtr;
        return false;

    }

    CanaryCodeON ( 

        if (stk -> CanaryStructBegin != CanaryDefault) {

            stk -> errcode = WrongCanaryStructBegin;
            return false;

        }

        if (stk -> CanaryStructEnd != CanaryDefault) {

            stk -> errcode = WrongCanaryStructEnd;
            return false;

        }

        if (*(stk -> CanaryDataBegin) != CanaryDataDefault) {

            stk -> errcode = WrongCanaryDataBegin;
            return false;

        }

        if (*(stk -> CanaryDataEnd) != CanaryDataDefault) {

            stk -> errcode = WrongCanaryDataEnd;
            return false;

        }   

    )

    HashCodeON (
    
        unsigned CtrlHash = MurmurHash2Light (stk, sizeof (Stack_t));

        if (*(stk -> HashStruct) != CtrlHash) { 
        
            stk -> errcode = WrongHashStruct;
            return false;

        }

        unsigned CtrlHashData = MurmurHash2Light (stk -> data, sizeof (Element_t) * (stk -> MaxSize));

        if (*(stk -> HashData) != CtrlHashData) {

            stk -> errcode = WrongHashData;
            return false;

        }

    )

    if (stk -> errcode > 0) {
        return false;
    }

    return true;

}

/*! This function expands size of stack data
*
*   @param stk - address of stack
*
*   @return true if expanding has been successful
*   @return false if expanding has failed
*
*/

bool ResizeStackUp (Stack_t* stk) {

    ASSERTSTK (stk);

    CanaryCodeON (--stk -> data;)

    Element_t* NewDataPtr = (Element_t*) realloc (stk -> data, sizeof (Element_t) * ((stk -> MaxSize) * ResizeCoef + canary));

    if (NewDataPtr == nullptr)
        return false;

    stk -> data = NewDataPtr;
    stk -> MaxSize *= ResizeCoef;

    memset (stk -> data + stk -> Size, Poison, sizeof (Element_t) * (stk -> MaxSize - stk -> Size));

    CanaryCodeON ( 

        if (stk -> data != nullptr);
            ++stk -> data;

        stk -> CanaryDataBegin = stk -> data - 1;
        stk -> CanaryDataEnd = stk -> data + (stk -> MaxSize);
    
        *(stk -> CanaryDataBegin) = CanaryDataDefault;
        *(stk -> CanaryDataEnd) = CanaryDataDefault;

    )
    
    return true;

}

/*! This function constricts size of stack data
*
*   @param stk - address of stack
*
*   @return true if constricting has been successful
*   @return false if constricting has failed
*
*/

bool ResizeStackDown (Stack_t* stk) {

    ASSERTSTK (stk);

    CanaryCodeON (--stk -> data;)

    if (stk -> MaxSize > MinSize) {

        Element_t* NewDataPtr = (Element_t*) realloc (stk -> data, sizeof (Element_t) * ((stk -> MaxSize / ResizeCoef) + canary));

        if (NewDataPtr == nullptr)
            return false;

        stk -> data = NewDataPtr;
        stk -> MaxSize /= ResizeCoef;

        CanaryCodeON ( 

            if (stk -> data != nullptr);
                ++stk -> data;
        
            stk -> CanaryDataBegin = stk -> data - 1;
            stk -> CanaryDataEnd = stk -> data + (stk -> MaxSize);
    
            *(stk -> CanaryDataBegin) = CanaryDataDefault;
            *(stk -> CanaryDataEnd) = CanaryDataDefault;

        )

    }

    return true;

}

/*! This function is constructor for Stack_t
*
*   @param stk - pointer to stack
*
*/

void StackConstructor (Stack_t* stk, const char* name) {

    strcpy (stk -> Name, name);
    stk -> MaxSize = MinSize;
    stk -> data = (Element_t*) calloc (stk -> MaxSize + canary, sizeof (Element_t));

    CanaryCodeON ( 
    
        if (stk -> data != nullptr);
            ++stk -> data;

        stk -> CanaryDataBegin = stk -> data - 1;
        *(stk -> CanaryDataBegin) = CanaryDataDefault;

        stk -> CanaryDataEnd = stk -> data + (stk -> MaxSize);
        *(stk -> CanaryDataEnd) = CanaryDataDefault;

    )

    stk -> Size = 0;

    HashCodeON ( 

        stk -> HashStruct = (unsigned*) calloc (1, sizeof (unsigned));
        stk -> HashData = (unsigned*) calloc (1, sizeof (unsigned));

    )

    stk -> errcode = 0;

    CanaryCodeON ( 

        stk -> CanaryStructBegin = (unsigned) CanaryDefault;
        stk -> CanaryStructEnd = (unsigned) CanaryDefault;

    )

    HashCodeON ( 

        *(stk -> HashData) = MurmurHash2Light (stk -> data, sizeof (Element_t) * (stk -> MaxSize)); 
        *(stk -> HashStruct) = MurmurHash2Light (stk, sizeof (Stack_t));
        
    )

    ASSERTSTK (stk);

}

/*! This function is destructor for Stack_t
*
*   @param stk - address of stack
*
*/

void StackDestruct (Stack_t* stk) {

    ASSERTSTK (stk);

    stk -> MaxSize = 0;

    free (stk -> data - canary/2);
    stk -> data = nullptr;

    stk -> Size = 0;

    HashCodeON ( 
        
        *(stk -> HashStruct) = 0;
        *(stk -> HashData) = 0;

        free (stk -> HashStruct);
        free (stk -> HashData);

    )

    stk -> errcode = 0;

    CanaryCodeON ( 

        stk -> CanaryDataBegin = nullptr;
        stk -> CanaryDataEnd = nullptr;

        stk -> CanaryStructBegin = 0;
        stk -> CanaryStructEnd = 0;

    )

    stk = nullptr;

}

/*! This function Pushes an element to stack
*
*   @param stk - address of stack
*   @param value - value of element that should be pushed
*
*   @return true if pushing has been successful
*   @return false if pushing has failed
*/

bool StackPush (Stack_t* stk, Element_t value) {

    ASSERTSTK (stk);

    if (stk -> Size >= stk -> MaxSize)

        if (!ResizeStackUp (stk)) {

        return false;

    }

    stk -> data[stk -> Size++] = value;

    HashCodeON ( 

        *(stk -> HashStruct) = MurmurHash2Light (stk, sizeof (Stack_t));
        *(stk -> HashData) = MurmurHash2Light (stk -> data, sizeof (Element_t) * stk -> MaxSize);

    )

    return true;

}

/*! This function pops an element from stack
*
*   @param stk - address of stack
*
*   @return value if popping has been successful
*   @return Poison if popping has failed
*/

Element_t StackPop (Stack_t* stk) {

    ASSERTSTK (stk);

    if (stk -> Size <= 0) {

        stk -> errcode = EmptyPop;
        ASSERTSTK (stk);
        return Poison;

    }

    if ((stk -> Size < (stk -> MaxSize / ResizeCoef - delta)) && (stk -> MaxSize > MinSize))

        if (!ResizeStackDown (stk))  {

        return Poison;

    }

    stk -> Size--;

    Element_t value = stk -> data[stk -> Size];

    stk -> data[stk -> Size] = Poison;

    HashCodeON ( 

        *(stk -> HashStruct) = MurmurHash2Light (stk, sizeof (Stack_t));
        *(stk -> HashData) = MurmurHash2Light (stk -> data, sizeof (Element_t) * stk -> MaxSize);

    )

    return value;

}

/*! This function prints information about stack (and elements in stack as a numbers if their size is less or equal size of int)
*
*   @param stk - address of stack
*   @param name - name of stack
*   @param comment - comment
*   @param Line - line from where StackDump has been called
*   @param Function - name of function from where StackDump has been called
*
*/

void StackDump (Stack_t* stk, const char* name, const char* comment, int Line, const char* Function) {

    bool ok = StackOk (stk);

    printf ("\n\t DUMP \n%s from %s (%d) %s \n", comment, __FILE__, Line, Function);

    printf ("Struct_t %s [%p]", name, stk);

    if (stk == nullptr)
        printf ("ERROR %s has null-address", name);

    else {

        if (stk -> errcode == 1 || stk -> errcode == 2)
            printf (" [error]\n MaxSize = %lu \n Size = %lu\n", stk -> MaxSize, stk -> Size);

        else {

            if (ok)
                printf ("[ok] \n");

            else printf ("[error] \n");

            printf ("Size = %lu \n", stk -> Size);

            if (stk -> Size >= 0 && stk -> Size <= stk -> MaxSize)
                printf ("data[%lu] = [%p]\n", stk -> MaxSize, stk -> data);

            if (sizeof(Element_t) <= sizeof(int)) {

                for (int i = 0; i < stk -> Size; ++i) {

                    printf ("\t*[%d] = %d", i, stk -> data[i]);

                    if (stk -> data[i] == Poison)
                        printf (" [Poison]\n");

                    else printf ("\n");
                }

                for (int i = stk -> Size; (i < stk -> MaxSize) && (i < stk -> Size + 5); ++i) {

                    printf ("\t[%d] = %d", i, stk -> data[i]);

                    if (stk -> data[i] == Poison)
                        printf (" [Poison]\n");

                    else printf ("\n");
                }

            }

        }

        printf ("\n errcode = %d ", stk -> errcode);

        ErrCodeDecode (stk -> errcode);

        HashCodeON (printf ("\n HashStruct: %u\n HashData: %u\n", *(stk -> HashStruct), *(stk -> HashData));)

        CanaryCodeON (printf ("\n CanaryStructBegin: %u\n CanaryStructEnd: %u\n", stk -> CanaryStructBegin, stk -> CanaryStructEnd);)

        printf ("\n");

    }
}

/*! This function decodes the errcode of stack and prints the result
*
*   @param errcode - code of error
*
*/

void ErrCodeDecode (int errcode) {

    switch (errcode) {

    case OK: printf ("(ok)");
            break;

    case LowSize: printf ("(Size is less then 0)");
            break;

    case OverSize: printf ("(Size more than MaxSize)");
                break;

    case NullDataPtr: printf ("(data is nullptr)");
            break;

    case WrongHashStruct: printf ("(Wrong HashStruct)");
            break;
    
    case WrongHashData: printf ("(Wrong HashData)");
            break;

    case WrongCanaryStructBegin: printf ("(Wrong CanaryStructBegin)");
            break;
    
    case WrongCanaryStructEnd: printf ("(Wrong CanaryStructEnd)");
            break;
    
    case WrongCanaryDataBegin: printf ("(Wrong CanaryDataBegin)");
            break;

    case WrongCanaryDataEnd: printf ("(Wrong CanaryDataEnd)");
            break;

    case EmptyPop: printf ("(Pop from Empty Stack)");
            break;

    }

}


#endif // STACK_T_H_INCLUDED
