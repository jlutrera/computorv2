# Mandatory
`computor-v2` is an instruction interpreter that, like a shell, retrieves user inputs for advanced computations. It will present itself as a simple command interpreter and must be able to address the following specifications:

- Support for the following mathematical types:
  - Rational numbers.
  - Complex numbers.
  - Matrices.
- Assignment of an expression to a variable by type inference.
- Reassignment of an existing variable with an expression of another type.
- Assignment of a variable to another variable.
- Resolution of a mathematical expression with defined variable(s)
- Resolution of an equation of degree less than or equal to 2
- Operations between types, as much as possible: +, - , *, /, %, ^, !, **
- Exit the program (typing the command `exit`).

&nbsp;  
 
# Bonus
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
- Delete the stored variables (`delete`).
- Computation with matrices: +, -, * (scalar product), / (inverse matrix), ^, ** (matrix product).
- Command to clear the screen (`clear`).
- History of inputs with results (pressing cursor keys).
- Viewing the calculating process or not (`visual on/off`).
- Help (`help`).

&nbsp;  

# Available commands

### 1. Display this help
*Syntax:* `help`

### 2. Exit the program
*Syntax:* `exit`

### 3. Print or not some intermediate calculations
*Syntax:* `visual on/off`

### 4. Clean the screen (as the `clear` command does)
*Syntax:* `clear`

### 5. Solve the given equation (linear, quadratic or cubic only)
*Syntax:* `solve "equation"`  
*Example:* `solve "x^2 - 4x + 4 = 0"`

### 6. Plot a function
*Syntax:* `plot "function"`  
*Note:* The function must be a valid mathematical expression and the variable has to be `x`.  
*Example:* `plot "sin(x)"`

### 7. Delete stored variables
*Syntax:* `delete`  
*Note:* This command deletes **ALL** stored variables.

### 8. List the variable values
*Syntax:* `list [var1] [var2] ...`  
*Examples:*
```
list x y z    # Display the value of x, y, and z
list x        # Display the value of x
list          # Display the value of ALL stored variables
```

### 9. Evaluate an expression
*Syntax:* `expression = ?`

*Examples:*
```
1 + 5 = ?     # computor will print: 6
f(2) = ?      # computor will return the value of the function for 2
```

&nbsp;  

# Assignments

### 1. Set a variable
*Syntax:* `var_name = expression`  
*Note:* The variable name must contain only alphabetic symbols.  
*Examples:*
```
x = 5 + 3*2 * 2*5^2
x = 5
x = 5 + 3i
x = sin(7) - 5%6
x = [[3,4];[-3, 1.4]] ** [[1,0];[0,1]]
```

### 2. Set a function
*Syntax:* `function_name(var_name) = expression`  
*Note:* The function name must contain only alphabetic symbols.  
*Examples:*
```
f(x) = x^2 + 3*x + 2
g(y) = sin(y)^2 + cos(y)^2
```
&nbsp;  

# Operations & functions

### 1. Arithmetic Operations
Supported operations:
- Addition (`+`)
- Subtraction (`-`)
- Multiplication (`*`)
- Division (`/`)
- Exponentiation (`^`)
- Factorial (`!`)
- Module (`%%`)

*Examples:*
```
5 + 3 = ?
4 * 2 = ?
2^3 = ?
```

### 2. Elementary Functions
The following functions are supported:
```
sin(x)  :   Sine of x (x in radians)
cos(x)  :   Cosine of x (x in radians)
tan(x)  :   Tangent of x (x in radians)
ln(x)   :   Natural logarithm of x
log(x)  :   Logarithm base 10 of x
sqrt(x) :   Square root of x
exp(x)  :   Exponential function e^x
abs(x)  :   Absolute value of x
asin(x) :   Arcsine of x
acos(x) :   Arccosine of x
atan(x) :   Arctangent of x
deg(x)  :   Convert x from radians to degrees
rad(x)  :   Convert x from degrees to radians
```

*Examples:*
```
sin(3.14)
log(100)
sqrt(25)
exp(1)
cos( rad(90) )
```

### 3. Matrix Operations
Matrices are defined using square brackets `[ ]` and can be combined using arithmetic operations.  

*Syntax for matrix definition:* 
```
[[a,b]; [c,d]] - Defines a 2x2 matrix with elements a, b (first row), c, d (second row).
```

*Examples:*
```
A = [[1,2];[3,4]]
B = [[2,0];[1,3]]
C = A + B
D = A * B
```

Supported matrix operations:
- Addition (`+`)
- Subtraction (`-`)
- Scalar multiplication (`*`)
- Matrix product (`**`)
- Exponentiation (`^`)
- Division (`/`)

*Examples:*
```
A = [[1,2];[3,4]]
B = A^2
x = A**A
D = A^(-1)
E = A / B
