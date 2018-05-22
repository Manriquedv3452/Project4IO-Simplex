int double_is_equal(double x, double y);

#define EPSILON 0.0001

int double_is_equal(double x, double y)
{
	return ((x - y) < EPSILON && (x - y) > -EPSILON);
}