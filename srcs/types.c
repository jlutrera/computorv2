/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:01:51 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/14 20:01:51 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static char	*typetostring(t_type type)
{
	if (type == MATRIX)
		return ("MATRIX");
	if (type == FUNCTION)
		return ("FUNCTION");
	if (type == RATIONAL)
		return ("RATIONAL");
	if (type == COMPLEX)
		return ("COMPLEX");
	if (type == ALGEBRAIC)
		return ("ALGEBRAIC");
	return ("NOT FOUND");
}

static void	oneTokenType(char *token, t_token **list)
{
	t_token	*ptr;
	int		found;

	ptr = *list;
	found = 0;
	
	while (ptr)
	{
		if (!strcmp(ptr->token, token))
		{
			printf("\t%s", ptr->token);
			printf("\t\t%s\n", typetostring(ptr->type));
			++found;
		}
		ptr = ptr->next;
	}
	if (found == 0)
	{
		printf("\t%s%s", RED, token);
		printf("\t\t%s%s\n", typetostring(-1), RESET);
	}
	free(token);
}

static void	allTokenTypes(t_token **list)
{
	t_token	*ptr;

	ptr = *list;
	printf("\n%s	Name		Type%s\n", CYAN, RESET);
	printf("\t__________________________\n");
	while (ptr)
	{
		
		printf("\t%s", ptr->token);
		printf("\t\t%s\n", typetostring(ptr->type));
		ptr = ptr->next;
	}
	printf("\n");
}

bool types(char *content, t_token **list)
{
	char	*order;
	char	*token;
	int		i;
	int		j;

	if (strchr(content, '='))
	 	return 0;

	i = 0;
	while (content[i] && content[i] != ' ')
		++i;

	order = ft_substr(content, 0, 4);
	if (strcmp(order, "type"))
	{
		free(order);
		return 0;
	}
	free(order);

	//Es el comando type pero no hay datos
	if (!*list)
	{
		printf("   (empty)\n");
		return 1;
	}

	//Es el comando type SIN parámetros
	if (!strcmp(content, "type"))
	{
		allTokenTypes(list);
		return 1;	
	}

	//Es el comando type CON parámetros separados por espacios
	order = ft_substr(content, 4, strlen(content));
	j = 0;
	i = 0;
	
	printf("\n%s	Name		Type%s\n", CYAN, RESET);
	printf("\t__________________________\n");
	while (order[i] != '\0')
	{
		while (order[i] == ' ' && order[i] != '\0')
			++i;
		if (order[i] == '\0')
			break;
		j = i;
		while (isalpha(order[j]) || order[j] == ')' || order[j] == '(')
			++j;
		if (order[j] != ' ' && order[j] != '\0')
		{
			free(order);
			return printf_error("Bad argument for type command", NULL, -1);
		}
	
		token = ft_substr(order, i, j);
		oneTokenType(token, list);
		i = j;
	}
	printf("\n");
	free(order);
	return 1;
}
