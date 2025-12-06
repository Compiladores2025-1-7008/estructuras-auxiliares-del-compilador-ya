#pragma once
#include "TypeTable.hpp"
#include <string>
#include <stdexcept>
#include <unordered_map>

/**
 * Type Manager (Manejador de Tipos)
 * 
 * Responsabilidades:
 * 1. Determinar jerarquía y compatibilidad entre tipos
 * 2. Manejar conversiones implícitas (ampliación) y explícitas (reducción)
 * 3. Proveer funciones para análisis semántico
 * 
 * Referencia: Aho, Sethi, Ullman - "Compiladores: Principios, Técnicas y Herramientas"
 *             Capítulo 6: Análisis Semántico, Sección 6.3: Comprobación de Tipos
 */
class TypeManager {
private:
    const TypeTable* typeTable;

    /**
     * Obtiene la prioridad de un tipo 
     * Jerarquía: void(0) < bool(1) < char(2) < int(3) < float(4) < double(5)
     * 
     * @param typeId ID del tipo en la tabla de tipos
     * @return Valor de prioridad (mayor = tipo más amplio)
     * @throws std::runtime_error si el tipo no es básico
     */
    int getPriority(int typeId) const {
        const TypeEntry& type = typeTable->get(typeId);
        
        if (type.kind != TypeKind::BASIC) {
            throw std::runtime_error("Solo los tipos básicos tienen jerarquía definida");
        }
        
        if (type.name == "void") return 0;
        if (type.name == "bool") return 1;
        if (type.name == "char") return 2;
        if (type.name == "int") return 3;
        if (type.name == "float") return 4;
        if (type.name == "double") return 5;
        
        throw std::runtime_error("Tipo desconocido: " + type.name);
    }

    /**
     * Verifica si un tipo es numérico (puede participar en operaciones aritméticas)
     * Según el libro Aho: los tipos numéricos son char, int, float, double
     * 
     * @param typeId ID del tipo a verificar
     * @return true si es tipo numérico
     */
    bool isNumericType(int typeId) const {
        const TypeEntry& type = typeTable->get(typeId);
        if (type.kind != TypeKind::BASIC) return false;
        
        std::string name = type.name;
        if (name == "char") return true;
        if (name == "int") return true;
        if (name == "float") return true;
        if (name == "double") return true;
        
        return false;
    }

public:
    /**
     * Constructor
     * 
     * @param tt Referencia a la tabla de tipos del compilador
     */
    TypeManager(const TypeTable& tt) : typeTable(&tt) {}

    /**
     * max - Obtiene el tipo de mayor jerarquía
     * Para operaciones aritméticas, se usa el tipo más amplio 
     * 
     * @param t1 ID del primer tipo
     * @param t2 ID del segundo tipo
     * @return ID del tipo con mayor jerarquía
     * @throws std::runtime_error si tipos no son compatibles
     */
    int max(int t1, int t2) const {
        if (t1 == t2) {
            return t1;
        }
        if (!isNumericType(t1) || !isNumericType(t2)) {
            throw std::runtime_error("Los tipos no son numéricos, por lo que no tienen una jerarquía comparable");
        }
        
        int priority1 = getPriority(t1);
        int priority2 = getPriority(t2);
        
        if (priority1 > priority2) {
            return t1;
        } else {
            return t2;
        }
    }

    /**
     * min - Obtiene el tipo de menor jerarquía
     * 
     * @param t1 ID del primer tipo
     * @param t2 ID del segundo tipo
     * @return ID del tipo con menor jerarquía
     * @throws std::runtime_error si tipos no son compatibles
     */
    int min(int t1, int t2) const {
        if (t1 == t2) {
            return t1;
        }
        
        if (!isNumericType(t1) || !isNumericType(t2)) {
            throw std::runtime_error("Los tipos no son numéricos, por lo que no tienen una jerarquía comparable");
        }
        
        int priority1 = getPriority(t1);
        int priority2 = getPriority(t2);
        
        if (priority1 < priority2) {
            return t1;
        } else {
            return t2;
        }
    }

    /**
     * ampliar - Maneja conversión implícita a tipo mayor
     * 
     * Según el libro Aho: Las conversiones implícitas ocurren de forma automatica
     * 
     * @param dir Dirección del operando origen
     * @param t1 Tipo del operando origen
     * @param t2 Tipo al que se quiere convertir
     * @return Nueva dirección del operando convertido
     * @throws std::runtime_error si conversión no es válida
     */
    int ampliar(int dir, int t1, int t2) const {
        if (!isValidConversion(t1, t2, true)) {
            throw std::runtime_error("Conversión implícita inválida");
        }
        if (t1 == t2) {
            return dir;
        }
        // En si, se debería generar código como:
        // std::string temp = codeGenerator->newTemp();
        // codeGenerator->emitConversion(temp, add, t1, t2);
        // return temp; 
        // Pero esto excede los alcances del PDF, pues solo se piden las estructuras de datos, no la generación de código real
        // Por lo que, por ahora,solo regresaremos una nueva dirección simbólica
        return dir + 100; 
    }

    /**
     * reducir - Maneja conversión explícita a tipo menor
     * Según el libro Aho: Las conversiones explícitas requieren de un casting
     * y pueden perder precisión
     * 
     * @param dir Dirección del operando origen
     * @param t1 Tipo del operando origen
     * @param t2 Tipo al que se quiere convertir
     * @return Nueva dirección del operando convertido
     * @throws std::runtime_error si conversión no es válida
     */
    int reducir(int dir, int t1, int t2) const {
        if (!areCompatible(t1, t2)) {
            throw std::runtime_error("Conversión explícita inválida");
        }
        if (t1 == t2) {
            return dir;
        }
        // En si, deberíamos generar código de casting mas o menos así
        // std::string temp = codeGenerator->newTemp();
        // codeGenerator->emitCast(temp, add, t1, t2);
        // return temp;
        // Pero esto excede los alcances del PDF, pues solo se piden las estructuras de datos, no la generación de código real
        // Por lo que, por ahora, solo regresaremos una nueva dirección simbólica
        return dir + 200;
    }

    // Funciones Auxiliares

    /**
     * Verifica si dos tipos son compatibles para operaciones binarias
     * 
     * @param t1 Primer tipo
     * @param t2 Segundo tipo
     * @return true si son compatibles
     */
    bool areCompatible(int t1, int t2) const {
        if (t1 == t2) {
            return true;
        }
        return isNumericType(t1) && isNumericType(t2);
    }

    /**
     * Verifica si una conversión entre tipos es válida
     * 
     * @param t1 Tipo origen
     * @param t2 Tipo destino
     * @param isImplicit true para conversión implícita, false para explícita
     * @return true si la conversión es válida
     */
    bool isValidConversion(int t1, int t2, bool isImplicit) const {
        if (t1 == t2) {
            return true;
        }
        
        if (!isNumericType(t1) || !isNumericType(t2)) {
            return false;
        }
        
        int t1Priority = getPriority(t1);
        int t2Priority = getPriority(t2);
        
        if (isImplicit) {
            return t1Priority < t2Priority;
        } else {
            return true;
        }
    }
};
