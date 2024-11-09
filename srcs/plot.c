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

static void draw(char *f)
{
	printf("Plotting %s%s%s\n", CYAN, f, RESET);
}

bool	plot(char *input)
{
	char*	k;
	char*	function;
	int		len;

	len = strlen(input);
	if (len < 5)
		return 0;
	
	k = ft_substr(input, 0, 4);
	if (strcmp(k, "plot") || input[4] != ' ')
	{
		free(k);
		return 0;
	}
	free(k);
	k = ft_substr(input, 5, len);
	remove_spaces(k);
	len = strlen(k);
	if (len < 3)
	{
		free(k);
		return printf_error("Function not found", NULL, -1);
	}
	if (k[0] != '\"')
	{
		free(k);
		return printf_error("Double quotes not found at the beginning.", NULL, -1);
	}
	if (k[len - 1] != '\"')
	{
		free(k);
		return printf_error("Double quotes not found at the end.", NULL, -1);
	}
	function = ft_substr(k, 1, len - 1);
	free(k);
	if (syntax_error_content(function, NULL))
	{
		free(function);
		return 1;
	}
	draw(function);
	free(function);
	return 1;
}
