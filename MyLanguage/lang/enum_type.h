#ifndef __NODETYPE__

    #define  __NODETYPE__

const int MaxComLength = 8;
const int MaxIdName    = 128;

typedef int varNum;
typedef int Tone_t; 

enum NodeType {

    TNum  = 0,
    TVar  = 1,
    TFunc = 2,
    TOp   = 3,
    TNote = 4,
    TDec  = 5,
    TId   = 6,
    TCon  = 7

}; 

enum DecType {

    D_dec  = 0,
    D_nota = 1,
    D_leg  = 2,
    D_op   = 3

};

#endif
