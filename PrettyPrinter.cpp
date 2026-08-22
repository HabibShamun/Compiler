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
}