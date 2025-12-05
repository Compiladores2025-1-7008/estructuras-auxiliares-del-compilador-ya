#include "../src/SymbolTable.hpp"
#include <gtest/gtest.h>

// Pruebas para símbolos de variable
TEST(SymbolTableTest, InsertAndQueryVariableSymbol)
{
    SymbolTable st;

    // Creamos una entrada "renglón" de la tabla de símbolos que será una variable:
    SymbolEntry var{
        "x",           // id
        3,             // typeId (asuimimos que 3 es el id de tipo int porque soportamos void, bool, char, int, ...)
        Category::VAR, // categoría: variable
        0,             // dirección de memoria 0 porque sería lo primero que insertamos
        {}             // params vacío (no es función)
    };

    // Verificamos que la inserción sea exitosa (el id no existía antes).
    EXPECT_TRUE(st.insert(var));

    // Consultamos los campos individuales del símbolo "x".
    EXPECT_EQ(st.getType("x"), 3);
    EXPECT_EQ(st.getAddress("x"), 0);
    EXPECT_EQ(st.getCategory("x"), Category::VAR);

    // Para una variable, la lista de parámetros debe estar vacía.
    auto params = st.getParams("x");
    EXPECT_TRUE(params.empty());

    // Usamos lookup para obtener la entrada completa.
    const SymbolEntry *sym = st.lookup("x");
    // Que no esté vacía la tabla
    ASSERT_NE(sym, nullptr);
    // El primer y único símbolo insertado debe ser x
    EXPECT_EQ(sym->id, "x");
}

// Pruebas para símbolos de función con lista de parámetros
TEST(SymbolTableTest, InsertFunctionSymbolWithParams)
{
    SymbolTable st;

    // Función de suma, devuelve int (tipo 3 según nuestro supuesto)
    // categoria FUNCTION, dirección inventada 32, y dos parámetros de tipo int (tipo 3).
    SymbolEntry fun{
        "sum",
        3, // typeId de retorno (por ejemplo, int)
        Category::FUNCTION,
        32,    // address (posición en tabla de funciones, etc.)
        {3, 3} // parámetros: (int, int) -> asumimos typeId=3 para int
    };

    // La inserción debe ser exitosa.
    EXPECT_TRUE(st.insert(fun));

    // Confirmamos que la categoría de "sum" es FUNCTION.
    EXPECT_EQ(st.getCategory("sum"), Category::FUNCTION);

    // Recuperamos y verificamos la lista de parámetros.
    auto params = st.getParams("sum");
    // Debe tener solo dos parámetros y ambos de tipo int (typeId 3).
    ASSERT_EQ(params.size(), 2u);
    EXPECT_EQ(params[0], 3);
    EXPECT_EQ(params[1], 3);

    // Verificamos también a través de lookup.
    const SymbolEntry *sym = st.lookup("sum");
    ASSERT_NE(sym, nullptr);
    EXPECT_EQ(sym->params.size(), 2u);
}

// Pruebas de inserción duplicada
TEST(SymbolTableTest, DuplicateInsertFails)
{
    SymbolTable st;
    // Insertamos dos símbolos con el mismo id, tipos diferentes para darle más sabor.
    // direcciones acorde a una inserción correcta (el primero del 0 y usa 4 bytes, el segundo del 4).
    SymbolEntry s1{"x", 3, Category::VAR, 0, {}};   // Supuesto int tipo 3
    SymbolEntry s2{"x", 4, Category::CONST, 4, {}}; // supuesto float tipo 4

    // El primero se debe insertar correctamente y el segundo debe fallar y devolver false porque el id ya existe en
    // la tabla de símbolos.
    EXPECT_TRUE(st.insert(s1));
    EXPECT_FALSE(st.insert(s2)); // mismo id, no debe insertar
}

// Prueba de la excpeción agregada a la función auxiliar que obtiene el símbolo.
TEST(SymbolTableTest, NotFoundThrowsOnGetters)
{
    SymbolTable st;

    // Insertamos un símbolo para tener algo en la tabla.
    SymbolEntry var{
        "x",           // id
        3,             // typeId (asuimimos que 3 es el id de tipo int porque soportamos void, bool, char, int, ...)
        Category::VAR, // categoría: variable
        0,             // dirección de memoria 0 porque sería lo primero que insertamos
        {}             // params vacío (no es función)
    };
    // Debe insertarse correctamente
    EXPECT_TRUE(st.insert(var));

    // Ahora intentamos consultar un id que no hemos insertado,no existe en la tabla.
    EXPECT_THROW(st.getType("y"), SymbolNotFoundError);
    EXPECT_THROW(st.getAddress("y"), SymbolNotFoundError);
    EXPECT_THROW(st.getCategory("y"), SymbolNotFoundError);
    EXPECT_THROW(st.getParams("y"), SymbolNotFoundError);

    // lookup devuelve un puntero nulo si no se encuentra el símbolo
    EXPECT_EQ(st.lookup("y"), nullptr);
}
