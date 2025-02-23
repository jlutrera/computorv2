/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
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
	char 	*cpycontent;
	char 	*cpytoken;
	char 	*functionname_cpytoken;
	char	*functionname;
	bool	foundsamefunction;

	new_token = (t_token *)calloc(sizeof(t_token), 1);
	if (!new_token)
		exit(EXIT_FAILURE);

	cpycontent = ft_substr(content, 0, strlen(content));
	cpytoken = ft_substr(token, 0, strlen(token));
	if (strchr(cpytoken, '('))
	{
		functionname_cpytoken = ft_substr(cpytoken, 0, strchr(cpytoken, '(') - cpytoken);
	}
	new_token->token = cpytoken;
	new_token->content = cpycontent;

	if (*list == NULL)
		*list = new_token;
	else
	{
		//Busco un token en la lista
		ptr = *list;
		while (ptr)
		{
			if (!strcmp(ptr->token, cpytoken))
				break;
			if (strchr(ptr->token, '(') && strchr(cpytoken, '('))
			{
				functionname = ft_substr(ptr->token, 0, strchr(ptr->token, '(') - ptr->token);
				foundsamefunction =  !strcmp(functionname, functionname_cpytoken);
				free(functionname);
				if (foundsamefunction)
					break;
			}
			ptr = ptr->next;
		}
		
		//Si existe reemplazo su contenido
		if (ptr)
		{
			free(new_token);
			free(ptr->content);
			ptr->content = cpycontent;
			free(ptr->token);
			ptr->token = cpytoken;
		}
		else
			ft_token_add_back(list, new_token);
	}
	if (strchr(cpytoken, '('))
		free(functionname_cpytoken);
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
	
	if (!strcmp(newinput, "clear"))
	{
		system("clear");
		splash();
		return 1;
	}
	if (!strcmp(newinput, "visual on"))
	{
		v_calc = 1;
		printf("   Visual mode %sON%s\n", GREEN, RESET);
		return 1;
	}
	if (!strcmp(newinput, "visual off"))
	{
		v_calc = 0;
		printf("   Visual mode %sOFF%s\n", RED, RESET);
		return 1;
	}
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
	if (strlen(content) != 0)
	{
		strcat(history, " = ");
		strcat(history, content);
	}
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
					if (!ptr)
					{
						printf("   %sError%s: Variable %s%s%s not found\n", RED, RESET, RED, aux, RESET);
						free(aux);
						return 1;
					}
					free(aux);
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

void print_result(char *str)
{
	if (strchr(str, '['))
	{
		if (onlydigits(str))
		{
			char ***response;
			int i = fixRows(str);
			int j = fixColumns(str);
			response = create_matrix(str, i, j);
			print_matrix(response, i, j);
			free_matrix(response, i, j);
		}
		else
		{
			int i = -1;
			printf("   ");
			while (str[++i])
			{
				if (str[i] == '#')
					printf("**");
				else
					printf("%c", str[i]);
			}
			printf("\n");
		}
	}
	else
		printf("   %s\n", str);
}

int parse(char **input, t_token **token_list)
{
	int		i;
	char	*token;
	char	*content;
	char	*response;
	int		error;
	
	*input = ft_trim(*input);
	if (*input && (*input)[0] == '\0')
		return 0;
	str_to_lower(*input);

	if (!strcmp(*input, "exit"))
		return clean_memory(token_list);
	if (execute_command(*input, token_list))
		return (0);

	i = strchr(*input, '=') - *input;
	if (i < 0)
		return (printf_error("Equal sign not found", NULL, -1) - 1);
	if (i == 0)
		return (printf_error("No value before assignment", *input, i) - 1);
	if (i == (int)strlen(*input) - 1)
		return (printf_error("No value after assignment", *input, i) - 1);

	token 	= ft_substr(*input, 0, i);
	token 	= ft_trim(token);
	content = ft_substr(*input, i+1, strlen(*input));
	content = ft_trim(content);

	error = 0;
	if (!strcmp(content, "?"))
		error = syntax_error_content(token, NULL);
	else
	{
		error = syntax_error_token(token);
		if (!error)
			error = syntax_error_content(content, token);
		if (!error)
			error = variable_not_found(token, content, *token_list);
	}
	if (error)
	{
		if (!strcmp(content, "?"))
			ft_add_history(token, "", "Error");
		else
			ft_add_history(token, content, "Error");
		free(token);
		free(content);
		return 0;
	}

	if (!strcmp(content, "?"))
	{
		response = ft_substr(token, 0, strlen(token));	
		if (compute(&response, token_list, NULL) == 0)
		{
			print_result(response);
			ft_add_history(token, "", response);
		}
		else
			ft_add_history(token, "", "Error");
	}
	else
	{		
		response = ft_substr(content, 0, strlen(content));
		if (compute(&response, token_list, token) == 0)
		{
			print_result(response);
			ft_add_history(token, content, response);
			if (strchr(token, '('))
				add_token_to_list(token_list, token, content);
			else
				add_token_to_list(token_list, token, response);
		}
		else
			ft_add_history(token, content, "Error");
	}
	free(response);
	free(token);
	free(content);
	return 0;
}
