#include "../src/SymbolTable.hpp"
#include <gtest/gtest.h>

TEST(SymbolTableTest, InsertAndRetrieveSymbol) {
    SymbolTable ts;

    ts.insert("x", 1, VarCategory, 100);
    auto sym = ts.get("x");

    ASSERT_TRUE(sym.has_value());
    EXPECT_EQ(sym->typeId, 1);
    EXPECT_EQ(sym->dir, 100);
    EXPECT_EQ(sym->category, VarCategory);
    EXPECT_TRUE(sym->params.empty());
}

TEST(SymbolTableTest, FunctionSymbol) {
    SymbolTable ts;
    ts.insert("sum", 3, FunctionCategory, 200, {1,2});

    auto sym = ts.get("sum");
    ASSERT_TRUE(sym.has_value());
    EXPECT_EQ(sym->params.size(), 2);
}
