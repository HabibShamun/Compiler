#pragma once

#include <memory>
#include <string>
#include <vector>

enum class ValueType {
    Int,
    String,
    Bool,
    Unknown
};

inline const char* valueTypeName(ValueType type) {
    switch (type) {
        case ValueType::Int: return "shonkha";
        case ValueType::String: return "lekha";
        case ValueType::Bool: return "bool";
        case ValueType::Unknown: return "unknown";
    }

    return "unknown";
}