#ifndef __FUNCS_AND_VARS__

    #define __FUNCS_AND_VARS__

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

const float ReCoef = 1.5;

const int FirstMaxSize = 256;
struct var_t {

    char* name;
    int pos;

};

struct Vars {

    var_t** var;
    int size;
    int MaxSize;

};

struct AvVars {

    Vars* gVars;
    int shift;
    Vars* fVars;

};

struct function {

    char* name;
    int argsNum;

};

struct AllFuncs {

    function** func;
    int size;
    int MaxSize;

};

function* NewFunc (const char* Name, const int ArgsNum);
AllFuncs* ConstructFuncs (AllFuncs* funcs);
bool AddFunc (AllFuncs* funcs, const char* Name, const int ArgsNum);
bool ExpendFuncs (AllFuncs* funcs);
int FindFunc (AllFuncs* funcs, const char* Name);
void DestructFuncs (AllFuncs* funcs);

var_t* NewVar (const char* Name, const int Pos);
AvVars* ConstructAvVars (AvVars* vars, const int Shift, Vars* glob = nullptr);
Vars* ConstructVars (Vars* vars);
bool AddVar (Vars* vars, const char* Name, const int Pos);
bool ExpendVars (Vars* vars);
int FindVar (Vars* vars, const char* Name);
void DestructVars (Vars* vars);
void DestructAvVars (AvVars* vars);


///////////////////////////////////////////////////////

function* NewFunc (const char* Name, const int ArgsNum) {

    function* newfunc = (function*) calloc (sizeof (function), 1);
    
    sscanf (Name, "%ms", &(newfunc -> name));

    if (newfunc -> name == nullptr)
        return nullptr;

    newfunc -> argsNum = ArgsNum;

    return newfunc;

}

AllFuncs* ConstructFuncs (AllFuncs* funcs) {

    if (funcs == nullptr)
        funcs = (AllFuncs*) calloc (sizeof (AllFuncs), 1);

    funcs -> size = 0;
    funcs -> MaxSize = FirstMaxSize;

    funcs -> func = (function**) calloc (sizeof (function*), FirstMaxSize);

    if (funcs -> func == nullptr)
        return nullptr;

    return funcs;

} 

bool AddFunc (AllFuncs* funcs, const char* Name, const int ArgsNum) {

    assert (funcs);
    assert (Name);

    if (funcs -> size >= funcs -> MaxSize)
        ExpendFuncs (funcs);

    funcs -> func[funcs -> size] = NewFunc (Name, ArgsNum);

    if (funcs -> func[funcs -> size] == nullptr)
        return false;

    funcs -> size++; 

    return true;

}

bool ExpendFuncs (AllFuncs* funcs) {

    assert (funcs);

    funcs -> MaxSize = (int) ReCoef * funcs -> MaxSize;

    funcs -> func = (function**) realloc (funcs -> func, sizeof (function*) * funcs -> MaxSize);

    if (funcs -> func == nullptr)
        return false;

    return true;

}

int FindFunc (AllFuncs* funcs, const char* Name) {

    assert (funcs);

    for (int i = 0; i < funcs -> size; i++) 
        if (strncmp (Name, funcs -> func[i] -> name, strlen (Name)) == 0)
            return i;

    return -1;

}

void DestructFuncs (AllFuncs* funcs) {

    assert (funcs);

    for (int i = 0; i < funcs -> size; i++)
        free (funcs -> func[i]);
    
    free (funcs -> func);
    funcs -> size = 0; 

    return;

}

////////////////////////////////////////////////////

var_t* NewVar (const char* Name, const int Pos) {

    assert (Name);

    var_t* newvar = (var_t*) calloc (sizeof (var_t), 1);

    sscanf (Name, "%ms", &(newvar -> name));

    if (newvar -> name == nullptr)
        return nullptr;

    newvar -> pos = Pos;

    return newvar;

}

AvVars* ConstructAvVars (AvVars* vars, const int Shift, Vars* glob) {

    if (vars == nullptr)
        vars = (AvVars*) calloc (sizeof (AvVars), 1);

    if (glob == nullptr)
        glob = ConstructVars (glob);

    vars -> gVars = glob;

    vars -> shift = Shift;

    if (vars -> fVars != nullptr)
        free (vars -> fVars);

    vars -> fVars = ConstructVars (nullptr);

    return vars;

}

Vars* ConstructVars (Vars* vars) {

    if (vars == nullptr)
        vars  = (Vars*) calloc (sizeof (Vars), 1);

    vars -> size = 0;
    vars -> MaxSize = FirstMaxSize;

    vars -> var = (var_t**) calloc (sizeof (var_t*), 1);

    *(vars -> var) = (var_t*) calloc (sizeof (var_t), FirstMaxSize);

    return vars;

}

bool AddVar (Vars* vars, const char* Name, const int Pos) {

    assert (vars);
    assert (Name);

    if (vars -> size >= vars -> MaxSize)
        ExpendVars (vars);

    vars -> var[vars -> size] = NewVar (Name, Pos);

    if (vars -> var[vars -> size] == nullptr)
        return false;

    vars -> size++; 

    return true;

}

bool ExpendVars (Vars* vars) {

    assert (vars);

    vars -> MaxSize = (int) ReCoef * vars -> MaxSize;

    vars -> var = (var_t**) realloc (vars -> var, sizeof (var_t*) * vars -> MaxSize);

    if (vars -> var == nullptr)
        return false;

    return true;

}

int FindVar (Vars* vars, const char* Name) {

    assert (vars);

    for (int i = 0; i < vars -> size; i++) {
        printf ("\r");
        if (strncmp (Name, vars -> var[i] -> name, strlen (Name) + 1) == 0) 
           return i;
        
    }

    return -1;

}

void DestructVars (Vars* vars) {

    assert (vars);

    for (int i = 0; i < vars -> size; i++) 
        free (vars -> var[i]);

    vars -> size = 0;
    return;

}

void DestructAvVars (AvVars* vars) {

    assert (vars);

    if (vars -> gVars != nullptr)
        DestructVars (vars -> gVars);

    if (vars -> fVars != nullptr)
        DestructVars (vars -> fVars);

    return;

}

#endif
