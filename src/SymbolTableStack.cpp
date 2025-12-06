#include "SymbolTableStack.hpp"
#include <iostream>

// Crea una nueva tabla de símbolos y la coloca en el tope de la pila.
void SymbolTableStack::pushScope()
{
    SymbolTable *newTable = new SymbolTable();
    stack.push_back(newTable);
}

// Elimina la tabla del tope de la pila, pero no libera memoria.
void SymbolTableStack::popScope()
{
    if (!stack.empty())
    {
        stack.pop_back();
    }
}

// Quita el tope de la pila y regresa el puntero a la tabla.
SymbolTable *SymbolTableStack::popSymbolTable()
{
    if (stack.empty())
    {
        return nullptr;
    }

    SymbolTable *top = stack.back();
    stack.pop_back();
    return top;
}

// Inserta un símbolo únicamente en el tope, si la pila está vacía, regresa false.
bool SymbolTableStack::insertTop(const SymbolEntry &entry)
{
    if (stack.empty())
    {
        return false;
    }
    return stack.back()->insert(entry);
}

// Inserta un símbolo únicamente en la base, si la pila está vacía, regresa false.
bool SymbolTableStack::insertBase(const SymbolEntry &entry)
{
    if (stack.empty())
    {
        return false;
    }
    return stack.front()->insert(entry);
}

// Busca un símbolo únicamente en el tope.
SymbolEntry *SymbolTableStack::lookupTop(const std::string &id)
{
    if (stack.empty())
    {
        return nullptr;
    }

    const SymbolEntry *result = stack.back()->lookup(id);
    return const_cast<SymbolEntry *>(result);
}

// Busca un símbolo únicamente en el ámbito global (primer elemento).
SymbolEntry *SymbolTableStack::lookupBase(const std::string &id)
{
    if (stack.empty())
    {
        return nullptr;
    }

    const SymbolEntry *result = stack.front()->lookup(id);
    return const_cast<SymbolEntry *>(result);
}