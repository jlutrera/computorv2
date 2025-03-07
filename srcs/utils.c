/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:31:57 by jutrera-          #+#    #+#             */
/*   Updated: 2024/07/30 14:31:57 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

void update_result(char **str, int start, int end, char *aux)
{
	char 	*temp;
	
	temp = (char*)calloc(strlen(*str) + strlen(aux) + 2, sizeof(char));
	if (!temp) exit(EXIT_FAILURE);

	strncpy(temp, *str, start);
	if (strlen(temp) > 0 && aux[0] != '(' && aux[0] == '-' && strchr("+-*/^", temp[strlen(temp)-1]) )
	{
		strcat(temp, "(");	
		strcat(temp, aux);
		strcat(temp, ")");
	}
	else
		strcat(temp, aux);
	strcat(temp, *str+end);
	free(*str);
	*str = temp;
}

int onlydigits(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (isalpha(s[i]))
			return 0;
		++i;
	}
	return 1;
}

bool isanumber(char *s)
{
	int i;

	if (s[0] == '(' && s[strlen(s)-1] == ')')
		i = 1;
	else
		i = 0;
	if (s[i] == '-' || s[i] == '+')
		++i;
	while (s[i])
	{
		if (!isdigit(s[i]) && s[i] != '.')
			break;
		++i;
	}
	if (s[i] == '\0' || (s[i] == ')' && s[i+1] == '\0'))
		return true;
	return false;
}

char get_letter(char *s)
{
	char	c = 0;
	int		i = 0;
	while (s[i])
	{
		if (isalpha(s[i]))
		{
			if (c == 0)
				c = s[i];
			else if (c != s[i] )
			{
				if (v_calc) printf("\n   Detected more than one variable\n");
				return 0;
			}
		}
		i++;
	}
	return c;
}

bool isinteger(double num)
{
	int part_int;

	part_int = (int)num;
	return (num - part_int) == 0.0;
}

char	*doubletostr(double d)
{
	char *aux;

	aux = malloc(100);
	if (!aux) exit(EXIT_FAILURE);
	if (isinteger(d))
		sprintf(aux,"%0.0f", d);
	else
		sprintf(aux, "%.2f", d);

	if (strcmp(aux, "-0") == 0)
		strcpy(aux, "0");
	return aux;
}

bool	bad_letters(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (isalpha(s[i]))
		{
			while (s[i] && isalpha(s[i]))
				++i;
			while (s[i] && isblank(s[i]))
				++i;
			if (s[i] && isalpha(s[i]))
				return printf_error("Invalid variable format", s, i);
		}
		else
			++i;
	}
	
	return 0;
}

bool	bad_digits(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (isdigit(s[i]) || s[i] == '.')
		{
			while (s[i] && (isdigit(s[i]) || s[i] == '.'))
				++i;
			while (s[i] && isblank(s[i]))
				++i;
			if (s[i] && (isdigit(s[i]) || s[i] == '.'))
				return printf_error("Invalid number format", s, i);
		}
		else
			++i;
	}
	
	return 0;
}

void	remove_spaces(char* source)
{
	char	*i = source;  // Pointer which moves up the string and makes the result
	char	*j = source;  // Pointer which goes the original string

	while (*j != '\0')
	{
		if (*j != ' ')
		{
			*i = tolower((unsigned char)*j);
			i++;
		}
		j++;
	}
	*i = '\0';
}

bool printf_error(char *msg, char *s, int i)
{
	int n;
	
	printf("   %sError%s: %s ", RED, RESET, msg);
	
	if (s)
	{
		printf("=> ");
		n= 0;
		while (n < i && s[n] != '\0')
		{
			if (s[n] == '#')
				printf("**");
			else
				printf("%c", s[n]);
			n++;
		}
		if (s[n] != '\0')
		{
			if (s[n] == '#')
				printf("%s**", RED);
			else
				printf("%s%c", RED, s[n]);
			if (i == -1)
				printf("%s", RED);
			else
				printf("%s", RESET);
			n++;
			while (s[n] != '\0')
			{
				if (s[n] == '#')
					printf("**");
				else
					printf("%c", s[n]);
				n++;
			}
		}
	}
	printf("%s\n", RESET);
	return 1;
}

char *ft_substr(const char *src, size_t i, size_t j)
{
	char	*dest; 
	size_t	n;

	dest = (char *)malloc((j -i + 1) * sizeof(char));
	if (!dest) exit(EXIT_FAILURE);

	for (n = 0; n < j - i  && src[n + i] != '\0'; n++)
		dest[n] = src[n + i];
	dest[n] = '\0';

	return (dest);
}

char	*ft_trim(char *str)
{
	int		end;
	int		init;
	char	*trimed;

	init = 0;
	while (isspace((unsigned char)str[init]))
		++init;
	if (str[init] == 0)
	{
		free(str);
		return strdup("");
	}
	end = strlen(str) - 1;
	while (end > init && isspace((unsigned char)str[end]))
		--end;
	trimed = ft_substr(str, init, end+1);
	free(str);
	return trimed;
}

bool	check_brackets(char *content)
{
	int curves = 0;
	int squares = 0;
	int	i = 0;

	while (content[i] != '\0')
	{
		if (content[i] == '(')
			curves++;
		else if (content[i] == ')')
			curves--;
		else if (content[i] == '[')
			squares++;
		else if (content[i] == ']')
			squares--;
		if (curves < 0 || squares < 0)
			break;
		i++;
	}
	if (curves != 0 || squares != 0)
		return 1;

	return 0;
}

bool isfunctionword(char *token)
{
	if (!strcmp(token, "sqrt")|| !strcmp(token, "exp")  ||
		!strcmp(token, "log") || !strcmp(token, "ln")   ||
		!strcmp(token, "cos") || !strcmp(token, "acos") ||
		!strcmp(token, "sin") || !strcmp(token, "asin") ||
		!strcmp(token, "tan") || !strcmp(token, "atan") ||
		!strcmp(token, "abs") || !strcmp(token, "deg")  ||
		!strcmp(token, "rad"))
			return 1;

	return 0;
}

void	free_tokens(t_token **token_list)
{
	t_token	*current_node;
	t_token	*next_node;

	if (token_list && *token_list)
	{
		current_node = *token_list;
		while (current_node != NULL)
		{
			next_node = current_node->next;
			if (current_node->token != NULL)
				free(current_node->token);
			if (current_node->content != NULL)
				free(current_node->content);
			free(current_node);
			current_node = next_node;
		}
		*token_list = NULL;
	}
}
