#include <gtest/gtest.h>
#include <iostream>

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
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


