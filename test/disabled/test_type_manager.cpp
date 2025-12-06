#include "../src/TypeManager.hpp"
#include <gtest/gtest.h>

TEST(TypeManagerTest, MaxType) {
    TypeManager tm;
    EXPECT_EQ(tm.maxType(0, 1), 1);
}

TEST(TypeManagerTest, MinType) {
    TypeManager tm;
    EXPECT_EQ(tm.minType(0, 1), 0);
}

TEST(TypeManagerTest, Amplify) {
    TypeManager tm;
    EXPECT_EQ(tm.amplify(0, 0, 1), 1);
}

TEST(TypeManagerTest, Reduce) {
    TypeManager tm;
    EXPECT_EQ(tm.reduce(0, 1, 0), 0);
}
