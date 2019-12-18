#include "grammar.h"
#include "middleend.h"
#include "backend.h"
#include "MusicFrontEnd.h"

int main () {

    Tree tree = {};

    TreeConstruct (&tree);
    GetTree ("test.muse", &tree);
     OptimiseTree (&tree);
    TreeDump (&tree);
   FILE* out = fopen ("asmtest.asm", "w");
    PrAsmCode (&tree, out);
    fclose (out);


    out = fopen ("music.abc", "w");
    PrintMusic (out, &tree);
    fclose (out);

    TreeDestruct (&tree);

    printf ("Test1 end\n");    

    TreeConstruct (&tree);
    GetTree ("fibo.muse", &tree);
    printf ("Got fibo tree\n");


//    TreeDump (&tree);

    out = fopen ("fibo.asm", "w");

    PrAsmCode (&tree, out);

    fclose (out);
    TreeDestruct (&tree);

    return 0;

}
