typedef struct variable
{
	char name[20];
    int in_base;
    int pos_row_of_one;

} Variable;

typedef struct pivot_division
{
    int divided;
    double a;
    double b;
    double result;
} Pivot_division;