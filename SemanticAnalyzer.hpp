#pragma once

#include "AST.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct Symbol {
    std::string name;
    ValueType type;
    int declaredLine;
    int scopeDepth;
};

class SymbolTable {
public:
    SymbolTable();

    void enterScope();
    void exitScope();
    bool declare(const std::string& name, ValueType type, int line);
    std::optional<Symbol> lookup(const std::string& name) const;
    bool existsInCurrentScope(const std::string& name) const;

    const std::vector<Symbol>& declarations() const;

private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes_;
    std::vector<Symbol> declarations_;
};

class SemanticAnalyzer {                                  
public:
    void analyze(const Program& program);                 

    const std::vector<std::string>& errors() const;       
    const SymbolTable& symbols() const;                    

private:
    SymbolTable symbols_;                                   
    std::vector<std::string> errors_;      
    
    void analyzeStmt(const Stmt& stmt);
    void analyzeBlock(const BlockStmt& block);
    ValueType typeOf(const Expr& expr);

    void addError(int line, const std::string& message);   
    static bool isComparison(BinaryOp op);
};
