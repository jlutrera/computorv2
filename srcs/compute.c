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

static int change_content(char **s, int j, int i, char *dest)
{
	int		len_s;
	int		len_dest;
	int		extra;
	int		added;
	char	*var_to_change;
	int 	new_len;
	char 	*new_str;

	len_s = strlen(*s);
	len_dest = strlen(dest);
	added = 0;
	while (check_brackets(dest))
	{
		dest[len_dest-1] = '\0';
		--len_dest;
	}

	var_to_change = ft_substr(*s, j, i);
	while (check_brackets(var_to_change))
	{
		var_to_change[strlen(var_to_change)-1] = '\0';
		--i;
	}
	if (!strcmp(var_to_change, dest))
	{
		free(var_to_change);
		return added;
	}

	free(var_to_change);
	if (j > 0 && isdigit((*s)[j-1]))
		extra = 1; // añado *()
	else
		extra = 0; //añado ()

	new_len = len_s - (i - j) + len_dest + 2 + extra;
	new_str = (char *)malloc((new_len + 1) * sizeof(char));
	if (!new_str)
		exit(EXIT_FAILURE);

	strncpy(new_str, *s, j);
	if (extra)
		new_str[j++] = '*';
	
	if ((i != len_s || j != 0) && (j > 0 && (*s)[j-1] != '(') && (i <= len_s && (*s)[i-1] != ')'))
	{
		new_str[j++] = '(';
		++added;
	}

	new_str[j]= '\0';
	strcat(new_str, dest);
	if ((i != len_s || j != 0) && (j > 0 && (*s)[j-1] != '(') && (i <= len_s && (*s)[i-1] != ')'))
	{
		strcat(new_str, ")");
		++added;
	}
	strcat(new_str, *s + i);
	free(*s);
	*s = new_str;
	return (added+extra);
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

static bool cont_in_token(char *var, char *cvar)
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

int	compute(char **s, t_token **list, bool fortype)
{
	int 	i;
	int		j;
	int		k;
	int		error;
	char 	*cvar = NULL;
	char	*var = NULL;

	i = 0;
	error = 0;
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
				if (cont_in_token(var, cvar))
				{
					error = 1;
					free(var);
					break;
				}
				change_content(s, j, i, cvar);
				i = 0;
			}

			free(var);
		}
		else
		{
			k = i;
			
			while ((*s)[i] && (*s)[i] != ')') 
				++i;
			while((*s)[i] == ')')
				++i;
			var = ft_substr(*s, j, i);
			cvar = search_content_in_functions(var, list);
			printf("cvar = %s\n", cvar);
			if (cvar)
			{
				calc(&cvar);
				change_content(s, j, i, cvar);
				i = 0;
				free(cvar);
			}
			else
				i = k;
			free(var);
		}
	}
	if (error && !fortype)
	{
		printf_error("The assignment contains a logical error", cvar, -1);
	}
	else if (!error)
	{
		error = calc(s);
	}
	return error;
}
