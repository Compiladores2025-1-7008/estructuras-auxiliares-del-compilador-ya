#include "TypeTable.hpp"
#include <iostream>
#include <stdexcept>

// Constructor: Actualmente no requiere inicialización compleja
TypeTable::TypeTable() {
    // Opcionalmente se podría inicializar con un tipo "inválido" o "error" en el índice 0
}

// Agrega un tipo básico a la tabla
int TypeTable::addBasicType(const std::string& name, int size) {
    TypeEntry entry;
    entry.id = static_cast<int>(types.size()); // El ID es el índice actual en el vector
    entry.kind = TypeKind::BASIC;
    entry.name = name;
    entry.size = size;
    // Inicializamos campos no usados con valores por defecto
    entry.elements = 0;
    entry.baseTypeId = -1;
    entry.structFields = nullptr;
    
    types.push_back(entry);
    return entry.id; // Retorna el ID asignado
}

// Agrega un tipo arreglo (ej: int[10])
int TypeTable::addArrayType(int baseTypeId, int elements) {
    // Validar que el tipo base exista
    if (baseTypeId < 0 || baseTypeId >= static_cast<int>(types.size())) {
        throw std::runtime_error("ID de tipo base inválido para arreglo");
    }

    TypeEntry entry;
    entry.id = static_cast<int>(types.size());
    entry.kind = TypeKind::ARRAY;
    
    // Construir nombre compuesto, ej: "int[10]"
    const TypeEntry& base = types[baseTypeId];
    entry.name = base.name + "[" + std::to_string(elements) + "]";
    
    // El tamaño total es el tamaño del tipo base multiplicado por la cantidad de elementos
    entry.size = base.size * elements;
    entry.elements = elements;
    entry.baseTypeId = baseTypeId;
    entry.structFields = nullptr;
    
    types.push_back(entry);
    return entry.id;
}

// Agrega un tipo estructura (struct)
int TypeTable::addStructType(const std::string& name, int size, SymbolTable* fields) {
    TypeEntry entry;
    entry.id = static_cast<int>(types.size());
    entry.kind = TypeKind::STRUCT;
    entry.name = name;
    entry.size = size;
    entry.elements = 0;
    entry.baseTypeId = -1;
    entry.structFields = fields; // Guarda la referencia a la tabla de campos del struct
    
    types.push_back(entry);
    return entry.id;
}

// Verifica si un ID existe en la tabla
bool TypeTable::exists(int id) const {
    return id >= 0 && id < static_cast<int>(types.size());
}

// Obtiene la entrada completa de un tipo
const TypeEntry& TypeTable::get(int id) const {
    if (!exists(id)) {
        throw std::out_of_range("ID de tipo fuera de rango");
    }
    return types[id];
}

// --- Implementación de Getters específicos ---

std::string TypeTable::getName(int id) const {
    return get(id).name;
}

int TypeTable::getSize(int id) const {
    return get(id).size;
}

int TypeTable::getNumElements(int id) const {
    return get(id).elements;
}

int TypeTable::getBaseType(int id) const {
    return get(id).baseTypeId;
}

SymbolTable* TypeTable::getStructFields(int id) const {
    return get(id).structFields;
}

// Imprime el contenido de la tabla para depuración
void TypeTable::print() const {
    std::cout << "=== Tabla de Tipos ===\n";
    std::cout << "ID\tNombre\tTam\tTipo\tElem\tBase\n";
    for (const auto& t : types) {
        std::string kindStr;
        switch (t.kind) {
            case TypeKind::BASIC: kindStr = "BASIC"; break;
            case TypeKind::ARRAY: kindStr = "ARRAY"; break;
            case TypeKind::STRUCT: kindStr = "STRUCT"; break;
        }
        std::cout << t.id << "\t" << t.name << "\t" << t.size << "\t" 
                  << kindStr << "\t" << t.elements << "\t" << t.baseTypeId << "\n";
    }
    std::cout << "======================\n";
}
