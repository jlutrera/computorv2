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

static void multiplybrackets(char **str, int i, int j, int k)
{
	int 	i2;
	int 	j2;
	char* 	stra;
	char* 	strb;
	char*   result;

	if (k == j + 1)
	{
		++k;
		if ((*str)[k] == '(')
		{
			i2 = ++k;
			while ((*str)[k] != ')' && (*str)[k] != '\0')
				++k;
		}
		else
		{
			i2 = k;
			while ((*str)[k] != '-' && (*str)[k] != '+' && (*str)[k] != '\0')
				++k;
		}
		j2 = k;
	}
	else if (k == i - 1)
	{
		j2 = k;
		while ((*str)[k] != '-' && (*str)[k] != '+' && k > 0)
			--k;
		i2 = k;
	}
	stra = ft_substr(*str, i2, j2);
	strb = ft_substr(*str, i + 1, j);
	result = (char *)calloc((strlen(stra)+1) * (strlen(strb)+1) * 2, sizeof(char));
	if (!result)
	{
		printf_error("Memory allocation error", NULL, -1);
		exit(EXIT_FAILURE);
	}

	printf("[%s]*[%s] = ", stra, strb);
	
	int q = 0;
	int r = 0;

	while (stra[r] != '\0')
	{
		if (stra[r] == '+' || stra[r] == '-')
			++r;
		while (stra[r] != '\0' && stra[r] != '+' && stra[r] != '-')
			++r;

		char *stra1 = ft_substr(stra, q, r);
		int s = 0;
		int t = 0;
		while (strb[t] != '\0')
		{
			if (strb[t] == '+' || strb[t] == '-')
				++t;
			while (strb[t] != '\0' && strb[t] != '+' && strb[t] != '-')
				++t;
			char *strb1 = ft_substr(strb, s, t);

			if ((q > 0|| s > 0) && stra1[0] != '+' && stra1[0] != '-') 
			{
				strcat(result, "+");
				printf("+");
			}
			printf("%s*%s", stra1, strb1);

			strcat(result, stra1);
			strcat(result, "*");
			strcat(result, strb1);
			free(strb1);
			s = t;
			if (strb[s] == '+')
				++s;
		}
		free(stra1);
		q = r;
		if (stra[q] == '+')
			++q;
	}	
	free(stra);
	free(strb);
	printf("\n");
	calc_with_variables(&result);
	printf("STR_in  = %s\n", *str);
	printf("RESULT  = %s\n", result);
	if (i2 < i)
	{
		char *temp = ft_substr(*str, j + 1, strlen(*str));
		printf("TEMP    = %s\n", temp);
		if (result[0] != '+' && result[0] != '-')
		{
			strcpy(*str + i2, "+");
			strcat(*str, result);
		}
		else
		{
			if (strlen(*str) < i2 + strlen(result))
				*str = (char *)realloc(*str, i2 + strlen(result) + 1);
			strcpy(*str + i2, result);
		}
		strcat(*str, temp);
		printf("STR_out = %s\n", *str);
		free(temp);
	}
	else
	{
		char *temp = ft_substr(*str, j2 + 1, strlen(*str));
		printf("TEMP    = %s\n", temp);
		if (result[0] != '+' && result[0] != '-')
		{
			strcpy(*str + i, "+");
			strcat(*str, result);
		}
		else
		{
			if (strlen(*str) < i + strlen(result))
				*str = (char *)realloc(*str, i + strlen(result) + 1);
			strcpy(*str + i, result);
		}
		strcat(*str, temp);
		printf("STR_out = %s\n", *str);
		free(temp);
	}
	free(result);
}

static int whereistheclosingbracket(char *str, int i)
{
	int 	b;

	b = 1;
	while (b != 0)
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
	int 	k;
	char 	*newp;

	i = strchr(*str, '(') - (*str);
	if (i >= 0)
	{
		j = whereistheclosingbracket(*str, i + 1);
		
		newp = ft_substr(*str, i+1, j);
		if (strchr(newp, '('))
		{
			calc_with_variables(&newp);
			for (int k = 0; k < (int)strlen(newp); ++k)
				(*str)[i + 1 + k] = newp[k];
			for (int k =  (int)strlen(newp); k < j - i - 1; ++k)
				(*str)[i + 1 + k] = ' ';
			remove_spaces(*str);
		}
		free(newp);

		i = strchr(*str, '(') - (*str);
		while ( ( i == 0 || (i > 0 && !isalpha((*str)[i - 1])) ) &&
			(!strchr(*str, '^') && !strchr(*str, '!') && !strchr(*str, '%') && !strchr(*str, '/')) )
		{
			j = whereistheclosingbracket(*str, i + 1);
			if ((i > 0 && (*str)[i - 1] == '*') || (*str)[j + 1] == '*' )
			{
				if (i > 0 && (*str)[i - 1] == '*')
					k = i - 1;
				else
					k = j + 1;
				multiplybrackets(str, i, j, k);
			}
			else if ( ((i > 0 && (*str)[i - 1] == '+') || i == 0 ) &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				addbrackets(str, i, j);

			else if ( (i > 0 && (*str)[i - 1] == '-') &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				subtractbrackets(str, i, j);

			i = strchr(*str, '(') - (*str);
		} 
	}
	transformexpression(str);
}
