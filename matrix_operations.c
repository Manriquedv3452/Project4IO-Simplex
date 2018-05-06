void multiply_row_by_constant(int ***matrix, int row, int row_size, int constant);
void add_multiplied_row_to_another(int ***matrix, int row_destiny, int row_to_add, 
                                        int row_size, int constant);

/*Multiply a entire row by a constant
Params:
    matrix pointer;
    row = row to be multiplied
    row_size = size of the row
    constant = value that will multiply the row

returns:
    void
*/
void multiply_row_by_constant(int ***matrix, int row, int row_size, int constant)
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
    constant = value that will multiply the values of row_to add

returns:
    void
*/
void add_multiplied_row_to_another(int ***matrix, int row_destiny, int row_to_add, 
                                        int row_size, int constant)
{
    //constant deafult 1;

    for (int i = 0; i < row_size; i++)
    {
        (*matrix)[row_destiny][i] += (*matrix)[row_to_add][i] * constant;
    }
}