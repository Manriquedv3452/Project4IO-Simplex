#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <math.h>
#include <gtk/gtk.h>
#include <string.h>
#include "structs.h"
#include "support_functions.h"

GtkWidget** varName_list;
int write_intermediate_tables = 0;
Variable *variable_list;   

#include "latex_maker.c"
#include "simplex.c"

void print_matrix(double **matrix, int row_size, int column_size);
void start_program(void);
void add_variable(void);
void initialize_variables_constraints(void);
void show_objfunction_constraints_fileds(void);
void fill_constraints_grid(void);
double** create_simplex_matrix(int excess_quantity, int artificial_quantity, int holgure_quantity);

GtkWidget* input_problem_name;
GtkWidget* input_var_quantity;
GtkWidget* input_constraint_quantity;

GtkWidget* grid_varName;
GtkWidget* grid_objFunction;
GtkWidget* grid_constraints;

GtkWidget* radio_max;
GtkWidget* radio_min;

GtkWidget*** constraints_objFunction_matrix;

GtkWidget* calculate_button;
GtkWidget* intermediate_tables;

GtkWidget* message_dialog;
GtkWidget* window;
GtkWidget* varNames_window;
GtkWidget* objective_contraints_window;

int var_quantity;
int constraint_quantity;

int is_max = 1;

char problem_name[50];

int main(int argc, char* argv[])
{
	startPresentation();

	GtkBuilder *builder;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "../glade/main_window.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	varNames_window = GTK_WIDGET(gtk_builder_get_object(builder, "variableName_window"));
	objective_contraints_window = GTK_WIDGET(gtk_builder_get_object(builder, "objective_contraints_window"));


	input_problem_name = GTK_WIDGET(gtk_builder_get_object(builder, "input_problem_name"));
	input_var_quantity = GTK_WIDGET(gtk_builder_get_object(builder, "input_var_quantity"));
	input_constraint_quantity = GTK_WIDGET(gtk_builder_get_object(builder, "input_constraint_quantity"));
	grid_varName = GTK_WIDGET(gtk_builder_get_object(builder, "grid_varName"));
	grid_objFunction = GTK_WIDGET(gtk_builder_get_object(builder, "grid_objFunction"));
	grid_constraints = GTK_WIDGET(gtk_builder_get_object(builder, "grid_constraints"));

	intermediate_tables = GTK_WIDGET(gtk_builder_get_object(builder, "check_tables"));

	calculate_button = GTK_WIDGET(gtk_builder_get_object(builder, "start_button"));
	message_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "message_dialog"));

	radio_max = GTK_WIDGET(gtk_builder_get_object(builder, "radio_maximize"));
	radio_min = GTK_WIDGET(gtk_builder_get_object(builder, "radio_minimize"));
	
	//gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(window);
	gtk_main();

	finishPresentation();


	return 0;
}

void show_var_names(void)
{
	gtk_widget_hide(window);
	initialize_variables_constraints();
	char default_name[5];

	for (int i = 0; i < var_quantity; i++)
	{
		sprintf(default_name, "x_{%d}", i + 1);
		varName_list[i] = gtk_entry_new();
		gtk_entry_set_width_chars(GTK_ENTRY(varName_list[i]), 10);
		gtk_entry_set_max_length(GTK_ENTRY(varName_list[i]), 6);
		gtk_grid_attach(GTK_GRID(grid_varName), varName_list[i], 0, i, 1, 1);
		gtk_entry_set_text(GTK_ENTRY(varName_list[i]), default_name);
	}

	gtk_widget_show(varNames_window);
	gtk_widget_show_all(grid_varName);
	
}


void show_objfunction_constraints_fileds(void)
{
	gtk_widget_hide(varNames_window);
	gtk_widget_show(objective_contraints_window);

	char label[20];
	GtkWidget* lbl_varName;
	GtkAdjustment* adjustment;
	for (int i = 0; i < var_quantity; i++)
	{
		strcpy(label, gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
		lbl_varName = gtk_label_new(label);
		gtk_label_set_label(GTK_LABEL(lbl_varName), gtk_entry_get_text(GTK_ENTRY(varName_list[i])));

		gtk_grid_attach(GTK_GRID(grid_objFunction), lbl_varName, i, 0, 1, 1);

		lbl_varName = gtk_label_new(label);
		//gtk_label_set_label(GTK_LABEL(lbl_varName), gtk_entry_get_text(GTK_ENTRY(varName_list[i])));

		gtk_grid_attach(GTK_GRID(grid_constraints), lbl_varName, i, 0, 1, 1);

		adjustment = gtk_adjustment_new(1.0, -999999, 999999, 1, 10, 0);
		constraints_objFunction_matrix[0][i] = gtk_spin_button_new(adjustment, 0, 4);
		gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(constraints_objFunction_matrix[0][i]), TRUE);
		gtk_grid_attach(GTK_GRID(grid_objFunction),constraints_objFunction_matrix[0][i], i, 1, 1, 1);
	}
	strcpy(label, "Valor a comparar:");
	lbl_varName = gtk_label_new(label);
	gtk_grid_attach(GTK_GRID(grid_constraints), lbl_varName, var_quantity + 1, 0, 1, 1);

	fill_constraints_grid();

	gtk_widget_show_all(grid_objFunction);
	gtk_widget_show_all(grid_constraints);
}

void fill_constraints_grid(void)
{
	GtkAdjustment* adjustment;
	int constraint_length = var_quantity;
	for (int i = 1; i <= constraint_quantity; i++)
	{
		for (int j = 0; j < var_quantity; j++)
		{
			adjustment = gtk_adjustment_new(1.0, -999999, 999999, 1, 10, 0);
			constraints_objFunction_matrix[i][j] = gtk_spin_button_new(adjustment, 0, 4);
			gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(constraints_objFunction_matrix[i][j]), TRUE);
			gtk_grid_attach(GTK_GRID(grid_constraints), constraints_objFunction_matrix[i][j], j, i + 1, 1, 1);
		}
		constraints_objFunction_matrix[i][constraint_length] = gtk_combo_box_text_new();
		gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(constraints_objFunction_matrix[i][constraint_length]), NULL, "<=");
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(constraints_objFunction_matrix[i][constraint_length]), NULL, ">=");
    	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(constraints_objFunction_matrix[i][constraint_length]), NULL, "=");
		gtk_combo_box_set_active(GTK_COMBO_BOX(constraints_objFunction_matrix[i][constraint_length]), 0);

		gtk_grid_attach(GTK_GRID(grid_constraints), 
						constraints_objFunction_matrix[i][constraint_length], constraint_length, i + 1, 1, 1);

		adjustment = gtk_adjustment_new(1, -999999, 999999, 1, 10, 0);
		constraints_objFunction_matrix[i][constraint_length + 1] = gtk_spin_button_new(adjustment, 0, 4);
		gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(constraints_objFunction_matrix[i][constraint_length + 1]), TRUE);

		gtk_grid_attach(GTK_GRID(grid_constraints), constraints_objFunction_matrix[i][constraint_length + 1], 
							constraint_length + 1, i + 1, 1, 1);

	}




	gtk_widget_show_all(grid_constraints);
}



void calculate_solution(void)
{
	int excess_quantity = 0;
	int artificial_quantity = 0;
	
	int holgure_quantity = 0;

	int *quantity_of_vars = calloc(4, sizeof(int));

	char* comparison;

	gtk_widget_hide(calculate_button);


	for (int i = 1; i <= constraint_quantity; i++)
	{
		comparison = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(constraints_objFunction_matrix[i][var_quantity]));

		if (comparison[0] == '>')
		{
			excess_quantity++;
			artificial_quantity++;
		}


		else if (comparison[0] == '=')
			artificial_quantity++;

		else
			holgure_quantity++;

	}


	int total_variables = var_quantity + holgure_quantity + excess_quantity + artificial_quantity;
	//varName_list = (GtkWidget**) realloc(varName_list, total_variables);
	variable_list = calloc(total_variables + 1, sizeof(Variable));

	int row_length = total_variables + 2; //2 = Z column and Result column
	int column_length = constraint_quantity + 2;

	double** simplex_matrix = create_simplex_matrix(excess_quantity, artificial_quantity, holgure_quantity);

	write_original_problem(constraints_objFunction_matrix, var_quantity + 2, constraint_quantity + 1, problem_name, is_max, varName_list, var_quantity);
	write_initial_table(simplex_matrix, row_length, column_length, var_quantity, holgure_quantity, excess_quantity,
														artificial_quantity, varName_list);

	quantity_of_vars[0] = var_quantity;
	quantity_of_vars[1] = holgure_quantity;
	quantity_of_vars[2] = excess_quantity;
	quantity_of_vars[3] = artificial_quantity;

	//print_matrix(simplex_matrix, row_length, column_length);


	int is_optimal = 0;
	if (is_max)
		is_optimal = maximize_algorithm(&simplex_matrix, row_length, column_length, quantity_of_vars);

	else
		is_optimal = minimize_algorithm(&simplex_matrix, row_length, column_length, quantity_of_vars);

	if (is_optimal == 1)
	{
		write_final_table(simplex_matrix, row_length, column_length, quantity_of_vars, varName_list);
	}
	write_problem_solution(variable_list, quantity_of_vars, simplex_matrix, row_length);

	gtk_widget_show(message_dialog);
	gtk_widget_hide(objective_contraints_window);
}


double** create_simplex_matrix(int excess_quantity, int artificial_quantity, int holgure_quantity)
{
	char* comparison;
	double value;

	int total_variables = var_quantity + holgure_quantity + excess_quantity + artificial_quantity;
	int row_length = total_variables + 2; //2 = Z column and Result column

	int comparator_var;
	int artificial_pos = 1;
	int holgure_pos = 1;
	int excess_pos = 1;

	char var_name[20];

	double **simplex_matrix = calloc(constraint_quantity + 2, sizeof(double)); //2 = Z + row of quanity of M
	simplex_matrix[0] = calloc(row_length, sizeof(double));
	simplex_matrix[1] = calloc(row_length, sizeof(double));

	//valores de funcion objetivo
	simplex_matrix[0][0] = 1;
	for (int i = 1; i <= var_quantity; i++)
	{
		value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(constraints_objFunction_matrix[0][i - 1]));
		simplex_matrix[0][i] = value * -1;
	} 


	//valores de las restricciones
	for (int i = 2; i < constraint_quantity + 2; i++)
	{
		simplex_matrix[i] = calloc(row_length, sizeof(double));
		for (int j = 1; j <= var_quantity; j++)
		{
			value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(constraints_objFunction_matrix[i - 1][j - 1]));
			simplex_matrix[i][j] = value;
			
		}
		comparison = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(constraints_objFunction_matrix[i - 1][var_quantity]));

		if (comparison[0] == '>')
		{
			simplex_matrix[i][var_quantity + holgure_quantity + excess_pos++] = -1;	//e_is
			simplex_matrix[i][var_quantity + holgure_quantity + excess_quantity + artificial_pos] = 1;			//a_i
			
			sprintf(var_name, "e_{%d}", excess_pos - 1);
			strcpy(variable_list[var_quantity + holgure_quantity + excess_pos-1].name, var_name);

			sprintf(var_name, "a_{%d}", artificial_pos);
			strcpy(variable_list[var_quantity + holgure_quantity + excess_quantity + artificial_pos].name, var_name);
			variable_list[var_quantity + holgure_quantity + excess_quantity + artificial_pos].pos_row_of_one = i;

			if (is_max)
				simplex_matrix[1][var_quantity+ holgure_quantity + excess_quantity + artificial_pos] = 1;		//M
			else
				simplex_matrix[1][var_quantity+ holgure_quantity + excess_quantity + artificial_pos] = -1;		//M

			artificial_pos++;
		}

		else if (comparison[0] == '=')
		{
			simplex_matrix[i][var_quantity + holgure_quantity + excess_quantity + artificial_pos] = 1;		//a_i
			sprintf(var_name, "a_{%d}", artificial_pos);
			strcpy(variable_list[var_quantity + holgure_quantity + excess_quantity + artificial_pos].name, var_name);
			variable_list[var_quantity + holgure_quantity + excess_quantity + artificial_pos].pos_row_of_one = i;

			if (is_max)
				simplex_matrix[1][var_quantity+ holgure_quantity + excess_quantity + artificial_pos] = 1;		//M
			else
				simplex_matrix[1][var_quantity+ holgure_quantity + excess_quantity + artificial_pos] = -1;		//M

			artificial_pos++;
		}

		else
		{
			simplex_matrix[i][var_quantity + holgure_pos++] = 1;		//s_i

			sprintf(var_name, "s_{%d}", holgure_pos - 1);
			strcpy(variable_list[var_quantity + holgure_pos - 1].name, var_name);
			variable_list[var_quantity + holgure_pos - 1].pos_row_of_one = i;
			variable_list[var_quantity + holgure_pos - 1].in_base = 1;

		}


		comparator_var = gtk_spin_button_get_value(GTK_SPIN_BUTTON(constraints_objFunction_matrix[i - 1][var_quantity + 1]));
		simplex_matrix[i][row_length - 1] = comparator_var;


	}

	return simplex_matrix;
}

void on_radio_button_toggled (GtkToggleButton *togglebutton)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_max)))
        is_max = 1;
	else
		is_max = 0;
}

void on_check_button_toggled(GtkToggleButton *togglebutton)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(intermediate_tables)))
		write_intermediate_tables = 1;

	else	
		write_intermediate_tables = 0;
}



void initialize_variables_constraints(void)
{
	strcpy(problem_name, gtk_entry_get_text(GTK_ENTRY(input_problem_name)));
	var_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input_var_quantity));
	constraint_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input_constraint_quantity));
	
	varName_list = calloc(var_quantity, sizeof(GtkWidget*));

	constraints_objFunction_matrix = calloc(constraint_quantity + 1, sizeof(GtkWidget*));
	for (int i = 0; i < constraint_quantity + 1; i++)
	{
		constraints_objFunction_matrix[i] = calloc(var_quantity + 2, sizeof(GtkWidget*));
	}
}

/*void change_varNames(void)
{
	//free(varName_list);
	for (int i = 0; i < constraint_quantity + 1; i++)
	{
		free(constraints_objFunction_matrix[i]);
	}
	free(constraints_objFunction_matrix);

	gtk_widget_hide(objective_contraints_window);

	gtk_widget_show(varNames_window);
}*/


void close_varNames_window(void)
{
	free(varName_list);
	//gtk_
}

/*void start_program(void)
{
	srand(time(NULL));
	double **matrix = calloc(4, sizeof(double));
	for (int i = 0; i < 4; i++)
	{
		matrix[i] = calloc(7, sizeof(double));
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			matrix[i][j] = rand() % 10 + 1;
		}
	}

	//fabrica de puertas y ventanas example test
	matrix[0][0] = 1;
	matrix[0][1] = -3;
	matrix[0][2] = -5;
	matrix[1][1] = 1;
	matrix[1][3] = 1;
	matrix[1][6] = 4;
	matrix[2][2] = 2;
	matrix[2][4] = 1;
	matrix[2][6] = 12;
	matrix[3][1] = 3;
	matrix[3][2] = 2;
	matrix[3][5] = 1;
	matrix[3][6] = 18;
	print_matrix(matrix, 4, 7);
	printf("------------------------------------------\n");

	maximize_algorithm(&matrix, 7, 4, 0, 0);

	print_matrix(matrix, 7, 4);
}*/
void print_matrix(double **matrix, int row_size, int column_size)
{
	for (int i = 0; i < column_size; i++)
	{
		for (int j = 0; j < row_size; j++)
		{
			printf("%.2f\t", matrix[i][j]);
		}
		printf("\n");
	}
}
