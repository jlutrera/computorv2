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

static int	findnumberoperation(int c, char *s)
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
		(*str)[0] = ' ';
		if ((*str)[2] != '+' && (*str)[2] != '-')
			(*str)[1] = '-';
		else
			(*str)[1] = ' ';
		i = 2;
		while ((*str)[i] && (*str)[i] != ')')
		{
			if ((*str)[i] == '+')
				(*str)[i] = '-';
			else if ((*str)[i] == '-')
				(*str)[i] = '+';
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
						return 2;
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

static bool anyvariables(char *s)
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

static void transform_in_power(char **str)
{
	int		i, j, p;
	char	*denominador, *exponente;

	while (strchr(*str, '/'))
	{
		j = strchr(*str, '/') - *str + 1;
		i = j;
		if ((*str)[i] != '(')
		{
			if ((*str)[i] == 'i')
				++i;
			else
				while (isdigit((*str)[i]) || (*str)[i] == '.')
					++i;
			denominador = ft_substr(*str, j, i);
		}
		else
		{
			int brackets = 1;
			while (brackets != 0)
			{
				++i;
				if ((*str)[i] == '(')
					++brackets;
				else if ((*str)[i] == ')')
					--brackets;
			}
			++i;
			denominador = ft_substr(*str, j, i);
		}
		if (strchr(*str, '('))
			calc_with_variables(&denominador);
		p = 0;
		if ((*str)[i] =='^')
			p = ++i;

		if (p != 0)
		{
			if ((*str)[i] != '(')
			{
				if ((*str)[i] == 'i')
					++i;
				else
					while (isdigit((*str)[i]) || (*str)[i] == '.')
						++i;
			}
			else
			{
				int brackets = 1;
				while ((*str)[i] && brackets != 0)
				{
					++i;
					if ((*str)[i] == '(')
						++brackets;
					else if ((*str)[i] == ')')
						--brackets;
				}
				++i;
			}
			exponente = ft_substr(*str, p, i);
		}
		else
			exponente = strdup("1");

		char *aux = (char *)calloc(100, sizeof(char));
		if (!aux) exit(EXIT_FAILURE);

		strncpy(aux, *str, j - 1);
		strcat(aux, "*");
		if (denominador[0] != '(')
			strcat(aux, "(");
		strcat(aux, denominador);
		if (denominador[0] != '(')
			strcat(aux, ")");
		strcat(aux, "^(-1*");
		strcat(aux, exponente);
		strcat(aux, ")");
		strcat(aux, *str+i);
		free(*str);
		*str = aux;
		free(denominador);
		free(exponente);
	}
}

static int check_complex_operators(char **str)
{
	int brackets;
	int i;
	int j;
	int start;
	int end;

	if (!strchr(*str, 'i'))
		return 0; 

	for (size_t i=0; i < strlen(*str); i++)
		if (isalpha((*str)[i])  && (*str)[i] != 'i')
			return 2;

	if (strchr(*str, '!'))
	{
		brackets = 0;
		i = strchr(*str, '!') - *str;
		if ((*str)[i - 1] == ')')
		{
			brackets = 1;
			--i;
		}
		j = i - 1;
		while (j > 0 && (isdigit((*str)[j]) || (*str)[j] == '.' || (*str)[j] == 'i' || (brackets && strchr("()+-*/%!", (*str)[j]))))
		{
			--j;
			if (brackets == 0)
				continue;
			if ((*str)[j] == '(')
				--brackets;
			else if ((*str)[j] == ')')
				++brackets;	
	
		}
		if (strchr("()+-*/%!", (*str)[j]))
			++j;
		char *num_str = ft_substr((*str), j, i+1);

		if (strchr(num_str, 'i'))
		{
			free(num_str);
			return printf_error("Factorial not allowed with complex numbers", *str, 0);
		}
		if (strchr(num_str, '-'))
		{
			free(num_str);
			return printf_error("Factorial not allowed with negative numbers", *str, 0);
		}
		if (strchr(num_str, '.'))
		{
			free(num_str);
			return printf_error("Factorial not allowed with floating point numbers", *str, 0);
		}
		double f = ft_factorial(strtod(num_str, NULL));
		free(num_str);
		char *aux = doubletostr(f);
		update_result(str, j, i+1, aux);
		free(aux);
	}
		
	if (strchr(*str, '%'))
	{
		brackets = 0;
		i = strchr(*str, '%') - *str;
		if ((*str)[i - 1] == ')')
		{
			brackets = 1;
			--i;
		}
		j = i - 1;
		while (j > 0 && (isdigit((*str)[j]) || (*str)[j] == '.' || (*str)[j] == 'i' || (brackets && strchr("()+-*/%!", (*str)[j]))))
		{
			--j;
			if (brackets == 0)
				continue;
			if ((*str)[j] == '(')
				--brackets;
			else if ((*str)[j] == ')')
				++brackets;	
	
		}
		if (strchr("()+-*/%!", (*str)[j]))
			++j;
		char *num_str_before = ft_substr((*str), j, i);
		start = j;
		if (strchr(num_str_before, 'i'))
		{
			free(num_str_before);
			return printf_error("Module not allowed with complex numbers", *str, 0);
		}
		if (strchr(num_str_before, '-'))
		{
			free(num_str_before);
			return printf_error("Module not allowed with negative numbers", *str, 0);
		}
		if (strchr(num_str_before, '.'))
		{
			free(num_str_before);
			return printf_error("Module not allowed with floating point numbers", *str, 0);
		}

		brackets = 0;
		i = strchr(*str, '%') - *str;
		if ((*str)[i + 1] == '(')
		{
			brackets = 1;
			++i;
		}
		j = i + 1;
		while ((*str)[j] != '\0' && (isdigit((*str)[j]) || (*str)[j] == '.' || (*str)[j] == 'i' || (brackets && strchr("()+-*/%!", (*str)[j]))))
		{
			j++;
			if (brackets == 0)
				continue;
			if ((*str)[j] == '(')
				++brackets;
			else if ((*str)[j] == ')')
				--brackets;
	
		}
		char *num_str_after = ft_substr(*str, i + 1, j);
		end = j;
		if (strchr(num_str_after, 'i'))
		{
			free(num_str_after);
			free(num_str_before);
			return printf_error("Module not allowed with complex numbers", *str, 0);
		}
		if (strchr(num_str_after, '-'))
		{
			free(num_str_after);
			free(num_str_before);
			return printf_error("Module not allowed with negative numbers", *str, 0);
		}
		if (strchr(num_str_after, '.'))
		{
			free(num_str_after);
			free(num_str_before);
			return printf_error("Module not allowed with floating point numbers", *str, 0);
		}
		double m = ft_mod(strtod(num_str_before, NULL), strtod(num_str_after, NULL));
		free(num_str_after);
		free(num_str_before);
		char *aux = doubletostr(m);
		update_result(str, start, end, aux);
		free(aux);
	}

	if (strchr(*str, '/'))
	{
		transform_in_power(str);
		calc(str);
	}

	while (strchr(*str, '^') && strchr(*str, 'i'))
	{
		calc_with_variables(str);
		if (strchr(*str, '^'))
			complex_power(str);
	}
	return 0;
}

static int detectbrackets(char **str)
{
	char 	*substr;
	int		i;
	int 	j;
	int		b1;
	int 	b2;
	int		start;
	int		e;

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

					if (v_calc) printf("%sBRACKET    <= %s%s%s\n", GREEN, CYAN, substr, RESET);
					if (detectbrackets(&substr) || lookupfunction(&substr))
					{
						free(substr);
						return 1;
					}
					e = check_complex_operators(&substr);
					if (e != 0)
					{
						free(substr);
						return e;
					}
	
					if (!anyvariables(substr))
						calc_with_variables(&substr);
					
					else
					{
						if (thereareoperations(substr))
							calc(&substr);
						if (start == 1 || (start > 1 && !isalpha((*str)[start - 2])))
						{
							--start;
							++i;
						}
					}
					update_result(str, start, i, substr);
					resolvedoblesigne(str);
					i = start + strlen(substr);

					free(substr);
					if (strchr(*str, '[') && strchr(*str, ')'))
					{
						i = strchr(*str, ')') - *str;
						(*str)[start - 1] = ' ';
						(*str)[i] = ' ';
						remove_spaces(*str);
					}
					if (v_calc) printf("%sBRACKET    => %s%s%s\n", GREEN, CYAN, *str, RESET);
				}
				++i;
			}
		}
	}
	return lookupfunction(str);
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

	isnegative = (*str)[0] == '(' && (*str)[1] == '-' && anyvariables(*str) && (strchr(*str, '!') || strchr(*str, '^'));
	if (isnegative)
	    (*str)[1] = '+';

	e = detectbrackets(str);
	if (e != 0)
		return (e % 2);
	
	resolvedoblesigne(str);
	
	if (!anyvariables(*str))
	{
		e = check_complex_operators(str);
		if (e != 0)
			return (e % 2);

		if (v_calc) printf("%sREDUCING    : %s%s%s", GREEN, CYAN, *str, RESET);
		bool temp = v_calc;
		v_calc = false;
		if (anyvariables(*str))
			calc(str);
		else
			calc_with_variables(str);
		v_calc = temp;
		if (v_calc) printf(" => %s%s%s\n", CYAN, *str, RESET);
		
		if (anyvariables(*str))
			calc(str);
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
		i = findnumberoperation(op, *str);
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
					printf("%sCALCULATING :%s %s => %s%.2f %c", GREEN, RESET, *str, CYAN, a, (*str)[i]);
					if ( (*str)[i] != '!')
						printf(" %.2f", b);
					printf(" = %s%s\n", aux, RESET);
				}
				update_result(str, start, end, aux);
				op = 0;
			}
			free(aux);
			if (e)
				break;
		}		
	}
	return e;
}