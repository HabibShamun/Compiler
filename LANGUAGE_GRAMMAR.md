# Bangla Programming Language Grammar

This document describes the grammar currently implemented by the C++ compiler.
The grammar is written in Backus-Naur Form (BNF). Bengali keywords are the
primary language spelling; the Romanized alternatives are also accepted by the
lexer.

## 1. Lexical grammar

```bnf
<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
          | "০" | "১" | "২" | "৩" | "৪" | "৫" | "৬" | "৭" | "৮" | "৯"

<number> ::= <digit> | <digit> <number>

<identifier-start> ::= <ASCII-letter> | "_" | <Unicode-non-ASCII-character>
<identifier-part> ::= <identifier-start> | <digit>
<identifier> ::= <identifier-start> | <identifier-start> <identifier-rest>
<identifier-rest> ::= <identifier-part> | <identifier-part> <identifier-rest>

<string> ::= '"' <string-character-sequence> '"'
<string-character-sequence> ::= <empty>
                              | <string-character> <string-character-sequence>

<string-character> ::= any character except an unescaped '"' or newline
<escape> ::= "\\n" | "\\\"" | "\\\\"

<comment> ::= "//" <characters-until-newline>
```

`<number>` may contain ASCII and Bengali digits. The compiler normalizes
Bengali digits internally before generating TAC or Python code.

## 2. Keywords

```bnf
<integer-type> ::= "সংখ্যা" | "shonkha"
<string-type> ::= "লেখা" | "lekha"
<print-keyword> ::= "দেখাও" | "dekhao"
<if-keyword> ::= "যদি" | "jodi"
<else-keyword> ::= "নাহলে" | "nahole"
<while-keyword> ::= "যতক্ষণ" | "jotokhon"
```

## 3. Program and statements

```bnf
<program> ::= <terminator-list> <statement-list> <terminator-list> <end-of-file>

<statement-list> ::= <empty>
                   | <statement> <terminator-list> <statement-list>

<statement> ::= <declaration>
              | <assignment>
              | <print-statement>
              | <if-statement>
              | <while-statement>
              | <block>

<declaration> ::= <type-keyword> <identifier>
                | <type-keyword> <identifier> "=" <expression>

<type-keyword> ::= <integer-type> | <string-type>

<assignment> ::= <identifier> "=" <expression>

<print-statement> ::= <print-keyword> <expression>

<if-statement> ::= <if-keyword> <expression> <block>
                 | <if-keyword> <expression> <block> <terminator-list> <else-keyword> <block>

<while-statement> ::= <while-keyword> <expression> <block>

<block> ::= "{" <terminator-list> <statement-list> "}"

<terminator-list> ::= <empty>
                    | <terminator> <terminator-list>

<terminator> ::= <newline> | ";"
```

Statements may end with a newline or semicolon. A statement immediately before
`}` or the end of the file may omit an explicit terminator.

## 4. Expressions

The following productions encode operator precedence. Productions lower in the
list bind more tightly.

```bnf
<expression> ::= <equality>

<equality> ::= <comparison>
             | <equality> "==" <comparison>
             | <equality> "!=" <comparison>

<comparison> ::= <term>
               | <comparison> "<" <term>
               | <comparison> "<=" <term>
               | <comparison> ">" <term>
               | <comparison> ">=" <term>

<term> ::= <factor>
         | <term> "+" <factor>
         | <term> "-" <factor>

<factor> ::= <unary>
           | <factor> "*" <unary>
           | <factor> "/" <unary>

<unary> ::= <primary>
          | "-" <unary>

<primary> ::= <number>
            | <string>
            | <identifier>
            | "(" <expression> ")"
```

## 5. Semantic rules

The BNF describes syntax only. The semantic analyzer additionally enforces:

1. Variables must be declared before use.
2. A declaration initializer must match its declared type.
3. An assignment must match the declared variable type.
4. Arithmetic operators require integer operands, except string concatenation
   with `+`.
5. Equality comparisons require matching operand types.
6. Ordering comparisons (`<`, `<=`, `>`, `>=`) require integer operands.
7. `if` and `while` conditions must produce Boolean values.
8. A name may not be declared twice in the same scope.

## 6. Example program

```text
সংখ্যা বয়স = ১৮
লেখা নাম = "Rafi"

দেখাও নাম

যদি বয়স >= ১৮ {
    দেখাও বয়স
} নাহলে {
    দেখাও "অপ্রাপ্তবয়স্ক"
}
```
