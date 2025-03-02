/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:27:51 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/01 14:27:51 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static bool brackets_is_needed(char *s, int len_dest)
{
	bool 	need_brackets;
	int i = strchr(s, '+') - s;
	int j = strchr(s, '-') - s;
	need_brackets = (s[0] != '[' || s[len_dest - 1] != ']') && (i >= 0 || j >= 0);
	
	return need_brackets;
}

static int change_content(char **s, int j, int i, char *dest)
{
	int		len_s;
	int		len_dest;
	int		extra;
	char	*var_to_change;
	int 	new_len;
	char 	*new_str;

	len_s = strlen(*s);
	len_dest = strlen(dest);

	extra = 0;
	while (check_brackets(dest))
	{
		dest[len_dest-1] = '\0';
		--len_dest;
	}

	var_to_change = ft_substr(*s, j, i);

	if (v_calc) printf("%sREPLACING   : %s%s=%s%s in %s", GREEN , CYAN, var_to_change, dest, RESET, *s);

	while (check_brackets(var_to_change))
	{
		var_to_change[strlen(var_to_change)-1] = '\0';
		--i;
	}
	if (!strcmp(var_to_change, dest))
	{
		free(var_to_change);
		if (v_calc) printf("\n");
		return 0;
	}

	free(var_to_change);
	if (j > 0 && isdigit((*s)[j-1]))
		++extra;

	new_len = len_s - (i - j) + len_dest + 2 + extra;
	new_str = (char *)calloc((new_len + 1), sizeof(char));
	if (!new_str) exit(EXIT_FAILURE);

	strncpy(new_str, *s, j);
	if (extra)
		new_str[j++] = '*';

	if (brackets_is_needed(dest, len_dest))
	{
		new_str[j++] = '(';
		++extra;
	}
	strcat(new_str, dest);
	if (brackets_is_needed(dest, len_dest))
	{
		strcat(new_str, ")");
		++extra;
	}
	strcat(new_str, *s + i);
	remove_spaces(new_str);
	free(*s);
	*s = new_str;
	if (v_calc) printf(" => %s%s%s\n", CYAN, *s, RESET);
	return (extra);
}

static char *get_name(char *t)
{
	int		i;
	char	*aux;

	i = 0;
	while (t[i] && t[i] != '(')
		++i;
	if (t[i] == '\0')
		return 0;

	aux = ft_substr(t, 0, i);
	return aux;
}

static char *get_variable(char *t)
{
	int		i;
	char	*aux;

	i = 0;
	while (t[i] && t[i] != '(')
		++i;
	aux = ft_substr(t, i+1, strlen(t) -1);
	return aux;
}

static char *search_content_in_variables(char *s, t_token **list)
{
	t_token *ptr;

	if (!strcmp(s, "i"))
		return 0;
	ptr = *list;
	while (ptr && strcmp(ptr->token, s))
		ptr = ptr->next;
	if (ptr)
		return ptr->content;
	return 0;
}

static void calc_function(char **temp, char *fvariable, char *value)
{
	char	*word;
	int		i;
	int		j;
	int		added;
	
	i = 0;
	while ((*temp)[i])
	{
		if (isalpha((*temp)[i]))
		{
			j = i;
			while (isalpha((*temp)[i]))
				++i;
			if (i != j)
			{
				word = ft_substr(*temp, j, i);
				if (!strcmp(value, fvariable))
				{
					free(word);
					break;
				}
				else if (!strcmp(word, fvariable))
				{
					added = change_content(temp, j, i, value);
					i = j + strlen(value) + added;
				}
				free(word);
			}
		}
		else
			++i;
	}
}

static bool cont_in_token(char *token, char *var, char *cvar, t_token **list)
{
	int	i;
	int	j;
	int len;

	i = 0;
	len = strlen(var);
	while (var[i] != '\0')
	{
		j = 0;
		while (var[i] != '\0' && cvar[j] != '\0' && var[i] == cvar[j])
		{
			++i;
			++j;
		}
		if (j == len)
			return 1;
		if (var[i] != '\0')
			++i;
	}
	t_token *ptr = *list;
	while (ptr)
	{
		if (strstr(ptr->token, var) && !strcmp(token, ptr->token))
			return 1;
		ptr = ptr->next;
	}
	return 0;
}

static char *search_content_in_functions(char *var, t_token **list)
{
	t_token	*ptr;
	char	*cname;
	char	*fname;
	char	*value;
	char	*fvariable;
	char	*temp;

	ptr = *list;
	cname = get_name(var);
	while (ptr)
	{
		fname = get_name(ptr->token);
		
		if (fname && !strcmp(fname, cname))
		{
			free(fname);
			free(cname);
			value = get_variable(var);
			fvariable = get_variable(ptr->token);
			temp = ft_substr(ptr->content, 0, strlen(ptr->content));
			calc_function(&temp, fvariable, value);
			
			free(value);
			free(fvariable);
			return temp;
		}
		ptr = ptr->next;
		if (fname)
			free(fname);
	}
	free(cname);
	return NULL;
}

int	compute(char **s, t_token **list, char *token)
{
	int 	i;
	int		j;
	int		bracket;
	int		numbrackets;
	char 	*cvar;
	char	*var;
	char	variable[2] = {0}; //En el caso de que haya función

	if (!token)
	{
		i = strchr(*s, '(') - *s;
		if (i > 0)
			variable[0] = (*s)[i + 1];
	}

	i = 0;
	while ((*s)[i])
	{
		j = i;
		while (isalpha((*s)[i]))
			++i;
		if (i == j)
			++i;
		else if ((*s)[i] != '(')
		{
			var = ft_substr(*s, j, i);
			cvar = search_content_in_variables(var, list);
			if (cvar)
			{
				if (token && cont_in_token(token, var, cvar, list))
				{
					free(var);
					return printf_error("Circular reference", *s, j);
				}
				change_content(s, j, i, cvar);
				i = 0;
			}
			else if (!token && strcmp(var, "i") && strcmp(var, variable))
			{
				free(var);
				return printf_error("Variable not found", *s, j);
			}
			free(var);
		}
		else 
		{
			bracket = i;
			numbrackets = 1;
			while ((*s)[++bracket] && numbrackets != 0)
				numbrackets += ( ((*s)[bracket] == '(') - ((*s)[bracket] == ')') );
			var = ft_substr(*s, j, bracket);
			cvar = search_content_in_functions(var, list);
			if (cvar)
			{			
				change_content(s, j, bracket, cvar);
				i = 0;
				free(cvar);
			}
			else
			{
				char *fname = get_name(var);
				if (!isfunctionword(fname))
				{
					free(var);
					free(fname);
					return printf_error("Function not found", *s, j);
				}
				free(fname);
			}
			free(var);
		}
	}
	return calc(s);
}
