# include <pl1.h>
# include <iostream> // cin cout endl
using namespace std;

int main() {
  Project1 test;
  string  testnum;
  char ch;
  int name;
  test.mend = false;
  test.mnowline = 1;
  test.mnowcolumn = 0;
  cout << "Welcome to OurScheme!\n";
  cin >> testnum;
  ch = cin.get();
  while ( !test.mend  ) {
    cout << "\n> ";
    test.Startexp();
  } // while

  cout << "\nThanks for using OurScheme!" ;

} // main()