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
			else
				break;
			i = strchr(*str, '(') - (*str);
		} 
	}
	
	transformexpression(str);
}

int complex_calc(char **str)
{
	printf("  Complex : %s%s%s\n", CYAN, *str, RESET);

	int i;
	
	i = strchr(*str, 'i') - (*str);
	if (i >= 0 && (*str)[i + 1] == '^')
	{
		int j = i + 2;
		while(isdigit((*str)[j]))
			++j;
		char *aux = ft_substr(*str, i + 2, j);

		int power = atof(aux);
		free(aux);
		
		while (--j != i)
		{
			(*str)[j] = ' ';
		}

		if (power % 2 == 0)
		{
			(*str)[j] = ' ';
			if ((*str)[j - 1] == '*')
				(*str)[j - 1] = ' ';
			while (i >= 0 && (*str)[i] != '+' && (*str)[i] != '-')
				--i;
			if (i >= 0)
			{
				if ((*str)[i] == '+')
					(*str)[i] = '-';
				else if (i > 0)
					(*str)[i] = '+';
				else
					(*str)[i] = ' ';
			}
			else
			{
				char *aux2 = (char *)calloc(strlen(*str) + 2, sizeof(char));
				if (!aux2)
					exit(EXIT_FAILURE);
				
				strcpy(aux2,"-");
				strcat(aux2, *str);
				free(*str);
				*str = aux2;
			}
		}
		remove_spaces(*str);
		if (strlen(*str) == 0 || (strlen(*str) > 0 && (*str)[strlen(*str) - 1] == '-'))
			strcat(*str, "+1");
		printf("  Complex result : %s%s%s\n", CYAN, *str, RESET);
	}

	return 0;
}