
void maximize_algorithm(double ***matrix, int row_size, int column_size);
void minimize_algorithm(double ***matrix, int row_size, int column_size);
int get_most_negative_column(double **matrix, int row_size);
int get_pivot_row(double **matrix, int row_size, int column_size, int simplex_column);
void make_column_canonical(double ***matrix, int row_size, int column_size, int pivot_row, int pivot_column);

#include "matrix_operations.c"
#define INFINITY 9999


void maximize_algorithm(double ***matrix, int row_size, int column_size)
{
    int column_to_maximize;
    int pivot_row;
    while(1)
    {
        column_to_maximize = get_most_negative_column(*matrix, row_size);
       
        if (column_to_maximize == -1)
            break;

        pivot_row = get_pivot_row(*matrix, column_size, row_size, column_to_maximize);

        make_column_canonical(&(*matrix), row_size, column_size, pivot_row, column_to_maximize);
    }

}

/*returns the position of the column that contains the most negative number
Params:
    matrix;
    column_size = size of the column to iterate

returns:
    int = position of the column that contains the most negative number
*/
int get_most_negative_column(double **matrix, int row_size)
{
    int most_negative_column = -1;
    int most_negative = 0;
    int current_value;
    for (int i = 0; i < row_size; i++)
    {
        current_value = matrix[0][i];
        if (current_value < most_negative)
        {
            most_negative = current_value;
            most_negative_column = i;
        }
    }
    return most_negative_column;
}


/*returns the position of the row that contains the pivot
Params:
    matrix pointer;
    row_size = size of the row
    column_size = size of the column
    simplex_column = column to maximize or minimize

returns:
    int = position of the row that contains the pivot
*/
int get_pivot_row(double **matrix, int column_size, int row_size, int simplex_column)
{
    double minor_division = INFINITY;
    int pivot_row = -1;
    double current_value;
    double current_division;
    for (int i = 2; i < column_size; i++)
    {
        current_value = matrix[i][simplex_column];
        if (current_value > 0)
        {
            current_division = matrix[i][row_size - 1] / current_value;
            if (current_division < minor_division)
            {
                minor_division = current_division;
                pivot_row = i;
            }
        }
    }
    return pivot_row;
}

/*make a column canonical
Params:
    matrix pointer;
    row_size = size of the row
    column_size = size of the column
    pivot_row = row position of pivot
    pivot_column = column_position of pivot

returns:
    void
*/
void make_column_canonical(double ***matrix, int row_size, int column_size, int pivot_row, int pivot_column)
{
    double pivot = (*matrix)[pivot_row][pivot_column];
    double current_value;

    multiply_row_by_constant(&(*matrix), pivot_row, row_size, 1/pivot);

    for (int i = 0; i < column_size; i++)
    {
        if (i == 1)
            continue;

        else if (i != pivot_row)
        {
            current_value = (*matrix)[i][pivot_column];
            add_multiplied_row_to_another(&(*matrix), i, pivot_row, row_size, -1.0 * current_value);
        }
    }
}