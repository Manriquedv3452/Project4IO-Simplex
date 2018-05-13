#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gtk/gtk.h>
#include <string.h>
#include "simplex.c"

void print_matrix(double **matrix, int row_size, int column_size);
void start_program(void);
void add_variable(void);
void initialize_variables_constraints(void);
void show_objfunction_constraints_fileds(void);
void fill_constraints_grid(void);

GtkWidget* input_var_quantity;
GtkWidget* input_constraint_quantity;

GtkWidget* grid_varName;
GtkWidget* grid_objFunction;
GtkWidget* grid_constraints;

GtkWidget** varName_list;
GtkWidget*** constraints_objFunction_matrix;

GtkWidget* varNames_window;
GtkWidget* objective_contraints_window;
int var_quantity;
int constraint_quantity;

int main(int argc, char* argv[])
{
	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "../glade/main_window.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	varNames_window = GTK_WIDGET(gtk_builder_get_object(builder, "variableName_window"));
	objective_contraints_window = GTK_WIDGET(gtk_builder_get_object(builder, "objective_contraints_window"));


	input_var_quantity = GTK_WIDGET(gtk_builder_get_object(builder, "input_var_quantity"));
	input_constraint_quantity = GTK_WIDGET(gtk_builder_get_object(builder, "input_constraint_quantity"));
	grid_varName = GTK_WIDGET(gtk_builder_get_object(builder, "grid_varName"));
	grid_objFunction = GTK_WIDGET(gtk_builder_get_object(builder, "grid_objFunction"));
	grid_constraints = GTK_WIDGET(gtk_builder_get_object(builder, "grid_constraints"));
	
	//gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(window);
	gtk_main();


	return 0;
}

void show_var_names(void)
{
	initialize_variables_constraints();
	char default_name[5];

	for (int i = 0; i < var_quantity; i++)
	{
		sprintf(default_name, "x_%d", i + 1);
		varName_list[i] = gtk_entry_new();
		gtk_entry_set_width_chars(GTK_ENTRY(varName_list[i]), 10);
		gtk_entry_set_max_length(GTK_ENTRY(varName_list[i]), 16);
		gtk_grid_attach(GTK_GRID(grid_varName), varName_list[i], 0, i, 1, 1);
		gtk_entry_set_text(GTK_ENTRY(varName_list[i]), default_name);
	}

	gtk_widget_show(varNames_window);
	gtk_widget_show_all(grid_varName);
	
}


void show_objfunction_constraints_fileds(void)
{
	gtk_window_close(GTK_WINDOW(varNames_window));
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
	strcpy(label, "Valor a igualar:");
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

		adjustment = gtk_adjustment_new(1.0, -999999, 999999, 1, 10, 0);
		constraints_objFunction_matrix[i][constraint_length + 1] = gtk_spin_button_new(adjustment, 0, 4);
		gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(constraints_objFunction_matrix[i][constraint_length + 1]), TRUE);

		gtk_grid_attach(GTK_GRID(grid_constraints), constraints_objFunction_matrix[i][constraint_length + 1], 
							constraint_length + 1, i + 1, 1, 1);

	}




	gtk_widget_show_all(grid_constraints);
}


void calculate_solution(void)
{
	double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(constraints_objFunction_matrix[0][0]));
	printf("%.2f\n", value);
}

void initialize_variables_constraints(void)
{
	var_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input_var_quantity));
	constraint_quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input_constraint_quantity));
	
	varName_list = calloc(var_quantity, sizeof(GtkWidget*));

	constraints_objFunction_matrix = calloc(constraint_quantity + 1, sizeof(GtkWidget*));
	for (int i = 0; i < constraint_quantity + 1; i++)
	{
		constraints_objFunction_matrix[i] = calloc(var_quantity + 2, sizeof(GtkWidget*));
	}
}


void close_varNames_window(void)
{
	free(varName_list);
	//gtk_
}

void start_program(void)
{
	srand(time(NULL));
	double **matrix = calloc(4, sizeof(double));
	for (int i = 0; i < 4; i++)
	{
		matrix[i] = calloc(7, sizeof(double));
	}
	/*for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			matrix[i][j] = rand() % 10 + 1;
		}
	}*/

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

	maximize_algorithm(&matrix, 7, 4);

	print_matrix(matrix, 4, 7);
}
void print_matrix(double **matrix, int row_size, int column_size)
{
	for (int i = 0; i < row_size; i++)
	{
		for (int j = 0; j < column_size; j++)
		{
			printf("%.2f\t", matrix[i][j]);
		}
		printf("\n");
	}
}
