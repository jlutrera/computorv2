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

static char	*doubletostr(double d)
{
	char *aux;

	aux = malloc(100);
	if (!aux)
		exit(EXIT_FAILURE);

	if (isinteger(d))
		sprintf(aux,"%0.0f", d);
	else
		sprintf(aux, "%.2f", d);

	return aux;
}

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
		if (*e)
			printf_error("Divison by zero is not defined", NULL, -1);
		else
			result = a / b;
	}
	else if (c == '^')
	{
		*e = (!isinteger(b) || (a == 0 && b <= 0));
		if (!isinteger(b))
			printf_error("^ is only defined for integers", NULL, -1);
		else if (a == 0 && b <= 0)
			printf_error("^ is not defined for 0^0 and 0^(-k)", NULL, -1);
		else
			result = ft_power(a, b);
	}
	else if (c == '%')
	{
		*e = (!(isinteger(a) && isinteger(b)) || b <= 0 || a < 0);
		if (!(isinteger(a) && isinteger(b)))
			printf_error("% is only defined for integer numbers", NULL, -1);
		else if (b == 0)
			printf_error("% by zero is not defined", NULL, -1);
		else if (a < 0 || b < 0)
			printf_error("% is only defined for positive numbers", NULL, -1);
		else
			result = ft_mod(a,b);
	}
	else if (c == '!')
	{
		*e = (a < 0);
		if (*e)
			printf_error("The factorial of negative numbers is not defined", NULL, -1);
		else
			result = ft_factorial(a);
	}

	return doubletostr(result);
}

static double	numberbefore(char *str, int i, int *start)
{
    double	a;
	char	*num_str;

	*start = i - 1;
    while (*start >= 0 && (isdigit(str[*start]) || str[*start] == '.'))
		--(*start);
	
	if (*start >= 0 && (str[*start] == '-' || str[*start] == '+') && !strchr("^!", str[i]) )
		--(*start);
	if (*start >= 0 && (isdigit(str[*start]) || str[*start] == '('))
		(*start)++;
	(*start)++;
    num_str = ft_substr(str, *start, i);

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
		*e = (a < 0);
		if (*e)
			printf_error("The even root of a negative number is not defined", NULL, -1);
		else
			result = ft_root(a, 2);
	}
	else if (!strcmp(aux, "exp"))
	{
		*e = (!isinteger(a));
		if (*e)
			printf_error("The exponential function is only defined for integers", NULL, -1);
		else
			result = ft_exp(a);
	}
	else if (!strcmp(aux, "log"))
	{
		*e = (a <= 0);
		if (*e)
			printf_error("The log function is only defined for x > 0", NULL, -1);
		else
			result = ft_log(a);
	}
	else if (!strcmp(aux, "ln"))
	{
		*e = (a <= 0);
		if (*e)
			printf_error("The ln function is only defined for x > 0", NULL, -1);
		else
			result = ft_ln(a);
	}
	else if (!strcmp(aux, "cos"))
	{
		result = ft_cos(a);
	}
	else if (!strcmp(aux, "acos"))
	{
		*e = (ft_abs(a) > 1);
		if (*e)
			printf_error("The acos function is only defined for -1 <= x <= 1", NULL, -1);
		else
			result = ft_acos(a);
	}
	else if (!strcmp(aux, "sin"))
	{
		result = ft_sin(a);
	}
	else if (!strcmp(aux, "asin"))
	{
		*e = (ft_abs(a) > 1);
		if (*e)
			printf_error("The asin function is only defined for -1 <= x <= 1", NULL, -1);
		else
			result = ft_asin(a);
	}
	else if (!strcmp(aux, "tan"))
	{
		*e = (a == 0);
		if (*e)
			printf_error("The tan function is not defined for PI/2 radians", NULL, -1);
		else
			result = ft_tan(a);
	}
	else if (!strcmp(aux, "atan"))
	{
		*e = (a == PI / 2);
		if (*e)
			printf_error("The atan function is not defined for PI/2 radians", NULL, -1);
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
			if ((pos2 && pos2 < pos) || !pos)
				pos = pos2;
			pos3 = strchr(s, '%');
			if ((pos3 && pos3 < pos) || !pos)
				pos = pos3;
			break;
		case 3:
			pos = strchr(s + 1, '+');
			pos2 = strchr(s + 1, '-');
			if ((pos2 && pos && pos2 < pos) || !pos)
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
			break;
	}
	return (s[i] != '\0');
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
			if (isfunctionword(aux))
			{
				k = i;
				if (strchr("-+", (*s)[i]))
					++i;

				while ((*s)[i] && strchr(".01234567890", (*s)[i]))
					++i;
				
				number = ft_substr(*s, k, i);
				if (k != i && isdigit(number[strlen(number) - 1]))
				{
					result = operatefunction(aux, number, &e);
					if (!e)
					{
						if (v_calc) printf("   Function %s%s(%s) = %s%s\n", CYAN, aux, number, result, RESET);
						update_result(s, j, i, result);
						i = 0;
					}
					free(result);
				}
				free(number);
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

static void splitter(char *s, char **strn, char **strl)
{
	int 	i;
	int		j;
	char	*substr;


	i = 0;
	while (s[i])
	{
		j = i;
		while (s[i] && s[i] != '+' && s[i] != '-')
		{
			if (s[i] == '(')
			{
				while (s[i] && s[i] != ')')
					++i;
			}
			++i;
		}
		if (j !=i)
		{
			if (j > 0)
				--j;
			substr = ft_substr(s, j, i);
			if (onlynumbers(substr))
			{
				strcat(*strn, substr);
			}
			else
			{
				reduce(&substr);
				if (substr[0] != '-' && substr[0] != '+')
					strcat(*strl, "+");
				strcat(*strl, substr);
			}
			free(substr);
		}
		else if (s[i])
			++i;
	}
	if ((*strl)[0] == '+' && (*strn)[0] == '\0')
		*strl[0] = ' ';

}

static int detectbrackets(char **str)
{
	char 	*substr;
	int		i;
	int		b;
	int		start;

	i = 0;
	while ((*str)[i])
	{
		while ((*str)[i] && (*str)[i] != '(')
			++i;
		if ((*str)[i] == '(')
		{
			b = 1;
			start = ++i;
			while ((*str)[i] && b !=0)
			{			
				b += ((*str)[i] == '(');
				b -= ((*str)[i] == ')');
				if (b == 0)
				{

					substr = ft_substr(*str, start, i);
					
					if (v_calc) printf("+ Bracket: %s%s%s\n", CYAN, substr, RESET);
				
					if (detectbrackets(&substr) || lookupfunction(&substr))
					{
						free(substr);
						return 1;
					}

					if (!onlynumbers(substr) && !strchr(substr, '(')) 
					{
						transformexpression(&substr);
						update_result(str, start, i, substr);
					}
					else if (onlynumbers(substr))
					{
						if (thereareoperations(substr))
							calc(&substr);
						update_result(str, start-1, i+1, substr);
						resolvedoblesigne(str);
						i = 0;
					}

					free(substr);
					remove_spaces(*str);
					if (v_calc) printf("- Bracket: %s%s%s\n", CYAN, *str, RESET);
				}
				++i;
			}
		}
	}
	return lookupfunction(str);
}

static void splitter2(char *s, char **strn, char **strl)
{
	int 	i;
	int		j;
	char	*substr;


	i = 0;
	while (s[i])
	{
		j = i;
		
		while (s[i] && s[i] != '*' && !isalpha(s[i]))
		{
			if (s[i] == '(')
			{
				while (s[i] && s[i] != ')')
					++i;
			}
			++i;
		}
		
		if (j !=i)
		{
			substr = ft_substr(s, j , i);

			if (onlynumbers(substr))
			{
				if ((*strn)[0] != '\0')
					strcat(*strn, "*");
				strcat(*strn, substr);
			}
			else
			{
				if (j != 0 && s[j - 1] != '+' && s[j - 1] != '-' && (*strl)[0] != '\0')
					strcat(*strl, "*");
				strcat(*strl, substr);
			}
			free(substr);
		}
		if (isalpha(s[i]))
		{
			int k = i;
			while (s[i] && isalpha(s[i]))
				++i;
			substr = ft_substr(s, k, i);
			if (!isfunctionword(substr))
			{
				if (k != 0 && s[k - 1] != '+' && s[k - 1] != '-' && (*strl)[0] != '\0')
					strcat(*strl, "*");
			}
			else
			{
				free(substr);
				while(s[i] != ')')
					++i;
				++i;
				substr = ft_substr(s, k, i);
			}
			strcat(*strl, substr);
			free(substr);
		}
		if (s[i])
			++i;
	}
}

void reduce(char **str)
{
	printf("   Reducing %s(%s)%s\n", CYAN, *str, RESET);

	char 	*strn;
	char 	*strl;
	
	strn = (char *)calloc(strlen(*str)+1, sizeof(char));
	if (!strn)
		exit(EXIT_FAILURE);
	strl = (char *)calloc(strlen(*str)+2, sizeof(char));
	if (!strl)
		exit(EXIT_FAILURE);
	
	if (strchr(*str, '*'))
	{
		splitter2(*str, &strn, &strl);

		printf("   Reduced to %s%s%s and %s%s%s\n", CYAN, strn, RESET, CYAN, strl, RESET);
		calc(&strn);
	
		free(*str);
		*str = (char *)calloc(strlen(strn) + strlen(strl) + 3, sizeof(char));
		if (!*str)
			exit(EXIT_FAILURE);
		strcat(*str, strn);
		if (*strn)
			strcat(*str, "*");
		strcat(*str, strl);
	}
	free(strn);
	free(strl);
}

int transformexpression(char **str)
{
	char 	*strn;
	char 	*strl;
	int		e;
	
	strn = (char *)calloc(strlen(*str)+1, sizeof(char));
	if (!strn)
		exit(EXIT_FAILURE);
	strl = (char *)calloc(strlen(*str)+2, sizeof(char));
	if (!strl)
		exit(EXIT_FAILURE);
	splitter(*str, &strn, &strl);
	e = calc(&strn);

	if (!e)
	{
		free(*str);
		*str = (char *)calloc(strlen(strn) + strlen(strl) + 1, sizeof(char));
		if (!*str)
			exit(EXIT_FAILURE);
		strcat(*str, strn);
		strcat(*str, strl);
	}
	free(strn);
	free(strl);
	return e;
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

	op = 0;
	isnegative = (*str)[0] == '(' && (*str)[1] == '-' && onlynumbers(*str) && (strchr(*str, '!') || strchr(*str, '^'));
	if (isnegative)
	    (*str)[1] = '+';
	
	e = detectbrackets(str);
	if (!e)
		resolvedoblesigne(str);

	if (!onlynumbers(*str))
	{
		if (v_calc)
		{
			printf("\nProvisional result : %s%s%s\n\n", CYAN, *str, RESET);
			printf("*********************************************\n");
			printf("* Now, I'm trying to reduce that expression *\n");
			printf("*********************************************\n");
		}
		calc_with_variables(str);
		return 0;
	}

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
					printf("   Operation %s %.2f %c", CYAN, a, (*str)[i]);
					if ( (*str)[i] != '!')
						printf(" %.2f", b);
					printf(" = %s %s\n", aux, RESET);
				}
				update_result(str, start, end, aux);
				op = 0;
			}
			free(aux);
		}		
	}
	return e;
}