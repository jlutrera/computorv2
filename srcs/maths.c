/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:30:33 by jutrera-          #+#    #+#             */
/*   Updated: 2024/07/30 14:30:33 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

double ft_mod(double a, double b)
{
	int mod;
	
	mod = (int)a % (int)b;
	return (double)mod;
}

double ft_power(double base, int exponent)
{
	double	result;
	double	newexp;

	result = 1.0;
	newexp = ft_abs(exponent);
	for (int i = 0; i < newexp; i++) {
		result *= base;
	}
	if (exponent < 0)
		return (1/result);
	return result;
}

double ft_powerfloat(double base, double exponent)
{
	double logbase = ft_ln(base);
	double result = ft_exp(exponent * logbase);
	return result;
}

double	ft_root(double a, int n)
{
	double	x;
	double	diference;
	double	xBefore;
	double	xPower;

	if (a == 0)
		return 0;

	x = a;
	do {
		xBefore = x;
		xPower = ft_power(x, n - 1);
		x = (1.0 / n) * ((n - 1) * x + a / xPower);
		diference = x > xBefore ? x - xBefore : xBefore - x;
	} while (diference > PRECISION);

	return x;
}

double ft_abs(double x)
{
	return x < 0 ? -x : x;
}

double	ft_factorial(int n)
{
	double result;

	result = 1.0;
	for (int i = 2; i <= n; i++)
		result *= i;
	
	return result;
}

double	ft_sin(double x)
{
	double	term;
	double	sum;

	if (x > 2*PI)
		while (x > 2*PI)
			x -= 2*PI;
	if (x < -2*PI)
		while (x < -2*PI)
			x += 2*PI;

	sum = 0;
	for (int n=0; n < TAYLOR_TERMS; n++)
	{
		term = ft_power(-1, n) * ft_power(x, 2*n + 1) / ft_factorial(2*n + 1);
		sum += term;
	}
	return sum;
}

double	ft_cos(double x)
{
	double	term;
	double	sum;

	if (x > 2*PI)
		while (x > 2*PI)
			x -= 2*PI;
	if (x < -2*PI)
		while (x < -2*PI)
			x += 2*PI;

	sum = 0;
	for (int n=0; n < TAYLOR_TERMS; n++)
	{
		term = ft_power(-1, n) * ft_power(x, 2*n) / ft_factorial(2*n);
		sum += term;
	}
	return sum;
}

double ft_tan(double x)
{	
	return ft_sin(x) / ft_cos(x);
}

double ft_acos(double x)
{
	return PI / 2 - ft_asin(x);
}

double ft_asin(double x)
{
	if (x < -1.0 || x > 1.0)
		return 0.0;

	if (x > 0.95 || x < -0.95)
	{
		double y = PI / 2 - ft_asin(ft_root(1 - x * x, 2));
		if (x < 0)
			y = -y;
		return y;
	}

	double term = x;
	double sum = term;

	for (int n = 1; n < TAYLOR_TERMS; n++)
	{
		term *= (ft_power(2*n-1, 2) * x * x) / (2*n * (2*n + 1));
		sum += term;
	}
	return sum;
}

double ft_atan(double x)
{
    if (x > 1)
        return PI / 2 - ft_atan(1 / x);
    if (x < -1)
        return -PI / 2 - ft_atan(1 / x);

    double term;
    double sum = 0.0;

    for (int n = 0; n < TAYLOR_TERMS; n++)
    {
        term = ft_power(-1, n) * ft_power(x, 2 * n + 1) / (2 * n + 1);
        sum += term;
    }
    return sum;
}

double	ft_ln(double x)
{
	if (x <= 0)
		return printf_error("ln(x) no está definido para x ≤ 0", NULL, 0);
	double resultado = 0.0;

	// Reduce x a un rango cercano a 1 usando ln(x) = ln(x/2) + ln(2)
	while (x > 2)
	{
		x /= 2;
		resultado += LN2;
	}
	while (x < 0.5)
	{
		x *= 2;
		resultado -= LN2;
	}

	// Aproximación de ln(x) para 0.5 < x ≤ 2 usando la serie de Taylor en x = 1
	double y = (x - 1) / (x + 1);  
	double y2 = y * y;
	double term = y;
	double sum = 0.0;
	int n;

	for (n = 1; n <= TAYLOR_TERMS; n += 2)
	{
		sum += term / n;
		term *= y2;
	}
	resultado += 2 * sum;
	return resultado;
}

double ft_log(double x)
{
	double ln_x;

	ln_x = ft_ln(x);
	return ln_x / LN10;
}

double ft_exp(double x)
{
    double result = 1.0;
    double termino = 1.0;
    int n = 1;

    while (termino > PRECISION || termino < -PRECISION)
	{
        termino *= x / n;
        result += termino;
        n++;
    }

    return result;
}

double ft_deg(double rad)
{
	return rad * 180 / 3.14;
}

double ft_rad(double deg)
{
	return deg * PI / 180;
}