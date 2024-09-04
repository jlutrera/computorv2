/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:30:44 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/05 14:30:44 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"

bool	help(char *newinput)
{
	if (strcmp(newinput, "help"))
		return 0;

	printf("\n");

	printf("*******************************\n");
	printf("*       AVAILABLE COMMANDS    *\n");
	printf("*******************************\n");

	printf("\n1. Display this help message\n");
	printf("   -------------------------\n");
	printf("   Syntax: help\n");
	printf("\n");

	printf("2. Exit the program\n");
	printf("   ----------------\n");
	printf("   Syntax: exit\n");
	printf("\n");

	printf("3. Solve the given equation (linear, quadratic or cubic only)\n");
	printf("   ----------------------------------------------------------\n");
	printf("   Syntax: solve \"equation\"\n");
	printf("   Example: solve \"x^2 - 4x + 4 = 0\"\n");
	printf("\n");

	printf("4. Plot a function\n");
	printf("   ---------------\n");
	printf("   Syntax: plot \"function\"\n");
	printf("   Example: plot \"f(x) = sin(x)\"\n");
	printf("\n");

	printf("5. Delete stored variables\n");
	printf("   -----------------------\n");
	printf("   Syntax: delete [var1] [var2] ...\n");
	printf("   Note: If no arguments are provided, this command deletes ALL stored variables.\n");
	printf("   Examples:\n");
	printf("     delete x y z    (delete the variables x, y, and z)\n");
	printf("     delete x        (delete the variable x)\n");
	printf("     delete          (delete ALL stored variables)\n");
	printf("\n");

	printf("6. List the variable values\n");
	printf("   ------------------------\n");
	printf("   Syntax: list [var1] [var2] ...\n");
	printf("   Examples:\n");
	printf("     list x y z    (displays the value of x, y, and z)\n");
	printf("     list x        (display the value of x)\n");
	printf("     list          (display the value of ALL stored variables)\n");
	printf("\n");

	printf("7. Display the variable type\n");
	printf("   -------------------------\n");
	printf("   Syntax: type [var1] [var2] ...\n");
	printf("   Examples:\n");
	printf("     type x y z    (displays the type of x, y, and z)\n");
	printf("     type x        (display the type of x)\n");
	printf("     type          (display the types of ALL stored variables)\n");
	printf("\n");

	printf("8. Evaluate an expression\n");
	printf("   ----------------------\n");
	printf("   Syntax: expression = ?\n");
	printf("   Examples:\n");
	printf("     1 + 5 = ?     (computor will print: 6)\n");
	printf("     f(2) = ?      (computor will return the value of the function for 2)\n");
	printf("\n");

	printf("*******************************\n");
	printf("*         ASSIGNMENTS         *\n");
	printf("*******************************\n");

	printf("\n1. Set a variable\n");
	printf("   --------------\n");
	printf("   Syntax: var_name = expression\n");
	printf("   Note: The variable name must contain only alphabetic symbols.\n");
	printf("   Examples:\n");
	printf("     x = 5 + 3*x * 2*x^2\n");
	printf("     x = 5\n");
	printf("     x = 5 + 3i\n");
	printf("     x = sin(x) - 5%%6\n");
	printf("     x = [[3,4];[-3, 1.4]] ** [[1,0];[0,1]]\n");
	printf("\n");

	printf("2. Set a function\n");
	printf("   --------------\n");
	printf("   Syntax: function_name(var_name) = expression\n");
	printf("   Note: The function name must contain only alphabetic symbols.\n");
	printf("   Examples:\n");
	printf("     f(x) = x^2 + 3*x + 2\n");
	printf("     g(y) = sin(y)^2 + cos(y)^2\n");
	printf("\n");

	printf("***********************************\n");
	printf("*       OPERATIONS & FUNCTIONS    *\n");
	printf("********************************^**\n");

	printf("\n1. Arithmetic Operations\n");
	printf("   ---------------------\n");
	printf("   Supported operations include addition (+), subtraction (-), multiplication (*), division (/), exponentiation (^), factorial (!) and module (%%).\n");
	printf("   Examples:\n");
	printf("     5 + 3\n");
	printf("     4 * 2\n");
	printf("     2^3\n");
	printf("\n");

	printf("2. Elementary Functions\n");
	printf("   ---------------------\n");
	printf("   The following functions are supported:\n");
	printf("     sin(x)  :   Sine of x (x in radians)\n");
	printf("     cos(x)  :   Cosine of x (x in radians)\n");
	printf("     tan(x)  :   Tangent of x (x in radians)\n");
	printf("     ln(x)   :   Natural logarithm of x\n");
	printf("     log(x)  :   Logarithm base 10 of x\n");
	printf("     sqrt(x) :   Square root of x\n");
	printf("     exp(x)  :   Exponential function e^x\n");
	printf("     abs(x)  :   Absolute value of x\n");
	printf("     asin(x) :   Arcsine of x\n");
	printf("     acos(x) :   Arccosine of x\n");
	printf("     atan(x) :   Arctangent of x\n");
	printf("     deg(x)  :   Convert x from radians to degrees\n");
	printf("     rad(x)  :   Convert x from degrees to radians\n");
	printf("   Examples:\n");
	printf("     sin(3.14)\n");
	printf("     log(100)\n");
	printf("     sqrt(25)\n");
	printf("     exp(1)\n");
	printf("	 cos( rad(90) )\n");	
	printf("\n");

	printf("3. Matrix Operations\n");
	printf("   -----------------\n");
	printf("   Matrices are defined using square brackets [ ] and can be combined using arithmetic operations.\n");
	printf("   Syntax for matrix definition:\n");
	printf("     [[a,b]; [c,d]] - Defines a 2x2 matrix with elements a, b (first row), c, d (second row).\n");
	printf("   Examples:\n");
	printf("     A = [[1,2];[3,4]]\n");
	printf("     B = [[2,0];[1,3]]\n");
	printf("     C = A + B\n");
	printf("     D = A * B\n");
	printf("\n");

	printf("   Supported matrix operations include addition (+), subtraction (-), multiplication (*), matrix product (**), exponentiation (^) and division (/).\n");
	printf("   Example:\n");
	printf("     A = [[1,2];[3,4]]\n");
	printf("     B = A^2\n");
	printf("     x = A**A \n");
	printf("     D = A^(-1)\n");
	printf("     E = A / B\n");
	printf("\n");

	return 1;
}
