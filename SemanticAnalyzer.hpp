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