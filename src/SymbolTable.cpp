#include "SymbolTable.hpp"
#include <iostream>

// Insertar un nuevo símbolo en la tabla
bool SymbolTable::insert(const SymbolEntry &entry)
{
    return false;
}

// Obtener tipo por id
int SymbolTable::getType(const std::string &id)
{
    return 0;
}

// Obtener dirección por id
int SymbolTable::getAddress(const std::string &id)
{
    return 0;
}

// Obtener categoría por id
Category SymbolTable::getCategory(const std::string &id)
{
    return Category::VAR;
}

// Obtener lista de parámetros por id
std::vector<int> SymbolTable::getParams(const std::string &id)
{
    return {};
}

// Imprimir la tabla para depuración
void SymbolTable::print() const
{
    return;
}
