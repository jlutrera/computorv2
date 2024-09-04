/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plot.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:37:55 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/05 21:37:55 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static int draw(char *f)
{
	printf("Representando la función %s\n", f);
	return 1;
}

static char	*get_function(char *content)
{
	int	i;
	int	len;

	i = 5;
	while (content[i] == ' ')
		++i;
	if (content[i] != '\"')
		return NULL;

	len = strlen(content);
	while (content[++i] == ' ');
	if (i == len - 1)
		return NULL;
	return ft_substr(content, i, len - 1);
}

bool	plot(char *input)
{
	char*	k;
	char*	function;
	int		len;

	len = strlen(input);
	if (len < 4)
		return 0;
	
	k = ft_substr(input, 0, 4);
	if (strcmp(k, "plot") || input[5] != ' ')
	{
		free(k);
		return 0;
	}
	if (len < 6)
	{
		free(k);
		return printf_error("Function not found", NULL, -1);
	}
	if (!strchr(input, '\"'))
	{
		free(k);
		return printf_error("Double quotes not found at the beginning.", NULL, -1);
	}
	if (input[len - 1] != '\"')
	{
		free(k);
		return printf_error("Double quotes not found at the end.", NULL, -1);
	}
	function = get_function(input);
	free(k);
	if (function != NULL)
		return draw(function);
	
	free(function);
	return printf_error("Bad syntax for \"plot\"", NULL, -1);
}
