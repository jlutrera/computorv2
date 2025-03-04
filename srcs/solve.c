/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:07:59 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/02 11:07:59 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static int	adjust_degree(double *reduced, int degree)
{
	int	i;

	i = degree;
	while (i > 0 && reduced[i] == 0)
		i--;
	return i;
}

static int calc_degree(char *str)
{
	int 	i;
	int 	degree;
	int		tmp; 

	i = 0;
	degree = 0;
	while (str[i] != '\0')
	{
		if (isalpha(str[i]) && degree == 0)
			degree = 1;
		else if (str[i] == '^')
		{
			i++;
			tmp = 0;
			while (isdigit(str[i]))
			{
				tmp *= 10;
				tmp += str[i] - '0';
				i++;
			}
			if (tmp > degree)
				degree = tmp;
		}
		if (str[i] != '\0')
			i++;
	}
	return degree;
}

static char	*get_equation(char *content)
{
	int	i;
	int	len;

	i = 5;
	while (content[i] == ' ')
		++i;
	if (content[i] != '\"')
		return NULL;

	len = strlen(content);
	while (content[++i] == ' ');
	if (i == len - 1)
		return NULL;
	return ft_substr(content, i, len - 1);
}

static double	*reduce_equation(char *str, int degree)
{
	double	*tmp;
	double	n;
	double 	coefficient;
	int 	deg;
	int 	minus;
	int 	decimal;
	int		i;
	int		sign;
	
	tmp = (double *)calloc(degree + 1, sizeof(double));
	if (!tmp)
		exit(EXIT_FAILURE);

	i = 0;
	sign = 1;
	coefficient = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
		{
			sign = -1;
			i++;
		}
		else if (isalpha(str[i]))
		{
			if ( i > 0 && str[i-1] == '-')
				coefficient = -1;
			if (str[i+1] == '^')
			{
				i += 2;
				deg = 0;
				while (isdigit(str[i]) && str[i] != '\0')
				{
					deg *= 10;
					deg += str[i] - '0';
					i++;
				}
			}
			else
			{
				deg = 1;
				i++;
			}
			tmp[deg] += coefficient * sign;
			coefficient = 1;
		}
		else if (isdigit(str[i]))
		{
			if (i > 0 && str[i-1] == '-')
				minus = -1;
			else
				minus = 1;
			coefficient = 0;
			while (isdigit(str[i]) && str[i] != '\0')
			{
				coefficient *= 10;
				coefficient += str[i] - '0';
				i++;
			}
			if (str[i] == '.')
			{
				i++;
				decimal = 10.0;
				
				while (isdigit(str[i]) && str[i] != '\0')
				{
					n = str[i] - '0';
					coefficient +=  n / decimal;
					decimal *= 10;
					i++;
				}
				if (str[i] == '.')
				{
					free(tmp);
					printf_error("Bad format in decimal number", NULL, -1);
					return NULL;
				}
			}
			coefficient *= minus;
			if (str[i] == '*')
				i++;	
			if (!isalpha(str[i]))
			{
				tmp[0] += coefficient * sign;
				coefficient = 1;
			}
		}
		else
			i++;
	}
	return tmp;
}

static bool	resolve(char *e)
{
	int		degree;
	double	*reduced;
	char	letter;

	if (bad_digits(e) || bad_letters(e)) //Check the numbers within spaces and the same for letters
	{
		free(e);
		return 1;
	}

	remove_spaces(e);//Removing blank spaces
	if (syntax_error_equation(e))//Checking syntax
	{
		free(e);
		return 1;
	}
	
	degree = calc_degree(e);//Calculating degree in a first step
	reduced = reduce_equation(e, degree);//Reducing the polynomial
	if (reduced == NULL)
	{
		free(e);
		return 1;
	}
	degree = adjust_degree(reduced, degree);//Recalculating the precise degree
	letter = get_letter(e);
	
	switch (degree)
	{
		case 0:
			solve_equality(reduced, letter);
			break;
		case 1:
			solve_linear(reduced, letter);
			break;
		case 2:
			solve_quadratic(reduced, letter);
			break;
		case 3:
			solve_cubic(reduced, letter);
			break;
		default:
			printf_error("I can't solve equations with a degree greater than 3", NULL, -1);
			break;
	}

	printf("\n");
	free(reduced);
	free(e);
	return 0;
}

int	solve(char *input)
{
	char	*k;
	char	*equation;
	int		len;

	len = strlen(input);
	if (len < 5)
		return 2;
	
	k = ft_substr(input, 0, 5);
	if (strcmp(k, "solve") || input[5] != ' ')
	{
		free(k);
		return 2;
	}
	if (len < 7)
	{
		free(k);
		return printf_error("Equation not found", NULL, -1);
	}
	if (!strchr(input, '\"'))
	{
		free(k);
		return printf_error("Double quotes not found at the beginning.", NULL, -1);
	}
	if (input[len - 1] != '\"')
	{
		free(k);
		return printf_error("Double quotes not found at the end.", NULL, -1);
	}
	equation = get_equation(input);
	free(k);
	if (equation != NULL)
		return resolve(equation);
	
	free(equation);
	return printf_error("Bad syntax for \"solve\"", NULL, -1);
}
