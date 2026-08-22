# Bangla Compiler Front-End Demo

This is a simple compiler front-end project written in C++.

It shows three main compiler stages:

1. Lexer
2. Parser
3. Semantic Analyzer

The input program is hardcoded in `main.cpp`. When the program runs, it prints the lexer output, parser output, and semantic analyzer output in the terminal.

## Build

Run this command from the project folder:

```powershell
g++ -std=c++17 Lexer.cpp Parser.cpp PrettyPrinter.cpp SemanticAnalyzer.cpp Token.cpp main.cpp -o compiler_demo.exe
```

## Run

```powershell
.\compiler_demo.exe
```

## Output

The program prints:

- Hardcoded source program
- Lexer token stream
- Parser abstract syntax tree
- Semantic analyzer symbol table
- Error messages, if any

## File Structure

`main.cpp`  
Connects all compiler stages and runs the hardcoded demo input.

`Token.hpp`, `Token.cpp`  
Define token types and printable token names.

`Lexer.hpp`, `Lexer.cpp`  
Convert source code into tokens.

`AST.hpp`  
Defines the abstract syntax tree nodes.

`Parser.hpp`, `Parser.cpp`  
Convert tokens into an abstract syntax tree.

`SemanticAnalyzer.hpp`, `SemanticAnalyzer.cpp`  
Check variable declarations, scopes, and type rules.

`PrettyPrinter.hpp`, `PrettyPrinter.cpp`  
Print tokens, AST, and symbol table in a readable format.


## Example Input

```text
shonkha boyos = 18
lekha naam = "Rafi"

dekhao naam

jodi boyos >= 18 {
    dekhao boyos
}
```

