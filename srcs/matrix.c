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

#define N 3  // Define el tamaño máximo de la matriz

// Función para intercambiar dos filas
void swap_rows(double matrix[N][N], int row1, int row2)
{
	double temp;

	for (int i = 0; i < N; i++)
	{
		temp = matrix[row1][i];
		matrix[row1][i] = matrix[row2][i];
		matrix[row2][i] = temp;
	}
}

// Función para calcular el determinante de una matriz usando eliminación de Gauss
double determinant(double matrix[N][N])
{
	double	det;  // Inicializa el determinante
	int		max_index;
	double	factor;

	// Proceso de eliminación de Gauss
	det = 1;
	for (int k = 0; k < N; k++)
	{
		// Pivoteo parcial
		max_index = k;
		for (int i = k + 1; i < N; i++)
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
		for (int i = k + 1; i < N; i++)
		{
			factor = matrix[i][k] / matrix[k][k];
			for (int j = k; j < N; j++)
				matrix[i][j] -= factor * matrix[k][j];
		}

		// Multiplica el determinante por el pivote
		det *= matrix[k][k];
	}

	return det;
}
