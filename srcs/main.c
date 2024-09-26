/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:50:25 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/01 13:50:25 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

bool	v_calc = false;

static void	renewprompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	init_computor(t_token **t, char **prompt)
{
	splash();

	signal(SIGINT, &renewprompt);
	signal(SIGQUIT, SIG_IGN);

	*t = NULL;

	*prompt = "\001\x1b[33m\002computor ->\001\x1b[0m\002 ";
	rl_set_prompt(*prompt);
}

int	main(int argc, char **argv)
{
	t_token	*token_list;
	char	*input;
	char	*prompt;
	int		finish;

 	if (argc > 2)
	{
		printf("Too many arguments\n");
		return (1);
	}
	
	if (argc == 2)
	{
		if (!strcmp(argv[1], "-v"))
			v_calc = true;
		else
		{
			printf("Invalid argument\n");
			return (1);
		}
	}

	init_computor(&token_list, &prompt);
	finish = 0;
	while (!finish)
	{	
		input = readline(prompt);
		if (input)
		{
			if (*input)
				finish = parse(&input, &token_list);
			free(input);
		}
	}

	return (0);
}
