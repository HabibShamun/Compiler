#include "Lexer.hpp"
#include "Parser.hpp"
#include "PrettyPrinter.hpp"
#include "SemanticAnalyzer.hpp"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace {

const char* DEMO_SOURCE = R"(shonkha boyos = 18
lekha naam = "Rafi"

dekhao naam

jodi boyos >= 18 {
    dekhao boyos
}
)";

bool runCompiler(bool showTokens, bool showAst, bool showSemantic, std::ostream& out) {
    out << "\n=== Hardcoded Source Program ===\n";
    out << DEMO_SOURCE << '\n';

    return true;
}

}

int main() {
    return runCompiler(true, true, true, std::cout) ? 0 : 1;
}