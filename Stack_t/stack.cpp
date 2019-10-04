#define _FullDefend

#include "Stack_t.h"

void TestPush (Stack_t* stk);
void TestPop (Stack_t* stk);

int main() {

    Stack_t stk1 = {};

    StackConstruct (&stk1);

	TestPush (&stk1);

	TestPop (&stk1);

    printf ("\n");

    StackDestruct (&stk1);

    return 0;

}

void TestPush (Stack_t* stk) {

    for (int i = 0; i < 300 ; ++i)

        if (i == 66) {

			bool ok = StackPush (stk, Poison);

            if (!ok)
				printf ("Error StackPush\n");

		}

        else {

			bool ok = StackPush (stk, i);

            if (!ok)
				printf ("Error StackPush\n");

		}

}

void TestPop (Stack_t* stk) {

	 for (int i = 0; i <= 300; ++i) {

        if (i%10 == 0)
            printf ("\n");

        printf ("%d\t", StackPop (stk));

    }

}
