# Bhasha

Bhasha is a small programming language and compiler project for writing programs with Bangla or Romanized keywords. The compiler is written in C++17. It checks Bangla source code, prints its tokens, syntax tree, symbol table, and three-address code, then generates Python that can be run.

The project includes a desktop practice window with editable examples, a quick language guide, and a Run button.

## What Bhasha supports

- Bangla and Romanized keywords, such as `দেখাও` and `dekhao`
- Bangla and ASCII digits
- Integer and text variables
- Variable declarations and assignments
- Printing values and text
- Arithmetic: `+`, `-`, `*`, `/`; `+` also joins text
- Comparisons: `==`, `!=`, `<`, `<=`, `>`, `>=`
- `if` / `else` conditions and `while` loops
- Braced blocks, nested scopes, semicolon or newline statement endings
- Single-line comments beginning with `//`
- Semantic checks for declaration, scope, and type errors

Bhasha currently supports the language constructs listed here. It does not run arbitrary Python, C++, or other programming languages. See [LANGUAGE_GRAMMAR.md](LANGUAGE_GRAMMAR.md) for the full grammar and semantic rules.

## Start the practice window on Windows

1. Make sure Python with Tkinter and `g++` are installed and available on `PATH`.
2. Double-click `Start Bhasha.bat`.
3. Select a practice program from the left. Edit the code in the center and click **Run** or press **F5**.
4. Read the program output or compiler errors in the bottom panel.

The practice window includes examples for greetings, variables, arithmetic, conditions, loops, and Bangla keywords. **Reset example** restores the selected example. **New**, **Open**, and **Save** let you work with your own `.bangla` files. You can run the text in the editor without saving it first; the window asks before replacing unsaved changes. The C++ compiler is built automatically the first time you run a program, and rebuilt when its source files change.

## Requirements

- Windows for the included `.bat` launcher and desktop practice window
- Python 3 with Tkinter
- A C++17 compiler; the launcher expects `g++` on `PATH`

The compiler can also be built and used from a terminal on systems with a C++17 compiler.

## Build the command-line compiler

Open PowerShell in the project folder and run:

```powershell
g++ -std=c++17 main.cpp Lexer.cpp parser.cpp PrettyPrinter.cpp SemanticAnalyzer.cpp Token.cpp PythonGenerator.cpp TAC.cpp -o compiler_demo.exe
```

## Compile and run a source file

Compile a `.bangla` program and choose the generated Python file path:

```powershell
.\compiler_demo.exe .\demo.bangla .\generated_program.py
```

The compiler prints the input, token stream, abstract syntax tree, symbol table, semantic diagnostics, and three-address code. It generates Python only if lexing, parsing, and semantic checks succeed. Run the generated program with UTF-8 output enabled so Bangla text displays correctly in Windows terminals:

```powershell
python -X utf8 .\generated_program.py
```

With no file argument, the command-line compiler uses the built-in example in `main.cpp` and writes `generated_program.py` in the current directory:

```powershell
.\compiler_demo.exe
```

## Language quick start

This example declares an integer and a text value, prints the name, and branches on the age:

```text
shonkha boyos = 18
lekha naam = "Rafi"

dekhao naam

jodi boyos >= 18 {
    dekhao boyos
} nahole {
    dekhao "অপ্রাপ্তবয়স্ক"
}
```

The same program can use Bangla keywords and Bangla digits:

```text
সংখ্যা বয়স = ১৮
লেখা নাম = "রাফি"

দেখাও নাম

যদি বয়স >= ১৮ {
    দেখাও বয়স
} নাহলে {
    দেখাও "প্রাপ্তবয়স্ক"
}
```

### Keywords

| Purpose | Romanized | Bangla |
| --- | --- | --- |
| Integer declaration | `shonkha` | `সংখ্যা` |
| Text declaration | `lekha` | `লেখা` |
| Print | `dekhao` | `দেখাও` |
| If | `jodi` | `যদি` |
| Else | `nahole` | `নাহলে` |
| While | `jotokhon` | `যতক্ষণ` |

### Variables, math, and loops

Declarations may include an initial value. Assignment updates a declared variable.

```text
shonkha count = 1

jotokhon count <= 5 {
    dekhao count
    count = count + 1
}
```

Statements end at a newline or semicolon. Text uses double quotes. Use `//` for a comment. Variables must be declared before use; assigned values and expressions must obey the integer and text type rules.

## Project files

- `main.cpp` — command-line entry point and compiler pipeline.
- `Token.hpp` / `Token.cpp` — token definitions and names.
- `Lexer.hpp` / `Lexer.cpp` — source text to tokens.
- `AST.hpp` — abstract syntax tree node definitions.
- `parser.hpp` / `parser.cpp` — tokens to an abstract syntax tree.
- `SemanticAnalyzer.hpp` / `SemanticAnalyzer.cpp` — variable, scope, and type checking.
- `TAC.hpp` / `TAC.cpp` — three-address code generation.
- `PythonGenerator.hpp` / `PythonGenerator.cpp` — Python code generation.
- `PrettyPrinter.hpp` / `PrettyPrinter.cpp` — readable compiler output.
- `bangla_ide.py` — Bhasha desktop practice window.
- `Start Bhasha.bat` — Windows launcher for the practice window.
- `LANGUAGE_GRAMMAR.md` — detailed grammar and semantic rules.
