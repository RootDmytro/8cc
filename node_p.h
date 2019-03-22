// node_p.h

AST_RO(int, kind)
AST_RW(Type *, ty)
AST_RW(SourceLoc *, sourceLoc)

// Char, int, or long
AST_RW(long, ival)

// Float or double
AST_RW(double, fval)
AST_RW(char *, flabel)

// String
AST_RW(char *, sval)
AST_RW(char *, slabel)

// Local/global variable
AST_RW(char *, varname)
// local
AST_RW(int, loff)
AST_RW(Vector *, lvarinit)
// global
AST_RW(char *, glabel)

// Binary operator
AST_RW(Node *, left)
AST_RW(Node *, right)

// Unary operator
AST_RW(Node *, operand)

// Function call or function declaration
AST_RW(char *, fname)
// Function call
AST_RW(Vector *, args)
AST_RW(Type *, ftype)
// Function pointer or function designator
AST_RW(Node *, fptr)
// Function declaration
AST_RW(Vector *, params)
AST_RW(Vector *, localvars)
AST_RW(Node *, body)

// Declaration
AST_RW(Node *, declvar)
AST_RW(Vector *, declinit)

// Initializer
AST_RW(Node *, initval)
AST_RW(int, initoff)
AST_RW(Type *, totype)

// If statement or ternary operator
AST_RW(Node *, cond)
AST_RW(Node *, then)
AST_RW(Node *, els)

// Goto and label
AST_RW(char *, label)
AST_RW(char *, newlabel)

// Return statement
AST_RW(Node *, retval)

// Compound statement
AST_RW(Vector *, stmts)

// Struct reference
AST_RW(Node *, struc)
AST_RW(char *, field)
AST_RW(Type *, fieldtype)
