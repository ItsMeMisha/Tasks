#define _FullDefend

#include "Stack_t.h"

struct TestStruct {

	unsigned CheckBegin[1];
	Stack_t Stack;
	unsigned CheckEnd[1];

};

void TestPush (Stack_t* stk);
void TestPop (Stack_t* stk);
void TestCrashCanaryStruct (TestStruct* test);
void TestCrashCanaryData (Stack_t* test);
void TestHashStruct (Stack_t* test);
void TestHashData (Stack_t* test);

int main(int argv, char* argc[]) {

    Stack_t stack = {};

	printf ("\n\tMain Test\n\n");

    StackConstruct (&stack);

	TestPush (&stack);

	TestPop (&stack);

    printf ("\n");

	StackDestruct (&stack);

	printf ("\n\tEnd Main Test\n\n");

	StackConstruct (&stack);

	if (argv > 1) {
	
		switch (*argc[1]) {

		case '1': {

			printf ("\n\tTestCrashCanaryStruct\n\n");

			TestStruct test = {{}, stack, {}};
			TestCrashCanaryStruct (&test);
		
			break; }

		case '2': {

			printf ("\n\tTestCrashCanaryData\n\n");

			TestCrashCanaryData (&stack);
	
			break; }

		case '3': {

			printf ("\n\tTestHashStruct\n\n");

			TestHashStruct (&stack);

			break; }

		case '4': {
		
			printf ("\n\tTestHashData\n\n");

			TestHashData (&stack);

			break; }
	
		default: {

			printf ("\n\tTestCrashCanaryStruct\n\n");

			TestStruct test = {{}, stack, {}};
			TestCrashCanaryStruct (&test);
		
			break; }
		
		}

	}

	else {

		printf ("\n\tTestCrashCanaryStruct\n\n");

		TestStruct test = {{}, stack, {}};
		TestCrashCanaryStruct (&test);
		
	}
	
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

void TestCrashCanaryStruct (TestStruct* test) {

	*(test -> CheckBegin + 1) = 213;
	*(test -> CheckEnd - 1)  = 213;

	StackPush (&(test -> Stack), 213);
	StackPop (&(test -> Stack));

}

void TestCrashCanaryData (Stack_t* test) {

	test -> data[-1] = 228;
	test -> data[test -> MaxSize] = 228;
	
	StackPush (test, 228);
	StackPop (test);

}

void TestHashStruct (Stack_t* test) {

	test -> MaxSize = 228;
	
	StackPush (test, 228);

}

void TestHashData (Stack_t* test) {
	
	StackPush (test, 228);
	StackPush (test, 420);

	Element_t buf = test -> data[0];
	test -> data[0] = test -> data[1];
	test -> data[1] = buf;

	StackPop (test);

}
