`computor-v2` is an instruction interpreter that, like a shell, retrieves user inputs for advanced computations. It will present itself as a simple command interpreter and must be able to address the following specifications:

- Support for the following mathematical types:
  - Rational numbers.
  - Complex numbers (with rational coefficients).
  - Matrices.
- Assignment of an expression to a variable by type inference.
- Reassignment of an existing variable with an expression of another type.
- Assignment of a variable to another variable.
- Resolution of a mathematical expression with or without defined variable(s)
- Resolution of an equation of degree less than or equal to 2 (`solve "equation"`).
- Operations between types, as much as possible: +, - , *, /, %, ^, !, **
- Exit the program (typing the command `exit`).


BONUS
- Function curve display (`plot "expression"`).
- Added useful functions:
	- Exponential.
	- Square root.
	- Absolute value.
	- Trigonometric functions: sin, cos, tan, asin, atan, acos.
	- Natural and decimal logarithms.
- Angle convertion between radians and degrees.
- Function composition.
- Display the stored variables and their values (`list`).
- Display the stored variables and their types (`type`).
- Delete the stored variables (`delete`).
- Computation with matrices: +, -, *, / (inverse matrix), ^, ** (scalar product if vectors).
- Command to clear the screen (`clear`).
- History of valid commands with results (pressing cursor keys).
- Viewing the calculating process or not (`visual on/off`).
- Help (`help`).

