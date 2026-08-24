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
}#include "Lexer.hpp"
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

void printErrors(const std::string& title, const std::vector<std::string>& errors, std::ostream& out) {
    if (errors.empty()) {
        out << title << ": none\n";
        return;
    }

    out << title << ":\n";
    for (const std::string& error : errors) {
        out << "  " << error << '\n';
    }
}

bool runCompiler(bool showTokens, bool showAst, bool showSemantic, std::ostream& out) {
    out << "\n=== Hardcoded Source Program ===\n";
    out << DEMO_SOURCE << '\n';

    Lexer lexer(DEMO_SOURCE);
    std::vector<Token> tokens = lexer.tokenize();

    if (showTokens) {
        out << "\n=== Lexer Output: Token Stream ===\n";
        printTokens(tokens, out);
    }

    printErrors("Lexer errors", lexer.errors(), out);
    if (!lexer.errors().empty()) {
        return false;
    }

    return true;
}

}

int main() {
    return runCompiler(true, true, true, std::cout) ? 0 : 1;
}