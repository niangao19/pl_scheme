# include <iostream> // cin cout endl
using namespace std;
#include "../include/pl3.h"

int main() {
  Project3 test;
  int  testnum;
  char ch;
  test.mend = false;
  test.mnowline = 1;
  test.mnowcolumn = 0;
  cout << "Welcome to OurScheme!\n";
  cin >> testnum;
  cout << "this test is " << testnum << endl;
  ch = cin.get();
  while ( !test.mend  ) {
    cout << "\n> ";
    test.Startexp();
  } // while

  cout << "\nThanks for using OurScheme!" ;

} // main()