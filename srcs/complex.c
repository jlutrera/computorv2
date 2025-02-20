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

static double *get_exponent(char *s)
{
	int i = strchr(s, '^') - s + 1;
	if (s[i] == '(')
		++i;
	int j = strlen(s) - 1;
	if (s[j] == ')')
		--j;
	char *aux = ft_substr(s, i, j+1);
	double *e = factors(aux);
	free(aux);
	return e;
}

static double *get_base(char *s)
{
	int i = strchr(s, '^') - s - 1;
	if (s[i] == ')')
		--i;
	int j = 0;
	if (s[j] == '(')
		++j;
	char *aux = ft_substr(s, j, i+1);
	double *b = factors(aux);
	free(aux);
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
	double realpart = ft_powerfloat(r, c) * ft_cos(c * t) / denom;
	double imagpart = ft_powerfloat(r, c) * ft_sin(c * t) / denom;

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
	char *complex1;
	char *complex2;

	base = get_base(*str);
	expo = get_exponent(*str);
	r = module(base);
	t = argument(base);
	complex1 = real_part(expo[0], r, t);
	complex2 = complex_part(expo[1], r, t);

	printf("base     =   %s%.2f%s + %s%.2f%s*i\n", YELLOW, base[0], RESET, YELLOW, base[1], RESET);
	printf("exponente =  %s%.2f%s + %s%.2f%s*i\n", YELLOW, expo[0], RESET, YELLOW, expo[1], RESET);
	free(base);
	free(expo);

	char *aux = (char *)calloc(100, sizeof(char));
	if (!aux)
		exit(EXIT_FAILURE);
	strcat(aux, complex1);
	strcat(aux, "*");
	strcat(aux, complex2);
	free(complex1);
	free(complex2);
	free(*str);
	*str = aux;
	printf("Resultado = %s\n", *str);
}
