void multiply_row_by_constant(double ***matrix, int row, int row_size, double constant);

void add_multiplied_row_to_another(double ***matrix, int row_destiny, int row_to_add, 
                                        int row_size, double constant);

void add_multiplied_row_bigM_to_another(double ***matrix, int row_to_add, 
                                        int row_size, double bigM_value);

/*Multiply a entire row by a constant
Params:
    matrix pointer;
    row = row to be multiplied
    row_size = size of the row
    constant = value that will multiply the row

returns:
    void
*/
void multiply_row_by_constant(double ***matrix, int row, int row_size, double constant)
{
    for (int i = 0; i < row_size; i++)
    {
        (*matrix)[row][i] = (*matrix)[row][i] * constant;
    }
}


/*add the values of a row multiplied by a number to another row
Params:
    matrix pointer;
    row_destiny = row to be added
    row_to_add = values of the row will be added 
    row_size = size of the row
    constant = value that will multiply the values of row_to_add

returns:
    void
*/
void add_multiplied_row_to_another(double ***matrix, int row_destiny, int row_to_add, 
                                        int row_size, double constant)
{
    //constant deafult 1;
    for (int i = 0; i < row_size; i++)
    {
        (*matrix)[row_destiny][i] += (*matrix)[row_to_add][i] * constant;
    }
}


/*add the values of a row multiplied bigM to the row of BigM
Params:
    matrix pointer;
    row_to_add = values of the row will be added 
    row_size = size of the row
    bigM = value that will multiply the values of row_to_add

returns:
    void
*/
void add_multiplied_row_bigM_to_another(double ***matrix, int row_to_add, int row_size, double bigM_value)
{
    for (int i = 0; i < row_size; i++)
        (*matrix)[1][i] += (*matrix)[row_to_add][i] * bigM_value;
}