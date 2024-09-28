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

// Función para desactivar la espera de la tecla Enter y capturar una única tecla sin mostrarla
char getch()
{
	struct termios oldt, newt;
	char 	ch;

	tcgetattr(STDIN_FILENO, &oldt);				// Guarda la configuración actual del terminal
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);			// Desactiva la espera de Enter y la visualización de teclas
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);	// Aplica la nueva configuración
	
	do
		ch = getchar();							// Captura una tecla
	while (ch != '\n' && ch != 'q');			// Sale del bucle si se pulsa Enter o 'q'

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);	// Restaura la configuración del terminal
	return ch;
}

bool help(char *newinput)
{
	const int LINES = 10;

	if (strcmp(newinput, "help") != 0)
		return 0;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);		// Obtener el tamaño de la terminal
	int lines_per_page = w.ws_row - LINES;			// Restar una fila para el prompt

	const char *help_text[] = {
		"\n*******************************",
		"*       AVAILABLE COMMANDS    *",
		"*******************************",
		"\n1. Display this help message",
		"   -------------------------",
		"   Syntax: help",
		"\n2. Exit the program",
		"   ----------------",
		"   Syntax: exit",
		"\n3. Solve the given equation (linear, quadratic or cubic only)",
		"   ----------------------------------------------------------",
		"   Syntax: solve \"equation\"",
		"   Example: solve \"x^2 - 4x + 4 = 0\"",
		"\n4. Plot a function",
		"   ---------------",
		"   Syntax: plot \"function\"",
		"   Example: plot \"f(x) = sin(x)\"",
		"\n5. Delete stored variables",
		"   -----------------------",
		"   Syntax: delete",
		"   Note: This command deletes ALL stored variables.",
		"\n6. List the variable values",
		"   ------------------------",
		"   Syntax: list [var1] [var2] ...",
		"   Examples:",
		"     list x y z    (displays the value of x, y, and z)",
		"     list x        (display the value of x)",
		"     list          (display the value of ALL stored variables)",
		"\n7. Display the variable type",
		"   -------------------------",
		"   Syntax: type [var1] [var2] ...",
		"   Examples:",
		"     type x y z    (displays the type of x, y, and z)",
		"     type x        (display the type of x)",
		"     type          (display the types of ALL stored variables)",
		"\n8. Evaluate an expression",
		"   ----------------------",
		"   Syntax: expression = ?",
		"   Examples:",
		"     1 + 5 = ?     (computor will print: 6)",
		"     f(2) = ?      (computor will return the value of the function for 2)",
		"\n*******************************",
		"*         ASSIGNMENTS         *",
		"*******************************",
		"\n1. Set a variable",
		"   --------------",
		"   Syntax: var_name = expression",
		"   Note: The variable name must contain only alphabetic symbols.",
		"   Examples:",
		"     x = 5 + 3*x * 2*x^2",
		"     x = 5",
		"     x = 5 + 3i",
		"     x = sin(x) - 5%6",
		"     x = [[3,4];[-3, 1.4]] ** [[1,0];[0,1]]",
		"\n2. Set a function",
		"   --------------",
		"   Syntax: function_name(var_name) = expression",
		"   Note: The function name must contain only alphabetic symbols.",
		"   Examples:",
		"     f(x) = x^2 + 3*x + 2",
		"     g(y) = sin(y)^2 + cos(y)^2",
		"\n***********************************",
		"*       OPERATIONS & FUNCTIONS    *",
		"********************************^**",
		"\n1. Arithmetic Operations",
		"   ---------------------",
		"   Supported operations include addition (+), subtraction (-), multiplication (*), division (/), exponentiation (^), factorial (!) and module (%%).",
		"   Examples:",
		"     5 + 3",
		"     4 * 2",
		"     2^3",
		"\n2. Elementary Functions",
		"   ---------------------",
		"   The following functions are supported:",
		"     sin(x)  :   Sine of x (x in radians)",
		"     cos(x)  :   Cosine of x (x in radians)",
		"     tan(x)  :   Tangent of x (x in radians)",
		"     ln(x)   :   Natural logarithm of x",
		"     log(x)  :   Logarithm base 10 of x",
		"     sqrt(x) :   Square root of x",
		"     exp(x)  :   Exponential function e^x",
		"     abs(x)  :   Absolute value of x",
		"     asin(x) :   Arcsine of x",
		"     acos(x) :   Arccosine of x",
		"     atan(x) :   Arctangent of x",
		"     deg(x)  :   Convert x from radians to degrees",
		"     rad(x)  :   Convert x from degrees to radians",
		"   Examples:",
		"     sin(3.14)",
		"     log(100)",
		"     sqrt(25)",
		"     exp(1)",
		"     cos( rad(90) )",
		"\n3. Matrix Operations",
		"   -----------------",
		"   Matrices are defined using square brackets [ ] and can be combined using arithmetic operations.",
		"   Syntax for matrix definition:",
		"     [[a,b]; [c,d]] - Defines a 2x2 matrix with elements a, b (first row), c, d (second row).",
		"   Examples:",
		"     A = [[1,2];[3,4]]",
		"     B = [[2,0];[1,3]]",
		"     C = A + B",
		"     D = A * B",
		"\n   Supported matrix operations include addition (+), subtraction (-), multiplication (*), matrix product (**), exponentiation (^) and division (/).",
		"   Example:",
		"     A = [[1,2];[3,4]]",
		"     B = A^2",
		"     x = A**A ",
		"     D = A^(-1)",
		"     E = A / B",
	    };

	int num_lines = sizeof(help_text) / sizeof(help_text[0]);
	int current_line = 0;

	for (int i = 0; i < num_lines; i++)
	{
		printf("%s\n", help_text[i]);
		current_line++;

		if (current_line >= lines_per_page)
		{
			printf("\033[7mPress Enter to continue, or 'q' to quit...\033[0m");
			char input = getch();		
			printf("\r                                                    \r");
			if (input == 'q')
				break;
			current_line = 0;  // Reinicia el conteo de líneas
		}
	}

	return 1;
}
