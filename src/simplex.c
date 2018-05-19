
void maximize_algorithm(double ***matrix, int row_size, int column_size, int artificial_quantity, int var_quantity);

void minimize_algorithm(double ***matrix, int row_size, int column_size, int artificial_quantity, int var_quantity);
int get_artificial_row_pivot(double **matrix, int column_size, int column_pos);
int get_most_negative_column(double **matrix, int row_size, int var_quantity);
int get_most_positive_column(double **matrix, int row_size, int var_quantity);
int get_pivot_row(double **matrix, int row_size, int column_size, int simplex_column);
void make_column_canonical(double ***matrix, int row_size, int column_size, int pivot_row, int pivot_column);
void make_artficials_canonical(double ***matrix, int row_size, int column_size, int artificial_quantity);

#include "matrix_operations.c"
#define INFINITY 9999


void maximize_algorithm(double ***matrix, int row_size, int column_size, int artificial_quantity, int var_quantity)
{
    int column_to_maximize;
    int pivot_row;

    
    if (artificial_quantity != 0)
        make_artficials_canonical(&(*matrix), row_size, column_size, artificial_quantity);

    while(1)
    {
        column_to_maximize = get_most_negative_column(*matrix, row_size, var_quantity);
       
        if (column_to_maximize == -1)
            break;

        pivot_row = get_pivot_row(*matrix, column_size, row_size, column_to_maximize);

        make_column_canonical(&(*matrix), row_size, column_size, pivot_row, column_to_maximize);
    }

}

void minimize_algorithm(double ***matrix, int row_size, int column_size, int artificial_quantity, int var_quantity)
{
    int column_to_minimize;
    int pivot_row;

    
    if (artificial_quantity != 0)
        make_artficials_canonical(&(*matrix), row_size, column_size, artificial_quantity);

    while(1)
    {
        column_to_minimize = get_most_positive_column(*matrix, row_size, var_quantity);
       
        if (column_to_minimize == -1)
            break;

          
        pivot_row = get_pivot_row(*matrix, column_size, row_size, column_to_minimize);

    

        make_column_canonical(&(*matrix), row_size, column_size, pivot_row, column_to_minimize);

    }
    
}


/*returns the position of the column that contains the most negative number
Params:
    matrix;
    column_size = size of the column to iterate

returns:
    int = position of the column that contains the most negative number
*/
int get_most_negative_column(double **matrix, int row_size, int var_quantity)
{
    int most_negative_column = -1;
    double most_negative = 0;
    double current_value;

    double current_bigM;
    double bigM_quantity = 0;

    for (int i = 1; i < var_quantity + 1; i++)
    {
        current_value = matrix[0][i];
        current_bigM = matrix[1][i];

        if (current_bigM == bigM_quantity)
        {
            if (current_value < most_negative)
            {
                 most_negative = current_value;
            
                bigM_quantity = current_bigM;
                most_negative_column = i;
            }
        }
        else if (current_bigM < bigM_quantity)
        {
            most_negative = current_value;
            
            bigM_quantity = current_bigM;
            most_negative_column = i;
        }
        else if (current_value < most_negative && bigM_quantity == 0)
        {
            most_negative = current_value;
            
            bigM_quantity = current_bigM;
            most_negative_column = i;
        }
    }
    return most_negative_column;
}

/*returns the position of the column that contains the most negative number
Params:
    matrix;
    column_size = size of the column to iterate

returns:
    int = position of the column that contains the most negative number
*/
int get_most_positive_column(double **matrix, int row_size, int var_quantity)
{
    int most_positive_column = -1;
    double most_positive = 0;
    double current_value;

    double current_bigM = 0;
    double bigM_quantity = 0;

    for (int i = 1; i < var_quantity + 1; i++)
    {
        current_value = matrix[0][i];
        current_bigM = matrix[1][i];
         

        if (current_bigM == bigM_quantity)
        {
            if (current_value > most_positive)
            {
                 most_positive = current_value;
            
                bigM_quantity = current_bigM;
                most_positive_column = i;
            }
        }
        else if (current_bigM > bigM_quantity)
        {
            most_positive = current_value;
            
            bigM_quantity = current_bigM;
            most_positive_column = i;
        }
        else if (current_value > most_positive && bigM_quantity == 0)
        {
            most_positive = current_value;

            bigM_quantity = current_bigM;
            most_positive_column = i;
        }
        
    }
    return most_positive_column;
}


/*make the artifical columns canonical
Params:
    matrix pointer;
    row_size = size of the row
    column_size = size of the column
    artificail_quantity = quanity of artificial variables

returns:
    void
*/
void make_artficials_canonical(double ***matrix, int row_size, int column_size, int artificial_quantity)
{
    int artificial_remaining = artificial_quantity;
    int column_pos = row_size - 2;  //-2 = -1 for result column &  -1 for the last artficial variable
    int artificial_row_pivot_pos;

    while (artificial_remaining--)
    {
        artificial_row_pivot_pos = get_artificial_row_pivot((*matrix), column_size, column_pos);

        add_multiplied_row_bigM_to_another(&(*matrix), artificial_row_pivot_pos, row_size, (*matrix)[1][column_pos] * -1);

        column_pos--;
    }
}

int get_artificial_row_pivot(double **matrix, int column_size, int column_pos)
{
    for (int i = 2; i < column_size; i++)
    {
        if (matrix[i][column_pos] == 1)
            return i;
    }
    return -1;
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

    double bigM_value = (*matrix)[1][pivot_column];

    if (bigM_value != 0)
    {
        add_multiplied_row_bigM_to_another(&(*matrix), pivot_row, row_size, (*matrix)[1][pivot_column] * -1.0);
        add_multiplied_row_to_another(&(*matrix), 0, pivot_row, row_size, (*matrix)[0][pivot_column] * -1.0);
    }

    else 
        add_multiplied_row_to_another(&(*matrix), 0, pivot_row, row_size, (*matrix)[0][pivot_column] * -1.0);

    for (int i = 2; i < column_size; i++)
    {
        if (i != pivot_row)
        {
            current_value = (*matrix)[i][pivot_column];
            add_multiplied_row_to_another(&(*matrix), i, pivot_row, row_size, current_value * -1.0);
        }
    }
}