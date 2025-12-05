#include "CodeGenerator.hpp"
#include <gtest/gtest.h>

TEST(CodeGeneratorTest, GenerateNewTemporary) {
    CodeGenerator cg;

    EXPECT_EQ(cg.newTemp(), "t0");
    EXPECT_EQ(cg.newTemp(), "t1");
    EXPECT_EQ(cg.newTemp(), "t2");
}

TEST(CodeGeneratorTest, GenerateNewLabel) {
    CodeGenerator cg;

    EXPECT_EQ(cg.newLabel(), "L0");
    EXPECT_EQ(cg.newLabel(), "L1");
    EXPECT_EQ(cg.newLabel(), "L2");
}
