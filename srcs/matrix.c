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

static double **creatematrixdouble(int r)
{
	double **result;

	result = (double **)malloc(r * sizeof(double *));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (double *)malloc(r * sizeof(double));
		if (!result[i])
			exit(EXIT_FAILURE);
	}
	return result;
}

static void freematrixdouble(double **matrix, int r)
{
	if (matrix == NULL)
		return;
	for (int i = 0; i < r; i++)
		free(matrix[i]);
	free(matrix);
}

// Función para intercambiar dos filas
static void	swap_rows(double **matrix, int c, int row1, int row2)
{
	double temp;

	for (int i = 0; i < c; i++)
	{
		temp = matrix[row1][i];
		matrix[row1][i] = matrix[row2][i];
		matrix[row2][i] = temp;
	}
}

// Función para calcular el determinante de una matriz usando eliminación de Gauss
static double	calcdeterminant(double **m, int l)
{
	double	det;  // Inicializa el determinante
	int		max_index;
	double	factor;
	double	**matrix;

	matrix = creatematrixdouble(l);
	for (int i = 0; i < l; ++i)
		for (int j = 0; j < l; ++j)
			matrix[i][j] = m[i][j];

	// Proceso de eliminación de Gauss
	det = 1;
	for (int k = 0; k < l; k++)
	{
		// Pivoteo parcial
		max_index = k;
		for (int i = k + 1; i < l; i++)
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
			swap_rows(matrix, l, k, max_index);
			det *= -1;  // Cambia el signo del determinante
		}
		// Eliminación de los elementos debajo del pivote
		for (int i = k + 1; i < l; i++)
		{
			factor = matrix[i][k] / matrix[k][k];
			for (int j = k; j < l; j++)
			{
				double a = matrix[i][j];
				double b = matrix[k][j];
				matrix[i][j] = a - factor * b;
			}
		}
		// Multiplica el determinante por el pivote
		det *= matrix[k][k];
	}

	freematrixdouble(matrix, l);
	return det;
}

static char ***addmatrix(char ***m, char ***m2, int r, int c)
{
	char 	***result;
	double	add;

	result = (char ***)malloc(r * sizeof(char **));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (char **)malloc(c * sizeof(char *));
		if (!result[i])
			exit(EXIT_FAILURE);
		for (int j = 0; j < c; j++)
		{
			add = strtod(m[i][j], NULL) + strtod(m2[i][j], NULL);
			result[i][j] = doubletostr(add);
		}
	}
	return result;
}

static char ***submatrix(char ***m, char ***m2, int r, int c)
{
	char 	***result;
	double	subst;

	result = (char ***)malloc(r * sizeof(char **));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (char **)malloc(c * sizeof(char *));
		if (!result[i])
			exit(EXIT_FAILURE);
		for (int j = 0; j < c; j++)
		{
			subst = strtod(m[i][j], NULL) - strtod(m2[i][j], NULL);
			result[i][j] = doubletostr(subst);
		}
	}
	return result;
}

static char ***multmatrix(char ***m, char ***m2, int r, int c, int c2)
{
	char 	***result;
	double	item;

	result = (char ***)malloc(r * sizeof(char **));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (char **)malloc(c2 * sizeof(char *));
		if (!result[i])
			exit(EXIT_FAILURE);
		for (int j = 0; j < c2; j++)
		{
			item = 0;
			for (int k = 0; k < c; k++)
				item += strtod(m[i][k], NULL) * strtod(m2[k][j], NULL);
			result[i][j] = doubletostr(item);
		}
	}
	return result;
}

static char *scalarproduct(char ***m, char ***m2, int c)
{
	char *result;
	double sum = 0;

	for (int j = 0; j < c; j++)
		sum += strtod(m[0][j], NULL) * strtod(m2[0][j], NULL);
	
	result = doubletostr(sum);
	return result;
}

static char ***multescalarmatrix(char ***m, int r, int c, double digit)
{
	char 	***result;
	double	product;

	result = (char ***)malloc(r * sizeof(char **));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (char **)malloc(c * sizeof(char *));
		if (!result[i])
			exit(EXIT_FAILURE);
		for (int j = 0; j < c; j++)
		{
			product = strtod(m[i][j], NULL) * digit;
			result[i][j] = doubletostr(product);
		}
	}
	return result;
}

static char ***divescalarmatrix(char ***m, int r, int c, double digit)
{
	char	***result;
	double	division;

	result = (char ***)malloc(r * sizeof(char **));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (char **)malloc(c * sizeof(char *));
		if (!result[i])
			exit(EXIT_FAILURE);
		for (int j = 0; j < c; j++)
		{
			division = strtod(m[i][j], NULL) / digit;
			result[i][j] = doubletostr(division);
		}
	}
	return result;
}

static char ***invertmatrix(char ***m, int r)
{
	char 	***result;
	double 	**temp;
	double 	**cofactor;
	double 	**inverse;
	double	**aux;
	double 	determinant;
	double	cof_determ;

	temp = creatematrixdouble(r);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < r; j++)
			temp[i][j] = strtod(m[i][j], NULL);

	determinant = calcdeterminant(temp, r);
	if (determinant == 0)
		return NULL;

	cofactor = creatematrixdouble(r);
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < r; j++)
		{
			aux = creatematrixdouble(r - 1);
			for (int k = 0; k < r; k++)
				if (k != i)
					for (int l = 0; l < r; l++)
						if (l != j)
							aux[k < i ? k : k - 1][l < j ? l : l - 1] = temp[k][l];
			cof_determ = calcdeterminant(aux, r - 1);
			cofactor[i][j] = (i + j) % 2 == 0 ? cof_determ : -cof_determ;
			freematrixdouble(aux, r - 1);
		}
	}

	inverse = creatematrixdouble(r);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < r; j++)
			inverse[i][j] = cofactor[j][i] / determinant;

	result = (char ***)malloc(r * sizeof(char **));
	if (!result)
		exit(EXIT_FAILURE);
	for (int i = 0; i < r; i++)
	{
		result[i] = (char **)malloc(r * sizeof(char *));
		if (!result[i])
			exit(EXIT_FAILURE);
		for (int j = 0; j < r; j++)
			result[i][j] = doubletostr(inverse[i][j]);
	}

	freematrixdouble(cofactor, r);
	freematrixdouble(temp, r);
	freematrixdouble(inverse, r);
	return result;
}

static void print_spaces(int n)
{
	for (int i = 0; i < n; i++)
		printf(" ");
}
static void	print_matrix(char ***matrix, int r, int c)
{
	size_t maxlen[c];
	
	if (r == 1 && c == 1)
	{
		printf("%s\n", matrix[0][0]);
		return;
	}

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

static bool checkdigitbefore(char *str, int i)
{
	int j;

	if (str[--i] == ')')
		--i;
	j = i;
	while (j >= 0 && (isdigit(str[j]) || str[j] == '.'))
		--j;
	return (i != j);
}

static bool checkdigitafter(char *str, int i)
{
	int j;

	if (str[++i] == '(')
		++i;
	if (str[i] == '+' || str[i] == '-')
		++i;
	j = i;
	while (str[j] && (isdigit(str[j]) || str[j] == '.'))
		++j;
	return (i != j);
}

static char *readdigitbefore(char *str, int i, int *j)
{
	char	*digit;

	if (str[--i] == ')')
		--i;	
	*j = i;
	while (*j >= 0 && (isdigit(str[*j]) || str[*j] == '.'))
		--(*j);
		
	if (*j > 1 && (str[*j] == '+' || str[*j] == '-') && str[*j - 1] == '(')
		*j -= 1;
	else if (*j != 0)
		++(*j); 
	digit = ft_substr(str, *j, i + 1);
	return digit;
}

static char *readdigitafter(char *str, int i, int *k)
{
	char	*digit;

	if (str[i + 1] == '(')
		++i;
	*k = i + 1;
	if (str[*k] == '+' || str[*k] == '-')
		++(*k);
	while (str[*k] && (isdigit(str[*k]) || str[*k] == '.'))
		++(*k);
	digit = ft_substr(str, i, *k);
	return digit;
}

static char *matrixtostr(char ***result, int r, int c)
{
	int 	length;
	char	*aux;

	length = 0;
	for (int i = 0; i < r; ++i)
	{
		++length;
		for (int j = 0; j < c; ++j)
		{
			length += strlen(result[i][j]);
			++length;
		}
		++length;
	}
	++length;

	aux = (char *)calloc(length + 1, sizeof(char));
	if (!aux)
		exit(EXIT_FAILURE);

	aux[0] = '[';
	for (int i = 0; i < r; ++i)
	{
		strcat(aux, "[");
		for(int j = 0; j < c; ++j)
		{
			strcat(aux, result[i][j]);
			if (j < c - 1)
				strcat(aux, ",");
			else
				strcat(aux,"]");
		}
		if (i < r - 1)
			strcat(aux, ";");
		else
			strcat(aux, "]");
	}
	free_matrix(result, r, c);
	return aux;
}

static int	findoperation(int c, char *s)
{
	char *pos;
	char *pos2;

	switch (c)
	{
		case 0:
			pos = strchr(s, '/');
			break;
		case 1:
			pos = strchr(s, '^');
			break;
		case 2:
			pos = strchr(s, '*');
			pos2 = strchr(s, '#');
			if (!pos || (pos2 && pos2 < pos))
				pos = pos2;
			break;
		case 3:
			pos = strchr(s + 1, '+');
			if (pos && (s[pos - s - 1] == '[' || s[pos - s - 1] == ','))
				pos = NULL;
			pos2 = strchr(s + 1, '-');
			if (pos2 && (s[pos2 - s - 1] == '[' || s[pos2 - s - 1] == ','))
				pos2 = NULL;
			if (!pos || (pos2 && pos2 < pos))
				pos = pos2;
			break;
	}

	if (!pos)
		return (-1);
	return (pos - s);
}

static char *matrixcalc(char *str, int *error)
{
	int		i;
	int		j;
	int		b;
	int		r, r2;
	int 	c, c2;
	char	*digit;
	char	*aux;
	char	***matrix;
	char	***matrix2 = NULL;
	char	***result = NULL;

	i = strchr(str, '[') - str;
	j = i;

	while (str[j])
	{
		if (str[j] == ']' && str[j - 1] == ']')
			break;
		++j;		
	}
	r = fixRows(str+i);
	c = fixColumns(str+i);
	matrix = create_matrix(str+i, r, c);

	if (i > 0)
	{
		if (str[i - 2] == ']')
		{
			r2 = fixRows(str);
			c2 = fixColumns(str);
			matrix2 = create_matrix(str, r2, c2);
		}

		if (str[i - 1] == '*')
		{
			if (checkdigitbefore(str, i - 1))
			{
				digit = readdigitbefore(str, i - 1, &j);
				result = multescalarmatrix(matrix, r, c, strtod(digit, NULL));
				aux = matrixtostr(result, r, c);
				free(digit);
				free_matrix(matrix, r, c);
				return aux;
			}
			if (matrix2 && r == 1 && r2 == 1 && c == c2)
			{
				aux = scalarproduct(matrix2, matrix, c);
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				return aux;
			}
			free_matrix(matrix, r, c);
			*error = 1;
			return NULL;
		}

		if (str[i - 1] == '/')
		{
			if (r != c)
			{
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				*error = 2;
				return NULL;
			}
			
			char ***inverse;
			inverse = invertmatrix(matrix, r);
			if (!inverse)
			{
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				*error = 3;
				return NULL;
			}
			
			printf("Inversa:\n"); print_matrix(inverse, r, c);
	
			if (checkdigitbefore(str, i - 1))
			{
				digit = readdigitbefore(str, i - 1, &j);
				char ***result;
				result = multescalarmatrix(inverse, r, c, strtod(digit, NULL));
				aux = matrixtostr(result, r, c);
				free(digit);
				free_matrix(matrix, r, c);
				free_matrix(inverse, r, c);
				return aux;
			}
			result = multmatrix(matrix2, inverse, r2, c2, c);
			aux = matrixtostr(result, r, c);
			free_matrix(inverse, r, c);
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			return aux;
		}

		if (str[i - 1] == '#')
		{
			if (matrix2 && c2 == r)
			{
				result = multmatrix(matrix2, matrix, r2, c2, c);
				aux = matrixtostr(result, r2, c);
				free_matrix(matrix2, r2, c2);
				free_matrix(matrix, r, c);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}
		if (str[i - 1] == '+')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				result = addmatrix(matrix, matrix2, r, c);
				aux = matrixtostr(result, r, c);
				free_matrix(matrix2, r2, c2);
				free_matrix(matrix, r, c);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}
		if (str[i - 1] == '-')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				result = submatrix(matrix2, matrix, r, c);
				aux = matrixtostr(result, r, c);
				free_matrix(matrix2, r2, c2);
				free_matrix(matrix, r, c);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}
	}
	else
	{
	 	b  = 1;
		j = i + 1;
		while (str[j] && b != 0)
		{
			b = b + (str[j] == '[') - (str[j] == ']');
			++j;
		}

		if (str[j] && str[j + 1] == '[')
		{
			r2 = fixRows(str + j + 1);
			c2 = fixColumns(str + j + 1);
			matrix2 = create_matrix(str + j + 1, r2, c2);
		}

		if (str[j] == '*')
		{
			if (checkdigitafter(str, j))
			{
				digit = readdigitafter(str, j + 1, &i);
				result = multescalarmatrix(matrix, r, c, strtod(digit, NULL));
				aux = matrixtostr(result, r, c);
				free(digit);
				free_matrix(matrix, r, c);
				return aux;
			}
			if (matrix2 && r == 1 && r2 == 1)
			{
				aux = scalarproduct(matrix, matrix2, c);
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}
		if (str[j] == '/')
		{
			if (checkdigitafter(str, j))
			{
				digit = readdigitafter(str, j + 1, &i);
				result = divescalarmatrix(matrix, r, c, strtod(digit, NULL));
				aux = matrixtostr(result, r, c);
				free(digit);
				free_matrix(matrix, r, c);
				return aux;
			}
			
			if (r2 != c2)
			{
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				*error = 2;
				return NULL;
			}
			char ***inverse;
			inverse = invertmatrix(matrix2, r2);
			if (!inverse)
			{
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				*error = 3;
				return NULL;
			}
			result = multmatrix(matrix, inverse, r, c, r2);
			aux = matrixtostr(result, r2, c2);
			free_matrix(inverse, r2, c2);
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			return aux;
		}

		if (str[j] == '#')
		{
			if (matrix2 && c == r2)
			{
				char ***result;
				result = multmatrix(matrix, matrix2, r, c, c2);
				aux = matrixtostr(result, r, c2);
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}

		if (str[j] == '+')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				char ***result;
				result = addmatrix(matrix, matrix2, r, c);
				aux = matrixtostr(result, r, c);
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}
		
		if (str[j] == '-')
		{
			if (matrix2 && r == r2 && c == c2)
			{
				char ***result;
				result = submatrix(matrix, matrix2, r, c);
				aux = matrixtostr(result, r, c);
				free_matrix(matrix, r, c);
				free_matrix(matrix2, r2, c2);
				return aux;
			}
			free_matrix(matrix, r, c);
			free_matrix(matrix2, r2, c2);
			*error = 1;
			return NULL;
		}
	}
	free_matrix(matrix, r, c);
	free_matrix(matrix2, r2, c2);
	*error = 1;
	return NULL;
}

int calc_with_matrices(char **str, int mode)
{
	int		error = 0;
	int		i;
	int		j;
	int		k;
	char	*result;
	char	*aux;
	char	*newstr;
	int		op;

	op = 0;
	while (op < 4 && strchr(*str, '['))
	{
		i = findoperation(op, *str);
		if ( i == -1)
			++op;
		else
		{
			if ((*str)[i + 1] == '[')
				k = strstr(*str+i, "]]") - *str + 2;
			else
				free(readdigitafter(*str, i, &k));
			if ((*str)[i - 1] == ']')
			{
				j = i;
				while ((*str)[j] != '[' || (*str)[j - 1] != '[')
					--j;
				j -= 1;
			}
			else
				free(readdigitbefore(*str, i, &j));
			aux = ft_substr(*str, j, k);
			result = matrixcalc(aux, &error);
			if (!strchr(result, ','))
			{
				result[0] = ' ';
				result[1] = ' ';
				result[strlen(result) - 1] = ' ';
				result[strlen(result) - 2] = ' ';
				result = ft_trim(result);
			}
			
			if (v_calc) printf("   Result = %s%s%s\n", GREEN, result, RESET);
			if (error)
			{
				free(aux);
				free(result);
				break;
			}
			newstr = (char *)calloc((strlen(*str) - k + j + strlen(result) + 1), sizeof(char));
			if (!newstr)
				exit(EXIT_FAILURE);
			strncpy(newstr, *str, j);
			strcat(newstr, result);
			if (k < (int)strlen(*str))
				strcat(newstr, *str + k);
			free(*str);
			free(aux);
			free(result);
			*str = newstr;
		}
	}
	if (error == 1)
		return printf_error("Operation is not valid", NULL, -1);
	if (error == 2)
		return printf_error("The matrix is not square\n", NULL, -1);
	if (error == 3)
		return printf_error("The matrix cannot be inverted", NULL, -1);
	if (mode && strchr(*str, '['))
	{
		char ***response;
		i = fixRows(*str);
		j = fixColumns(*str);
		response = create_matrix(*str, i, j);
		print_matrix(response, i, j);
		free_matrix(response, i, j);
	}
	return 0;
}