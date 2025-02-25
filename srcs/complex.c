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
	{
		if (s[1] < 0)
			return (-PI / 2);
		return (PI / 2);
	}
	double t = ft_atan(s[1] / s[0]);
	if (s[0] < 0)
	{
		if (s[1] >= 0)
			return (t + PI);
		return (t - PI);
	}
	return (t);
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

static double *get_exponent(char *s, int *end)
{
	int brackets = 0;
	int i = strchr(s, '^') - s + 1;
	if (s[i] == '(')
	{
		brackets = 1;
		++i;
	}
	int j = i;
	while (s[j] != '\0' && (isdigit(s[j]) || s[j] == '.' || s[j] == 'i' || (brackets && strchr("()+-*/%!", s[j]))))
	{
		j++;
		if (s[j] == '(')
			++brackets;
		else if (s[j] == ')')
			--brackets;

	}
	char *aux = ft_substr(s, i, j);
	double *e = factors(aux);
	printf("aux exponente = %s\n", aux);
	free(aux);
	*end = j + brackets;
	printf("expo = %.2f + %.2fi\n", e[0], e[1]);
	return e;
}

static double *get_base(char *s, int *start)
{
	int brackets = 0;
	int i = strchr(s, '^') - s - 1;
	if (s[i] == ')')
	{
		brackets = 1;
		--i;
	}
	int j = i;
	while (j > 0 && (isdigit(s[j]) || s[j] == '.' || s[j] == 'i' || (brackets && strchr("()+-*/%!", s[j]))))
	{
		--j;
		if (s[j] == '(')
			--brackets;
		else if (s[j] == ')')
			++brackets;	

	}
	if (s[j] == '(')
		++j;
	char *aux = ft_substr(s, j, i+1);
	printf("aux base = %s\n", aux);
	double *b = factors(aux);
	free(aux);
	*start = j - brackets;
	printf("base = %.2f + %.2fi\n", b[0], b[1]);
	return b;
}

static char *real_part(double c, double r, double t)
{
	double denom = 1.0;
	if (c < 0)
	{
		c *= -1;
		denom = ft_powerfloat(r*r, c);
		t *= -1;
	}
	double f1 = ft_powerfloat(r, c) / denom;
	double f2 = c * t;
	double realpart = f1 * ft_cos(f2);
	double imagpart = f1 * ft_sin(f2);

	char *result = (char *)calloc(100, sizeof(char));
	if (!result)
		exit(EXIT_FAILURE);

	strcat(result, "(");
	if (realpart != 0)
	{
		char *real_string = doubletostr(realpart);
		strcat(result, real_string);
		free(real_string);
	}
	if  (imagpart != 0)
	{
		char *img_string = doubletostr(imagpart);
		if (imagpart > 0 || imagpart == 1)
			strcat(result, "+");
		if (imagpart == -1.0)
			strcat(result, "-");
		if (imagpart != 1.0 && imagpart != -1.0)
			strcat(result, img_string);
		strcat(result, "i");
		free(img_string);
	}
	strcat(result, ")");

	return result;
}

static char *complex_part(double d, double r, double t)
{
	double f1 = ft_exp(-1 * d * t);
	double f2 = d * ft_ln(r);

	double realpart = f1 * ft_cos(f2);
	double imgpart = f1 * ft_sin(f2);

	char *result = (char *)calloc(100, sizeof(char));
	if (!result)
		exit(EXIT_FAILURE);

	strcat(result, "(");
	if (realpart != 0)
	{
		char *real_string = doubletostr(realpart);
		strcat(result, real_string);
		free(real_string);
	}
	if  (imgpart != 0)
	{
		char *img_string = doubletostr(imgpart);
		if (imgpart > 0 || imgpart == 1)
			strcat(result, "+");
		if (imgpart == -1.0)
			strcat(result, "-");
		if (imgpart != 1.0 && imgpart != -1.0)
			strcat(result, img_string);
		strcat(result, "i");
		free(img_string);
	}
	strcat(result, ")");

	return result;
}

void	complex_power(char **str)
{
	double	*base;
	double	*expo;
	double	r;
	double	t;
	char 	*complex1;
	char 	*complex2;
	int		start = 0;
	int		end = 0;

	base = get_base(*str, &start);
	expo = get_exponent(*str, &end);
	printf("base = (%.2f, %.2f)    exponente  = (%.2f, %.2f)\n", base[0], base[1], expo[0], expo[1]);
	r = module(base);
	t = argument(base);
	complex1 = real_part(expo[0], r, t);
	complex2 = complex_part(expo[1], r, t);
	free(base);
	free(expo);
	char *aux = (char *)calloc(100, sizeof(char));
	if (!aux)
		exit(EXIT_FAILURE);
	strncat(aux, *str, start);
	// if (start > 0)	printf(" start = %d  str[star-1] = %c\n", start, (*str)[start-1]);
	if (start > 0 && (*str)[start-1] != '-' && (*str)[start-1] != '+' && (*str)[start-1] != '*')
		strcat(aux, "+");
	strcat(aux, complex1);
	strcat(aux, "*");
	strcat(aux, complex2);
	strcat(aux, *str + end);
	// printf("Resultado de aux = %s\n", aux);
	free(complex1);
	free(complex2);
	free(*str);
	*str = aux;
	if (strchr(*str, '^'))
		complex_power(str);
}
