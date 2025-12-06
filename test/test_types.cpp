#include "../src/TypeTable.hpp"
#include <gtest/gtest.h>

// Pruebas para Tipos Básicos
TEST(TypeTableTest, AddAndRetrieveBasicType) {
    TypeTable tt;
    // Agregamos tipos básicos como 'int' y 'float'
    int idInt = tt.addBasicType("int", 4);
    int idFloat = tt.addBasicType("float", 8);

    // Verificamos que los IDs sean válidos (>= 0)
    ASSERT_GE(idInt, 0);
    ASSERT_GE(idFloat, 0);
    ASSERT_NE(idInt, idFloat); // Los IDs deben ser diferentes

    // Verificamos propiedades del tipo 'int'
    EXPECT_EQ(tt.getName(idInt), "int");
    EXPECT_EQ(tt.getSize(idInt), 4);
    EXPECT_EQ(tt.getBaseType(idInt), -1); // -1 indica que es básico (no tiene tipo base)

    // Verificamos propiedades del tipo 'float'
    EXPECT_EQ(tt.getName(idFloat), "float");
    EXPECT_EQ(tt.getSize(idFloat), 8);
}

// Pruebas para Tipos Arreglo
TEST(TypeTableTest, AddAndRetrieveArrayType) {
    TypeTable tt;
    int idInt = tt.addBasicType("int", 4);
    
    // Creamos un arreglo de enteros de tamaño 10: int[10]
    int idArray = tt.addArrayType(idInt, 10);
    
    ASSERT_GE(idArray, 0);
    
    // Verificamos propiedades
    // El nombre se construye automáticamente como "base[elementos]"
    EXPECT_EQ(tt.getName(idArray), "int[10]");
    EXPECT_EQ(tt.getSize(idArray), 40); // Tamaño = 4 bytes * 10 elementos
    EXPECT_EQ(tt.getNumElements(idArray), 10);
    EXPECT_EQ(tt.getBaseType(idArray), idInt);
    
    // Verificamos directamente la entrada en la tabla
    const TypeEntry& entry = tt.get(idArray);
    EXPECT_EQ(entry.kind, TypeKind::ARRAY);
}

// Pruebas para Tipos Estructura (Struct)
TEST(TypeTableTest, AddAndRetrieveStructType) {
    TypeTable tt;
    // Pasamos nullptr para SymbolTable* ya que en esta etapa solo probamos almacenamiento.
    // En una integración real, pasaríamos un puntero válido a una tabla de símbolos.
    int idStruct = tt.addStructType("Persona", 32, nullptr);
    
    ASSERT_GE(idStruct, 0);
    EXPECT_EQ(tt.getName(idStruct), "Persona");
    EXPECT_EQ(tt.getSize(idStruct), 32);
    EXPECT_EQ(tt.getStructFields(idStruct), nullptr);
    
    const TypeEntry& entry = tt.get(idStruct);
    EXPECT_EQ(entry.kind, TypeKind::STRUCT);
}

// Pruebas de Manejo de Errores
TEST(TypeTableTest, InvalidAccess) {
    TypeTable tt;
    // Intentar acceder a un ID que no existe debe lanzar excepción
    EXPECT_THROW(tt.get(999), std::out_of_range);
    
    // Intentar crear un arreglo con un tipo base inválido debe lanzar error
    EXPECT_THROW(tt.addArrayType(999, 5), std::runtime_error);
}
