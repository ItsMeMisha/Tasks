#ifndef __NODETYPE__

    #define  __NODETYPE__

const int MaxComLength = 8;
const int MaxIdName    = 128;

typedef short varNum;

enum NodeType {

    TNum  = 0,
    TVar  = 1,
    TFunc = 2,
    TOp   = 3

}; 


#endif
