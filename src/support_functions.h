#include <math.h>

int double_is_equal(double x, double y);

#define EPSILON 0.00001

int double_is_equal(double x, double y)
{
	return (fabs(x - y) < EPSILON && fabs(x - y) > -EPSILON);
}