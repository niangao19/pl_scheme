# include <gtest/gtest.h>
# include "pl3.h"
# include <iostream>
# include <fstream>
# include <string> 

using namespace std ;



TEST(IsnumTest, Digits) {
    Project3 pl ;
    for (char c = '0' ; c <= '9' ; c++) {
        EXPECT_TRUE(pl.Isnum(c));
    }
}


TEST(IsnumTest, NonDigits) {
    Project3 pl ;
    for (char c = 'a' ; c <= 'z' ; c++) {
        EXPECT_FALSE(pl.Isnum(c));
    }
    for (char c = 'A' ; c <= 'Z';  c++) {
        EXPECT_FALSE(pl.Isnum(c));
    }
}

TEST(IswhitespTest, Whitespace) {
    // test Whitespace
    Project3 pl ;
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
    Project3 pl ;
    for (char c = 'a' ; c <= 'z' ; c++) {
        EXPECT_TRUE(pl.Ischar(c));
    }
    for (char c = 'A' ; c <= 'Z';  c++) {
        EXPECT_TRUE(pl.Ischar(c));
    }
}

TEST(IscharTest, NonChar) {
    Project3 pl ;
    for (char c = '0' ; c <= '9' ; c++) {
        EXPECT_FALSE(pl.Ischar(c));
    }
}

TEST(IsspreadTest, SpreadChars) {
    Project3 pl ;
    char testcase[] = {' ', '\t', '\n', '\0', '\'', '\"', ';', '(', ')'} ;
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_TRUE(pl.Isspread(testcase[i])) ;
    }
}

TEST(IsspreadTest, NonSpreadChars) {
    Project3 pl ;
    char testcase[] = {'a', 'Z', '1', '!', '-'} ;
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_FALSE(pl.Isspread(testcase[i])) ;
    }
}

TEST(IsBasicTest, BasicOperators) {
    // test BasicOperators
    Project3 pl ;
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
    Project3 pl ;
    string testcase[] = { "atom?", "pair?", "list?", "null?", "integer?", 
    "real?", "number?", "string?", "boolean?", "symbol?" };
    int size = sizeof(testcase)/sizeof(testcase[0]);
    for (int i = 0 ; i < size ; i++) {
        EXPECT_TRUE(pl.IsPrimitive(testcase[i])) ;
    }
}

TEST(IsPrimitiveTest, NonPrimitive) {
    Project3 pl ;
    EXPECT_FALSE(pl.IsPrimitive("not a primitive")) ;
    EXPECT_FALSE(pl.IsPrimitive("another non-primitive")) ;
}

void testinput(   Project3 &pl ) { // do readexp if have error print

  pl.mnowchar = '\0';
  pl.mlinelp = 0;
  pl.mnowline = 1 ;
  pl.mprinttok = false;
  Expptr testline;
  pl.mlinename = "";
  try {
    testline = pl.Readexp( false );
    pl.mnowcolumn = 0;
    while ( cin.peek() != EOF && pl.Iswhitesp( cin.peek() ) && cin.peek() != '\n' )
      pl.Getchar();

    if ( cin.peek() == ';' ) {
      while (  cin.peek() != EOF && cin.peek() != '\n'  ) // read explain 
        pl.Getchar();
    } // if

    if ( cin.peek() == '\n'  )
      pl.Getchar();
      
    if ( testline != NULL  ) {
      pl.Printexp(  testline , false  );
    } // if

  } // try
  catch ( ThrowReadError  &readerror ) {
    pl.mlinename = "error";
    if (  readerror.merrortype != 4  )
      cout << readerror.Error() << endl;
    else {
      pl.mend = true;
      cout << readerror.Error();        
      return;        
    } // else
    
    while ( !pl.mend && pl.mnowchar != '\n' )
      pl.Getchar();   
  } // catch
  catch ( End &end ) {
    pl.mend = true;
    return;      
  } // catch

} // Startexp()

TEST(input_test, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_input1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        testinput( pl );
        string output = testing::internal::GetCapturedStdout();
        test_outcome += output;
    } // while

    // cout << "test : " << test_outcome << endl << "end\n";
    cin.rdbuf(originalCinBuf);
    inputFile.close();

    // Open file
    ifstream ansFile;
    string line;
    string ans_outcome = "";
    ansFile.open("./test/ansfile/test_input1_ans.txt");

    // Print file content
    while (getline(ansFile, line)) {
        ans_outcome += line + "\n";
        // cout  << line << endl;
    } // while

    // cout << "ans"<< ans_outcome << endl;
    // Close file
    ansFile.close();
    ASSERT_EQ(test_outcome, ans_outcome);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}
