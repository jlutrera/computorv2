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

int R;  // Define el número de filas de la matriz
int C;  // Define el número de columnas de la matriz
/*
// Función para intercambiar dos filas
static void	swap_rows(double matrix[R][C], int row1, int row2)
{
	double temp;

	for (int i = 0; i < C; i++)
	{
		temp = matrix[row1][i];
		matrix[row1][i] = matrix[row2][i];
		matrix[row2][i] = temp;
	}
}

// Función para calcular el determinante de una matriz usando eliminación de Gauss
static double	determinant(double matrix[R][C])
{
	double	det;  // Inicializa el determinante
	int		max_index;
	double	factor;

	// Proceso de eliminación de Gauss
	det = 1;
	for (int k = 0; k < C; k++)
	{
		// Pivoteo parcial
		max_index = k;
		for (int i = k + 1; i < C; i++)
		{
			if (ft_abs(matrix[i][k]) > ft_abs(matrix[max_index][k]))
				max_index = i;
		}

		// Si el pivote es cero, el determinante es cero
		if (matrix[max_index][k] == 0)
			return 0;

		// Intercambio de filas si es necesario
		if (max_index != k) 
		{
			swap_rows(matrix, k, max_index);
			det *= -1;  // Cambia el signo del determinante
		}

		// Eliminación de los elementos debajo del pivote
		for (int i = k + 1; i < R; i++)
		{
			factor = matrix[i][k] / matrix[k][k];
			for (int j = k; j < C; j++)
				matrix[i][j] -= factor * matrix[k][j];
		}

		// Multiplica el determinante por el pivote
		det *= matrix[k][k];
	}

	return det;
}
*/
static void	print_matrix(char *(matrix[R][C]))
{
	for (int rows = 0; rows < R; rows++)
	{
		printf("[");
		for (int columns = 0; columns < C; columns++)
		{
			if (matrix[rows][columns][0] != '-')
				printf(" ");
			printf("%s\t", matrix[rows][columns]);
		}
		printf("]\n");
	}
}

static int	fixRows(char *str)
{
	int i = 0;
	int rows = 1;

	while (str[i])
	{
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

	while (str[i] && str[i] != ';')
	{
		if (str[i] == ',')
			columns++;
		i++;
	}
	return columns;
}

static void	create_matrix(char *str, char *(matrix[R][C]))
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;

	while (str[k])
	{
		while (str[k] == '[')
			++k;
		
		while (str[k] != ']')
		{
			l = k;
			while (str[k] != ',' && str[k] != ']')
				++k;
			matrix[i][j] = ft_substr(str, l, k);
			++j;
			if (j == C)
			{
				++i;
				j = 0;
			}
			if (str[k] == ',')
				++k;
		}
		while (str[k] && (str[k] != '['))
			++k;
	}
}

static void	free_matrix(char *(matrix[R][C]))
{
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
			free(matrix[i][j]);
	}
}

int calc_with_matrices(char **str)
{
	if (v_calc)
	{
		printf("********************\n");
		printf("*     Matrices     *\n");
		printf("********************\n");
	}

	R = fixRows(*str);
	C = fixColumns(*str);
	char *(matrix[R][C]);
	
	if (v_calc)
		printf("Rows: %d\nColumns: %d\n", R, C);

	create_matrix(*str, matrix);
	print_matrix(matrix);

	free_matrix(matrix);
	return 1;
}