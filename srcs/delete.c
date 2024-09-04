/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:45:16 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/05 19:45:16 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "computor.h"

static void	delete_token(char *token, t_token **list)
{
	t_token	*ptr1;
	t_token	*ptr2;

	ptr1 = *list;
	ptr2 = NULL;
	while (ptr1)
	{
		if (!strcmp(ptr1->token, token))
		{
			if (ptr2)
				ptr2->next = ptr1->next;
			else
				*list = ptr1->next;
			free(ptr1->token);
			free(ptr1->content);
			free(ptr1);
			printf("   %s%s%s deleted !\n", CYAN, token, RESET);
			break;
		}
		ptr2 = ptr1;
		ptr1 = ptr1->next;
	}
	if (!ptr1)
		printf("   %sError%s: \"%s\" not found !\n", RED, RESET, token);
	free(token);
}

bool	delete(char *content, t_token **list)
{
	char	*order;
	char	*token;
	int		i;
	int		j;
	int		len;

	len = strlen(content);
	//Si no es el comando delete
	if (len < 6)
		return 0;
	else
	{
		order = ft_substr(content, 0, 6);
		if (strcmp(order, "delete") || strchr(content, '='))
		{
			free(order);
			return 0;
		}
		free(order);
	}


	//Es el comando delete pero no hay datos
	if (!*list)
	{
		printf("Nothing to delete");
		return 1;
	}

	//Es el comando delete SIN parámetros
	if (!strcmp(content, "delete"))
	{
		free_tokens(list);
		printf("   All data deleted !\n");
		return 1;
	}

	//Es el comando delete CON parámetros separados por espacios
	order = ft_substr(content, 6, len);
	j = 0;
	i = 0;
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
			return printf_error("Bad argument for delete command", NULL, -1);
		}
	
		token = ft_substr(order, i, j);
		delete_token(token, list);
		i = j;
	}
	free(order);
	token_type(list);
	return 1;
}
