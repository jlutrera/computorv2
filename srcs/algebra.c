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

void calc_with_variables(char **str, int mode)
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
			
			calc_with_variables(&newp, mode);
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
			else
				break;
			i = strchr(*str, '(') - (*str);
		}
	}
	
	transformexpression(str, mode);
}
