/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:50:21 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/01 13:50:21 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"


static void	str_to_lower(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		s[i] = tolower(s[i]);
}

static void	clean_memory(char **i, t_token **t)
{
	if (i && *i)
		free(*i);
	free_tokens(t);
	write(STDOUT_FILENO, "\n", 1);
	rl_clear_history();
}

static t_token	*ft_token_last(t_token *lst)
{
	t_token	*aux;

	if (!lst)
		return (NULL);
	aux = lst;
	while (aux->next != NULL)
		aux = aux->next;
	return (aux);
}

static void	ft_token_add_back(t_token **lst, t_token *new)
{
	t_token	*ptr;

	if (!(*lst))
		*lst = new;
	else
	{
		ptr = ft_token_last(*lst);
		ptr->next = new;
	}
}

static void	add_token_to_list(t_token **list, char *token, char *content)
{
	t_token	*new_token;
	t_token	*ptr;

	new_token = (t_token *)calloc(sizeof(t_token), 1);
	if (!new_token)
		exit(EXIT_FAILURE);

	new_token->token = token;
	new_token->content = content;

	if (*list == NULL)
		*list = new_token;
	else
	{
		//Busco un token en la lista
		ptr = *list;
		while (ptr && strcmp(ptr->token, token))
			ptr = ptr->next;
		//Si existe reemplazo su  y tipo
		if (ptr)
		{
			free(ptr->content);
			ptr->content = content;
			free(token);
			free(new_token);
		}
		else
			ft_token_add_back(list, new_token);
	}
	token_type(list);
}



static bool execute_command(char *newinput, t_token **token_list)
{
	if (solve(newinput) < 2)
		return 1;

	if (delete(newinput, token_list))
		return 1;

	if (plot(newinput))
		return 1;

	if (list(newinput, token_list))
		return 1;

	if (help(newinput))
		return 1;

	if (types(newinput, token_list))
		return 1;

	return 0;
}

static void	ft_add_history(char *token, char *content, char *result)
{
	char	*history;
	int		len;

	len = strlen(token) + strlen(content) + 10 + strlen(result);
	history = (char *)malloc(len * sizeof(char));
	if (!history)
		exit(EXIT_FAILURE);
	strcpy(history, token);
	strcat(history, " = ");
	strcat(history, content);
	strcat(history, "  >>  ");
	strcat(history, result);
	add_history(history);
	free(history);
}



int parse(char *input, t_token **token_list)
{
	int		i;
	char	*token;
	char	*content;
	char	*newinput;
	char 	*cpytoken;
	char	*response;

	if (input && !*input)
	{
		free(input);
		return 0;
	}

	str_to_lower(input);
	newinput = ft_trim(input);
	
	if (!strcmp(newinput, "exit"))
	{
		clean_memory(&newinput, token_list);
		return 1;
	}

	if (execute_command(newinput, token_list))
	{
		free(newinput);
		return 0;
	}

	if (strchr(newinput, '=') == newinput || !strchr(newinput, '='))
	{
		printf_error("Bad syntax", NULL, -1);
		free(newinput);
		return 0;	
	}
	
	i = strchr(newinput, '=') - newinput;
	if (i == (int)strlen(newinput) - 1)
	{
		printf_error("No value after assignment", newinput, i);
		free(newinput);
		return 0;
	}

	token = ft_substr(newinput, 0, i);
	token = ft_trim(token);
	content = ft_substr(newinput, i+1, strlen(newinput));
	content = ft_trim(content);
	if (!strcmp(content, "?"))
	{
		if (!syntax_error_content(token, NULL))
		{
			if (compute(&token, token_list, false) == 0)
				printf("   %s\n", token);
		}
		free(token);
		free(content);
	}
	else if (!syntax_error_token(token) && !syntax_error_content(content, token))
	{
		cpytoken = ft_substr(token, 0, strlen(token));
		add_token_to_list(token_list, token, content);
			
		response = ft_substr(content, 0, strlen(content));
		compute(&response, token_list, false);
			
		printf("\n   %s\n", response);
		ft_add_history(cpytoken, content, response);

		free(response);
		free(cpytoken);
	}
	else
	{
		free(token);
		free(content);
	}
	
	free(newinput);
	return 0;
}
