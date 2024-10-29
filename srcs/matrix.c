/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:01:19 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/14 20:01:19 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

// Función para intercambiar dos filas
static void	swap_rows(char ***matrix, int c, int row1, int row2)
{
	char *temp;

	for (int i = 0; i < c; i++)
	{
		temp = matrix[row1][i];
		matrix[row1][i] = matrix[row2][i];
		matrix[row2][i] = temp;
	}
}

// Función para calcular el determinante de una matriz usando eliminación de Gauss
static double	determinant(char ***matrix, int r, int c)
{
	double	det;  // Inicializa el determinante
	int		max_index;
	double	factor;

	// Proceso de eliminación de Gauss
	det = 1;
	for (int k = 0; k < c; k++)
	{
		// Pivoteo parcial
		max_index = k;
		for (int i = k + 1; i < c; i++)
		{
			if (ft_abs(strtod(matrix[i][k], NULL)) > ft_abs(strtod(matrix[max_index][k], NULL)))
				max_index = i;
		}

		// Si el pivote es cero, el determinante es cero
		if (strtod(matrix[max_index][k], NULL) == 0)
			return 0;

		// Intercambio de filas si es necesario
		if (max_index != k) 
		{
			swap_rows(matrix, c, k, max_index);
			det *= -1;  // Cambia el signo del determinante
		}

		// Eliminación de los elementos debajo del pivote
		for (int i = k + 1; i < r; i++)
		{
			factor = strtod(matrix[i][k], NULL) / strtod(matrix[k][k], NULL);
			for (int j = k; j < c; j++)
			{
				double a = strtod(matrix[i][j], NULL);
				double b = strtod(matrix[k][j], NULL);
				sprintf(matrix[i][j], "%.6f", a - factor * b);
			}
		}

		// Multiplica el determinante por el pivote
		det *= strtod(matrix[k][k], NULL);
	}

	return det;
}

static double invertible(char ***matrix, int r, int c)
{
	if (r != c)
	{
		printf("La matriz no es cuadrada\n");
		return false;
	}
	return determinant(matrix, r, c) != 0;
}

static void print_spaces(int n)
{
	for (int i = 0; i < n; i++)
		printf(" ");
}
static void	print_matrix(char ***matrix, int r, int c)
{
	size_t maxlen[c];
	
	for (int columns = 0; columns < c; columns++)
		maxlen[columns] = 0;
	
	for (int columns = 0; columns < c; columns++)
		for (int rows = 0; rows < r; rows++)
			if (maxlen[columns] < strlen(matrix[rows][columns]))
				maxlen[columns] = strlen(matrix[rows][columns]);

	printf("\u250C ");
	for (int columns = 0; columns < c-1; columns++)
		print_spaces(maxlen[columns] + 2);
	print_spaces(maxlen[c - 1] + 1);
	printf("\u2510\n");

	for (int rows = 0; rows < r; rows++)
	{
		printf("\u2502 ");
		for (int columns = 0; columns < c-1; columns++)
		{
			print_spaces(maxlen[columns] - strlen(matrix[rows][columns]));
			printf("%s  ", matrix[rows][columns]);
		}
		print_spaces(maxlen[c -1] - strlen(matrix[rows][c - 1]));
		printf("%s", matrix[rows][c - 1]);
		printf(" \u2502\n");
	}

	printf("\u2514 ");
	for (int columns = 0; columns < c-1; columns++)
		print_spaces(maxlen[columns] + 2);
	print_spaces(maxlen[c - 1] + 1);
	printf("\u2518\n");
}

static int	fixRows(char *str)
{
	int i = 0;
	int rows = 1;
	int b = 0;

	while (str[i])
	{
		b = b + (str[i] == '[') - (str[i] == ']');
		if (b == 0)
			break;

		if (str[i] == ';')
			rows++;
		i++;
	}
	return rows;
}

static int	fixColumns(char *str)
{
	int i = 0;
	int columns = 1;

	while (str[i] && str[i] != ']')
	{
		if (str[i] == ',')
			columns++;
		i++;
	}
	return columns;
}

static char	***create_matrix(char *str, int r, int c)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int b = 0;
	char ***m;

	m = (char ***)malloc(r * sizeof(char **));
	if (!m)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		m[i] = (char **)malloc(c * sizeof(char *));
		if (!m[i])
			exit(EXIT_FAILURE);
	}

	while (str[k])
	{
		while (str[k] == '[')
		{
			++b;
			++k;
		}
		while (str[k] != ']')
		{
			l = k;
			while (str[k] != ',' && str[k] != ']')
				++k;
			m[i][j] = ft_substr(str, l, k);
			++j;
			if (j == c)
			{
				++i;
				j = 0;
			}
			if (str[k] == ',')
				++k;
		}
		while (str[k] && (str[k] == ']'))
		{
			--b;
			++k;
		}
		while (str[k] && (str[k] != '['))
			++k;
		if (b == 0)
			break;
	}
	return m;
}

static void	free_matrix(char ***matrix, int r, int c)
{
	if (matrix == NULL)
		return;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
			free(matrix[i][j]);
		free(matrix[i]);
	}
	free(matrix);
}

static char *readdigit(char *str, int i)
{
	int		j;
	char	*digit;

	if (str[i - 2] == ')')
		--i;	
	j = i - 2;
	while (j > 0 && (isdigit(str[j]) || str[j] == '.'))
		--j;
	if (str[j] != '+' && str[j] != '-')
		++j;
	digit = ft_substr(str, j, i - 1);
	return digit;
}

static char *readdigit2(char *str, int i)
{
	int		j;
	char	*digit;

	if (str[i + 1] == '(')
		++i;
	j = i + 1;
	if (str[j] == '+' || str[j] == '-')
		++j;
	while (str[j] && (isdigit(str[j]) || str[j] == '.'))
		++j;
	digit = ft_substr(str, i + 1, j);
	return digit;
}

static bool checkdigit(char *str, int i)
{
	int j;

	if (str[i] == ')')
		--i;
	j = i;
	while (j > 0 && (isdigit(str[j]) || str[j] == '.'))
		--j;
	return (i != j);
}

static bool checkdigit2(char *str, int i)
{
	int j;

	if (str[i] == '(')
		++i;
	if (str[i] == '+' || str[i] == '-')
		++i;
	j = i;
	while (str[j] && (isdigit(str[j]) || str[j] == '.'))
		++j;
	return (i != j);
}

int calc_with_matrices(char **str)
{
	int	i;
	int j;
	int b;
	int r, r2;
	int c, c2;
	char *digit;
	char ***matrix;
	char ***matrix2 = NULL;

	if (v_calc)
	{
		printf("********************\n");
		printf("*     Matrices     *\n");
		printf("********************\n");
	}

	i = strchr(*str, '[') - *str;
	r = fixRows(*str+i);
	c = fixColumns(*str+i);
	matrix = create_matrix(*str+i, r, c);

	if (i > 0)
	{
		if ((*str)[i - 2] == ']')
		{
			r2 = fixRows(*str);
			c2 = fixColumns(*str);
			matrix2 = create_matrix(*str, r2, c2);
		}

		if ((*str)[i - 1] == '*')
		{
			if (checkdigit(*str, i - 2))
			{
				digit = readdigit(*str, i);
				printf("Multiplicación de %s por :\n", digit);
				print_matrix(matrix, r, c);
				free(digit);
			}
			else if (matrix2 && r == 1 && r2 == 1)
			{
				printf("Producto escalar de :\n");
				print_matrix(matrix2, r2, c2);
				printf(" por :\n");
				print_matrix(matrix, r, c);
			}
			else
				printf("Operación no válida\n");
		}
		else if ((*str)[i - 1] == '/')
		{
			if (checkdigit(*str, i - 2))
			{
				digit = readdigit(*str, i);
				printf("División de %s por :\n", digit);
				print_matrix(matrix, r, c);
				free(digit);
			}
			else if (matrix2 && invertible(matrix, r, c))
			{
				printf("División de :\n");
				print_matrix(matrix2, r2, c2);
				printf(" por :\n");
				print_matrix(matrix, r, c);
			}
			else
				printf("Operación no válida\n");
		}
		else if ((*str)[i - 1] == '#')
		{
			if (matrix2 && c2 == r)
			{
				printf("Multiplicación de:\n");
				print_matrix(matrix2, r2, c2);
				printf(" por :\n");
				print_matrix(matrix, r, c);
			}
			else
				printf("Operación no válida\n");;
		}
		else if ((*str)[i - 1] == '+')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				printf("Suma de :\n");
				print_matrix(matrix2, r2, c2);
				printf(" y :\n");
				print_matrix(matrix, r, c);
				
			}
			else
				printf("Operación no válida\n");
		}
		else if ((*str)[i - 1] == '-')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				printf("Resta de :\n");
				print_matrix(matrix2, r2, c2);
				printf(" y :\n");
				print_matrix(matrix, r, c);
			}
			else
				printf("Operación no válida\n");
		}
		else
			print_matrix(matrix, r, c);
	}
	else
	{
	 	b  = 1;
		j = i + 1;
		while ((*str)[j] && b != 0)
		{
			b = b + ((*str)[j] == '[') - ((*str)[j] == ']');
			++j;
		}

		if ((*str)[j] && (*str)[j + 1] == '[')
		{
			r2 = fixRows(*str + j + 1);
			c2 = fixColumns(*str + j + 1);
			matrix2 = create_matrix(*str + j + 1, r2, c2);
		}

		if ((*str)[j] == '*')
		{
			if (checkdigit2(*str, j + 1))
			{
				digit = readdigit2(*str, j);
				printf("Multiplicación de :\n");
				print_matrix(matrix, r, c);
				printf(" por %s\n", digit);
				free(digit);
			}
			else if (matrix2 && r == 1 && r2 == 1)
			{
				printf("Producto escalar de :\n");
				print_matrix(matrix, r, c);
				printf("por :\n");
				print_matrix(matrix2, r2, c2);
			}
			else
				printf("Operación no válida\n");
		}
		else if ((*str)[j] == '/')
		{
			if (checkdigit2(*str, j + 1))
			{
				printf("División de :\n");
				print_matrix(matrix, r, c);
				digit = readdigit2(*str, j);
				printf(" por %s\n", digit);
				free(digit);
			}
			else if (matrix2 && invertible(matrix2, r2, c2))
			{
				printf("División de :\n");
				print_matrix(matrix, r, c);
				printf(" por :\n");
				print_matrix(matrix2, r2, c2);
			}
			else
				printf("Operación no válida\n");
		} 
		else if ((*str)[j] == '#')
		{
			if (matrix2 && c == r2)
			{
				printf("Multiplicación de :\n");
				print_matrix(matrix, r, c);
				printf(" por :\n");
				print_matrix(matrix2, r2, c2);
			}
			else
				printf("Operación no válida\n");
		}
		else if ((*str)[j] == '+')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				printf("Suma de :\n");
				print_matrix(matrix, r, c);
				printf(" y :\n");
				print_matrix(matrix2, r2, c2);
			}
			else
				printf("Operación no válida\n");
		}
		else if ((*str)[j] == '-')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				printf("Resta de :\n");
				print_matrix(matrix, r, c);
				printf(" y :\n");
				print_matrix(matrix2, r2, c2);
			}
			else
				printf("Operación no válida\n");
		}
		else
			print_matrix(matrix, r, c);
	}
	
	free_matrix(matrix, r, c);
	free_matrix(matrix2, r2, c2);
	return 1;
}