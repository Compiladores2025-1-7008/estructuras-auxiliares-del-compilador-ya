#include "SymbolTable.hpp"
#include <iostream>

// Funciones auxiliares
namespace
{
    // Función para depuración: convierte categoría a string
    std::string categoryToString(Category c)
    {
        switch (c)
        {
        case Category::VAR:
            return "VAR";
        case Category::CONST:
            return "CONST";
        case Category::STRUCT:
            return "STRUCT";
        case Category::FUNCTION:
            return "FUNCTION";
        case Category::PARAM:
            return "PARAM";
        }
        return "UNKNOWN";
    }

    // Función auxiliar: obtiene referencia al símbolo o lanza error
    const SymbolEntry &getSymbol(const std::unordered_map<std::string, SymbolEntry> &table,
                                 const std::string &id)
    {
        // Busca el id del símbolo en la tabla
        auto it = table.find(id);
        if (it == table.end())
        {
            // Si no lo encuentra lanza una excepción personalizada
            throw SymbolNotFoundError(id);
        }
        // el iterador nos devuelve llave y valor, regresamos el valor (SymbolEntry)
        return it->second;
    }
}

// Insertar un nuevo símbolo en la tabla
bool SymbolTable::insert(const SymbolEntry &entry)
{
    // it es el iterador, inserted es bool que indica si se insertó
    auto [it, inserted] = table.emplace(entry.id, entry);
    return inserted;
}

/*
 Getters muy sencillos, obtienen el símbolo usando la función auxiliar y devuelven el campo solicitado del símbolo.
*/

// Obtener tipo por id
int SymbolTable::getType(const std::string &id)
{
    const auto &sym = getSymbol(table, id);
    return sym.typeId;
}

// Obtener dirección por id
int SymbolTable::getAddress(const std::string &id)
{
    const auto &sym = getSymbol(table, id);
    return sym.address;
}

// Obtener categoría por id
Category SymbolTable::getCategory(const std::string &id)
{
    const auto &sym = getSymbol(table, id);
    return sym.category;
}

// Obtener lista de parámetros por id
std::vector<int> SymbolTable::getParams(const std::string &id)
{
    const auto &sym = getSymbol(table, id);
    return sym.params;
}

/*
 * Imprimir la tabla para depuración
 * Notación:
 * id | type | category | address | [params]
 */
void SymbolTable::print() const
{
    std::cout << "Symbol Table:\n";
    for (const auto &pair : table)
    {
        const auto &entry = pair.second;
        std::cout << entry.id << " | "
                  << entry.typeId << " | "
                  << categoryToString(entry.category) << " | "
                  << entry.address << " | [";
        for (size_t i = 0; i < entry.params.size(); ++i)
        {
            std::cout << entry.params[i];
            if (i < entry.params.size() - 1)
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
}
