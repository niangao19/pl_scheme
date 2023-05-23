# include <gtest/gtest.h>
# include "pl3.h"
# include <iostream>

using namespace std ;

Project3 pl ;

TEST(IsnumTest, Digits) {
    for (char c = '0' ; c <= '9' ; c++) {
        EXPECT_TRUE(pl.Isnum(c));
    }
}


TEST(IsnumTest, NonDigits) {
    for (char c = 'a' ; c <= 'z' ; c++) {
        EXPECT_FALSE(pl.Isnum(c));
    }
    for (char c = 'A' ; c <= 'Z';  c++) {
        EXPECT_FALSE(pl.Isnum(c));
    }
}

TEST(IswhitespTest, Whitespace) {
    // test Whitespace
    char testcase[] = {' ', '\t', '\n', '\0'} ;
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_TRUE(pl.Iswhitesp(testcase[i])) ;
    }

    // test Non Whitespace
    EXPECT_FALSE(pl.Iswhitesp('a')) ;
    EXPECT_FALSE(pl.Iswhitesp('1')) ;
    EXPECT_FALSE(pl.Iswhitesp('@')) ;
}

TEST(IscharTest, Char) {
    for (char c = 'a' ; c <= 'z' ; c++) {
        EXPECT_TRUE(pl.Ischar(c));
    }
    for (char c = 'A' ; c <= 'Z';  c++) {
        EXPECT_TRUE(pl.Ischar(c));
    }
}

TEST(IscharTest, NonChar) {
    for (char c = '0' ; c <= '9' ; c++) {
        EXPECT_FALSE(pl.Ischar(c));
    }
}

TEST(IsspreadTest, SpreadChars) {
    char testcase[] = {' ', '\t', '\n', '\0', '\'', '\"', ';', '(', ')'} ;
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_TRUE(pl.Isspread(testcase[i])) ;
    }
}

TEST(IsspreadTest, NonSpreadChars) {
    char testcase[] = {'a', 'Z', '1', '!', '-'} ;
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_FALSE(pl.Isspread(testcase[i])) ;
    }
}

TEST(IsBasicTest, BasicOperators) {
    // test BasicOperators
    string testcase[] = { "+", "-", "*", "/", "not", "and", "or", ">", ">=",
    "<", "<=", "=", "string-append", "string>?", "string<?", "string=?" } ;
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_TRUE(pl.IsBasic(testcase[i])) ;
    }

    // test Non BasicOperators
    EXPECT_FALSE(pl.IsBasic("++")) ;
    EXPECT_FALSE(pl.IsBasic("--")) ;
    EXPECT_FALSE(pl.IsBasic("not+")) ;
}

TEST(IsPrimitiveTest, Primitive) {
    string testcase[] = { "atom?", "pair?", "list?", "null?", "integer?", 
    "real?", "number?", "string?", "boolean?", "symbol?" };
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_TRUE(pl.IsPrimitive(testcase[i])) ;
    }
}

TEST(IsPrimitiveTest, NonPrimitive) {
    EXPECT_FALSE(pl.IsPrimitive("not a primitive")) ;
    EXPECT_FALSE(pl.IsPrimitive("another non-primitive")) ;
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}
