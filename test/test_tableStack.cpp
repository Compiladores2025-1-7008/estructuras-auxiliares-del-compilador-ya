#include "../src/SymbolTableStack.hpp"
#include "../src/SymbolTable.hpp"
#include <gtest/gtest.h>

// Pruebas básicas de creación
TEST(SymbolTableStackTest, PushScopeIncreasesLevels)
{
    SymbolTableStack stack;

    EXPECT_EQ(stack.levels(), 0u);

    stack.pushScope();
    EXPECT_EQ(stack.levels(), 1u);

    stack.pushScope();
    EXPECT_EQ(stack.levels(), 2u);

    stack.pushScope();
    EXPECT_EQ(stack.levels(), 3u);
}

// Pruebas de popScope, quita el tope sin liberar memoria
TEST(SymbolTableStackTest, PopScopeDecreasesLevels)
{
    SymbolTableStack stack;

    stack.pushScope();
    stack.pushScope();
    stack.pushScope();

    EXPECT_EQ(stack.levels(), 3u);

    stack.popScope();
    EXPECT_EQ(stack.levels(), 2u);

    stack.popScope();
    EXPECT_EQ(stack.levels(), 1u);

    stack.popScope();
    EXPECT_EQ(stack.levels(), 0u);

    // popScope en pila vacía no debe fallar
    stack.popScope();
    EXPECT_EQ(stack.levels(), 0u);
}

// Prueba de popSymbolTable, extrae el tope y conserva el puntero
TEST(SymbolTableStackTest, PopSymbolTableReturnsTopTable)
{
    SymbolTableStack stack;

    stack.pushScope();
    SymbolTable *t1 = stack.currentScope();

    stack.pushScope();
    SymbolTable *t2 = stack.currentScope();

    EXPECT_EQ(stack.levels(), 2u);

    // popSymbolTable debe regresar la tabla actual sin destruirla
    SymbolTable *popped = stack.popSymbolTable();
    EXPECT_EQ(popped, t2);
    EXPECT_EQ(stack.levels(), 1u);

    // El tope actual ahora debe ser t1
    EXPECT_EQ(stack.currentScope(), t1);
}

// Inserción en el tope
TEST(SymbolTableStackTest, InsertTopInsertsInCurrentScope)
{
    SymbolTableStack stack;

    stack.pushScope();

    SymbolEntry entry = {"x", 1, Category::VAR, 0, {}};

    EXPECT_TRUE(stack.insertTop(entry));

    SymbolEntry *found = stack.lookupTop("x");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->id, "x");
    EXPECT_EQ(found->typeId, 1);
}

// Inserción en la base
TEST(SymbolTableStackTest, InsertBaseInsertsInGlobalScope)
{
    SymbolTableStack stack;

    stack.pushScope(); // global
    stack.pushScope(); // local

    SymbolEntry globalVar = {"g", 5, Category::CONST, 100, {}};

    EXPECT_TRUE(stack.insertBase(globalVar));

    // Debe existir en el global
    SymbolEntry *foundBase = stack.lookupBase("g");
    ASSERT_NE(foundBase, nullptr);
    EXPECT_EQ(foundBase->id, "g");
    EXPECT_EQ(foundBase->typeId, 5);

    // No debe existir en el tope
    EXPECT_EQ(stack.lookupTop("g"), nullptr);
}

// Prueba para distinguir entre tope y base cuando tienen mismo id
TEST(SymbolTableStackTest, LookupTopAndBaseAreIndependent)
{
    SymbolTableStack stack;

    stack.pushScope(); // base
    stack.pushScope(); // tope

    SymbolEntry baseVar = {"a", 10, Category::VAR, 111, {}};
    SymbolEntry topVar = {"a", 20, Category::VAR, 222, {}};

    stack.insertBase(baseVar);
    stack.insertTop(topVar);

    // lookupTop debe regresar el del tope
    SymbolEntry *foundTop = stack.lookupTop("a");
    ASSERT_NE(foundTop, nullptr);
    EXPECT_EQ(foundTop->typeId, 20);
    EXPECT_EQ(foundTop->address, 222);

    // lookupBase debe regresar el de la base
    SymbolEntry *foundBase = stack.lookupBase("a");
    ASSERT_NE(foundBase, nullptr);
    EXPECT_EQ(foundBase->typeId, 10);
    EXPECT_EQ(foundBase->address, 111);
}

// Comportamiento correcto con pila vacía
TEST(SymbolTableStackTest, EmptyStackBehaviors)
{
    SymbolTableStack stack;

    EXPECT_EQ(stack.currentScope(), nullptr);
    EXPECT_EQ(stack.globalScope(), nullptr);

    EXPECT_FALSE(stack.insertTop({"x", 1, Category::VAR, 0, {}}));
    EXPECT_FALSE(stack.insertBase({"y", 2, Category::VAR, 0, {}}));

    EXPECT_EQ(stack.lookupTop("x"), nullptr);
    EXPECT_EQ(stack.lookupBase("y"), nullptr);

    EXPECT_EQ(stack.popSymbolTable(), nullptr);
}
