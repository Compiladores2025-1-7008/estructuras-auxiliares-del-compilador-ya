#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>

enum class Category
{
    VAR,
    CONST,
    STRUCT,
    FUNCTION,
    PARAM
};

/*
 * Definición de una expeción personalizada para un mejor manejo de errores
 */
class SymbolNotFoundError : public std::runtime_error
{
public:
    explicit SymbolNotFoundError(const std::string &id)
        : std::runtime_error("Symbol not found: " + id) {}
};

// Recordatorio de cómo se ve la tabla de símbolos:
// id | tipo | categoría | dirección | lista de parámetros
struct SymbolEntry
{
    std::string id;
    int typeId;
    Category category;
    int address;
    std::vector<int> params;
};

class SymbolTable
{
private:
    std::unordered_map<std::string, SymbolEntry> table;

public:
    // insert va a regresar regresa false si ya existía el id, true si se insertó correctamente
    bool insert(const SymbolEntry &entry);

    // -----------------------------------------
    // Consultas individuales simples
    // -----------------------------------------
    // Devuelve el tipo asociado al id
    int getType(const std::string &id);

    // Devuelve la dirección asociada al id
    int getAddress(const std::string &id);

    // Devuelve la categoría asociada al id
    Category getCategory(const std::string &id);

    // Devuelve la lista de parámetros asociada al id
    std::vector<int> getParams(const std::string &id);

    // -----------------------------------------
    // Consulta completa (si necesitas todos los datos)
    // -----------------------------------------
    const SymbolEntry *lookup(const std::string &id) const
    {
        auto it = table.find(id);
        return (it != table.end()) ? &it->second : nullptr;
    }

    // Para imprimir/depurar
    void print() const;
};
