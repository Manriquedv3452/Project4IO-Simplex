#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

void startPresentation(void);
void finishPresentation(void);
void write_original_problem(GtkWidget ***matrix, int row_length, int column_length, char problem_name[50],
								 int is_max, GtkWidget** varName_list, int var_quantity);

void write_initial_table(double **matrix, int row_length, int column_length, int var_quantity, int holgure_quantity,
											int excess_quantity, int artificial_quantity, GtkWidget** varName_list);

void write_final_table(double **matrix, int row_length, int column_length, int* quantity_of_vars, GtkWidget** varName_list);

void write_table_values(double** matrix, int row_limit, int column_length, int start_j);

void write_table_with_color(double **matrix, int row_length, int column_length, 
				int *quantity_of_vars, GtkWidget** varName_list, int column_choose, 
				int pivot_row, char* color, char* frame_title, Variable *variable_list, int variables_quantity);

void write_cell_with_color(double** matrix, int row_limit, int column_length, int start_j, int column_choose, int pivot_row, char* column_color, Variable *variable_list);

void write_problem_solution(Variable *variable_list, int *variables_quantity, double **matrix, int row_length);

void write_not_bounded_solution(double **matrix, int row_length, int column_length, 
				int *quantity_of_vars, GtkWidget** varName_list, int column_choose,
				Variable *variable_list, int variables_quantity);

void write_not_feasible_solution(double **matrix, int row_length, int column_length, 
				int *quantity_of_vars, GtkWidget** varName_list, int column_choose,
				Variable *variable_list, int variables_quantity);

void begin_frame(char* frame_title);
void end_frame(void);

int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);

FILE *file;
int tab = 0;
int keyCount = 0;
int newLine = 0;
int isOpenPar = 0;

void startPresentation(void)
{
	file = fopen("../latex/beamer.tex", "w");
	fprintf(file, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", 
			"\\documentclass{beamer}\n\\usepackage{listings}\n\\usepackage{setspace}\n\\usepackage[utf8]{inputenc}\n\\usepackage{pgfplots}\n\\usepackage{lmodern}\n\\usepackage{newverbs}\n\\usepackage{natbib}\n\\pgfplotsset{compat=1.13}\n\\setlength\\parindent{20pt}", 
			"\\usetheme{Warsaw}\n\\newcommand\\tab[1][1cm]{\\hspace*{#1}}\n\\usepackage{array, longtable}\n\\usepackage{tabu}\n"
			"\\usepackage{xcolor}\n\\usepackage{colortbl}\n",
			"\\setbeamercolor{normal text}{fg=white,bg=black!90}\n\\setbeamercolor{structure}{fg=white}\n\\setbeamercolor{alerted text}{fg=red!85!black}\n\\setbeamercolor{item projected}{use=item,fg=black,bg=item.fg!35}\n\\setbeamercolor*{palette primary}{use=structure,fg=structure.fg}\n\\setbeamercolor*{palette secondary}{use=structure,fg=structure.fg!95!black}\n\\setbeamercolor*{palette tertiary}{use=structure,fg=structure.fg!90!black}\n\\setbeamercolor*{palette quaternary}{use=structure,fg=structure.fg!95!black,bg=green!80}\n\\setbeamercolor*{framesubtitle}{fg=white}\n\\setbeamercolor*{block title}{parent=structure,bg=black!60}\n\\setbeamercolor*{block body}{fg=gray,bg=black!10}\n\\setbeamercolor*{block title alerted}{parent=alerted text,bg=black!15}\n\\setbeamercolor*{block title example}{parent=example text,bg=black!15}",
			"\\title{Proyecto 4: Otro SIMPLEX m\\'as}",
			"\\subtitle{Investigaci\\'on de Operaciones \\newline Semestre 1}",
			"\\author{Manrique J. Dur\\'an V\\'asquez - Randy M. Morales Gamboa}",
			"\\institute{Instituto Tecnol\\'ogico de Costa Rica}", 		
			"\\date{\\today}\n\\newverbcommand{\\kwverb}{\\color{red}}{}\n\\newverbcommand{\\smverb}{\\color{cyan}}{}\n\\newverbcommand{\\idverb}{\\color{brown}}{}\n\\newverbcommand{\\assverb}{\\color{orange}}{}\n\\newverbcommand{\\cverb}{\\color{pink}}{}\n\\newverbcommand{\\opverb}{\\color{green}}{}\n\\newverbcommand{\\slverb}{\\color{olive}}{}");

	fprintf(file,"%s\n%s\n", "\\begin{document}",
			"\\begin{frame}\n\\maketitle\n\\end{frame}"
			"\\begin{frame}\n\\frametitle{Algoritmo SIMPLEX}"
			"El algoritmo SIMPLEX fue inventado por George Dantzig"
			"\\end{frame}");
}



void write_original_problem(GtkWidget ***matrix, int row_length, int column_length, char problem_name[50],
								 int is_max, GtkWidget** varName_list, int var_quantity)
{

	double value;
	char* symbol;
	double number_comparator;

	int current_constraints_num = 0;

	fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{Problema: %s}\n", problem_name);

	if (is_max)
		fprintf(file, "\\noindent \\textbf{Maximizar:}\\\\\n\\tab $Z = $");
	else
		fprintf(file, "\\noindent \\textbf{Minimizar:}\\\\\n\\tab $Z = $");

	for (int i = 0; i < var_quantity; i++)
	{
		value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(matrix[0][i]));
		if (value > 0 && i != 0)
		{
			if (value != 1)
				fprintf(file, " + (%.2f)\\textcolor{yellow}{$%s$}", value, gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
			else
				fprintf(file, " + \\textcolor{yellow}{$%s$}", gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
		}
		else if (value < 0)
		{
			if (value != 1)
				fprintf(file, " - (%.2f)\\textcolor{yellow}{$%s$}", value * -1, gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
			else	
				fprintf(file, " - \\textcolor{yellow}{$%s$}", gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
		}

		else 
		{
			if (value != 1)
				fprintf(file, " (%.2f)\\textcolor{yellow}{$%s$}", value, gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
			else if (double_is_equal(value, 1.000))
				fprintf(file, " \\textcolor{yellow}{$%s$}", gtk_entry_get_text(GTK_ENTRY(varName_list[i])));
		}
	}

	fprintf(file, "\\newline\\newline\n\\textbf{Sujeto a:}\\\\\n");

	for (int i = 1; i < column_length; i++)
	{
		for (int j = 0; j < var_quantity; j++)
		{
			value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(matrix[i][j]));
			if (value >= 0 && j != 0)
			{
				if (value != 1)
					fprintf(file, " + (%.2f)\\textcolor{yellow}{$%s$}", value, gtk_entry_get_text(GTK_ENTRY(varName_list[j])));
				else
					fprintf(file, " + \\textcolor{yellow}{$%s$}", gtk_entry_get_text(GTK_ENTRY(varName_list[j])));
			}
			else if (value < 0)
			{
				if (value != 1)
					fprintf(file, " - (%.2f)\\textcolor{yellow}{$%s$}", value * -1, gtk_entry_get_text(GTK_ENTRY(varName_list[j])));
				else	
					fprintf(file, " - \\textcolor{yellow}{$%s$}", gtk_entry_get_text(GTK_ENTRY(varName_list[j])));
			}
			else 
			{
				if (value != 1)
					fprintf(file, "\\tab (%.2f)\\textcolor{yellow}{$%s$}", value, gtk_entry_get_text(GTK_ENTRY(varName_list[j])));
				else if (double_is_equal(value, 1.000))
					fprintf(file, "\\tab \\textcolor{yellow}{$%s$}", gtk_entry_get_text(GTK_ENTRY(varName_list[j])));
			}
		}
		symbol =  gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(matrix[i][var_quantity]));
		number_comparator = gtk_spin_button_get_value(GTK_SPIN_BUTTON(matrix[i][var_quantity + 1]));

		if (symbol[0] == '>')
			fprintf(file, " $\\geq$ %.2f\\newline\\newline", number_comparator);
		else if (symbol[0] == '<')
			fprintf(file, " $\\leq$ %.2f\\newline\\newline", number_comparator);
		else 
			fprintf(file, "= %.2f\\newline\\newline", number_comparator);

		current_constraints_num++;

		if (current_constraints_num == 4 && column_length - 1 > 4)
			fprintf(file, "\\end{frame}\n\\begin{frame}");
	}

	fprintf(file, "\\end{frame}");
}


void write_table_values(double** matrix, int row_limit, int column_length, int start_j)
{
	fprintf(file, "\\\\\\hline\n");
	for(int i = 0; i < column_length; i++)
	{
		if (i != 1)
		{
			for (int j = start_j; j < row_limit; j++)
			{
				if (double_is_equal(matrix[1][j], 0.000) || i != 0.0)
					fprintf(file, "%s %.2f ", j == start_j ? "" : "&", matrix[i][j]);

				else if (matrix[1][j] != 0 && double_is_equal(matrix[i][j], 0.000) && i == 0)
				{
					if (double_is_equal(matrix[1][j], 1.000) || double_is_equal(matrix[1][j], -1.000))
						fprintf(file, "%s %cM ", j == start_j ? "" : "&", matrix[1][j] < 0 ? '-' : '+');
					else 
						fprintf(file, "%s %.1fM ", j == start_j ? "" : "&", matrix[1][j]);
				}

				else if (i == 0)
				{
					if (double_is_equal(matrix[1][j], 1.000) || double_is_equal(matrix[1][j], -1.000))
						fprintf(file, "%s %.2f %c M", j == start_j ? "" : "&", matrix[i][j], matrix[1][j] < 0 ? '-' : '+');
					else 
						fprintf(file, "%s %.2f %s %.1fM", j == start_j ? "" : "&", matrix[i][j], matrix[1][j] > 0 ? "+" : "", matrix[1][j]);
				}
			}
			fprintf(file, "\\\\\\hline\n");
		}
	}
}

void write_initial_table(double **matrix, int row_length, int column_length, int var_quantity, int holgure_quantity,
											int excess_quantity, int artificial_quantity, GtkWidget** varName_list)
{
	int columns_latex_quantity = 0;
	int columns_per_frame = 8;
	int start_j = 0;
	int current_columns = 0;

	fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{Tabla Inicial}\n");
				
	
	fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
	
	fprintf(file, "$Z$");
	current_columns++;

	for (int i = 0; i < var_quantity; i++)
	{
		fprintf(file, "& $%s$ ", gtk_entry_get_text(GTK_ENTRY(varName_list[i]))); 
		columns_latex_quantity++;
		current_columns++;
		
		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;

			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Inicial Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}





	for (int i = 0; i < holgure_quantity; i++)
	{
		fprintf(file, "%s $s_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Inicial Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	for (int i = 0; i < excess_quantity; i++)
	{
		fprintf(file, "%s $e_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Inicial Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	for (int i = 0; i < artificial_quantity; i++)
	{
		fprintf(file, "%s $a_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Inicial Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	fprintf(file, "%s result", (columns_latex_quantity == 0) ? "" : "&");
	current_columns++;
	write_table_values(matrix, current_columns, column_length, start_j);


	fprintf(file, "%s", "\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");

	fprintf(file, "\\end{frame}");
}

void write_cell_with_color(double** matrix, int row_limit, int column_length, int start_j, 
					int column_choose, int pivot_row, char* column_color, Variable* variable_list)
{
	fprintf(file, "\\\\\\hline\n");
	for(int i = 0; i < column_length; i++)
	{
		if (i != 1)
		{
			for (int j = start_j; j < row_limit; j++)
			{
				if (variable_list[j].in_base == 0 && strstr(variable_list[j].name, "a") != NULL)
				{
					if (j == start_j)
						start_j = j+1;
					continue;
				}

				else
				{
					if (double_is_equal(matrix[1][j], 0.000) || i != 0.0)
					{
						if (pivot_row == i && j == column_choose)
							fprintf(file, "%s\\cellcolor{green!100} %.2f ", j == start_j ? "" : "&", matrix[i][j]);
						
						else if (j == column_choose)
							fprintf(file, "%s\\cellcolor{%s!100} %.2f ", j == start_j ? "" : "&", column_color, matrix[i][j]);
						
						else
							fprintf(file, "%s %.2f ", j == start_j ? "" : "&", matrix[i][j]);

					}

					else if (matrix[1][j] != 0 && double_is_equal(matrix[i][j], 0.000) && i == 0)
					{
						if (double_is_equal(matrix[1][j], 1.000) || double_is_equal(matrix[1][j], -1.000))
						{
							if (pivot_row == i && j == column_choose)
								fprintf(file, "%s\\cellcolor{green!100} %cM ", j == start_j ? "" : "&", matrix[1][j] < 0 ? '-' : '+');

							else if (j == column_choose)
								fprintf(file, "%s\\cellcolor{%s!100} %cM ", j == start_j ? "" : "&", column_color, matrix[1][j] < 0 ? '-' : '+');
							
							else 
								fprintf(file, "%s %cM ", j == start_j ? "" : "&", matrix[1][j] < 0 ? '-' : '+');
						}
							
						else 
							if (pivot_row == i && j == column_choose)
								fprintf(file, "%s\\cellcolor{green!100} %.1fM ", j == start_j ? "" : "&", matrix[1][j]);

							else if (j == column_choose)
								fprintf(file, "%s\\cellcolor{%s!100} %.1fM ", j == start_j ? "" : "&", column_color, matrix[1][j]);

							else
								fprintf(file, "%s %.1fM ", j == start_j ? "" : "&", matrix[1][j]);
					}

					else if (i == 0)
					{
						if (double_is_equal(matrix[1][j], 1.000) || double_is_equal(matrix[1][j], -1.000))
						{
							if (pivot_row == i && j == column_choose)
								fprintf(file, "%s\\cellcolor{green!100} %.2f %c M", j == start_j ? "" : "&", matrix[i][j], matrix[1][j] < 0 ? '-' : '+');

							else if (j == column_choose)
								fprintf(file, "%s\\cellcolor{%s!100} %.2f %c M", j == start_j ? "" : "&", column_color, matrix[i][j], matrix[1][j] < 0 ? '-' : '+');

							else
								fprintf(file, "%s %.2f %c M", j == start_j ? "" : "&", matrix[i][j], matrix[1][j] < 0 ? '-' : '+');
						}
						else 
						{
							if (pivot_row == i && j == column_choose)
								fprintf(file, "%s\\cellcolor{green!100} %.2f %s %.1fM", j == start_j ? "" : "&", matrix[i][j], matrix[1][j] > 0 ? "+" : "", matrix[1][j]);

							else if (j == column_choose)
								fprintf(file, "%s\\cellcolor{%s!100} %.2f %s %.1fM", j == start_j ? "" : "&", column_color, matrix[i][j], matrix[1][j] > 0 ? "+" : "", matrix[1][j]);

							else
								fprintf(file, "%s %.2f %s %.1fM", j == start_j ? "" : "&", matrix[i][j], matrix[1][j] > 0 ? "+" : "", matrix[1][j]);
						}
					}
				}
			}
			fprintf(file, "\\\\\\hline\n");
		}
	}
}

void begin_frame(char* frame_title)
{
		fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{%s}\n", frame_title);
}

void end_frame(void)
{
	fprintf(file, "\\end{frame}\n");
}

void write_table_with_color(double **matrix, int row_length, int column_length, 
				int *quantity_of_vars, GtkWidget** varName_list, int column_choose, 
				int pivot_row, char* color, char* frame_title, Variable *variable_list, int variables_quantity)
{
	int columns_latex_quantity = 0;
	int columns_per_frame = 8;
	int start_j = 0;
	int current_columns = 0;
				
	
	fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
	
	fprintf(file, "$Z$");
	current_columns++;

	for (int i = 0; i < quantity_of_vars[0]; i++)
	{
		fprintf(file, "& $%s$ ", gtk_entry_get_text(GTK_ENTRY(varName_list[i]))); 
		columns_latex_quantity++;
		current_columns++;
		
		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_cell_with_color(matrix, current_columns, column_length, start_j, column_choose, pivot_row, color, variable_list);
			start_j = current_columns;

			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{%s Cont.}\n", frame_title);
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}


	for (int i = 0; i < quantity_of_vars[1]; i++)
	{
		fprintf(file, "%s $s_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_cell_with_color(matrix, current_columns, column_length, start_j, column_choose, pivot_row, color, variable_list);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{%s Cont.}\n", frame_title);
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	for (int i = 0; i < quantity_of_vars[2]; i++)
	{
		fprintf(file, "%s $e_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_cell_with_color(matrix, current_columns, column_length, start_j, column_choose, pivot_row, color, variable_list);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{%s Cont.}\n", frame_title);
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}


	int var_he_total = quantity_of_vars[0] + quantity_of_vars[1] + quantity_of_vars[2];
	for (int i = 0; i < quantity_of_vars[3]; i++)
	{
		if (variable_list[var_he_total + i + 1].in_base)
		{
			fprintf(file, "%s $a_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
			columns_latex_quantity++;
			current_columns++;

			if (columns_latex_quantity == columns_per_frame)
			{
				columns_latex_quantity = 0;
				write_cell_with_color(matrix, current_columns, column_length, start_j, column_choose, pivot_row, color, variable_list);
				start_j = current_columns;
				
				fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
				"\\end{tabu}\n}\n");
				fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{%s Cont.}\n", frame_title);
				fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
			}
		}
	}

	fprintf(file, "%s result", (columns_latex_quantity == 0) ? "" : "&");
	current_columns++;

	write_cell_with_color(matrix, row_length, column_length, start_j, column_choose, pivot_row, color, variable_list);


	fprintf(file, "%s", "\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");

}


void write_final_table(double **matrix, int row_length, int column_length, int* quantity_of_vars, GtkWidget** varName_list)
{
	int columns_latex_quantity = 0;
	int columns_per_frame = 8;
	int start_j = 0;
	int current_columns = 0;

	fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{Tabla Final}\n");
				
	
	fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
	
	fprintf(file, "$Z$");
	current_columns++;

	for (int i = 0; i < quantity_of_vars[0]; i++)
	{
		fprintf(file, "& $%s$ ", gtk_entry_get_text(GTK_ENTRY(varName_list[i]))); 
		columns_latex_quantity++;
		current_columns++;
		
		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;

			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Final Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}


	for (int i = 0; i < quantity_of_vars[1]; i++)
	{
		fprintf(file, "%s $s_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Final Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	for (int i = 0; i < quantity_of_vars[2]; i++)
	{
		fprintf(file, "%s $e_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Final Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	for (int i = 0; i < quantity_of_vars[3]; i++)
	{
		fprintf(file, "%s $a_{%d}$", (columns_latex_quantity == 0) ? "" : "&", i + 1);
		columns_latex_quantity++;
		current_columns++;

		if (columns_latex_quantity == columns_per_frame)
		{
			columns_latex_quantity = 0;
			write_table_values(matrix, current_columns, column_length, start_j);
			start_j = current_columns;
			
			fprintf(file, "\n%s", "\n\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");
			fprintf(file, "\n\\end{frame}\n\\begin{frame}\n\\frametitle{Tabla Final Cont.}\n");
			fprintf(file, "{\n\\centering\n\\begin{tabu}{|@{}*{%d}{p{1.0cm}@{}|}}\n%s\n", row_length, "\\rowcolor{black}%");
		}
	}

	fprintf(file, "%s result", (columns_latex_quantity == 0) ? "" : "&");
	current_columns++;
	write_table_values(matrix, current_columns, column_length, start_j);


	fprintf(file, "%s", "\\rowcolor{black}%\n"
			"\\end{tabu}\n}\n");

	fprintf(file, "\\end{frame}");
}


void write_problem_solution(Variable *variable_list, int *variables_quantity, double **matrix, int row_length)
{
	int frame_size = 1;
	fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{Solución}\n\\centering\n{");

	fprintf(file, "$Z = %.2f$\\\\\n", matrix[0][row_length - 1]);

	int total_variables = variables_quantity[0] + variables_quantity[1] + variables_quantity[2] + variables_quantity[3];
	for (int i = 0; i < variables_quantity[0]; i++)
	{
		
		if (variable_list[i + 1].in_base)
			fprintf(file, "\\textcolor{yellow}{$%s$} $ = %.2f$", gtk_entry_get_text(GTK_ENTRY(varName_list[i])), 
													matrix[variable_list[i + 1].pos_row_of_one][row_length - 1]);
		else	
			fprintf(file, "\\textcolor{yellow}{$%s$} $ = 0$", gtk_entry_get_text(GTK_ENTRY(varName_list[i])));

		fprintf(file, "\\\\\n");

		frame_size++;
		if (frame_size == 16)
		{
			fprintf(file, "}\n\\end{frame}");
			fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{Solución Cont.}\n\\centering\n{");
		}

	}

	for (int i = variables_quantity[0] + 1; i < total_variables; i++)
	{

		if (variable_list[i].in_base)
		{
			fprintf(file, "\\textcolor{green}{$%s$} $ = %.2f$\\\\\n", variable_list[i].name, matrix[variable_list[i].pos_row_of_one][row_length - 1]);
			frame_size++;
		}
		else if (variable_list[i].name[0] != 'a')
		{	
			fprintf(file, "\\textcolor{green}{$%s$} $ = 0$\\\\\n", variable_list[i].name);
			frame_size++;
		}

		if (frame_size == 16)
		{
			fprintf(file, "}\n\\end{frame}");
			fprintf(file, "\\begin{frame}\n"
				  "\\frametitle{Solución Cont.}\n\\centering\n{");
		}
	}
	fprintf(file, "}\n\\end{frame}");
}


void write_not_bounded_solution(double **matrix, int row_length, int column_length, 
				int *quantity_of_vars, GtkWidget** varName_list, int column_choose,
				Variable *variable_list, int variables_quantity)
{
	begin_frame("Soluci\\'on: NO ACOTADA");
	fprintf(file, "Un problema es no acotado cuando al escoger el pivote las entradas son 0 o negativas.\\newline\\newline\n"
					"En este problema en la "
					"columna '%d', todas las celdas son 0 o negativas, se adjunta la tabla:\\newline\\newline\n", column_choose + 1);
	 write_table_with_color(matrix, row_length, column_length, quantity_of_vars, varName_list, column_choose, -1, "red", "Soluci\\'on: NO ACOTADA", variable_list, variables_quantity);
	 end_frame();
}

void write_not_feasible_solution(double **matrix, int row_length, int column_length, 
				int *quantity_of_vars, GtkWidget** varName_list, int column_choose,
				Variable *variable_list, int variables_quantity)
{
	begin_frame("Soluci\\'on: NO FACTIBLE");
	fprintf(file, "Un problema tiene una soluci\\'on no factible cuando al menos una de las variables artificiales "
					"queda en la base y no existen m\\'as columnas donde pivotear.\\newline\\newline\n"
					"En este problema en la "
					"columna '%d', se encuentra la variable artificial, se adjunta la tabla:\\newline\\newline\n", column_choose + 1);
	 write_table_with_color(matrix, row_length, column_length, quantity_of_vars, varName_list, column_choose, -1, "red", "Soluci\\'on: NO FACTIBLE", variable_list, variables_quantity);
	 end_frame();
}


void finishPresentation(void)
{
	fprintf(file, "\n%s\n", "\\end{document}");
	fclose(file);
	freopen("/dev/tty", "a", stdout);
	
	system("cd ../latex; pdflatex beamer.tex");
	system("evince -s ../latex/beamer.pdf");
}
