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

} // testinput()

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
        // testinput( pl );
        pl.Startexp();
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

TEST(input_test, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_input2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_input2_ans.txt");

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

TEST(input_test, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_input3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_input3_ans.txt");

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

TEST(input_test, test4) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_input4.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_input4_ans.txt");

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

TEST(input_test, test5) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_input5.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_input5_ans.txt");

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

TEST(input_test, test6) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_input6.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_input6_ans.txt");

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

TEST(Constructors, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_constructors1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_constructors1_ans.txt");

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

TEST(Constructors, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_constructors2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_constructors2_ans.txt");

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

TEST(Constructors, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_constructors3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_constructors3_ans.txt");

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

TEST(define, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_define1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_define1_ans.txt");

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

TEST(define, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_define2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_define2_ans.txt");

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

TEST(define, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_define3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_define3_ans.txt");

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

TEST(define, nodefine) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_define4.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_define4_ans.txt");

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

TEST(part_accessors, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_part_accessors1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_part_accessors1_ans.txt");

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

TEST(part_accessors, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_part_accessors2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_part_accessors2_ans.txt");

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

TEST(part_accessors, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_part_accessors3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_part_accessors3_ans.txt");

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

TEST(Conditionals, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_conditionals1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_conditionals1_ans.txt");

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

TEST(Conditionals, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_conditionals2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_conditionals2_ans.txt");

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

TEST(Conditionals, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_conditionals3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_conditionals3_ans.txt");

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

TEST(functional_composition, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_functional_composition1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_functional_composition1_ans.txt");

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

TEST(functional_composition, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_functional_composition2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_functional_composition2_ans.txt");

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

TEST(functional_composition, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_define.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_define_ans.txt");

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

TEST(Equivalent, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_eqv1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_eqv1_ans.txt");

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

TEST(Equivalent, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_eqv2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_eqv2_ans.txt");

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

TEST(Equivalent, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_eqv3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_eqv3_ans.txt");

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

TEST(Equivalent, test4) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_eqv4.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_eqv4_ans.txt");

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

TEST(User_defined_functions, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_let.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_let_ans.txt");

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

TEST(User_defined_functions, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_lambda.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_lambda_ans.txt");

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

TEST(User_defined_functions, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_define.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_define_ans.txt");

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

TEST(Primitive_predicates, test1) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates1.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates1_ans.txt");

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

TEST(Primitive_predicates, test2) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates2.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates2_ans.txt");

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

TEST(Primitive_predicates, test3) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates3.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates3_ans.txt");

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

TEST(Primitive_predicates, test4) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates4.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates4_ans.txt");

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

TEST(Primitive_predicates, test5) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates5.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates5_ans.txt");

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

TEST(Primitive_predicates, test6) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates6.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates6_ans.txt");

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

TEST(Primitive_predicates, test7) {
    Project3 pl;
    pl.mend = false;
    pl.mnowline = 1;
    pl.mnowcolumn = 0;
    ifstream inputFile("./test/testfile/test_primitive_predicates7.txt");
    streambuf* originalCinBuf = cin.rdbuf(inputFile.rdbuf());
    string test_outcome = "";
    
    while( cin.peek() != EOF  ) {
        testing::internal::CaptureStdout();
        // testinput( pl );
        pl.Startexp();
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
    ansFile.open("./test/ansfile/test_primitive_predicates7_ans.txt");

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
