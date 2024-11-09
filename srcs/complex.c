/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 20:18:42 by jutrera-          #+#    #+#             */
/*   Updated: 2024/10/13 20:18:42 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static void complex_power(char **str, int *i)
{
	int		j;
	int		power;
	char	*aux;
	char	*aux2;

	j = *i + 2;
	while(isdigit((*str)[j]))
		++j;
	aux = ft_substr(*str, *i + 2, j);
	power = atof(aux);
	free(aux);
	while (--j != *i)
		(*str)[j] = ' ';

	if (power % 2 == 0)
		(*str)[j] = ' ';
	if ((*str)[j - 1] == '*')
		(*str)[j - 1] = ' ';
	while (*i >= 0 && (*str)[*i] != '+' && (*str)[*i] != '-')
		--(*i);
	if (*i >= 0)
	{
		if ((*str)[*i] == '+')
			(*str)[*i] = '-';
		else if (*i > 0)
			(*str)[*i] = '+';
		else
			(*str)[*i] = ' ';
	}
	else
	{
		aux2 = (char *)calloc(strlen(*str) + 2, sizeof(char));
		if (!aux2)
			exit(EXIT_FAILURE);
			
		strcpy(aux2,"-");
		strcat(aux2, *str);
		free(*str);
		*str = aux2;	
	}
	remove_spaces(*str);
	if ( strlen(*str) == 0 ||
		(strlen(*str) > 0 && ((*str)[strlen(*str) - 1] == '-'|| (*str)[strlen(*str) - 1] == '+')))
		strcat(*str, "1");
}

static void complex_product(char **str, int *i)
{
	if ((*str)[*i+2] == 'i')
	{
		(*str)[*i] = ' ';
		(*str)[*i + 1] = '-';
		(*str)[*i + 2] = '1';
	

	if (*i > 0 && isdigit((*str)[*i - 1]))
		(*str)[*i] = '*';
	}
}

static void complex_bracket(char **str, int *i)
{
	int		j;
	int		k;
	char	*aux;
	char 	*aux2;

	j = *i;
	while ((*str)[j] != '(')
	{
		if ((*str)[j] != '*' && !isdigit((*str)[j]) && (*str)[j] != 'i')
			break;
		--j;
	}
	if ((*str)[j] != '(')
		return;
	
	k = *i;
	k += 3;
	while (isdigit((*str)[k]))
		++k;
	aux = ft_substr(*str, *i + 3, k);
	aux2 = ft_substr(*str, j + 1, *i + 1);
	if (v_calc) printf("   Complex bracket : %s%s%s elevado a %s%s%s\n", CYAN, aux2, RESET, CYAN, aux, RESET);
	free(aux);
	free(aux2);
}

static bool iscomplex(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (isalpha(str[i]) && str[i] != 'i')
			return false;
		++i;
	}
	return true;
}

int complex_calc(char **str)
{
	int		i;
	
	if (!iscomplex(*str))
		return 0;

	i = strchr(*str, 'i') - (*str);
	while (i >= 0)
	{
		if ((*str)[i + 1] == ')' && (*str)[i + 2] == '^')
			complex_bracket(str, &i);
		if ((*str)[i + 1] == '^')
			complex_power(str, &i);
		if ((*str)[i + 1] == '*')
			complex_product(str, &i);
		remove_spaces(*str);
		if (v_calc) printf("   Complex result : %s%s%s\n", CYAN, *str, RESET);
		i = strchr(*str + i + 1, 'i') - (*str);
	}
	doingproducts(str, *str);
	return 0;
}
