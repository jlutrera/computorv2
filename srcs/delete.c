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

bool	delete(char *content, t_token **list)
{	
	if (strcmp(content, "delete"))
		return 0;

	//Es el comando delete pero no hay datos
	if (!*list)
	{
		printf("   Nothing to delete\n");
		return 1;
	}

	//Es el comando delete
	free_tokens(list);
	printf("   All data deleted !\n");
	return 1;
}
