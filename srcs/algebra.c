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

static bool checkstr(char **str)
{
	bool specialoperators = false;
	bool letters = false;
	int i = 0;

	while ((*str)[i])
	{
		if ((*str)[i] == '%' || (*str)[i] == '/' || (*str)[i] == '!')
			specialoperators = true;
		if (isalpha((*str)[i]))
			letters = true;
		++i;
	}

	if (!letters)
	{
		calc(str);
		return false;
	}
	else if (specialoperators)
		return true;

	return false;
}

static int multiplybrackets(char **str, int i, int j, int k)
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

	printf("STRa = %s\n", stra);
	printf("STRb = %s\n", strb);
	if (checkstr(&stra) || checkstr(&strb))
	{
		free(stra);
		free(strb);
		return 1;
	}


	result = (char *)calloc((strlen(stra)+1) * (strlen(strb)+1) * 2, sizeof(char));
	if (!result)
	{
		printf_error("Memory allocation error", NULL, -1);
		exit(EXIT_FAILURE);
	}

	if (v_calc) printf("%s%s%s * %s%s%s = ", CYAN, stra, RESET, CYAN, strb, RESET);
	
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
				if (v_calc) printf(" + ");
			}
			if (v_calc) printf("%s*%s", stra1, strb1);

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
	
	if (v_calc) printf("\n");
	
	calc_with_variables(&result);
	// printf("STR_in  = %s\n", *str);
	// printf("RESULT  = %s\n", result);
	if (i2 < i)
	{
		char *temp = ft_substr(*str, j + 1, strlen(*str));
		// printf("TEMP    = %s\n", temp);
		if (strlen(*str) < i2 + strlen(result))
				*str = (char *)realloc(*str, i2 + strlen(result) + 2);

		bool flag = (*str)[i2 + strlen(result) + 2] == '*';
		if (flag) strcpy(*str + i2, "(");

		if (result[0] != '+' && result[0] != '-')
		{
			strcpy(*str + i2 + flag, "+");
			strcat(*str, result);
		}
		else
			strcpy(*str + i2, result);

		if (flag) strcat(*str, ")");

		strcat(*str, temp);
		// printf("STR_out = %s\n", *str);
		free(temp);
	}
	else
	{
		char *temp = ft_substr(*str, j2 + 1, strlen(*str));
		// printf("TEMP    = %s\n", temp);
		if (strlen(*str) < i + strlen(result))
				*str = (char *)realloc(*str, i + strlen(result) * 2);

		if ((*str)[i + 1] == '*' )
			strcat(*str, "(");

		if (result[0] != '+' && result[0] != '-')
		{
			strcpy(*str + i, "+");
			strcat(*str, result);
		}
		else
			strcpy(*str + i, result);

		if ((*str)[i2 + 1] == '*' )
			strcat(*str, ")");

		strcat(*str, temp);
		// printf("STR_out = %s\n", *str);
		free(temp);
	}
	free(result);
	return 0;
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
		while ( ( i == 0 || (i > 0 && !isalpha((*str)[i - 1])) ) )
		{
			j = whereistheclosingbracket(*str, i + 1);
			if ((i > 0 && (*str)[i - 1] == '*') || (*str)[j + 1] == '*' )
			{
				if (i > 0 && (*str)[i - 1] == '*')
					k = i - 1;
				else
					k = j + 1;
				if (multiplybrackets(str, i, j, k))
					return;
			}
			else if ( ((i > 0 && (*str)[i - 1] == '+') || i == 0 ) &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				addbrackets(str, i, j);

			else if ( (i > 0 && (*str)[i - 1] == '-') &&
				  ((*str)[j + 1] == '+' || (*str)[j + 1] == '-' || (*str)[j + 1] == '\0') )
				subtractbrackets(str, i, j);
			else
				break;
			i = strchr(*str, '(') - (*str);
		} 
	}
	
	transformexpression(str);
}
