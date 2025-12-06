#pragma once
#include <vector>
#include <memory>
#include "SymbolTable.hpp"

class SymbolTableStack
{
private:
    std::vector<SymbolTable *> stack;

public:
    // Crea nuevo ámbito
    void pushScope();

    // Sale de un ámbito
    void popScope();

    // Sale el ámbito y retorna la referencia a la tabla de símbolos en la cima
    SymbolTable *popSymbolTable();

    // Insertar solo en tope
    bool insertTop(const SymbolEntry &entry);

    // Insertar solo en la base (ámbito global)
    bool insertBase(const SymbolEntry &entry);

    // Buscar solo en tope
    SymbolEntry *lookupTop(const std::string &id);

    // Buscar solo en la base
    SymbolEntry *lookupBase(const std::string &id);

    // Depuración
    SymbolTable *currentScope()
    {
        if (stack.empty())
            return nullptr;
        return stack.back();
    }

    SymbolTable *globalScope()
    {
        if (stack.empty())
            return nullptr;
        return stack.front();
    }

    size_t levels() const { return stack.size(); }
};