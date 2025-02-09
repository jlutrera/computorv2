/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:40:06 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/05 14:40:06 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static void	listone(char *token, t_token **list)
{
	t_token	*ptr;
	int		found;

	ptr = *list;
	found = 0;
	
	while (ptr)
	{
		if (!strcmp(ptr->token, token))
		{
			printf("\t%s\t\t%s\n", ptr->token, ptr->content);
			++found;
		}
		ptr = ptr->next;
	}
	if (found == 0)
		printf("\t%s%s\t\tNOT FOUND%s\n", RED, token, RESET);
	free(token);
}

static void	listall(t_token **list)
{
	t_token	*aux;

	if (list)
	{
		aux = *list;
		printf("\n%s	Name		Value%s\n", CYAN, RESET);
		printf("\t__________________________\n");
		while (aux != NULL)
		{
			printf("\t%s\t\t%s\n", aux->token, aux->content);
			aux = aux->next;
		}
		printf("\n");
	}
	else
		printf("   (empty)\n");
}

bool list(char *content, t_token **list)
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
	order = ft_substr(content, 0, i);
	if (strcmp(order, "list"))
	{
		free(order);
		return 0;
	}
	free(order);
	
	//Es el comando delete pero no hay datos
	if (!*list)
	{
		printf("   (empty)\n");
		return 1;
	}

	//Es el comando delete SIN parámetros
	if (!strcmp(content, "list"))
	{
		listall(list);
		return 1;	
	}

	//Es el comando delete CON parámetros separados por espacios
	order = ft_substr(content, 4, strlen(content));
	j = 0;
	i = 0;
	
	printf("\n%s	Name		Value%s\n", CYAN, RESET);
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
			return printf_error("Bad argument for list command", NULL, -1);
		}
	
		token = ft_substr(order, i, j);
		listone(token, list);
		i = j;
	}
	printf("\n");
	free(order);
	return 1;
}