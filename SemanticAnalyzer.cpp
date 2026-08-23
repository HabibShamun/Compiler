#include "SemanticAnalyzer.hpp"

#include <sstream>


SymbolTable::SymbolTable() {
    enterScope();
}

void SymbolTable::enterScope() {
    scopes_.push_back({});
}

void SymbolTable::exitScope() {
    if (scopes_.size() > 1) {
        scopes_.pop_back();
    }
}

bool SymbolTable::declare(const std::string& name, ValueType type, int line) {
    if (existsInCurrentScope(name)) {
        return false;
    }

    Symbol symbol{name, type, line, static_cast<int>(scopes_.size()) - 1};
    scopes_.back()[name] = symbol;
    declarations_.push_back(symbol);
    return true;
}

std::optional<Symbol> SymbolTable::lookup(const std::string& name) const {
    for (auto scope = scopes_.rbegin(); scope != scopes_.rend(); ++scope) {
        const auto found = scope->find(name);
        if (found != scope->end()) {
            return found->second;
        }
    }

    return std::nullopt;
}

bool SymbolTable::existsInCurrentScope(const std::string& name) const {
    return !scopes_.empty() && scopes_.back().count(name) > 0;
}

const std::vector<Symbol>& SymbolTable::declarations() const {
    return declarations_;
}

const std::vector<std::string>& SemanticAnalyzer::errors() const {
    return errors_;
}

const SymbolTable& SemanticAnalyzer::symbols() const {
    return symbols_;
}

void SemanticAnalyzer::addError(int line, const std::string& message) {
    std::ostringstream out;
    out << "Line " << line << ": " << message;
    errors_.push_back(out.str());
}