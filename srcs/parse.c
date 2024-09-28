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

static int	clean_memory(t_token **t)
{
	free_tokens(t);
	write(STDOUT_FILENO, "\n", 1);
	rl_clear_history();
	return 1;
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

static bool variable_not_found(char *token, char *content, t_token *token_list)
{
	t_token	*ptr;
	char	*aux;
	char    variable;
	int		i;
	int		j;

	i = strchr(token, '(') - token;
	if (i > 0)
		variable = token[i+1];
	else
		variable = 0;

	i = 0;
	while (content[i] != '\0')
	{
		j = i;
		while (content[i] && isalpha(content[i]))
			++i;
		if (i - j > 1 || (i - j == 1 && content[j] != 'i' && content[j] != variable))
		{
			aux = ft_substr(content, j, i);
			if (!isfunctionword(aux))
			{
				if (content[i] == '(')
				{
					free(aux);
					if (content[i] == '(' && isalpha(content[i+1] && content[i+1] != variable))
						return printf_error("Variable cannot be a function", NULL, -1);

					if (isdigit(content[i+1]))
						while (content[i] && content[i] != ')')
							++i;
				}
				else
				{
					ptr = token_list;
					while (ptr)
					{
						if (!strcmp(ptr->token, aux))
							break;
						ptr = ptr->next;
					}
					free(aux);
					if (!ptr)
						return printf_error("Variable not found", NULL, -1);
				}
			}
			else
				free(aux);
		}
		else if (j == i)
			++i;
	}
	return 0;
}

int parse(char **input, t_token **token_list)
{
	int		i;
	char	*token;
	char	*content;
	char 	*cpytoken;
	char	*response;

	str_to_lower(*input);
	*input = ft_trim(*input);
	
	if (!strcmp(*input, "exit"))
		return clean_memory(token_list);

	if (execute_command(*input, token_list))
		return 0;

	if (strchr(*input, '=') == *input || !strchr(*input, '='))
	{
		printf_error("Bad syntax", NULL, -1);
		return 0;	
	}

	i = strchr(*input, '=') - *input;
	if (i == (int)strlen(*input) - 1)
	{
		printf_error("No value after assignment", *input, i);
		return 0;
	}

	token = ft_substr(*input, 0, i);
	token = ft_trim(token);
	content = ft_substr(*input, i+1, strlen(*input));
	content = ft_trim(content);
	
	if (!strcmp(content, "?"))
	{
		if (!syntax_error_content(token, NULL))
		{
			if (compute(&token, token_list, NULL) == 0)
				printf("   %s\n", token);
			free(token);
			free(content);
		}
	}
	else if (!syntax_error_token(token) &&
			 !syntax_error_content(content, token) &&
			 !variable_not_found(token, content, *token_list))
	{
		cpytoken = ft_substr(token, 0, strlen(token));			
		response = ft_substr(content, 0, strlen(content));
		if (compute(&response, token_list, cpytoken) == 0)
		{
			add_token_to_list(token_list, token, content);
			printf("   %s\n", response);
			ft_add_history(cpytoken, content, response);
		}
		else
		{
			free(token);
			free(content);
		}
		free(response);
		free(cpytoken);
	}
	else
	{
		free(token);
		free(content);
	}
	
	return 0;
}
