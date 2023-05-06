# include <stdio.h>
# include <stdlib.h> // atoi
# include <iostream> // cin cout endl
# include <string>
# include <vector>
# include <iomanip> // fix
# include <exception> // try catch
# include <sstream> // itos
# include "../include/pl1.h"
using namespace std;


// # define STRING 1
// # define INT    2
// # define FLOAT  3
// # define NILL   4
// # define TRUE   5
// # define SYMBAL 6
// # define QUOTE  7
// # define NONOPERTOR 8
// # define CONSTRUCTOR 9
// # define DEFINE 10
// # define PARTACCESSOR 11 // car cdr
// # define PRIMITIVEPREDICATE 12
// # define BASIC 13
// # define EQIV  14
// # define BEGIN 15
// # define COND  16
// # define CLEAN 17


// struct Token {
//   int type;
//   string name;
// };


// typedef Token* Tokenptr;

// struct Exp {
//   Exp* lefte ;
//   Exp* righte ;
//   Token* leftt ;
//   Token* rightt ;
// };

// typedef Exp* Expptr;

// struct Symbal {
//   string symbalname ;
//   Expptr symdefine ;
// };

void Itos( string & temp, int num   ) {
  stringstream sstream;
  sstream << num;
  temp = sstream.str() ;        
} // Itos()

void Ftos( string & temp, float num   ) {
  stringstream sstream;
  sstream << num;
  temp = sstream.str() ;        
} // Itos()


ReadError::ReadError( int line, int column, int etype, string tokenname ) {
  Itos( mline, line ) ;
  Itos( mcolumn, column  ) ;
  merrortype = etype;  
  mnowtoken = tokenname;
} // ReadError()

string ReadError::Error() {
  string readerror;
  if ( merrortype == 1 ) { // when input string program get "    \n not closing
    readerror = "ERROR (no closing quote) : END-OF-LINE encountered at Line " + mline;
    readerror += " Column " + mcolumn ;
  } // if
  else if ( merrortype == 2  ) { // input not correct is not atom or '('
    readerror = "ERROR (unexpected token) : atom or '(' expected when token at Line " + mline ;
    readerror +=  " Column " + mcolumn + " is >>" +  mnowtoken + "<<";
  } // else if
  else { // input is not ')' when parse is ( {exp} . [exp] )
    readerror = "ERROR (unexpected token) : ')' expected when token at Line " + mline ;
    readerror += " Column " + mcolumn + " is >>" +  mnowtoken + "<<";
  } // else 

  return readerror;
} // Error()

bool Project1::Isnum( char com ) { // 數字 
  if ( ( int ) com >= 48 && ( int ) com <= 57 )
    return true;
  return false;  
} // Isnum()

bool Project1::Iswhitesp(  char com  ) {
  if (  com == ' ' || com == '\t' || com == '\n' || com == '\0' )
    return true; 
  return false;
} // Iswhitesp()

bool Project1::Ischar( char com ) { // 英文 
  if ( ( ( int ) com >= 65 && ( int ) com <= 90 ) || (  ( int ) com >= 97 && ( int ) com <= 122 ) ) {
    return true;
  } // if

  return false;
} // Ischar()

bool Project1::Isspread( char com  ) {
  // cout << com << endl;
  if (  Iswhitesp( com  )  || com == '\'' || com == '\"' || com == ';' || com == '(' || com == ')' )
    return true;
  return false;  
} // Isspread()

void Project1::Getchar() {
  if ( cin.peek() != EOF ) {
    mnowchar = cin.get();
    mnowcolumn++;
  } // if
  else {
    mend = true;
    return;
  } // else

  if (  mnowchar == '\n'  ) {
    mnowline++;
    mnowcolumn = 0;
  } // if
} // Getchar()

void Project1::Readsymbal() {

  while (  cin.peek() != EOF  && !Isspread( cin.peek() )  ) {
    Getchar();
    mnowtoken += mnowchar;
  } // while
  
  if (  mnowtoken == "#t" || mnowtoken == "t" ) {
    mnowtype = TRUE ;
  } // if
  else if ( mnowtoken == "#f" || mnowtoken == "nil"  ) {
    mnowtype = NILL;
  } // else if
  else
    mnowtype = SYMBAL ;

} // Readsymbal()

void Project1::Readnumber(  bool isfloat   ) {

  // cout << mnowtoken << endl;
  bool  havenum = false;
  while (  cin.peek() != EOF && !Isspread( cin.peek()  )  ) {
    if (  Isnum( mnowchar )  )
      havenum = true;
    Getchar();
    mnowtoken += mnowchar;
    if ( mnowchar == '.' ) {
      if ( isfloat ) {
        Readsymbal();
        return;
      } // if
      else
        isfloat = true;
    } // if
    else if ( !Isnum( mnowchar )  ) {
      Readsymbal();
      return;
    } // else if

  } // while    

  if (  Isnum( mnowchar )  )
    havenum = true;

  if ( isfloat && havenum ) { // ex : 123.45
    mnowtype = FLOAT;
  } // if
  else if ( havenum )  // ex: 123
    mnowtype = INT;  
  else // '+' or '-'
    mnowtype = SYMBAL;
} // Readnumber()

void Project1::Readstring() {
  int previouscolumn = mnowcolumn+1, previousline = mnowline;
  Getchar();
  mnowtoken = "";
  while ( !mend && mnowchar != '\"' && mnowchar != '\n' ) {
    if ( mnowchar == '\\'  ) {
      previouscolumn = mnowcolumn + 1;
      // if nextchar is enter need to send error and send previous position
      previousline = mnowline;
      Getchar(); 
      if ( mnowchar == 'n'  )
        mnowtoken += '\n';
      else if ( mnowchar == 't' )
        mnowtoken += '\t';
      else if (  mnowchar == '\"' ||  mnowchar == '\\' )
        mnowtoken += mnowchar;  
      else if (  mnowchar == '\n'  )  {
        throw  ReadError( previousline, previouscolumn, 1, "" );
      } // else if
      else if (  mnowchar != '\n'  )
        mnowtoken =  mnowtoken+ '\\' + mnowchar; 
      /*
      else if( mnowchar == '0' ) // 有可能會有錯 
      mnowtoken += '\0'; 
      */
    } // if
    else
      mnowtoken += mnowchar;
    previouscolumn = mnowcolumn+1;
    previousline = mnowline;        
    Getchar();
  } // while
  
  if (  mnowchar == '\n'  ) {
    throw  ReadError( previousline, previouscolumn, 1, ""  ) ;
  } // if
  
  //mnowtoken += mnowchar;    
  mnowtype = STRING;
}  // Readstring()

void Project1::Gettoken() {

  mnowtoken = ""; // previous string is arleady done
  mnowtype = -1;

  while ( Iswhitesp( mnowchar ) && !mend ) { // if get white space cin
    Getchar();
  }  // while

  if ( mend )
    return;

  mtokenline = mnowline;
  mtokencolumn = mnowcolumn;
  mnowtoken += mnowchar;

  if (  Iswhitesp( mnowchar )  ) // maybe the least one is whitespace
    return;
  else if ( Ischar( mnowchar )   ) { // SYMBAL 
    Readsymbal();
  }  // else if
  else if ( Isnum( mnowchar ) ) { // INT or SYMBAL 
    Readnumber( false );
  }  // else if
  else if (  mnowchar == '.' ) { // look the next one char is whitespace or other char

    if ( Isnum( cin.peek() ) ) { // float or SYMBAL 
      Readnumber( true );
    }  // if
    else if ( !Iswhitesp( cin.peek() )  ) { // SYMBAL
      Readsymbal();
    }  // if      
    // it is only'.' 

  }  // else if
  else if (  mnowchar == '('  ) { // look for NULL or '(' 
    // read all whitespace and look next legical char
    while (  cin.peek() != EOF && Iswhitesp(  cin.peek()  )  )
      Getchar();


    if (  cin.peek() == ')'  ) { // is NILL 
      Getchar();
      mnowtoken += mnowchar;
      mnowtype = NILL;
    }  // if
    

  }  // else if
  else if ( mnowchar == '\"'  ) { // if char is '\"' is string
    try {
      Readstring();
    } // try
    catch ( ReadError  &readerror ) {
      throw readerror;
    } // catch

  } // else if
  else {
    if (  mnowchar == '+' || mnowchar == '-'  ) 
      Readnumber( false );
    else if ( mnowchar != ';' && !Isspread( mnowchar ) )
      Readsymbal();  
    if ( mnowtoken == "\'"  ) 
      mnowtype = QUOTE;


  } // else

  // cout << "test token : " << mnowtoken << "\t\t" << mnowtype << endl;	
  if ( mnowtoken == ";"  ) {
    while (  cin.peek() != EOF && cin.peek() != '\n'  ) // 把註解讀掉 
      Getchar();
    mnowchar = '\0';
    Gettoken();
  } // if
  else {
    mlinename += mnowtoken;
  } // else

  mnowchar = '\0';

  if (  cin.peek() == EOF  )
    mend = true;

} // Gettoken()	

void Project1::AllNULL( Expptr temp  ) {
  temp->lefte = NULL;
  temp->leftt = NULL;
  temp->righte = NULL;
  temp->rightt = NULL;
} // AllNULL()



Expptr Project1::Readexp( bool haveread ) {
  Expptr temp = new Exp();
  AllNULL( temp );
  Expptr head = temp;
  Tokenptr temp2 = new Token();
  if (  !haveread  ) {
    try {
      Gettoken();
    } // try
    catch ( ReadError  &readerror ) {
      throw readerror;
    }  // catch

  }  // if

  if (  mnowtoken == "\'"  )  { // 'exp
    // cout << "test token 1: " << mnowtoken << "\t\t" << mnowtype << endl;    
    temp->lefte = new Exp();
    temp = temp->lefte ;
    AllNULL( temp );
    temp2->name = mnowtoken ;
    temp2->type = QUOTE ;
    temp->leftt = temp2 ;
    Expptr texp = NULL;
    try {
      texp = Readexp( false ) ;
      temp->righte = texp;
    } // try
    catch ( ReadError  &readerror ) {
      throw readerror;
    } // catch


  } // if
  else if (  mnowtoken == "("  ) { // ( {exp} [. exp] )
    // cout << "test token 2: " << mnowtoken << "\t\t" << mnowtype << endl;	    
    Expptr texp = NULL;
    try {
      texp = Readexp( false ) ;
      temp->lefte = texp;
      temp = temp->lefte;        
      Gettoken();        
    } // try
    catch ( ReadError  &readerror ) {
      throw readerror;
    } // catch


    while (  mnowtoken != ")"  &&  mnowtoken != "." && !mend ) { // deal with {exp}
      Expptr texp2 = NULL;
      try {
        texp2 = Readexp( true ) ;
        temp->righte = texp2;
        temp = temp->righte;          
        Gettoken();          
      } // try
      catch ( ReadError  &readerror ) {
        throw readerror;
      } // catch        

    } // while

    if ( mnowtoken == "." ) { // [. exp]
      Expptr texp3 = NULL;
      try {
        Gettoken(); // to judgement next token is what
        texp3 = Readexp( true ) ;
        if ( texp3 != NULL  ) {
          if ( texp3->lefte != NULL )
            temp->righte = texp3->lefte;
          else if ( texp3->leftt->type != NILL ) {
            temp->rightt = texp3->leftt;             
          } // if

          Gettoken(); 
        } // if
        else {
          throw  ReadError( mtokenline, mtokencolumn, 2, mnowtoken  ) ;
        } // else

      } // try
      catch ( ReadError  &readerror ) {
        throw readerror;
      } // catch

    } // if

    // if the least one is not ')' need to send error
    if (  mnowtoken != ")"  ) {
      if ( mend ) 
        mtokencolumn++; 
              
      throw  ReadError( mtokenline, mtokencolumn, 3,  mnowtoken  ) ;             
    } // if


  } // else if
  else if (  mnowtype != -1   ) { // atom
    // cout << "test token 3: " << mnowtoken << "\t\t" << mnowtype << endl;    
    temp2->name = mnowtoken ;
    temp2->type = mnowtype;
    head->leftt = temp2;
  } // else if 
  else { // is wrong type in the exp
    if ( mnowtoken == ""  ) // notthing input but not error
      return NULL;
    throw  ReadError( mtokenline, mtokencolumn, 2, mnowtoken  ) ;
  } // else

  return head;

} // Readexp()



void Project1::Printwhitesp() {
  int n = mlinelp * 2;
  for ( int i = 0; n > i ; i++  )
    cout << " ";
} // Printwhitesp()

void Project1::Printtoken(  Tokenptr temp1, bool right  ) { // recoginize token type and then change their name

  if ( right ) { // if righttoken != NiLL cout ". tokenname"
    Printwhitesp() ;
    cout << ".\n" ;
  } // if

  if ( mprinttok ) // char'(' next one doesn't print whitespace 
    Printwhitesp() ; // prevent ". nill " print whitespace
  mprinttok = true;
    
  if ( temp1->type == SYMBAL  ) // string || symbal
    cout << temp1->name << endl;
  else if ( temp1->type == STRING )
    cout << "\"" << temp1->name << "\"" << endl;
  else if ( temp1->type == INT   )  { // int
    int num = atoi( temp1->name.c_str() ) ;
    cout << num << endl;      
  } // if
  else if ( temp1->type == FLOAT  ) { // float
    double flo = 0.0;
    flo = atof(  temp1->name.c_str()  );
    cout << fixed <<  setprecision( 3 )  << flo  <<  endl;
  } // if  
  else if ( temp1->type == NILL  ) // Nill
    cout << "nil" << endl;
  else if ( temp1->type == TRUE  ) // true
    cout << "#t" << endl; 
  else if ( temp1->type == QUOTE  ) // QUOTE
    cout << "quote" << endl;

} // Printtoken()

void Project1::Printexp( Expptr temp ) {
  // leftexp
  if ( temp->lefte != NULL  ) {
    // maybe the previous one is '(' so doesn't need to print ' '
    if ( mprinttok  )
      Printwhitesp();
    cout << "( " ;
    mprinttok = false;
    mlinelp++;
    Printexp( temp->lefte );
    mlinelp--;
    Printwhitesp();
    cout << ")\n" ;
  } // if
  else if (  temp->leftt != NULL ) {
    Printtoken( temp->leftt, false ) ;
  } // else if

  // rightexp

  if (  temp->righte != NULL )
    Printexp( temp->righte ) ;
  else if ( temp->rightt != NULL  ) 
    Printtoken( temp->rightt, true ) ;

} // Printexp()

bool Project1::Isend(  Expptr temp ) {  // consider is needed exit yes or not
  if ( temp == NULL ) // maybe is "\' "
    return false;
  Tokenptr temp2;
  if ( temp->lefte != NULL  ) {
    temp = temp->lefte;
    if ( temp->leftt != NULL && temp->righte == NULL ) {
      temp2 = temp->leftt;
      if ( temp2->name == "exit"  ) {
        if (  temp->rightt != NULL ) { // if input is ( exit . nil ) can exit
          temp2 = temp->rightt;
          if ( temp2->type == 4  )
            return true;
        } // if
        else
          return true;
      } // if
      
    } // if

  } // if

  return false;
}  // Isend()

void Project1::Startexp() { // do readexp if have error print
  mnowchar = '\0';
  mlinelp = 0;
  mprinttok = false;
  Expptr testline;
  mlinename = "";
  try {
    testline = Readexp( false );
    mnowcolumn = 0;
    if (  !Isend( testline )  ) {
      if ( testline != NULL  )
        Printexp(  testline  );
        
    } // if
    else {
      mend = true;
      return;
    } // else

    while ( cin.peek() != EOF && Iswhitesp( cin.peek() ) && cin.peek() != '\n' )
      Getchar();

    if ( cin.peek() == ';' ) {
      while (  cin.peek() != EOF && cin.peek() != '\n'  ) // read explain 
        Getchar();
    } // if

    if ( cin.peek() == '\n'  )
      Getchar();   
  } // try
  catch ( ReadError  &readerror ) {
    cout << readerror.Error() << endl;
    while ( !mend && mnowchar != '\n' )
      Getchar();   
  } // catch

  if ( mend && ( mlinename == "" || mlinename == "\'" ) ) // read EOF and have input and onli quote
    cout << "ERROR (no more input) : END-OF-FILE encountered" ;
  else if ( mend  )  // read EOF and not input
    cout << "\n> ERROR (no more input) : END-OF-FILE encountered" ;    

  mnowchar = '\0' ;
  mnowline = 1;
} // Startexp()



// int main() {
//   Project1 test;
//   string  testnum;
//   char ch;
//   int name;
//   test.mend = false;
//   test.mnowline = 1;
//   test.mnowcolumn = 0;
//   cout << "Welcome to OurScheme!\n";
//   cin >> testnum;
//   ch = cin.get();
//   while ( !test.mend  ) {
//     cout << "\n> ";
//     test.Startexp();
//   } // while

//   cout << "\nThanks for using OurScheme!" ;

// } // main()