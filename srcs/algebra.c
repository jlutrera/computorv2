/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algebra.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 20:45:48 by jutrera-          #+#    #+#             */
/*   Updated: 2024/09/06 20:45:48 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

char detect_variable(char *s)
{
	char c = 0;
	while (*s)
	{
		if (isalpha(*s))
		{
			if (c == 0)
				c = *s;
			else if (c != *s)
			{
				if (v_calc)
					printf("More than one variable\n");
				return 0;
			}
		}
		s++;
	}
	return c;
}

double *get_factors(char *s, int *g, char c)
{
	int i;
	int sign;
	double factor;
	int grade;
	double *factors;
	
	factors = (double *)calloc(100, sizeof(double));
	if (factors == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}

	i = 0;
	sign = 1;
	while (s[i])
	{
		//Determino el signo del coeficiente
		if (s[i] == '-')
		{
			sign = -1;
			i++;
		}
		else if (s[i] == '+')
		{
			sign = 1;
			i++;
		}

		//Determino el valor del coeficiente
		factor = 0.0;
		if (isdigit(s[i]) || s[i] == '.')
		{
			while (isdigit(s[i]))
			{
				factor =  factor * 10 + (double)(s[i] - '0');
				++i;
			}
			if (s[i] == '.')
			{
				double decimalpart = 0.0;
				int divisor = 1;
				while (isdigit(s[++i]))
				{
					decimalpart = decimalpart * 10 + (double)(s[i] - '0');
					divisor *= 10;
				}
				factor = factor + decimalpart/divisor;
			}
			factor *= sign;
		}
		else
			factor = (double)sign;

		//Determino el grado del coeficiente = posición del factor en el array factors[]
		if (s[i] == c)
		{
			if (s[++i] == '^')
			{
				grade = 0;
				while (isdigit(s[++i]))
					grade = grade * 10 + s[i] - '0';
			}
			else
				grade = 1;
		}
		else
			grade = 0;
		if (grade > *g)
			*g = grade;
		factors[grade] += factor;
	}
	return factors;
}

char *multiply(char *m1, char *m2, char c)
{
	int grade_max1 = 0;
	int grade_max2 = 0;

	double *factors1;
	factors1 = get_factors(m1, &grade_max1, c);
	double *factors2;
	factors2 = get_factors(m2, &grade_max2, c);

	double *factors_r = (double *)calloc(grade_max1+grade_max2+1, sizeof(double));
	if (factors_r == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	double r;
	for (int i=0; i<= grade_max1; i++)
	{
		for (int j=0; j <= grade_max2; j++)
		{
			 r = factors1[i] * factors2[j];
			 //Gestiona el caso de que haya números imaginarios
			 if (c != 'i')
				factors_r[i+j] += r;
			else
			{
				if ((i+j) % 4 == 0)
					factors_r[0] = factors_r[0] + r;
				else if ((i+j) % 4 == 1)
					factors_r[1] = factors_r[1] + r;
				else if ((i+j) % 4 == 2)
					factors_r[0] = factors_r[0] - r;
				else if ((i+j) % 4 == 3)
					factors_r[1] = factors_r[1] - r;
			}
		}
	}

	char *m = (char *)calloc(100, sizeof(char));
	if (m == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	
	for (int i=0; i<= (grade_max1+grade_max2); i++)
	{
		char temp[50];
		if (factors_r[i] == 0.0)
			continue;
		
		if (factors_r[i] > 0.0 && strlen(m) > 0)
			strcat(m, "+");
		if (i == 0)
		{
			if (factors_r[i] == -1.0)
				strcat(m, "-1");
			else if (factors_r[i] == 1.0)
				strcat(m, "1");
			else if (factors_r[i] == (int)factors_r[i])
			{
				sprintf(temp, "%d", (int)factors_r[i]);
				strcat(m, temp);
			}
			else
			{
				sprintf(temp, "%0.2f", factors_r[i]);
				strcat(m, temp);
			}
		}
		else
		{
			if (factors_r[i] == -1.0)
				strcat(m, "-");
			else if (factors_r[i] != 1.0 && factors_r[i] == (int)factors_r[i])
			{
				sprintf(temp, "%d", (int)factors_r[i]);
				strcat(m, temp);
			}
			else if (factors_r[i] != 1.0)
			{
				sprintf(temp, "%0.2f", factors_r[i]);
				strcat(m, temp);
			}
			sprintf(temp, "%c", c);
			strcat(m, temp);
			if (i > 1)
			{
				sprintf(temp, "%d", i);
				strcat(m, "^");
				strcat(m, temp);
			}
		}
	}
	free(factors1);
	free(factors2);
	free(factors_r);
	return m;
}

char *divide(char *m1, char *m2, char c)
{
	int grade_max1 = 0;
	int grade_max2 = 0;

	double *factors1;
	factors1 = get_factors(m1, &grade_max1, c);
	double *factors2;
	factors2 = get_factors(m2, &grade_max2, c);
	double *factors_r = (double *)calloc(grade_max1-grade_max2+1, sizeof(double));
	if (factors_r == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	
	int grade_div = grade_max1-grade_max2;
	int grade_top = grade_max1;
	while (grade_div >= 0)
	{
		double r = factors1[grade_top] / factors2[grade_max2];
		for (int j=0; j <= grade_max2; j++)
		{
			factors1[j+grade_div] -= r * factors2[j];
			factors_r[grade_div] = r;
		}
		grade_top--;
		grade_div--;
	}
	if (factors1[0] != 0)
	{
		free(factors1);
		free(factors2);
		free(factors_r);
		return "Division not exact";
	}
	char *m = (char *)calloc(100, sizeof(char));
	if (m == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	
	for (int i=0; i<= grade_max1 - grade_max2; i++)
	{
		char temp[50];
		if (factors_r[i] == 0.0)
			continue;
		
		if (factors_r[i] > 0.0 && strlen(m) > 0)
			strcat(m, "+");
		if (i == 0)
		{
			if (factors_r[i] == -1.0)
				strcat(m, "-1");
			else if (factors_r[i] == 1.0)
				strcat(m, "1");
			else if (factors_r[i] == (int)factors_r[i])
			{
				sprintf(temp, "%d", (int)factors_r[i]);
				strcat(m, temp);
			}
			else
			{
				sprintf(temp, "%0.2f", factors_r[i]);
				strcat(m, temp);
			}
		}
		else
		{
			if (factors_r[i] == -1.0)
				strcat(m, "-");
			else if (factors_r[i] != 1.0 && factors_r[i] == (int)factors_r[i])
			{
				sprintf(temp, "%d", (int)factors_r[i]);
				strcat(m, temp);
			}
			else if (factors_r[i] != 1.0)
			{
				sprintf(temp, "%0.2f", factors_r[i]);
				strcat(m, temp);
			}
			sprintf(temp, "%c", c);
			strcat(m, temp);
			if (i > 1)
			{
				sprintf(temp, "%d", i);
				strcat(m, "^");
				strcat(m, temp);
			}
		}
	}
	free(factors1);
	free(factors2);
	free(factors_r);
	return m;
}

char *algebraic_calc(char *s)
{
	int index_operation;
	int start;
	int end;
	char *m;
	char c = detect_variable(s);
	char *m1;
	char *m2;
	int i, j;

	if (c == 0)
		return s;
	//Copia de la cadena s sin espacios
	char *new_s = (char *)calloc(strlen(s)+1, sizeof(char));
	if (new_s == NULL)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	strcat(new_s, s);

	while (1)
	{
		int p = strchr(new_s, '*') - new_s;
		int d = strchr(new_s, '/') - new_s;

		// Si no hay multiplicaciones ni divisiones, devuelvo la cadena reducida = mutiplicada por 1
		if (p < 0 && d < 0)
		{
			m = multiply("1", new_s, c);
			free(new_s);
			if (m[0] == 0)
				m[0] = '0';
			return m;
		}

		//Cadenas auxiliares para almacenar los multiplicandos, o dividendo y divisor
		m1 = (char *)calloc(strlen(new_s), sizeof(char));
		if (m1 == NULL)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		m2 = (char *)calloc(strlen(new_s), sizeof(char));
		if (m2 == NULL)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}

		// Busca la primera multiplicación
		if ((p > 0 && d < 0) || (p > 0 && d > 0 && p < d) )
			index_operation = p;
		// Si no hay multiplicaciones, busca la primera división
		else if ((p < 0 && d > 0) || (p > 0 && d > 0 && d < p))
			index_operation = d;

		// Determina los índices del primer multiplicando
		if (new_s[index_operation - 1] == ')')
		{
			start = index_operation;
			while (new_s[start] != '(')
			{
				start--;
			}
			i = start;
			if (start > 0)
				--i;
			start++;
			end = index_operation - 2;
		}
		else
		{
			start = index_operation - 1;
			end = start;
			while (new_s[start] != '+' && new_s[start] != '-' && start > 0)
				start--;
			i = start;
		}
		strncat(m1, new_s+start, end - start + 1);

		// Determina los índices del segundo multiplicando
		if (new_s[index_operation + 1] == '(')
		{
			int bracket = 1;
			start = index_operation+1;
			end = start+1;
			while (bracket != 0)
			{
				if (new_s[end] == ')')
					bracket--;
				
				if (new_s[end] == '(')
					bracket++;
				end++;
			}
			--end;
			j = end;
			
		}
		else
		{
			start = index_operation;
			end = index_operation + 1;
			while (new_s[end] != '+' && new_s[end] != '-' && new_s[end] != '*' && new_s[end] != '/' && new_s[end] != '\0')
				end++;
			j = end-1;
		}
		strncat(m2, new_s+start+1, end-start-1);
		
		//Si el segundo multiplicando contiene una multiplicación o división
		// y está entre paréntesis, hago llamada recursiva
		if ((strchr(m2, '*') || strchr(m2, '/')) && new_s[index_operation + 1] == '(')
		{
			char *aux = algebraic_calc(m2);
			free(m2);
			m2 = aux;
		}
		
		//Realizo la operación
		if (new_s[index_operation] == '*')
		{
			m = multiply(m1, m2, c);
		}
		else
		{
			m = divide(m1, m2, c);
			if (strcmp(m, "Division not exact") == 0)
			{
				free(m1);
				free(m2);
				return new_s;
			}
		}

		//Reemplazo la operación que se ha hecho por el resultado
		char *temp = (char *)calloc(100, sizeof(char));
		if (temp == NULL)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		strncpy(temp, new_s, i);
		if (new_s[i] == '-' && new_s[i+1] == '(')
			strcat(temp, "-1*(");
		else if (new_s[j+1] == '*' || new_s[j+1] == '/')
			strcat(temp, "(");
		else if (i > 0 && m[0] != '-')
			strcat(temp, "+");
		strcat(temp, m);
		if ((new_s[i] == '-' && new_s[i+1] == '(') || new_s[j+1] == '*' || new_s[j+1] == '/')
			strcat(temp, ")");
		strcat(temp, new_s+j+1);

		free(m1);
		free(m2);
		free(m);
		free(new_s);
		new_s = temp;
	}
}

static void addbrackets(char **str, int i, int j)
{
	(*str)[i] = ' ';
	(*str)[j] = ' ';
	if (i > 0 && ((*str)[i + 1] == '-' || (*str)[i + 1] == '+'))		
			(*str)[i - 1] = ' '; 
	remove_spaces(*str);
}

static void subtractbrackets(char **str, int i, int j)
{
	(*str)[i] = ' ';
	(*str)[j] = ' ';
	if ((*str)[i + 1] == '-' || (*str)[i + 1] == '+')
		(*str)[i - 1] = ' ';
	while (i < j)
	{
		if ((*str)[i] == '+' )
			(*str)[i] = '-';
		else if ((*str)[i] == '-' )
			(*str)[i] = '+';
		++i;
	}
	remove_spaces(*str);
}

static int whereistheclosingbracket(char *str, int i)
{
	int 	b;

	b = 1;
	while (str[i] && b != 0)
	{
		b = b + (str[i] == '(') - (str[i] == ')');
		++i;
	}
	return --i;
}

void calc_with_variables(char **str)
{
	int 	i;
	int		j;
	char 	*newp;

	
	i = strchr(*str, '(') - (*str);
	if (i >= 0)
	{
		j = whereistheclosingbracket(*str, i + 1);
		
		newp = ft_substr(*str, i+1, j);
		
		if (v_calc) printf("   Calculating : %s%s%s\n", CYAN, newp, RESET);
		if (strchr(newp, '('))
		{
			calc_with_variables(&newp);
			remove_spaces(newp);
			
			for (int k = 0; k < (int)strlen(newp); ++k)
				(*str)[i + 1 + k] = newp[k];

			for (int k = (int)strlen(newp); k < j - i - 1; ++k)
				(*str)[i + 1 + k] = ' ';

			remove_spaces(*str);
		}
		free(newp);
		i = strchr(*str, '(') - (*str);
		while ( ( i == 0 || (i > 0 && !isalpha((*str)[i - 1])) ) )
		{
			j = whereistheclosingbracket(*str, i + 1);
			if ( ((i > 0 && (*str)[i - 1] == '+') || i == 0 ) &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )	
				addbrackets(str, i, j);

			else if ( (i > 0 && (*str)[i - 1] == '-') &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				subtractbrackets(str, i, j);
			
			else if ((*str)[j + 1] == '^')
			{
				char *exponent;
				char *base = ft_substr(*str, i, j+1);
				j += 2;
				int k = j;
				while (isdigit((*str)[k]) || (*str)[k] == '.')
					++k;
				exponent = ft_substr(*str, j, k);
				if (!strchr(base, '+') && !strchr(base, '-'))
				{
					base[0] = ' ';
					base[strlen(base) - 1] = ' ';
				}
				
				int num_exp = atoi(exponent);
				char *newstr = (char *)calloc(strlen(*str) * (num_exp + 2)+ 1, sizeof(char));
				while (num_exp > 1)
				{
					strcat(newstr, base);
					strcat(newstr, "*");
					--num_exp;
				}
				strcat(newstr, base);
		
				char *aux = (char *)calloc(100, sizeof(char));
				if (!aux)
					exit(EXIT_FAILURE);
				strncpy(aux, *str, i);
				strcat(aux, newstr);
				strcat(aux, *str + j + 1);

				free(*str);
				*str = aux;
				
				free(base);
				free(exponent);
				free(newstr);
				remove_spaces(*str);
				char *aux2 = algebraic_calc(*str);
				free(*str);
				*str = aux2;
			}
			i = strchr(*str+i+1, '(') - (*str);
		}
	}
	char *aux = algebraic_calc(*str);
	free(*str);
	*str = aux;
}
