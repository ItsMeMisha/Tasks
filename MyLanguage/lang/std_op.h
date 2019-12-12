const char* op[] = {

    "+", //1
    "-", //2
    "*", //3
    "/", //4
    "%", //5
    "sin", //6
    "cos", //7
    "meno", //8
    "piu", //9
    "!=", //10
    "==", //11
    "|", //12
    "||", //13
    "=", //14
    "||:", //15
    ":||", //16
    "volta", //17
    "secvolta", //18
    "]", //19
    "[" //20

};

const int NumOfOp = 11;

enum OpType {

    OP_add  = 1,
    OP_sub  = 2,
    OP_mul  = 3,
    OP_div  = 4,
    OP_mod  = 5,
    OP_sin  = 6,
    OP_cos  = 7
    OP_meno = 8,
    OP_piu  = 9,
    OP_neq  = 10,
    OP_eq   = 11,
    OP_bar  = 12,
    OP_dbar = 13,
    OP_ass  = 14,
    OP_whl  = 15,
    OP_clwhl= 16,
    OP_vol  = 17,
    OP_svol = 18,
    OP_clbr = 19,
    OP_opbr = 20

};
