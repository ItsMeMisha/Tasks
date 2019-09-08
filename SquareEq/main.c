#include <stdio.h>
#include <math.h>
#include <assert.h>

/*! Infinite number */
const int INF_NUMBER = -1;
/*! Accuracy of comparison */
const double ACC = 1e-6;

	/*!
       ReadCoefficients is a function that reads three coefficients from standard input

       \param[in] a, b, c - coefficients

	*/

void ReadCoefficients(double* a, double* b, double* c);

/*!
	IsEquale is a function that compares two numbers with some accuracy

	\param[in] x - the first number
	\param[in] y - the second number

	\return 1 in case of equality
	\return 0 in case of inequality
*/

int IsEquale(double x, double y);

/*!
*	SolveLinear is a function that solves a linear equalation

	The form of equations: bx + c = 0;

	\param[in] b is the coefficient near x
	\param[in] c is the free coefficient
	\param[out] x is the root of the equale

	\return number of roots of the linear equalation

*/

int SolveLinear(double b, double c, double* x );

/*!
*	SolveSquare is a function that solves a square equalation
	The form of an equalation: ax2+bx+c=0

	\param[in] a is the first coefficient
	\param[in] b is the second coefficient
	\param[in] c is the free coefficient

	\param[out] x1 is the adress of the first root
	\param[out] x2 is the adress of the second root

	\return Number of roots
*/

int SolveSquare(double a, double b, double c, double* x1, double* x2);

/*!
	Main function shows the roots of square or linear equations

	\return 0 in case of successful running
	\return 1 in case of error in SolveSquare function
	\return 2 in case of error: "Coefficients are not numbers"

*/

int main() {
	printf("This program solves square equations of the form ax2+bx+c=0 \n");
	printf("Please enter a, b and c \n");

	double a = 0, b = 0, c = 0;

    ReadCoefficients(&a, &b, &c);

	double x1 = 0, x2 = 0;
	int nRoots = SolveSquare(a, b, c, &x1, &x2);

	switch (nRoots) {
	case 1: {
		printf("This equation has one root: x1 = %lg \n", x1);
		break; }
	case 2: {
		printf("This equation has two roots: x1 = %lg , x2 = %lg \n", x1, x2);
		break; }
	case 0: {
		printf("This equation has no roots \n");
		break; }
	case INF_NUMBER: {
		printf("Infinite number of roots \n");
		break; }
	default: {
		printf("Unexpected error");
		return 1; }
	}

	return 0;
}

void ReadCoefficients(double* a, double* b, double* c){

    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

	int are_numbers = scanf("%lg %lg %lg", a, b, c);

	while (are_numbers != 3) {
		printf("Coefficients are not numbers, try one more time\n");
		scanf("%*s");
		are_numbers = scanf("%lg %lg %lg", a, b, c);
	}
}

int IsEquale(double x, double y) {

	assert(isfinite(x) != 0);
	assert(isfinite(y) != 0);

	x -= y;
	x = fabs(x);

	if (x <= ACC)
		return 1;
	else
		return 0;
}

int SolveLinear(double b, double c, double* x ) {
	assert(isfinite(b) != 0);
	assert(isfinite(c) != 0);

	assert(x != NULL);

	if (IsEquale(b, 0))
		return 0;
	else {
		*x = -c / b;
		return 1;
	}
}

int SolveSquare(double a, double b, double c, double* x1, double* x2) {
	assert(isfinite(a) != 0);
	assert(isfinite(b) != 0);
	assert(isfinite(c) != 0);

	assert(x1 != NULL);
	assert(x2 != NULL);
	assert(x1 != x2);

	if (IsEquale(a, 0) && IsEquale(b, 0)){
		if (IsEquale(c, 0))
			return INF_NUMBER;
		else
			return 0;
    }

	if (IsEquale(a, 0) && !(IsEquale(b, 0) == 1)) {
		return SolveLinear(b, c, x1);
	}

	double discr = b * b - 4 * a * c;

	if (IsEquale(discr, 0))
		discr = 0;

	if (discr < 0)
		return 0;
	else
		discr = sqrt(discr);

	if (!(IsEquale(a, 0) == 1)) {
		*x1 = (-b - discr) / (2 * a);
		*x2 = (-b + discr) / (2 * a);
		if (*x1 == *x2)
			return 1;
		else
			return 2;
	}
	else
		return 0;
}
