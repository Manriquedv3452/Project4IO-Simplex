#include "matrix_operations.c"

int maximize_algorithm(double ***matrix, int row_size, int column_size, int *quantity_of_vars);
int minimize_algorithm(double ***matrix, int row_size, int column_size, int *quantity_of_vars);

/*
pos 0 = var_quantity;
pos 1 = holgure_quantity;
pos 2 = excess_quantity;
pos 3 = artificial_quanity;
*/
int get_artificial_row_pivot(double **matrix, int column_size, int column_pos);
int get_most_negative_column(double **matrix, int row_size, int *quantity_of_vars);
int get_most_positive_column(double **matrix, int row_size, int *quantity_of_vars);
int get_pivot_row(double **matrix, int row_size, int column_size, int simplex_column);
void make_column_canonical(double ***matrix, int row_size, int column_size, int pivot_row, int pivot_column);
void make_artficials_canonical(double ***matrix, int row_size, int column_size, int* quantity_of_vars);
void kick_out_var_from_base(Variable **variable_list, int total_variables, int pivot_row, int column_to_join);
int get_artificial_column_in_base(double **matrix, int row_size, int column_size, int* quantity_of_vars);
int verify_degenerated(double **matrix, int row_length, Variable *variable_list, int variables_quantity);
void search_another_optimal_solution(double ***matrix, int row_length, int column_length, Variable* variable_list, 
                                            int *quantity_of_vars);
void get_solution_of_vars(double** matrix, int row_length, int* quantity_of_vars, double** let_solution, Variable* variable_list);


#define INFINIT 9999

//RETURNS 1 IF A SOLUTION IS FOUND, -1 IF IT IS NOT BOUNDED, 0 IF IT IS NOT DEASIBLE SOLUTION
int maximize_algorithm(double ***matrix, int row_size, int column_size, int *quantity_of_vars)
{
    int column_to_maximize;
    int pivot_row;

    int artificial_column;
    int degenerated_problem_column = -1;

    int total_variables = quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2] + quantity_of_vars[3];
    
    if (quantity_of_vars[3] != 0)
        make_artficials_canonical(&(*matrix), row_size, column_size, quantity_of_vars);

    while(1)
    {
        if (degenerated_problem_column == -1)
        {
            degenerated_problem_column = verify_degenerated(*matrix, row_size, variable_list, total_variables);
            if (degenerated_problem_column != -1)
                write_degenerate_problem(*matrix, row_size, column_size, quantity_of_vars, 
                            varName_list, degenerated_problem_column, variable_list, total_variables);

        }

        column_to_maximize = get_most_negative_column(*matrix, row_size, quantity_of_vars);
       
        if (column_to_maximize == -1)
            break;

        if (write_intermediate_tables)
        {
            begin_frame("Tabla Intermedia");
            write_table_with_color(*matrix, row_size, column_size, quantity_of_vars, 
                        varName_list, column_to_maximize, -1, "blue", "Tabla Intermedia",  variable_list, total_variables);
        }

        pivot_row = get_pivot_row(*matrix, column_size, row_size, column_to_maximize);

        if (pivot_row == -1)
        {
            write_final_table(*matrix, row_size, column_size, quantity_of_vars, varName_list);
            if (degenerated_problem_column != -1)
            {
                exchange_degenerate_problem();
            }

             write_not_bounded_solution(*matrix, row_size, column_size, quantity_of_vars, 
                            varName_list, column_to_maximize, variable_list, total_variables);
            return -1;
        }

        if (write_intermediate_tables)
        {
            begin_frame("Tabla Intermedia con pivote");
            write_table_with_color(*matrix, row_size, column_size, quantity_of_vars, 
                varName_list, column_to_maximize, pivot_row, "blue", "Tabla Intermedia con pivote", variable_list, total_variables);
            end_frame();
        }
        
        kick_out_var_from_base(&variable_list, 
                            total_variables, 
                            pivot_row, column_to_maximize);


        make_column_canonical(&(*matrix), row_size, column_size, pivot_row, column_to_maximize);
    }

    artificial_column = get_artificial_column_in_base(*matrix, row_size, column_size, quantity_of_vars);

    if (artificial_column != -1)
    {
        write_final_table(*matrix, row_size, column_size, quantity_of_vars, varName_list);
        if (degenerated_problem_column != -1)
        {
            exchange_degenerate_problem();
        }

        write_not_feasible_solution(*matrix, row_size, column_size, quantity_of_vars, 
                            varName_list, artificial_column, variable_list, total_variables);
        return 0;
    }
    if (degenerated_problem_column != -1)
    {
        exchange_degenerate_problem();
    }

    return 1;

}

//RETURNS 1 IF A SOLUTION IS FOUND, -1 IF IT IS NOT BOUNDED, 0 IF IT IS NOT DEASIBLE SOLUTION
int minimize_algorithm(double ***matrix, int row_size, int column_size, int* quantity_of_vars)
{
    int column_to_minimize;
    int pivot_row;
    int artificial_column;
    int total_variables = quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2] + quantity_of_vars[3];
    int degenerated_problem_column = -1;
    
    if (quantity_of_vars[3] != 0)
        make_artficials_canonical(&(*matrix), row_size, column_size, quantity_of_vars);


    while(1)
    {

        if (degenerated_problem_column == -1)
        {
            degenerated_problem_column = verify_degenerated(*matrix, row_size, variable_list, total_variables);
            if (degenerated_problem_column != -1)
                write_degenerate_problem(*matrix, row_size, column_size, quantity_of_vars, 
                            varName_list, degenerated_problem_column, variable_list, total_variables);
   
        }


        column_to_minimize = get_most_positive_column(*matrix, row_size, quantity_of_vars);
       
        if (column_to_minimize == -1)
            break;

        if (write_intermediate_tables)
        {
            begin_frame("Tabla Intermedia");
            write_table_with_color(*matrix, row_size, column_size, quantity_of_vars, 
                varName_list, column_to_minimize, -1, "blue", "Tabla Intermedia",  variable_list, total_variables);
        
        }

          
        pivot_row = get_pivot_row(*matrix, column_size, row_size, column_to_minimize);

         if (pivot_row == -1)
        {
            write_final_table(*matrix, row_size, column_size, quantity_of_vars, varName_list);
            if (degenerated_problem_column != -1)
            {
                exchange_degenerate_problem();
            }
             write_not_bounded_solution(*matrix, row_size, column_size, quantity_of_vars,
                 varName_list, column_to_minimize,  variable_list, total_variables);
            return -1;
        }

        if (write_intermediate_tables)
        {
            begin_frame("Tabla Intermedia con pivote");
            write_table_with_color(*matrix, row_size, column_size, quantity_of_vars, 
                varName_list, column_to_minimize, pivot_row, "blue", "Tabla Intermedia con Pivote",  variable_list, total_variables);
            end_frame();
        }

        kick_out_var_from_base(&variable_list, 
                            total_variables, 
                            pivot_row, column_to_minimize);


        make_column_canonical(&(*matrix), row_size, column_size, pivot_row, column_to_minimize);
    }

    artificial_column = get_artificial_column_in_base(*matrix, row_size, column_size, quantity_of_vars);

    if (artificial_column != -1)
    {
        write_final_table(*matrix, row_size, column_size, quantity_of_vars, varName_list);
        if (degenerated_problem_column != -1)
        {
            exchange_degenerate_problem();
        }

        write_not_feasible_solution(*matrix, row_size, column_size, quantity_of_vars, 
                            varName_list, artificial_column, variable_list, total_variables);
        return 0;
    }
    if (degenerated_problem_column != -1)
    {
        exchange_degenerate_problem();
    }
    return 1;
}

void kick_out_var_from_base(Variable **variable_list, int total_variables, int pivot_row, int column_to_join)
{
    for (int i = 1; i < total_variables + 1; i++)
    {
        if ((*variable_list)[i].in_base && (*variable_list)[i].pos_row_of_one == pivot_row)
            (*variable_list)[i].in_base = 0;
    }
    (*variable_list)[column_to_join].in_base = 1;
    (*variable_list)[column_to_join].pos_row_of_one = pivot_row;
}
/*returns the position of the column that contains the most negative number
Params:
    matrix;
    column_size = size of the column to iterate

returns:
    int = position of the column that contains the most negative number
*/
int get_most_negative_column(double **matrix, int row_size, int *quantity_of_vars)
{
    int most_negative_column = -1;
    double most_negative = 0;
    double current_value;

    double current_bigM;
    double bigM_quantity = 0;

    for (int i = 1; i < quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2] + 1; i++)
    {
        current_value = matrix[0][i];
        current_bigM = matrix[1][i];
        if (double_is_equal(current_bigM, 0.000) || current_bigM < 0.000)
        {
            if (double_is_equal(current_bigM, bigM_quantity))
            {
                if (current_value < most_negative)
                {
                    most_negative = current_value;
                
                    bigM_quantity = current_bigM;
                    most_negative_column = i;
                }
            }
            else if (current_bigM < bigM_quantity && current_bigM < 0.000)
            {
                most_negative = current_value;
                
                bigM_quantity = current_bigM;
                most_negative_column = i;
            }
            else if (current_value < most_negative && double_is_equal(bigM_quantity, 0.000))
            {
                most_negative = current_value;
                
                bigM_quantity = current_bigM;
                most_negative_column = i;
            }
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
int get_most_positive_column(double **matrix, int row_size, int *quantity_of_vars)
{
    int most_positive_column = -1;
    double most_positive = 0;
    double current_value;

    double current_bigM = 0;
    double bigM_quantity = 0;

    for (int i = 1; i < quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2] + 1; i++)
    {
        current_value = matrix[0][i];
        current_bigM = matrix[1][i];
         
       // if (!variable_list[i].in_base)
        //{
            if (double_is_equal(current_bigM, 0.000) || current_bigM > 0.000)
            {
                if (double_is_equal(current_bigM, bigM_quantity))
                {
                    if (current_value > most_positive)
                    {
                        most_positive = current_value;
                    
                        bigM_quantity = current_bigM;
                        most_positive_column = i;
                    }
                }
                else if (current_bigM > bigM_quantity && current_bigM > 0.000)
                {
                    most_positive = current_value;
                    
                    bigM_quantity = current_bigM;
                    most_positive_column = i;
                }
                else if (current_value > most_positive && double_is_equal(bigM_quantity, 0.000))
                {
                    most_positive = current_value;

                    bigM_quantity = current_bigM;
                    most_positive_column = i;
                }
            }
        //}
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
void make_artficials_canonical(double ***matrix, int row_size, int column_size, int* quantity_of_vars)
{
    int artificial_remaining = quantity_of_vars[3];
    int column_pos = row_size - 2;  //-2 = -1 for result column &  -1 for the last artficial variable
    int artificial_row_pivot_pos;

    int total_variables = quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2];
    while (artificial_remaining--)
    {
        artificial_row_pivot_pos = get_artificial_row_pivot((*matrix), column_size, column_pos);

        add_multiplied_row_bigM_to_another(&(*matrix), artificial_row_pivot_pos, row_size, (*matrix)[1][column_pos] * -1);

        column_pos--;

        variable_list[total_variables + artificial_remaining + 1].in_base = 1;
    }
}

//returns -1 if there isnt artificial in base
int get_artificial_column_in_base(double **matrix, int row_size, int column_size, int* quantity_of_vars)
{
    int artificial_remaining = quantity_of_vars[3];
    int artificial_column_pos = -1;

    int total_variables = quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2];
    while (artificial_remaining--)
    {
        artificial_column_pos = total_variables + artificial_remaining + 1;

        if (variable_list[artificial_column_pos].in_base == 1)
            return artificial_column_pos;

    }
    return -1;
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
    double minor_division = INFINIT;
    int pivot_row = -1;
    int pivot_row_same = -1;
    time_t t;

    double current_value;
    double current_division;
    int are_divisions = 0;

    Pivot_division *division = calloc(column_size - 2, sizeof(Pivot_division));

    srand((unsigned) time(&t));

    for (int i = 2; i < column_size; i++)
    {
        current_value = matrix[i][simplex_column];
        if (current_value > 0)
        {
            current_division = matrix[i][row_size - 1] / current_value;

            division[i - 2].divided = 1;
            division[i - 2].a = matrix[i][row_size - 1];
            division[i - 2].b = current_value;
            division[i - 2].result = current_division;

            are_divisions = 1;

            if (current_division < minor_division)
            {
                minor_division = current_division;
                pivot_row = i;
                pivot_row_same = -1;
            }
            else if (double_is_equal(current_division, minor_division))
            {
                pivot_row_same = i;
            }
        }
    }


    if (pivot_row_same != -1)
    {
        if (rand() % 2)
            return pivot_row;
        else 
            return pivot_row_same;
    }

    if (are_divisions && write_intermediate_tables)
    {
        write_pivot_divisions(division, column_size - 2, pivot_row);
        end_frame();
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
    //printf("\n\npivot_row: %d, pivot_column: %d", pivot_row, pivot_column);
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

//returns the column of the variable that is degenerated
int verify_degenerated(double **matrix, int row_length, Variable *variable_list, int variables_quantity)
{
    int is_equal_zero = 0;
    for (int i = 1; i < variables_quantity; i++)
    {
        is_equal_zero = double_is_equal(matrix[variable_list[i].pos_row_of_one][row_length - 1], 0.000);

        if (variable_list[i].in_base && is_equal_zero)
        {

            return i;
        }
    }
    return -1;
}

void search_another_optimal_solution(double ***matrix, int row_length, int column_length, Variable* variable_list, 
                                            int *quantity_of_vars)
{
    int pivot_row;
    double *solution1 = calloc(quantity_of_vars[0], sizeof(double));
    get_solution_of_vars(*matrix, row_length, quantity_of_vars, &solution1, variable_list);

    int total_variables = quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2] + quantity_of_vars[3];

    double *solution2 = calloc(quantity_of_vars[0], sizeof(double));
    double *solution3 = calloc(quantity_of_vars[0], sizeof(double));
    double *solution4 = calloc(quantity_of_vars[0], sizeof(double));

    for (int i = 1; i < row_length - quantity_of_vars[3] - 1; i++)
    {
        if (double_is_equal((*matrix)[0][i], 0.000) && !variable_list[i].in_base)
        {
            pivot_row = get_pivot_row(*matrix, column_length, row_length, i);

            if (pivot_row != -1)
            {
                begin_frame("Soluciones M\\'ultiples");
                write_table_with_color(*matrix, row_length, column_length, quantity_of_vars, 
                            varName_list, i, -1, "green", "Soluciones M\\'ultiples",  variable_list, total_variables);
                end_frame();

                make_column_canonical(&(*matrix), row_length, column_length, pivot_row, i);
                kick_out_var_from_base(&variable_list, 
                            total_variables, 
                            pivot_row, i);

                get_solution_of_vars(*matrix, row_length, quantity_of_vars, &solution2, variable_list);
                write_multiple_solution(quantity_of_vars, solution1, solution2, solution3, solution4);
                break;
            }
        }
    }
}

void get_solution_of_vars(double** matrix, int row_length, int* quantity_of_vars, double** let_solution, Variable* variable_list)
{
    for (int i = 1; i < quantity_of_vars[0] + 1; i++)
    {
        if (variable_list[i].in_base)
            (*let_solution)[i - 1] = matrix[variable_list[i].pos_row_of_one][row_length - 1];
        
        else
            (*let_solution)[i - 1] = 0.000;
    }
}