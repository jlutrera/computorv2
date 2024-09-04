/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equations.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:30:27 by jutrera-          #+#    #+#             */
/*   Updated: 2024/07/30 14:30:27 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static void	print_number(double d)
{
	int part_int;
	
	part_int = (int)d;
	if (d == 0)
		printf("0");
	else if (d - part_int == 0)
		printf("%0.0f", d);
	else
		printf("%0.2f", d);
}

void	solve_equality(double *reduced, char letter)
{
	if (reduced[0] == 0)
	{
		if (letter)
			printf("   %c \u2208 \u211D ", letter);
		else
			printf("   Every real number satisfies the equation.");
	}
	else
	{
		if (!letter)
			printf("   \u2204 solution");
		else
			printf("   \u2204 %c", letter);
	}
}

void	solve_linear(double *reduced, char letter)
{
	double	solution;

	solution = -reduced[0] / reduced[1];
	
	printf("   %c = ", letter);
	print_number(solution);
}

void	solve_quadratic(double *reduced, char letter)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	part1;
	double	part2;

	a = reduced[2];
	b = reduced[1];
	c = reduced[0];
	part1 = -b / (2 * a);
	discriminant = b * b - 4 * a * c;
	//Studing the three cases
	if (discriminant == 0)
	{
		printf("   %c = ", letter);
		print_number(part1);
		printf(" (double)");
	}
	else if (discriminant > 0)
	{
		part2 = ft_root(discriminant, 2) / (2 * a);
		if (part1 != 0)
		{
			printf("   %c1 = ", letter);
			print_number(part1 - part2);
			printf("\n");
			printf("   %c2 = ", letter);
			print_number(part1 + part2);
		}
		else
		{
			printf("   %c = \u00B1 ", letter);
			print_number(part2);
		}
	}
	else //discriminant < 0
	{
		part2 = ft_root(-discriminant, 2) / (2 * a);
		if (part1 != 0)
		{
			printf("   %c = ", letter);
			print_number(part1);
			printf(" \u00B1 ");
			print_number(part2);
			printf("i");
		}
		else
		{
			printf("   %c = \u00B1 ", letter);
			print_number(part2);
			printf("i");
		}
	}
}

void	solve_cubic(double *reduced, char letter)
{
	double	a;
	double	b;
	double	c;
	double	p;
	double 	q;
	double	u;
	double	v;
	double	sol1;
	double	sol2;
	double	sol3;
	double	angle;
	double	realPart;
	double	imaginaryPart;
	double 	discriminant;

	a = reduced[2] / reduced[3];
	b = reduced[1] / reduced[3];
	c = reduced[0] / reduced[3];

	p = b - a * a / 3;
	q = 2 * a * a * a / 27 - a * b / 3 + c;
	discriminant = q * q / 4 + p * p * p / 27;

	if (discriminant > 0)
	{	
		u = ft_root(-q / 2 + ft_root(discriminant, 2), 3);
		v = ft_root(-q / 2 - ft_root(discriminant, 2), 3);
		sol1 = u + v - a / 3;
	
		printf("   %c1 = ", letter);
		print_number(sol1);
		printf("\n");
	
		realPart = -0.5 * (u + v) - a / 3;
		imaginaryPart = 0.5 * ft_root(3, 2) * (u - v);
		
		if (realPart == 0)
		{
			printf("   %c2 = \u00B1 ", letter);
			print_number(imaginaryPart);
			printf("i");
		}
		else
		{
			printf("   %c2 = ", letter);
			print_number(realPart);
			printf(" \u00B1 ");
			print_number(imaginaryPart);
			printf("i");
		}
	}
	else if (discriminant == 0)
	{
		sol1 = 2 * ft_root(-q / 2, 3) - a / 3;
		sol2 = -ft_root(-q / 2, 3) - a / 3; //This root is double
		
		if (sol1 == sol2)
		{
			printf("   %c = ", letter);
			print_number(sol1);
			printf(" (triple)");
		}
		else
		{
			printf("   %c1 = ", letter);
			print_number(sol1);
			printf("\n");
			printf("   %c2 = ", letter);
			print_number(sol2);
			printf(" (double)");
		}
	}
	else //discriminant < 0
	{	
		angle = ft_acos(1.5 * q / p * ft_root(-3 / p, 2)) / 3;
		sol1 = 2 * ft_root(-p / 3, 2) * ft_cos(angle) - a / 3;
		sol2 = 2 * ft_root(-p / 3, 2) * ft_cos(angle + 2 * PI / 3) - a / 3;
		sol3 = 2 * ft_root(-p / 3, 2) * ft_cos(angle + 4 * PI / 3) - a / 3;
		
		printf("   %c1 = ", letter);
		print_number(sol1);
		printf("\n");
		printf("   %c2 = ", letter);
		print_number(sol2);
		printf("\n");
		printf("   %c3 = ", letter);
		print_number(sol3);
	}
}
