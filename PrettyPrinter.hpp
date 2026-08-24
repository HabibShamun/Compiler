#pragma once

#include "AST.hpp"
#include "SemanticAnalyzer.hpp"
#include "Token.hpp"

#include <iosfwd>
#include <vector>

void printTokens(const std::vector<Token>& tokens, std::ostream& out);
void printAst(const Program& program, std::ostream& out);
void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out);
