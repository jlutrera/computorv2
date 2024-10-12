/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:50:57 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/01 13:50:57 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_H
# define COMPUTOR_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include <termios.h>


# ifdef __APPLE__
#  include <mlx.h>
# else
#  include "../mlx_linux/mlx.h"
# endif

# define PI				3.141592653589793
# define E				2.718281828459045
# define LN10			2.302585092994046
# define LN2			0.693147180559945
# define TAYLOR_TERMS 	30
# define PRECISION		1e-15

# define GREEN 	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define CYAN	"\033[0;36m"
# define RED	"\033[1;31m"
# define RESET	"\033[0m"

typedef enum	e_token_type
{
	MATRIX,
	FUNCTION,
	RATIONAL,
	COMPLEX,
	ALGEBRAIC
} 	t_type;

typedef struct s_token
{
	char			*token;
	char			*content;
	t_type			type;
	struct s_token	*next;

}	t_token;

//Variable global para visualizar las operaciones
extern bool	v_calc;

// algebra.c
void	calc_with_variables(char **str);
int 	complex_calc(char **str);
// calc.c
int		calc(char **str);
int 	transformexpression(char **str);
// compute.c
int 	compute(char **content, t_token **list, char *token);
// delete.c
bool	delete(char *content, t_token **list);
// equations.c
void	solve_equality(double *reduced, char letter);
void	solve_linear(double *reduced, char letter);
void	solve_quadratic(double *reduced, char letter);
void	solve_cubic(double *reduced, char letter);
// help.c
bool	help(char *newinput);
// list.c
bool	list(char *content, t_token **list);
// maths.c
bool 	isinteger(double num);
double	ft_mod(double a, double b);
double	ft_power(double base, int exponent);
double	ft_root(double a, int n);
double	ft_abs(double x);
double	ft_factorial(int n);
double	ft_cos(double x);
double	ft_sin(double x);
double	ft_tan(double x);
double	ft_acos(double x);
double	ft_asin(double x);
double	ft_atan(double x);
double	ft_log(double x);
double	ft_ln(double x);
double	ft_exp(double x);
double	ft_deg(double x);
double	ft_rad(double x);
// parse.c
int		parse(char **input, t_token **token_list);
// plot.c
bool	plot(char *input);
// solve.c
int		solve(char *input);
// splash.c
void	splash(void);
// syntax.c
bool	syntax_error_token(char *token);
bool 	syntax_error_content(char *content, char *token);
bool	syntax_error_equation(char *str);
// types.c
bool 	types(char *content, t_token **list);
// utils.c
char	*ft_trim(char *str);
char	*ft_substr(const char *src, size_t i, size_t j);
void	remove_spaces(char *source);
bool	printf_error(char *msg, char *s, int i);
bool	bad_digits(char *s);
bool	bad_letters(char *s);
bool	check_brackets(char *content);
bool	isfunctionword(char *token);
void	free_tokens(t_token **token_list);
void	token_type(t_token **list);

#endif