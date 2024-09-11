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
	if (!strchr(*str, '('))
		transformexpression(str);
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
	if (!strchr(*str, '('))
		transformexpression(str);
}

static void multiplybrackets(char **str, int i, int j)
{
	printf("***Procesando la multiplicación de ");
	while (i <= j )
	{
		printf("%c", (*str)[i]);
		++i;
	}
	printf("\n");
}

void calc_with_variables(char **str)
{
	int 	i;
	int		j;
	int		k;
	int		b;
	char	*p1;
	char	*p2;
	char 	*newp;
	bool	isfunction;

	p1 = strchr(*str, '(');
	if (!p1)
		transformexpression(str);
	else
	{
		b = 1;
		i = p1 - (*str);
		j = i + 1;
		while (b != 0)
		{
			b = b + ((*str)[j] == '(') - ((*str)[j] == ')');
			++j;
		}
		--j;
		newp = ft_substr(*str, i+1, j);
		if (strchr(newp, '('))
		{
			printf("   Entro con %s\n", newp);
			calc_with_variables(&newp);
			printf("   Salgo con %s\n", newp);
			k = 0;
			while (k < (int)strlen(newp))
			{
				(*str)[i + 1 + k] = newp[k];
				k++;
			}
			while (k < j - i - 1)
			{
				(*str)[i + 1 + k] = ' ';
				++k;
			}
			remove_spaces(*str);
			printf("   Me queda %s\n", *str);
			p2 = strchr(*str, ')');
			j = p2 - (*str);
		}
		free(newp);
	
		if ((i > 0 && (*str)[i - 1] == '*') || (*str)[j + 1] == '*' )
			multiplybrackets(str, i, j);

		else if (i == 0 ||
			((*str)[i - 1] == '+' && (*str)[j + 1]!= '*'  &&
			 (*str)[j + 1] != '%' && (*str)[j + 1] != '/' &&
			 (*str)[j + 1] != '^' && (*str)[j + 1] != '!'))
			addbrackets(str, i, j);

		else if ((*str)[i - 1] == '-' && (*str)[j + 1]!= '*'  &&
				 (*str)[j + 1] != '%' && (*str)[j + 1] != '/' &&
				 (*str)[j + 1] != '^' && (*str)[j + 1] != '!')
			subtractbrackets(str, i, j);

		else if (!strchr(*str, '(') && !strchr(*str, '^') &&
				 !strchr(*str, '*') && !strchr(*str, '!') &&
				 !strchr(*str, '%') && !strchr(*str, '/'))
				transformexpression(str);

		p2 = strchr(*str, '(');
		if (p2)
		{
			i = p2 - (*str);
			isfunction = (i > 0 && isalpha((*str)[i - 1]));
			if (!isfunction)
				i = 0;
			if (strchr(*str+i, '(')   && !strstr(*str+i, "*(") &&
				!strstr(*str+i, ")*") && !strstr(*str+i, ")^") &&
				!strstr(*str+i, "^(") && !strstr(*str+i, ")!") &&
				!strstr(*str+i, "%(") && !strstr(*str+i, ")%") &&
				!strstr(*str+i, "/(") && !strstr(*str+i, ")/"))
			{
				printf("  Vuelvo a llamar para arreglar %s\n", *str);
				calc_with_variables(str);
			}
		}
	}
}
