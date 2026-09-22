#include "Lexer.hpp"
#include "Parser.hpp"
#include "PrettyPrinter.hpp"
#include "PythonGenerator.hpp"
#include "SemanticAnalyzer.hpp"
#include "TAC.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
const char* DEMO_SOURCE = R"(সংখ্যা বয়স = ১৮
লেখা নাম = "Rafi"

দেখাও নাম

যদি বয়স >= ১৮ {
    দেখাও বয়স
}
)";

void printErrors(const std::string& title, const std::vector<std::string>& errors, std::ostream& out) {
    if (errors.empty()) { out << title << ": none\n"; return; }
    out << title << ":\n";
    for (const std::string& error : errors) out << "  " << error << '\n';
}

bool writePython(const Program& program, const std::string& path, std::ostream& out) {
    std::ofstream file(path, std::ios::binary);
    if (!file) { out << "Could not write Python output file: " << path << '\n'; return false; }
    PythonGenerator generator;
    generator.generate(program, file);
    out << "Generated executable Python source: " << path << '\n';
    return static_cast<bool>(file);
}

bool runCompiler(const std::string& source, const std::string& outputPath, std::ostream& out) {
    out << "\n=== Source Program ===\n" << source << '\n';
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    out << "\n=== Lexer Output: Token Stream ===\n";
    printTokens(tokens, out);
    printErrors("Lexer errors", lexer.errors(), out);
    if (!lexer.errors().empty()) return false;

    Parser parser(std::move(tokens));
    Program program = parser.parseProgram();
    out << "\n=== Parser Output: Abstract Syntax Tree ===\n";
    printAst(program, out);
    printErrors("Parser errors", parser.errors(), out);
    if (!parser.errors().empty()) return false;

    SemanticAnalyzer semantic;
    semantic.analyze(program);
    out << "\n=== Semantic Analyzer Output: Symbol Table ===\n";
    printSymbols(semantic.symbols().declarations(), out);
    printErrors("Semantic errors", semantic.errors(), out);
    if (!semantic.errors().empty()) return false;

    TACGenerator tac;
    out << "\n=== Three-Address Code ===\n";
    printTAC(tac.generate(program), out);
    out << "\n=== Python Target ===\n";
    return writePython(program, outputPath, out);
}

bool readFile(const std::string& path, std::string& contents) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    std::ostringstream buffer;
    buffer << file.rdbuf();
    contents = buffer.str();
    return true;
}
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::string source;
    std::string outputPath = "generated_program.py";
    if (argc == 1) source = DEMO_SOURCE;
    else {
        if (!readFile(argv[1], source)) {
            std::cerr << "Could not open source file: " << argv[1] << '\n';
            return 1;
        }
        if (argc >= 3) outputPath = argv[2];
    }
    return runCompiler(source, outputPath, std::cout) ? 0 : 1;
}
