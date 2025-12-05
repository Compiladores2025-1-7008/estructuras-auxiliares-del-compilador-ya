#pragma once
#include <string>
#include <vector>
#include <map>

class SymbolTable; // Declaración adelantada (Forward declaration) para evitar dependencias circulares

// Enumeración para distinguir los tipos de datos soportados
enum class TypeKind {
    BASIC,  // Tipos primitivos: int, float, void, etc.
    ARRAY,  // Arreglos: int[10], float[5], etc.
    STRUCT  // Estructuras definidas por el usuario
};

// Estructura que almacena toda la información de un tipo
struct TypeEntry {
    int id;             // Identificador numérico único
    TypeKind kind;      // Categoría del tipo (Básico, Arreglo, Estructura)
    std::string name;   // Nombre del tipo (ej: "int", "float[10]", "Persona")
    int size;           // Tamaño en bytes
    
    // Campos específicos para Arreglos
    int elements;       // Número de elementos (ej: 10 para int[10])
    int baseTypeId;     // ID del tipo base (ej: ID de 'int' para int[10])
    
    // Campos específicos para Estructuras
    SymbolTable* structFields; // Puntero a la tabla de símbolos que contiene los campos de la estructura
};

// Clase que administra la Tabla de Tipos
class TypeTable {
private:
    std::vector<TypeEntry> types; // Almacenamiento principal de los tipos. El índice del vector es el ID del tipo.

public:
    TypeTable();
    ~TypeTable() = default;

    // --- Métodos para creación de tipos ---
    
    // Agrega un tipo básico (int, float, void, etc.)
    int addBasicType(const std::string& name, int size);
    
    // Agrega un tipo arreglo basado en un tipo existente
    int addArrayType(int baseTypeId, int elements);
    
    // Agrega un tipo estructura con sus campos definidos en una tabla de símbolos
    int addStructType(const std::string& name, int size, SymbolTable* fields);

    // --- Consultas Generales ---
    
    // Verifica si un ID de tipo es válido
    bool exists(int id) const;
    
    // Obtiene la entrada completa del tipo (lanza excepción si no existe)
    const TypeEntry& get(int id) const;
    
    // --- Getters Específicos (Simplifican el acceso a propiedades) ---
    std::string getName(int id) const;
    int getSize(int id) const;
    int getNumElements(int id) const;      // Útil para arreglos
    int getBaseType(int id) const;         // Útil para arreglos
    SymbolTable* getStructFields(int id) const; // Útil para estructuras
    
    // Función auxiliar para depuración (imprime la tabla en consola)
    void print() const;
};
