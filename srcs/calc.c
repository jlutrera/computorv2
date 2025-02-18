/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:33:02 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/21 13:33:02 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static char	*makeoperation(char c, double a, double b, int *e)
{
	double	result;

	result = 0;

	if (c == '+')
		result = a + b;
	else if (c  == '-')
		result = a - b;
	else if (c == '*')
		result = a * b;
	else if (c == '/')
	{
		*e = (b == 0);
		if (!*e)
			result = a / b;
		else if (!plotting)
			printf_error("Divison by zero is not defined", NULL, -1);
	}
	else if (c == '^')
	{
		*e = (a == 0 && b <= 0);
		if (!*e)
		{
			if (!isinteger(b))
				result = ft_powerfloat(a, b);
			else
				result = ft_power(a, b); 
		}
		else if (!plotting) 
			printf_error("^ is not defined for 0^0 and 0^(-k)", NULL, -1);
	}
	else if (c == '%')
	{
		*e = !isinteger(a) || !isinteger(b) || b <= 0 || a < 0;
		if (!*e)
			result = ft_mod(a,b);
		else if (!plotting && (!isinteger(a) || !isinteger(b)))
			printf_error("% is only defined for integer numbers", NULL, -1);
		else if (!plotting && b == 0)
			printf_error("% by zero is not defined", NULL, -1);
		else if (!plotting && (a < 0 || b <= 0))
			printf_error("% is only defined for positive numbers", NULL, -1);
	}
	else if (c == '!')
	{
		*e = a < 0 || !isinteger(a);
		if (!*e)
			result = ft_factorial(a);
		else if (!plotting && a < 0)
			printf_error("The factorial of negative numbers is not defined", NULL, -1);
		else if (!plotting && !isinteger(a))
			printf_error("The factorial is only defined for integers", NULL, -1);
	}

	return doubletostr(result);
}

static double	numberbefore(char *str, int i, int *start)
{
    double	a;
	char	*num_str;
	bool	brackets;

	brackets = (str[i - 1] == ')');
	i -= brackets;
	*start = i - 1;
    while (*start >= 0 && (isdigit(str[*start]) || str[*start] == '.'))
		--(*start);
	
	if (*start >= 0 && (str[*start] == '-' || str[*start] == '+') && !strchr("^!", str[i]) )
		--(*start);
	if (*start >= 0 && (isdigit(str[*start]) || str[*start] == ')'))
		(*start)++;
	(*start)++;
    num_str = ft_substr(str, *start, i);
	if (*start > 0 && str[*start - 1] == '(')
		--(*start);
    a = strtod(num_str, NULL);
	free(num_str);
	return a;
}

static double	numberafter(char *str, int i, int *end)
{
	double 	b;
	char	*num_str;
	bool	brackets;

	brackets = (str[i + 1] == '(');
	i += brackets;
	*end = i + 1;
	if (str[*end] == '-' || str[*end] == '+')
		++(*end);
	while (str[*end] && (isdigit(str[*end]) || str[*end] == '.'))
		++(*end);

	num_str = ft_substr(str, i + 1, *end);
	b = strtod(num_str, NULL);
	free(num_str);
	*end = *end + brackets;
	return b;
}

static char *operatefunction(char *aux, char *number, int *e)
{
	double	a;
	double	result;

	a = strtod(number, NULL);
	result = 0;

	if (!strcmp(aux, "sqrt"))
	{	
		if (a < 0)
		{
			if (!plotting) 
			{
				result = ft_root(-a, 2);
				return strcat(doubletostr(result),"i");
			}
		}
		else
			result = ft_root(a, 2);
	}

	else if (!strcmp(aux, "exp"))
	{
		*e = a > 220;
		if (!*e)
			result = ft_exp(a);
		else if (!plotting)
			printf_error("The exponential function is too large", NULL, -1);
	}
	else if (!strcmp(aux, "log"))
	{
		*e = (a <= 0);
		if (!*e)
			result = ft_log(a);
		else if (!plotting)
			printf_error("The log function is only defined for x > 0", NULL, -1);
	}
	else if (!strcmp(aux, "ln"))
	{
		*e = (a <= 0);
		if (!*e)
			result = ft_ln(a);
		else if (!plotting)
			printf_error("The ln function is only defined for x > 0", NULL, -1);		
	}
	else if (!strcmp(aux, "cos"))
	{
		result = ft_cos(a);
	}
	else if (!strcmp(aux, "acos"))
	{
		*e = (ft_abs(a) > 1);
		if (!*e)
			result = ft_acos(a);
		else if (!plotting)
			printf_error("The acos function is only defined for -1 <= x <= 1", NULL, -1);
	}
	else if (!strcmp(aux, "sin"))
	{
		result = ft_sin(a);
	}
	else if (!strcmp(aux, "asin"))
	{
		*e = (ft_abs(a) > 1);
		if (!*e)
			result = ft_asin(a);
		else if (!plotting)
			printf_error("The asin function is only defined for -1 <= x <= 1", NULL, -1);
	}
	else if (!strcmp(aux, "tan"))
	{
		*e = (a == PI / 2 || a == 3 * PI / 2);
		if (*e)
		{
			if (!plotting) printf_error("The tan function is not defined for PI/2 and 3*PI/2 radians", NULL, -1);
		}
		else
			result = ft_tan(a);
	}
	else if (!strcmp(aux, "atan"))
	{
		*e = (a == PI / 2 || a == 3 * PI / 2);
		if (*e)
		{
			if (!plotting) printf_error("The atan function is not defined for PI/2 and 3*PI/2 radians", NULL, -1);
		}
		else
			result = ft_atan(a);
	}
	else if (!strcmp(aux, "abs"))
	{
		result = ft_abs(a);
	}
	else if (!strcmp(aux, "deg"))
	{
		result = ft_deg(a);
	}
	else if (!strcmp(aux, "rad"))
	{
		result = ft_rad(a);
	}
	return doubletostr(result);
}

static int	findoperation(int c, char *s)
{
	char *pos;
	char *pos2;
	char *pos3;

	switch (c)
	{
		case 0:
			pos = strchr(s, '!');
			break;
		case 1:
			pos = strchr(s, '^');
			break;
		case 2:
			pos = strchr(s, '*');
			pos2 = strchr(s, '/');
			if (!pos || (pos2 && pos2 < pos))
				pos = pos2;
			pos3 = strchr(s, '%');
			if (!pos || (pos3 && pos3 < pos))
				pos = pos3;
			break;
		case 3:
			pos = strchr(s + 1, '+');
			pos2 = strchr(s + 1, '-');
			if (!pos || (pos2 && pos2 < pos))
				pos = pos2;
			break;
	}

	if (!pos)
		return (-1);
	return (pos - s);
}

static void update_result(char **str, int start, int end, char *aux)
{
	char 	*temp;
	
	temp = (char*)calloc(strlen(*str) + strlen(aux), sizeof(char));
	if (!temp)
		exit(EXIT_FAILURE);

	strncat(temp, *str, start);
	strcat(temp, aux);
	strcat(temp, *str+end);
	free(*str);
	*str = temp;
}

static bool thereareoperations(char *s)
{
	int i;

	i = -1;
	while (s[++i])
	{
		if (i > 0 && strchr("+-*/%^", s[i]))
			return true;
	}
	return false;
}

static void resolvedoblesigne(char **str)
{
	int	i;

	if 	( ((*str)[0] == '-' && (*str)[1] == '-') ||
		  ((*str)[0] == '+' && (*str)[1] == '+') )
	{
		i = -1;
		while (++i < (int)strlen(*str) - 1)
			(*str)[i] = (*str)[i+2];
		(*str)[i] = '\0';
	}
	else if ( ((*str)[0] == '+' && (*str)[1] == '-') ||
			  ((*str)[0] == '-' && (*str)[1] == '+') )
	{
		i = 0;
		(*str)[0] = '-';
		while (++i < (int)strlen(*str))
			(*str)[i] = (*str)[i+1];
	}
	else if ((*str)[0] == '+' && isdigit((*str)[1]))
	{
		i = -1;
		while (++i < (int)strlen(*str))
			(*str)[i] = (*str)[i+1];
	}
	else if ((*str)[0] == '-' && (*str)[1] == '(')
	{
		i = 1;
		(*str)[1] = ' ';
		while ((*str)[i] && (*str)[i] != ')')
		{
			if ((*str)[i] == '+')
				(*str)[i] = ' ';
			else if ((*str)[i] == '-')
			{
				(*str)[0] = ' ';
				(*str)[i] = '+';
			}
			++i;
		}
		if ((*str)[i] == ')')
			(*str)[i] = ' ';
		remove_spaces(*str);
	}
}

static int lookupfunction(char **s)
{
	int 	i;
	int 	j;
	int 	k;
	int		e;
	char	*aux;
	char 	*number;
	char 	*result;

	i = 0;
	e = 0;
	while ((*s)[i] && !e)
	{
		j = i;
		while ((*s)[i] && isalpha((*s)[i]))
			++i;
		if (j != i)
		{
			aux = ft_substr(*s, j, i);
		
			if ((*s)[i] == '(' && isfunctionword(aux))
			{
				if (v_calc) printf("   Function: %s%s%s", CYAN, aux, RESET);
				k = ++i;
				if (strchr("-+", (*s)[i]))
					++i;

				while ((*s)[i])
				{
					if ((*s)[i] == ')')
						break;
					if (isalpha((*s)[i]))
					{
						free(aux);
						if (v_calc) printf("\n");
						return 1;
					}
					++i;
				}
				
				number = ft_substr(*s, k, i);
				if (v_calc)  printf("(%s%s%s)", YELLOW, number, RESET);
				if (k != i && isdigit(number[strlen(number) - 1]))
				{
					result = operatefunction(aux, number, &e);
					if (!e)
					{
						if (v_calc) printf(" = %s%s%s", GREEN, result, RESET);
						if ((*s)[i] == ')')
							++i;
						update_result(s, j, i, result);
						i = 0;
					}
					free(result);
				}
				free(number);
				if (v_calc) printf("\n");
			}
			free(aux);
		}
		else
		 	++i;
	}
	return e;
}

static bool onlynumbers(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (isalpha(s[i]))
		{
			int j = i;
			while (s[i] && isalpha(s[i]))
				++i;
			char *sb = ft_substr(s, j, i);
			if (!isfunctionword(sb))
			{
				free(sb);
				return false;
			}
			free(sb);
		}
		if (s[i])
			++i;
	}
	return true;
}

static bool isanumber(char *s)
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

static bool thereareonlydigits(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (!isdigit(s[i]) && s[i] != '.' && s[i] != 'i')
			return false;
		++i;
	}
	return true;
}

static int detectbrackets(char **str)
{
	char 	*substr;
	int		i;
	int 	j;
	int		b1;
	int 	b2;
	int		start;

	if (isanumber(*str))
		return 0;

	i = 0;
	while ((*str)[i])
	{
		while ((*str)[i] && (*str)[i] != '(')
			++i;
		if ((*str)[i] == '(')
		{
			b1 = 1;
			start = ++i;
			while ((*str)[i] && b1 !=0)
			{			
				b1 = b1 + ((*str)[i] == '(') - ((*str)[i] == ')');
				if (b1 == 0)
				{

					substr = ft_substr(*str, start, i);
					while (substr[0] == '(' && substr[strlen(substr)-1] == ')')
					{
						j  = 1;
						b2 = 1;
						while (substr[j] && b2 != 0)
						{
							b2 = b2 + (substr[j] == '(') - (substr[j] == ')');
							++j;
						}
						if (substr[j] == '\0' && b2 == 0)
						{
							substr[0] = ' ';
							substr[strlen(substr)-1] = ' ';
							substr = ft_trim(substr);
						}
						else
							break;
					}

					if (v_calc) printf("%s+ BRACKET: %s%s%s\n", GREEN, CYAN, substr, RESET);
					if (detectbrackets(&substr) || lookupfunction(&substr))
					{
						free(substr);
						return 1;
					}

					if (!onlynumbers(substr) && !strchr(substr, '(')) 
					{
						calc_with_variables(&substr);
					}
					
					else if (onlynumbers(substr))
					{
						if (thereareoperations(substr))
							calc(&substr);
						if (start == 1 || (thereareonlydigits(substr) && start > 1 && !isalpha((*str)[start - 2])))
						{
							--start;
							++i;
						}
						update_result(str, start, i, substr);
						resolvedoblesigne(str);
						i = start + strlen(substr);
					}

					free(substr);
					if (strchr(*str, '[') && strchr(*str, ')'))
					{
						i = strchr(*str, ')') - *str;
						(*str)[start - 1] = ' ';
						(*str)[i] = ' ';
						remove_spaces(*str);
					}
					if (v_calc) printf("%s- BRACKET: %s%s%s\n", GREEN, CYAN, *str, RESET);
				}
				++i;
			}
		}
	}
	return lookupfunction(str);
}

static int check_complex_operators(char *str)
{
	if (!strchr(str, 'i'))
		return 0;
	if (strchr(str, '!'))
		return printf_error("Factorial not allowed with complex numbers", str, 0);
	if (strchr(str, '/'))
		return printf_error("Division not allowed with complex numbers", str, 0);
	if (strchr(str, '%'))
		return printf_error("Modulus not allowed with complex numbers", str, 0);
	if (strchr(str, '^'))
	{
		int i = strchr(str, '^') - str;
		if (strchr(str, 'i') - str > i)
			return  printf_error("Complex exponents are not allowed", str, strchr(str+i, 'i') - str);
		if (str[i+1] == '(' && str[i+2] == '-')
			return  printf_error("Negative exponents with complex base are not allowed", str, i+2);
		if (strchr(str, '.') - str > i)
			return  printf_error("Decimal exponents with complex are not allowed", str, strchr(str, '.') - str);
	}

	if (v_calc) printf("   Checking Complex operators in : %s%s%s (%sOK%s)\n", CYAN, str, RESET, GREEN, RESET);
	return 0;
}

int	calc(char **str)
{
	int		op;
	int		i;
	int		start;
	int		end;
	int 	e;
	double	a;
	double	b;
	char	*aux;
	bool	isnegative;

	isnegative = (*str)[0] == '(' && (*str)[1] == '-' && onlynumbers(*str) && (strchr(*str, '!') || strchr(*str, '^'));
	if (isnegative)
	    (*str)[1] = '+';

	e = detectbrackets(str);
	if (!e)
		resolvedoblesigne(str);

	if (!onlynumbers(*str))
	{
		if (v_calc) printf("\n%sREDUCING : %s%s%s\n", GREEN, CYAN, *str, RESET);
		if (strchr(*str, 'i') && check_complex_operators(*str))
			return 1;
		calc_with_variables(str);
		return 0;
	}
	
	if (strchr(*str, '['))
	{
		e = calc_with_matrices(str);
		if (strchr(*str, '['))
			return e;
	}
	if ((*str[0] == '(') && (*str)[strlen(*str)-1] == ')' && isanumber(*str))
	{
		(*str)[0] = ' ';
		(*str)[strlen(*str)-1] = ' ';
		*str = ft_trim(*str);
	}
	op = 0;
	while (op < 4 && !e)
	{
		i = findoperation(op, *str);
		if (i == -1)
			++op;
		else
		{
			a = numberbefore(*str, i, &start);
			if (isnegative)
			{
				a = -a;
				isnegative = false;
			}
			if (op == 0)
				end = i+1;
			else
				b = numberafter(*str, i, &end);

			aux = makeoperation((*str)[i], a, b, &e);
			if (!e)
			{
				if (v_calc) 
				{
					printf("   Calc    : %s%.2f %c", CYAN, a, (*str)[i]);
					if ( (*str)[i] != '!')
						printf(" %.2f", b);
					printf(" = %s%s\n", aux, RESET);
				}
				update_result(str, start, end, aux);
				if (v_calc) printf("   Result  : %s%s%s\n", CYAN, *str, RESET);
				op = 0;
			}
			free(aux);
			if (e)
				break;
		}		
	}
	return e;
}