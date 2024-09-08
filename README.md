`computor-v2` is an instruction interpreter that, like a shell, retrieves user inputs for advanced computations. It will present itself as a simple command interpreter and must be able to address the following specifications (features detailed in the following pages):

- Support for the following mathematical types:
  - Rational numbers
  - Complex numbers (with rational coefficients)
  - Matrices
  - Polynomial equations of degrees less than or equal to 2
- Assignment of an expression to a variable by type inference
- Reassignment of an existing variable with an expression of another type
- Assignment of a variable to another variable (existing or not)
- Resolution of a mathematical expression with or without defined variable(s)
- Resolution of an equation of degree less than or equal to 2
- Operations between types, as much as possible
- Exit the program itself (by keyword, signal, keyboard shortcut, etc.)

If you want to see the calculation process, you have to write the flag `-v` after the programme name.
