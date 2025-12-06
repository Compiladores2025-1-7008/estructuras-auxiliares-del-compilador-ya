#include "../src/TypeManager.hpp"
#include "../src/TypeTable.hpp"
#include <gtest/gtest.h>

// PRUEBA 1: Función max() 
TEST(TypeManager, MaxComparaTipos) {
    TypeTable tabla;
    int tipoChar = tabla.addBasicType("char", 1);
    int tipoInt = tabla.addBasicType("int", 4);
    int tipoFloat = tabla.addBasicType("float", 4);
    
    TypeManager manager(tabla);
    
    EXPECT_EQ(manager.max(tipoChar, tipoInt), tipoInt);
    EXPECT_EQ(manager.max(tipoInt, tipoFloat), tipoFloat);
}

// PRUEBA 2: Función min() 
TEST(TypeManager, MinComparaTipos) {
    TypeTable tabla;
    int tipoInt = tabla.addBasicType("int", 4);
    int tipoFloat = tabla.addBasicType("float", 4);
    
    TypeManager manager(tabla);
    
    EXPECT_EQ(manager.min(tipoInt, tipoFloat), tipoInt);
    EXPECT_EQ(manager.min(tipoFloat, tipoFloat), tipoFloat);
}

// PRUEBA 3: Función ampliar() 
TEST(TypeManager, AmpliarConversionesValidas) {
    TypeTable tabla;
    int tipoInt = tabla.addBasicType("int", 4);
    int tipoFloat = tabla.addBasicType("float", 4);
    
    TypeManager manager(tabla);

    int direccionOriginal = 100;
    int nuevaDireccion = manager.ampliar(direccionOriginal, tipoInt, tipoFloat);

    EXPECT_NE(direccionOriginal, nuevaDireccion);
    EXPECT_EQ(nuevaDireccion, 200); 
}


// PRUEBA 4: Función reducir() 
TEST(TypeManager, ReducirConversionesValidas) {
    TypeTable tabla;
    int tipoInt = tabla.addBasicType("int", 4);
    int tipoFloat = tabla.addBasicType("float", 4);
    
    TypeManager manager(tabla);

    int direccionOriginal = 150;
    int nuevaDireccion = manager.reducir(direccionOriginal, tipoFloat, tipoInt);
    
    EXPECT_NE(direccionOriginal, nuevaDireccion);
    EXPECT_EQ(nuevaDireccion, 350); 
}


// PRUEBA 5: Mismo tipo no cambia dirección
TEST(TypeManager, MismoTipoSinConversion) {
    TypeTable tabla;
    int tipoInt = tabla.addBasicType("int", 4);
    
    TypeManager manager(tabla);
    
    int direccion = 500;
    
    EXPECT_EQ(manager.ampliar(direccion, tipoInt, tipoInt), direccion);
    EXPECT_EQ(manager.reducir(direccion, tipoInt, tipoInt), direccion);
}

// PRUEBA 6: Error en conversión inválida
TEST(TypeManager, AmpliarInvalidoLanzaExcepcion) {
    TypeTable tabla;
    int tipoInt = tabla.addBasicType("int", 4);
    int tipoFloat = tabla.addBasicType("float", 4);
    
    TypeManager manager(tabla);
    
    EXPECT_THROW(manager.ampliar(100, tipoFloat, tipoInt), std::runtime_error);
}

// PRUEBA 7: Jerarquía completa
TEST(TypeManager, JerarquiaCompleta) {
    TypeTable tabla;
    int tipoChar = tabla.addBasicType("char", 1);
    int tipoInt = tabla.addBasicType("int", 4);
    int tipoFloat = tabla.addBasicType("float", 4);
    int tipoDouble = tabla.addBasicType("double", 8);
    
    TypeManager manager(tabla);
    
    EXPECT_EQ(manager.max(tipoChar, tipoInt), tipoInt);
    EXPECT_EQ(manager.max(tipoInt, tipoFloat), tipoFloat);
    EXPECT_EQ(manager.max(tipoFloat, tipoDouble), tipoDouble);
    EXPECT_EQ(manager.max(tipoChar, tipoDouble), tipoDouble);
}

