#pragma once

#include "AST.hpp"

#include <iosfwd>
#include <string>

class PythonGenerator {
public:
    void generate(const Program& program, std::ostream& out) const;

private:
    static void generateStmt(const Stmt& stmt, std::ostream& out, int indent);
    static void generateBlock(const BlockStmt& block, std::ostream& out, int indent);
    static std::string generateExpr(const Expr& expr);
    static std::string quoteString(const std::string& value);
    static void writeIndent(std::ostream& out, int indent);
};
