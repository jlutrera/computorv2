/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algebra.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 20:45:48 by jutrera-          #+#    #+#             */
/*   Updated: 2024/09/06 20:45:48 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static void addbrackets(char **str, int i, int j)
{
	(*str)[i] = ' ';
	(*str)[j] = ' ';
	if (i > 0 && ((*str)[i + 1] == '-' || (*str)[i + 1] == '+'))		
			(*str)[i - 1] = ' '; 
	remove_spaces(*str);
}

static void subtractbrackets(char **str, int i, int j)
{
	(*str)[i] = ' ';
	(*str)[j] = ' ';
	if ((*str)[i + 1] == '-' || (*str)[i + 1] == '+')
		(*str)[i - 1] = ' ';
	while (i < j)
	{
		if ((*str)[i] == '+' )
			(*str)[i] = '-';
		else if ((*str)[i] == '-' )
			(*str)[i] = '+';
		++i;
	}
	remove_spaces(*str);
}

static int whereistheclosingbracket(char *str, int i)
{
	int 	b;

	b = 1;
	while (str[i] && b != 0)
	{
		b = b + (str[i] == '(') - (str[i] == ')');
		++i;
	}
	return --i;
}

void calc_with_variables(char **str)
{
	int 	i;
	int		j;
	char 	*newp;

	
	i = strchr(*str, '(') - (*str);
	if (i >= 0)
	{
		j = whereistheclosingbracket(*str, i + 1);
		
		newp = ft_substr(*str, i+1, j);
		
		if (v_calc) printf("   Calculating : %s%s%s\n", CYAN, newp, RESET);
		if (strchr(newp, '('))
		{
			
			calc_with_variables(&newp);
			remove_spaces(newp);
			
			for (int k = 0; k < (int)strlen(newp); ++k)
				(*str)[i + 1 + k] = newp[k];

			for (int k = (int)strlen(newp); k < j - i - 1; ++k)
				(*str)[i + 1 + k] = ' ';

			remove_spaces(*str);
		}
		free(newp);
		i = strchr(*str, '(') - (*str);
		while ( ( i == 0 || (i > 0 && !isalpha((*str)[i - 1])) ) )
		{
			j = whereistheclosingbracket(*str, i + 1);
			if ( ((i > 0 && (*str)[i - 1] == '+') || i == 0 ) &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				addbrackets(str, i, j);

			else if ( (i > 0 && (*str)[i - 1] == '-') &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				subtractbrackets(str, i, j);
			else if ((*str)[j + 1] == '^')
			{
				char *exponent;
				char *base = ft_substr(*str, i, j+1);
				j += 2;
				i = j;
				while (isdigit((*str)[i]) || (*str)[i] == '.')
					++i;
				exponent = ft_substr(*str, j, i);
				if (v_calc) printf("   Base : %s%s%s\n   Exponent : %s%s%s\n", CYAN, base, RESET, CYAN, exponent, RESET);
				
				int num_exp = atoi(exponent);
				char *newstr = (char *)calloc(strlen(*str) * (num_exp + 2)+ 1, sizeof(char));
				
				while (num_exp > 1)
				{
					strcat(newstr, base);
					strcat(newstr, "*");
					--num_exp;
				}
				strcat(newstr, base);
				free(*str);
				*str = newstr;
				
				free(base);
				free(exponent);
				remove_spaces(*str);
			} 
			else if (( (i > 0 && (*str)[i - 1] == '*') || (*str)[j + 1] == '*' ))
			{
				if (!strchr(*str, '+') && !strchr(*str, '-'))
				{
					if (v_calc) printf("   Multiplying : %s%s%s\n", CYAN, *str, RESET);
					calc(str);
				}
				else
					break;
			}			
			else
				break;
			i = strchr(*str, '(') - (*str);
		}
	}
	transformexpression(str);
}
