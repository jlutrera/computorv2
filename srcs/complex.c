/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 19:06:33 by jutrera-          #+#    #+#             */
/*   Updated: 2025/02/19 19:06:33 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static double module(double *s)
{
	return ft_root(s[0]*s[0] + s[1]*s[1], 2);
}

static double argument(double *s)
{
	if (s[0] == 0)
		return PI / 2;
	return ft_atan(s[1] / s[0]);
}

static double *factors(char *s)
{
	double *f;
	int i;
	int j;
	char *aux;

	f = (double *)calloc(2, sizeof(double));
	if (!f)
		exit(EXIT_FAILURE);
	i = strchr(s, 'i') - s;
	if (i >= 0)
	{
		j = i;
		while (j > 0 && s[j] != '+' && s[j] != '-')
			--j;
		aux = ft_substr(s, j, i);
		if (!strcmp(aux, "+") || j == i)
			f[1] = 1.0;
		else if (!strcmp(aux, "-"))
			f[1] = -1.0;
		else
			f[1] = atof(aux);
		free(aux);
		if (j == 0 && i == (int)(strlen(s) - 1))
			f[0] = 0.0;
		else
		{
			if (j == 0)
			{
				j = ++i;
				i = strlen(s);
			}
			else
			{
				i = j;
				j = 0;
			}
			aux = ft_substr(s, j, i);
		if (!strcmp(aux, "+") || j == i - 1)
			f[0] = 1.0;
		else if (!strcmp(aux, "-"))
			f[0] = -1.0;
		else
			f[0] = atof(aux);
		free(aux);
		}
	}
	else
	{
		f[1] = 0;
		f[0] = atof(s);
	}
	return f;
}

static char *get_exponent(char *s)
{
	int i = strchr(s, '^') - s + 1;
	if (s[i] == '(')
		++i;
	int j = strlen(s) - 1;
	if (s[j] == ')')
		--j;
	return ft_substr(s, i, j+1);
}

static char *get_base(char *s)
{
	int i = strchr(s, '^') - s - 1;
	if (s[i] == ')')
		--i;
	int j = 0;
	if (s[j] == '(')
		++j;
	return ft_substr(s, j, i+1);
}

// static void	complex_exponent(char *s)
// {

// }

// static void	decimal_exponent(char *s)
// {

// }

void	complex_power(char **str)
{
	char *s = (char *)calloc(strlen(*str)+1, sizeof(char));
	if (!s)
		exit(EXIT_FAILURE);
	strcpy(s, *str);
	char *b = get_base(s);
	char *e = get_exponent(s);
	double *base;
	double *expo;
	base = factors(b);
	expo = factors(e);
	printf(" base = %s\n", b);
	printf("   %.2f + i * %.2f\n", base[0], base[1]);
	printf(" exponente = %s\n", e);
	printf("   %.2f + i * %.2f\n", expo[0], expo[1]);
	double m = module(base);
	double a = argument(base);
	printf("Forma polar de la base : %.2f * exp( i * %.2f)\n", m, a);
	free(base);
	free(expo);
	free(b);
	free(e);
	free(s);
}
