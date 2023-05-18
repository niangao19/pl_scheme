#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>

int readUserInput()
{
    int value;
    std::cout << "Enter a number: ";
    std::cin >> value;

    return value;
}

TEST(cin_test, Basic)
{
    // Create pipe to mock stdin
    int fildes[2];
    int status = pipe(fildes);
    ASSERT_NE(status, -1);

    // Swap `stdin` fd with the "read" end of the pipe
    status = dup2(fildes[0], STDIN_FILENO);
    ASSERT_NE(status, -1);

    // Create payload
    const char buf[] = "2\n";
    const int bsize  = strlen(buf);

    // Send payload through pipe
    ssize_t nbytes = write(fildes[1], buf, bsize);
    close(fildes[1]);
    ASSERT_EQ(nbytes, bsize);

    ASSERT_EQ(readUserInput(), 2);
    close(fildes[0]);
    testing::internal::CaptureStdout();
    std::cout << "My test";
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "My test");
}

void processInput() {
    std::string input;
    std::cin >> input;
    std::cout << input << std::endl;
    // 處理輸入...
}

TEST(cin_test, file)
{
    std::ifstream inputFile("input.txt");

    std::streambuf* originalCinBuf = std::cin.rdbuf(inputFile.rdbuf());


    processInput();


    std::cin.rdbuf(originalCinBuf);

 
    inputFile.close();
}

TEST(cout_test, Basic)
{
    std::cout << "My test output" << std::endl;
    testing::internal::CaptureStdout();
    std::cout << "My test";
    std::string output = testing::internal::GetCapturedStdout();
    std::cout << "My test output"<< std::endl;
    ASSERT_EQ(output, "My test");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

