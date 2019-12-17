#include "grammar.h"
#include "backend.h""

int main () {

    Tree tree = {};

    TreeConstruct (&tree);
    GetTree ("test.muse", &tree);
    TreeDump (&tree);
    FILE* out = fopen ("asmtest.asm", "w");
    PrAsmCode (&tree, out);
    fclose (out);
    TreeDestruct (&tree);

    return 0;

}
