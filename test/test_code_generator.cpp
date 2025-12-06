#include "CodeGenerator.hpp"
#include <gtest/gtest.h>

// Prueba que los temporales se generen de forma secuencial
TEST(CodeGeneratorTest, GeneratesSequentialTemps) {
    CodeGenerator gen;

    EXPECT_EQ(gen.newTemp(), "t0");
    EXPECT_EQ(gen.newTemp(), "t1");
    EXPECT_EQ(gen.newTemp(), "t2");
}

// Prueba que las etiquetas se generan de forma secuencial
TEST(CodeGeneratorTest, GeneratesSequentialLabels) {
    CodeGenerator gen;

    EXPECT_EQ(gen.newLabel(), "L0");
    EXPECT_EQ(gen.newLabel(), "L1");
    EXPECT_EQ(gen.newLabel(), "L2");
}


// Prueba que que se reincian ambos contadores correctamente
TEST(CodeGeneratorTest, ResetReinitializesCounters) {
    CodeGenerator gen;

    gen.newTemp(); 
    gen.newLabel(); 

    gen.reset();

    EXPECT_EQ(gen.newTemp(), "t0");
    EXPECT_EQ(gen.newLabel(), "L0");
}
