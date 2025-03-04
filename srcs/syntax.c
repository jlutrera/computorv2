/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:50:28 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/01 13:50:28 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

static bool iscommandword(char *token)
{
	if (!strcmp(token, "list") || !strcmp(token, "exit")   ||
		!strcmp(token, "solve")|| !strcmp(token, "delete") ||
		!strcmp(token, "help") || !strcmp(token, "plot")   ||
		!strcmp(token, "clear")|| !strcmp(token, "visual"))
			return 1;

	return 0;
}

static int check_matrix_row(char *r)
{
	int	l;
	int i;
	int	c;
	int	j;

	l = strlen(r);
	if (check_brackets(r) || l < 3 || (r[0] != '[' || r[l-1] != ']'))
		return 0;

	if (r[l-2] == ',')
	{
		printf_error("Bad syntax", r, l-2);
		return -1;
	}
	if (r[1] == ',')
	{
		printf_error("Bad syntax", r, 1);
		return -1;
	}
	i = 1;
	c = 0;
	while (i < l-1)
	{
		j = i;
		while (!strchr(",]", r[i]))
			++i;
		char *word = ft_substr(r, j, i);
		if (!isanumber(word))
		{
			free(word);
			printf_error("Matrixes must contain only numbers", NULL, 0);
			return -1;
		}
		free(word);
		if (!strchr(",]", r[i]))
			return 0;
		++c;
		++i;
	}
	return c;  //devuelvo el número de elementos del vector = número de columnas
}

static bool	check_matrix(char *content)
{
	int		i;
	int 	j;
	int		columns;
	int 	temp_columns;
	int 	len;
	char	*r;

	i = 1;
	columns = 0;
	len = strlen(content);

	if (len < 5 || content[1] != '[') //como mínimo debe haber [[n]]
		return printf_error("Bad syntax", content, 1);

	while (content[i])
	{
		j = i;
		while (content[j] != '\0' && content[j] != ';')
			j++;
		if (content[j] == '\0')
			r = ft_substr(content, i, j-1);
		else
			r =ft_substr(content, i, j);
		if (columns == 0)
		{
			columns = check_matrix_row(r);
			free(r);
			if (columns == -1)
				return 1;
			if (columns == 0)
				return printf_error("Bad syntax", content, -1);
		}
		else
		{
			temp_columns = check_matrix_row(r);
			free(r);
			if (temp_columns == -1)
				return 1;
			if (columns != temp_columns)
				return printf_error("Diferent column size", NULL, -1);
		}
		i = j;
		if (content[j] != '\0')
			++i;
	}
	return 0;
}

static bool content_is_token(char *word, char *token, bool is_function)
{
	int	i;

	i = 0;
	while (word[i] != '\0')
	{
		if (token[i] != word[i])
			return false;
		++i;
	}
	return (is_function == (token[i] == '('));
}

bool	syntax_error_content(char *content, char *token) 
// Numbers 	: decimal, integer and complex numbers
// Variables: the name has the same rules as tokens
// Operators: it can be, +, -, *, /, %, ^, !, ** (matrix product)
// Functions: with variables, numbers, operators, and/or some elemental functions (sin, cos, tan, root, exp, log, ln, ....)
// Matrixes	: the format is [[x1,y1];[x2,y2]];......], where x and y are numbers

{
	int		i;
	int		j;
	int		m;
	int		last;
	char 	*subcontent;
	bool	coma;
	char 	*word;

	if (strlen(content) ==  0)
		return printf_error("Nothing to compute", NULL, -1);

	if (check_brackets(content))
		return printf_error("Bracket error", content, -1);
	 
	 if (bad_digits(content) || bad_letters(content))
		return 1;

	//Por facilitar programación, cambio internamente el operador ** por #
	i = 0;
	while (content[i] != '\0')
	{
		//Inicialmente, compruebo que no está ese símbolo
		if (content[i] == '#')
			return printf_error("Invalid symbol", content, i);

		//Cambio "**"" por "# " 
		if (content[i] == '*' && content[i+ 1] == '*')
		{
			content[i] = '#';
			content[i+1] = ' ';
		}
		i++;
	}
	
	remove_spaces(content);
	
	last = strlen(content) - 1;
	i = 0;
	while (content[i] != '\0')
	{
		if (isdigit(content[i]))
		{
			coma = false;
			while (content[i+1] != '\0' && strchr("0123456789.", content[i+1]))
			{
				if (content[i+1] == '.')
				{
					if (coma)
						return printf_error("Bad syntax", content, i+1);
					coma = true;
				}
				++i;
			}
			if (content[i] == '.')
				--i;
			if (i < last && !strchr("+-*/^%)!", content[i+1]) && !isalpha(content[i+1])) 
				return printf_error("Bad syntax", content, i+1);
		}
		
		else if (isalpha(content[i]))
		{
			j = i;
			// If it is a function name or variable, check if it is a command
			if (isalpha(content[i + 1]))
			{
				while (content[i] != '\0' && isalpha(content[i]))
					i++;
				if (isdigit(content[i]))
					return printf_error("Invalid name", content, i);
				word = ft_substr(content, j, i);
				if (iscommandword(word))
				{
					free(word);
					return printf_error("Reserved word", NULL, -1);
				}
				if (isfunctionword(word) && content[i] != '(')
				{
					free(word);
					return printf_error("Invalid function format", content, i - 1);
				}
				if (token && content_is_token(word, token, content[i] == '('))
				{
					free(word);
					return printf_error("Same name in asignament and computational part", content, i-1);
				}

				if (content[i] == '\0' || content[i] == '(')
					--i;
				free(word);
			}
			else
			{
				if (i < last && !strchr("+-*/%^!()#", content[i+1]))
					return printf_error("Bad syntax", content, i+1);
				if (token && ((strlen(token) == 1 && content[i] == token[0] && content[i+1] != '(') ||
					(strlen(token) > 1 && content[i] == token[0] && content[i+1] == token[1])))
					return printf_error("Same name on both sides of the assignment statement", content, i);
			}
		}
		
		else if (strchr("+-*/%^", content[i]))
		{
			if (strchr("*^/%", content[0]))
				return printf_error("A number before is needed", content, i);

			if (i == last)
				return printf_error("A number after is needed", content, i);

			if (strchr( "+-*/%^", content[i+1]))
				return printf_error("Two consecutive operators", content, i+1);
			
			if (strchr("%^", content[i]) && content[i+1] == '[')
				return printf_error("Operation cannot be performed", content, i);

			if (!isalpha(content[i+1]) && !strchr("0123456789([", content[i+1]))
				return printf_error("Bad syntax", content, i+1);
		}
		
		else if (content[i] == '!')
		{
			if (i < last && !strchr("+-*/%^)", content[i+1]))
				return printf_error("Factorial error", content, i+1);
			if (i == 0)
				return printf_error("A number before is needed", content, i);
		}
		
		else if (content[i] == '.' )
		{
			if (i == 0)
				return printf_error("There isn't integer part", content, i);
			if (!strchr("0123456789", content[i+1]))
				return printf_error("There aren't decimals", content, i+1);
		}
		
		else if (content[i] == ')')
		{
			if (i < last && !strchr("+-*/%^!)#", content[i+1]))
				return printf_error("Bad syntax", content, i+1);
		}
		
		else if (content[i] == '(')
		{
			if (!strchr("0123456789-+([", content[i+1]) && !isalpha(content[i+1]))
				return printf_error("Bad syntax", content, i+1);
		}
		
		else if (content[i] == '[')
		{
			if (content[i+1] != '[')
				return printf_error("Bad syntax", content, i);
			j = i;
			while (content[j] != '\0' && (content[j] != ']' || content[j+1] != ']'))
				++j;
			subcontent = ft_substr(content, i, j+2);
			m = check_matrix(subcontent);
			free(subcontent);
			if (m == 1)
				return 1;
			i = j;
		}
		
		else if (content[i] == ']')
		{
			if ( i < last && strchr("!%", content[i+1]))
				return printf_error("Operation can't be performed", content, i+1);
			if ( i < last && !strchr("*/+-^#)", content[i+1]))
				return printf_error("Bad syntax", content, i+1);
		}

		else if (content[i] == '#')
		{
			if (i == last)
				return printf_error("A matrix after is needed", content, i);
			if (content[i+1] != '[' && !isalpha(content[i+1]))
				return printf_error("Operator \"**\" can only be performed with matrixes", NULL, i);
		}

		else if (content[i] != ' ')
			return printf_error("Bad syntax", content, i);
		
		++i;
	}
	return 0;
}

bool	syntax_error_token(char *token)
//Reserved words:  exit, list, solve, sqrt, sin, cos, ....
//Only alphabetic symbols for the names
//  Functions:  function_name(variable_name)
//  Variables:  variable_name (NOT "i" = imaginary unit)
{
	int		i;
	char	*function;

	if (!token)
		return 0;

	if (iscommandword(token) || isfunctionword(token) || !strcmp(token, "i"))
		return printf_error("Invalid name", token, -1);

	i = -1;
	while (token[++i] != '\0')
	{
		if (token[i] == '(' && i > 0)
		{
			function = ft_substr(token, 0, i);
			if (iscommandword(function) || isfunctionword(function))
			{
				free(function);
				return printf_error("Invalid function name", NULL, -1);
			}
	
			free(function);
			++i;
			if (!isalpha(token[i]) || token[i] == 'i')
				return printf_error("Invalid character", token, i);

			++i;
			if (token[i] == ')' && token[i+1] == '\0')
				return 0;
			return printf_error("Bad variable name on function", token, -1);
		}
		else if (!isalpha(token[i]))
			return printf_error("Invalid character", token, i);
	}
	return 0;
}

bool	syntax_error_equation(char *str)
{
	int 	i;
	int 	equalSign;
	char 	letter;
	bool 	isPower;

	i = 0;
	equalSign = 0;
	letter = 0;
	isPower = 0;

	while (str[i] != '\0')
	{
		if (str[i] == '=')
		{
			equalSign++;
			if (equalSign > 1 || i == 0)
				return printf_error("There is an issue with equal symbol", str, i);
			if (str[i+1] == '\0')
				return printf_error("Unexpected equation ending", str, i);
			i++;
			isPower = 0;
		}
		else if (isalpha(str[i]))
		{
			if (!letter)
				letter = str[i];
			else if (letter != str[i])
				return printf_error("Only one variable name must be written", str, i);
			i++;
			if (str[i] != '^' && str[i] != '\0' && str[i] != '+' && str[i] != '-' && str[i] != '=')	
				return printf_error("Invalid symbol after variable", str, i);
			isPower = 0;
		}
		else if (str[i] == '^')
		{
			isPower = 1;
			if (str[i+1] == '\0')
				return printf_error("Invalid symbol found", str, i);
			i++;
			if (!isdigit(str[i]))
				return printf_error("Invalid symbol found", str, i);
		}
		else if (str[i] == '+' || str[i] == '-')
		{
			i++;
			if (str[i] == '\0')
				return printf_error("Unexpected equation ending", str, i-1);
			if (!isalpha(str[i]) && !isdigit(str[i]))
				return printf_error("Invalid symbol found", str, i);
			isPower = 0;
		}
		else if (str[i] == '*' && i != 0)
		{
			if (str[i+1] == '\0')
				return printf_error("Unexpected equation ending", str, i);
			i++;
			if (!isalpha(str[i]))
				return printf_error("It must be [number]*[variable]", str, i);
			isPower = 0;
		}
		else if (str[i] == '.')
		{
			if (i == 0 || !isdigit(str[i-1]))
				return printf_error("Invalid symbol found", str, i);
			if (isPower == 1)
				return printf_error("Degree has to be an integer", str, i);
			i++;
			if (!isdigit(str[i]))
				return printf_error("Invalid symbol found", str, i);
		}
		else if (isdigit(str[i]))
		{
			i++;	
		}
		else if (str[i] != '\0')
			return printf_error("Invalid symbol found", str, i);
	}

	if (equalSign == 0)
		return printf_error("Invalid syntax. Equal symbol not found", str, -1);

	if (i < 2)
		return printf_error("Invalid syntax. Equation not found", str, -1);

	return 0;
}
