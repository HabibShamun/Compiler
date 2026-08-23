#include "PrettyPrinter.hpp"

#include <iomanip>
#include <ostream>

namespace {

void indent(std::ostream& out, int depth) {
    for (int i = 0; i < depth; ++i) {
        out << "  ";
    }
}

}

void printTokens(const std::vector<Token>& tokens, std::ostream& out) {
}

void printAst(const Program& program, std::ostream& out) {
}

void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out) {
}#include "PrettyPrinter.hpp"

#include <iomanip>
#include <ostream>

namespace {

void indent(std::ostream& out, int depth) {
    for (int i = 0; i < depth; ++i) {
        out << "  ";
    }
}

}

void printTokens(const std::vector<Token>& tokens, std::ostream& out) {
    for (const Token& token : tokens) {
        out << std::setw(14) << tokenTypeName(token.type)
            << "  " << std::setw(12) << std::quoted(token.lexeme)
            << "  line " << token.line
            << ", col " << token.column << '\n';
    }
}

void printAst(const Program& program, std::ostream& out) {
}

void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out) {
}