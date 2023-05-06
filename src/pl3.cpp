# include <stdio.h>
# include <stdlib.h> // atoi
# include <iostream> // cin cout endl
// # include <string>
// # include <vector>
# include <iomanip> // fix
# include <exception> // try catch
# include <sstream> // itos
# include "../include/pl3.h"
using namespace std;

 // =========================== define operator 
// # define STRING 1
// # define INT    2
// // {0-9}
// # define FLOAT  3
// // [0-9]*.{0-9}
// # define NILL   4
// // nil or #f or ()
// # define TRUE   5
// // #t or t
// # define SYMBAL 6
// // expect operator
// # define QUOTE  8
// // 'or quote
// # define NONOPERTOR 7
// # define CONSTRUCTOR 9
// // list cons
// # define DEFINE 10
// // define
// # define PARTACCESSOR 11 
// // car cdr
// # define PRIMITIVE 12
// // atom? list? ....etc
// # define BASIC 13
// // + - * / ....etc
// # define EQIV  14
// // eqv? equal?
// # define BEGIN 15
// // begin
// # define COND  16
// // if cond
// # define CLEAN 17 
// //  (clean-enviornment) exit
// # define FUNCTION 18
// // let lambda or define function
// # define SETF 19
// // lambda
// // ==============================define operator end

// struct Token {
//   int type;
//   string name;
//   int site;
// };


// typedef Token* Tokenptr;

// struct Exp {
//   Exp* lefte ;
//   Exp* righte ;
//   Token* leftt ;
//   Token* rightt ;
//   int   arge;
// };

// typedef Exp* Expptr;

// struct Symbal {
//   string symbalname ;
//   Expptr symdefine ;
// };

// struct Function {
//   vector<Symbal>  locsym;
//   Expptr  runline;
// };

// struct Funsym {
//   bool islet;
//   vector<Symbal>  locsym;  
// };

// =====================================struct

void Itos( string & temp, int num   ) {
  stringstream sstream;
  sstream << num;
  temp = sstream.str() ;        
} // Itos()

void Ftos( string & temp, double num   ) {
  stringstream sstream;
  sstream << num;
  temp = sstream.str() ;        
} // Ftos()

ThrowReadError::ThrowReadError( int line, int column, int etype, string tokenname ) {
  Itos( mline, line ) ;
  Itos( mcolumn, column  ) ;
  merrortype = etype;  
  mnowtoken = tokenname;
} // ThrowReadError()

string ThrowReadError::Error() {
  string readerror;
  if ( merrortype == 1 ) { 
  // when input string program get "\n" not closing
    readerror = "ERROR (no closing quote) : END-OF-LINE encountered at Line " + mline;
    readerror += " Column " + mcolumn ;
  } // if
  else if ( merrortype == 2  ) { 
  // input not correct is not atom or '('
    readerror = "ERROR (unexpected token) : atom or '(' expected when token at Line " + mline ;
    readerror +=  " Column " + mcolumn + " is >>" +  mnowtoken + "<<";
  } // else if
  else if (  merrortype == 3  ) {
  // input is not ')' when parse is ( {exp} . [exp] )
    readerror = "ERROR (unexpected token) : ')' expected when token at Line " + mline ;
    readerror += " Column " + mcolumn + " is >>" +  mnowtoken + "<<";
  } // if 
  else if (   merrortype == 4 ) {
    readerror = "ERROR (no more input) : END-OF-FILE encountered" ;
  } // if

  return readerror;
} // Error()


RunError::RunError( int type, string name, Expptr errorline   ) {
  mtype = type;
  mname = name;
  merrorline = errorline;
  mhave = true;
} // RunError()

string RunError::Error() {
  string runerror;
  if ( mtype == 1 )
    runerror = "ERROR (non-list) : ";
  else if ( mtype == 2 )
    runerror = "ERROR (incorrect number of arguments) : " + mname +"\n";
  else if ( mtype == 3 )
    runerror = "ERROR ("+ mname +" with incorrect argument type) : ";
  else if ( mtype == 4 )
    runerror = "ERROR (attempt to apply non-function) : ";
  else if ( mtype == 5 )
    runerror = "ERROR (no return value) : " ;
  else if ( mtype == 6 )
    runerror = "ERROR (unbound symbol) : " + mname + "\n";
  else if ( mtype == 7 )
    runerror = "ERROR (division by zero) : /\n" ;
  else if ( mtype == 8 )
    runerror = "ERROR (" + mname + " format) : ";  
  else if ( mtype == 9 )
    runerror = "ERROR (level of " + mname + ")\n" ; 
  else if ( mtype == 10 )
    runerror = "ERROR (unbound parameter) : " ; 
  else if ( mtype == 11 )
    runerror = "ERROR (unbound test-condition) : " ; 
  else if ( mtype == 12 )
    runerror = "ERROR (unbound condition) : " ;                   
  return runerror;     
          
} // Error()
  


// ====================================================error


bool Project3::Isnum( char com ) { 
  // number
  if ( ( int ) com >= 48 && ( int ) com <= 57 )
    return true;
  return false;  
} // Isnum()

bool Project3::Iswhitesp(  char com  ) {
  // whitespace  
  if (  com == ' ' || com == '\t' || com == '\n' || com == '\0' )
    return true; 
  return false;
} // Iswhitesp()

bool Project3::Ischar( char com ) { 
// char -> a-z or A-Z
  if ( ( ( int ) com >= 65 && ( int ) com <= 90 ) || (  ( int ) com >= 97 && ( int ) com <= 122 ) ) {
    return true;
  } // if

  return false;
} // Ischar()

bool Project3::Isspread( char com  ) { 
  // encounter this need to cut token ( expect token )
  // cout << com << endl;
  if (  Iswhitesp( com  )  || com == '\'' || com == '\"' || com == ';' || com == '(' || com == ')' )
    return true;
  return false;  
} // Isspread()

void Project3::Getchar() {
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

bool Project3::IsBasic( string op  ) {
  // operator (BASIC)
  string basic[16] = { "+", "-", "*", "/", "not", "and", "or", ">", ">=",
    "<", "<=", "=", "string-append", "string>?", "string<?", "string=?" } ;
  for ( int i = 0; 16 > i ; i++  ) {
    if ( basic[i] == op  )
      return true;  
  } // for 
  
  return false;
} // IsBasic()

bool Project3::IsPrimitive( string op  ) {
  // operator (PRIMITIVE)
  string primitive[10] = { "atom?", "pair?", "list?", "null?", "integer?", 
    "real?", "number?", "string?", "boolean?", "symbol?" };
  for ( int a = 0; 10 > a ; a++  ) {
    if ( primitive[a] == op  )
      return true;  
  } // for 
  
  return false;  
} // IsPrimitive()

void Project3::Readsymbal() {

  while (  cin.peek() != EOF  && !Isspread( cin.peek() )  ) {
    Getchar();
    mnowtoken += mnowchar;
  } // while
  
  if (  mnowtoken == "#t" || mnowtoken == "t" ) {
    mnowtoken = "#t";
    mnowtype = TRUE ;
  } // if
  else if ( mnowtoken == "#f" || mnowtoken == "nil"  ) {
    mnowtoken = "nil";
    mnowtype = NILL;
  } // else if
  else if ( mnowtoken == "define" )
    mnowtype = DEFINE;
  else if ( mnowtoken == "quote" )
    mnowtype = QUOTE;
  else if ( mnowtoken == "begin" )
    mnowtype = BEGIN;
  else if ( mnowtoken == "cons" || mnowtoken == "list" )
    mnowtype =  CONSTRUCTOR;
  else if ( mnowtoken == "car" || mnowtoken == "cdr"  )
    mnowtype =   PARTACCESSOR;
  else if (  mnowtoken == "eqv?" || mnowtoken == "equal?"  )
    mnowtype =   EQIV;
  else if ( mnowtoken == "if" || mnowtoken == "cond"  )
    mnowtype = COND;
  else if ( mnowtoken == "clean-environment" || mnowtoken == "exit" )
    mnowtype = CLEAN;
  else if (  mnowtoken == "let" )
    mnowtype = FUNCTION;
  else if ( mnowtoken == "lambda" )
    mnowtype = SETF;
  else if ( IsBasic( mnowtoken  )  )
    mnowtype = BASIC;
  else if ( IsPrimitive( mnowtoken )  )
    mnowtype = PRIMITIVE;
  else
    mnowtype = SYMBAL ;

} // Readsymbal()

void Project3::Readnumber(  bool isfloat   ) {

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
    mnowtype = BASIC;
} // Readnumber()


void Project3::Readstring() {
  int previouscolumn = mnowcolumn-1, previousline = mnowline;
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
        throw  ThrowReadError( previousline, previouscolumn, 1, "" );
      } // else if
      else if (  mnowchar != '\n'  )
        mnowtoken =  mnowtoken+ '\\' + mnowchar; 
      /*
      else if( mnowchar == '0' ) // ���i��|���� 
      mnowtoken += '\0'; 
      */
    } // if
    else
      mnowtoken += mnowchar;
    previouscolumn = mnowcolumn+1;
    previousline = mnowline;        
    Getchar();
  } // while
  
  if (  mnowchar != '\"'  ) {
    throw  ThrowReadError( previousline, previouscolumn, 1, ""  ) ;
  } // if
  
  // mnowtoken += mnowchar;    
  mnowtype = STRING;
}  // Readstring()

void Project3::Gettoken() {
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
    else if ( !Isspread( cin.peek() )  ) { // SYMBAL
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
    catch ( ThrowReadError  &readerror ) {
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
    while (  cin.peek() != EOF && cin.peek() != '\n'  ) // �����Ū�� 
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

void Project3::AllNULL( Expptr temp  ) {
  temp->lefte = NULL;
  temp->leftt = NULL;
  temp->righte = NULL;
  temp->rightt = NULL;
  temp->arge = 0;
} // AllNULL()



Expptr Project3::Readexp( bool haveread ) {
  Expptr temp = new Exp();
  AllNULL( temp );
  Expptr head = temp;
  Tokenptr temp2 = new Token();
  if (  !haveread  ) {
    try {
      Gettoken();
    } // try
    catch ( ThrowReadError  &readerror ) {
      throw readerror;
    }  // catch

  }  // if

  if (  mnowtoken == "\'" && mnowtype == QUOTE )  { // 'exp
    // cout << "test token 1: " << mnowtoken << "\t\t" << mnowtype << endl;    
    temp->lefte = new Exp();
    temp = temp->lefte ;
    AllNULL( temp );
    temp2->name = "quote" ;
    temp2->type = QUOTE ;
    temp->leftt = temp2 ;
    Expptr texp = NULL;
    try {
      texp = Readexp( false ) ;
      temp->righte = texp;
      if ( texp != NULL )
        head->arge = 1;
    } // try
    catch ( ThrowReadError  &readerror ) {
      throw readerror;
    } // catch


  } // if
  else if (  mnowtoken == "(" && mnowtype == -1 ) { // ( {exp} [. exp] )
    // cout << "test token 2: " << mnowtoken << "\t\t" << mnowtype << endl;	    
    Expptr texp = NULL;
    try {
      texp = Readexp( false ) ;
      temp->lefte = texp;
      temp = temp->lefte;
      Gettoken();
    } // try
    catch ( ThrowReadError  &readerror ) {
      throw readerror;
    } // catch

    int num = 0 ;
    while (  ( mnowtoken != ")" || mnowtype != -1 ) && ( mnowtoken != "." ||  mnowtype != -1  )
            && !mend  ) { // deal with {exp}
      Expptr texp2 = NULL;
      try {
        texp2 = Readexp( true ) ;
        temp->righte = texp2;
        temp = temp->righte;   
        num++;
        Gettoken();          
      } // try
      catch ( ThrowReadError  &readerror ) {
        throw readerror;
      } // catch        

    } // while

    if ( mnowtoken == "." &&  mnowtype == -1 ) { // [. exp]
      Expptr texp3 = NULL;
      try {
        Gettoken(); // to judgement next token is what
        texp3 = Readexp( true ) ;
        if ( texp3 != NULL  ) {
          if ( texp3->lefte != NULL ) {
            num++;
            temp->righte = texp3->lefte;
          } // if
          else if ( texp3->leftt != NULL && texp3->leftt->type != NILL ) {
            temp->rightt = texp3->leftt;             
          } // if

          Gettoken(); 
        } // if
        else {
          throw  ThrowReadError( mtokenline, mtokencolumn, 2, mnowtoken  ) ;
        } // else

      } // try
      catch ( ThrowReadError  &readerror ) {
        throw readerror;
      } // catch

    } // if

    // if the least one is not ')' need to send error
    if (  mnowtoken != ")" ||  mnowtype != -1 ) {
      int errortype = 3;        
      if ( mend )
        errortype = 4;         
      throw  ThrowReadError( mtokenline, mtokencolumn, errortype,  mnowtoken  ) ;             
    } // if
    
    head->arge = num; // store arge number
  } // else if
  else if (  mnowtype != -1   ) { // atom
    // cout << "test token 3: " << mnowtoken << "\t\t" << mnowtype << endl;    
    temp2->name = mnowtoken ;
    temp2->type = mnowtype;
    head->leftt = temp2;
  } // else if 
  else { // is wrong type in the exp
    int errortype = 2;
    if ( mnowtoken == ""  ) // notthing input but not error
      errortype = 4; 
    throw  ThrowReadError( mtokenline, mtokencolumn, errortype, mnowtoken  ) ;
  } // else

  return head;
} // Readexp()

void Project3::Printwhitesp() {
  int n = mlinelp * 2;
  for ( int i = 0; n > i ; i++  )
    cout << " ";
} // Printwhitesp()

void Project3::Printtoken(  Tokenptr temp1, bool right, bool error  ) {
// recoginize token type and then change their name
  if ( right ) { // if righttoken != NiLL cout ". tokenname"
    Printwhitesp() ;
    cout << ".\n" ;
  } // if

  if ( mprinttok ) // char'(' next one doesn't print whitespace 
    Printwhitesp() ; // prevent ". nill " print whitespace
  mprinttok = true;
    

  if ( temp1->type == STRING )
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
  else if ( temp1->type == SYMBAL || temp1->type == NONOPERTOR || error ) // NONOPERTOR || symbal
    cout << temp1->name << endl;      
  else if ( temp1->type > NONOPERTOR  )  
    cout << "#<procedure " +   temp1->name + ">\n" ;

} // Printtoken()

void Project3::Printexp( Expptr temp, bool error ) {
  if ( temp == NULL  )
    return;  
  // leftexp
  if ( temp->lefte != NULL  ) {
    // maybe the previous one is '(' so doesn't need to print ' '
    if ( mprinttok  )
      Printwhitesp();
    cout << "( " ;
    mprinttok = false;
    mlinelp++;
    Printexp( temp->lefte, error );
    mlinelp--;
    Printwhitesp();
    cout << ")\n" ;
  } // if
  else if (  temp->leftt != NULL ) {
    Printtoken( temp->leftt, false, error ) ;
  } // else if

  // rightexp

  if (  temp->righte != NULL )
    Printexp( temp->righte, error ) ;
  else if ( temp->rightt != NULL  )
    Printtoken( temp->rightt, true, error ) ;

} // Printexp()


Expptr Project3::Arith(  Expptr temp, int op ) {
  int total1 = 0;
  double total2 = 0.0;
  bool isfloat = false;
  Expptr temp1 = new Exp();
  Tokenptr token2 = NULL;
  string name = "";
  if ( op == 1 )
    name = "+";
  else if ( op == 2 )
    name = "-";
  else if ( op == 3 )
    name = "*";
  else
    name = "-";  

  try {
    temp1 = Runexp( temp, false );      
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch


  if ( temp1->leftt != NULL ) {
    token2 = temp1->leftt;
    if ( token2->type == FLOAT ) {
      isfloat = true;  
      double num2 = atof( token2->name.c_str() );
      total2 = num2;
    } // if
    else if ( token2->type == INT  ) {
      int num1 =  atoi( token2->name.c_str() );
      total1 = num1; 
    } // if
    else {
      Expptr error = new Exp();
      AllNULL( error );
      error->leftt = token2;
      throw RunError( 3, name, error ); 
    } // else 

    temp = temp->righte;       
  } // if
  else // list is not number
    throw RunError( 3, name, temp1 );

  while ( temp != NULL ) {
    // if temp->rightt == NULL
    try {
      temp1 = Runexp( temp, false );        
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch

    if ( temp1->leftt != NULL ) {
      token2 = temp1->leftt;
      if ( token2->type == FLOAT || ( isfloat && token2->type == INT ) ) {
        if ( !isfloat ) {
          total2 =  ( double ) total1;
          isfloat = true;
        } // if
        
        double num2 = atof( token2->name.c_str() );
        if ( op == 1 )
          total2 = total2 + num2;
        else if ( op == 2 )            
          total2 = total2 - num2;
        else if ( op == 3 )            
          total2 = total2 * num2;   
        else if ( op == 4 )  { 
          if ( num2 != 0 )
            total2 = total2 / num2;
          else 
            throw RunError( 7, name, NULL );                           
        } // if                
      } // if
      else if ( token2->type == INT  ) {
        int num1 =  atoi( token2->name.c_str() );
        if ( op == 1 )
          total1 = total1 + num1;
        else if ( op == 2 )            
          total1 = total1 - num1;
        else if ( op == 3 )            
          total1 = total1 * num1;   
        else if ( op == 4 )  {
          if ( num1 != 0 )
            total1 = total1 / num1;
          else 
            throw RunError( 7, name, NULL );               
        } // if        
      } // if
      else {
        Expptr error = new Exp();
        AllNULL( error );
        error->leftt = token2;
        throw RunError( 3, name, error ); 
      } // else
      
    } // if
    else {
      Expptr error = temp1;
      string ename = name;
      throw RunError( 3, ename, error );        
    } // else

      
    temp = temp->righte; 
  } // while
  
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();
  if ( isfloat ) {
    Ftos( token3->name, total2  );
    token3->type = FLOAT;
  } // if
  else {
    Itos( token3->name, total1  );
    token3->type = INT;
  } // else
  
  ans->leftt = token3;
  return ans;
} // Arith()

Expptr Project3::SAdd( Expptr temp ) { // string-append
  string name;
  Expptr temp1;
  Tokenptr token1; 
  while ( temp != NULL ) {
    // if temp->rightt == NULL  
    try {
      temp1 = Runexp( temp, false );        
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch

    if ( temp1->leftt != NULL ) {
      token1 = temp1->leftt;
      if ( token1->type == STRING  ) {
        name +=  token1->name;
      } // if
      else {
        Expptr error = new Exp();
        AllNULL( error );
        error->leftt = token1;
        throw RunError( 3, "string-append", error ); 
      } // else        
    } // if
    else { // list is not number
      string ename = "string-append";
      throw RunError( 3, ename, temp1 );
    } // else
    
    temp = temp->righte; 
  } // while 
  
  Expptr ans = new Exp();
  AllNULL( ans ); 
  Tokenptr token2 = new Token();
  token2->name = name;
  token2->type = STRING;     
  ans->leftt = token2;
  return ans;
} // SAdd()

Expptr Project3::Compnum( Expptr temp, int op, string name ) {
  double com1 = 0.0;
  double com2 = 0.0;
  Expptr temp1 = new Exp();
  bool   correct = true;
  Tokenptr token2 = NULL;
  try {
    temp1 = Runexp( temp, false );      
  } // try
  catch  ( RunError & runerror ) {
    throw runerror;
  } // catch

  if ( temp1->leftt != NULL ) {
    token2 = temp1->leftt;
    if ( token2->type == FLOAT || token2->type == INT ) { 
      com1 = atof( token2->name.c_str() );
    } // if
    else {
      Expptr error = new Exp();
      AllNULL( error );
      error->leftt = token2;
      throw RunError( 3, name, error ); 
    } // else        
    
    temp = temp->righte;       
  } // if 
  else { // list is not number
    string ename = name;
    throw RunError( 3, ename, temp1 );
  } // else
  
  while (  temp != NULL ) {
    try {
      temp1 = Runexp( temp, false );        
    } // try
    catch  ( RunError & runerror ) {
      throw runerror;
    } // catch
    
    if ( temp1->leftt != NULL ) {
      token2 = temp1->leftt;
      if ( token2->type == FLOAT || token2->type == INT ) { 
        com2 = atof( token2->name.c_str() );
      } // if
      else {
        Expptr error = new Exp();
        AllNULL( error );
        error->leftt = token2;
        throw RunError( 3, name, error ); 
      } // else        
    } // if 
    else { // list is not number
      string ename = name;
      throw RunError( 3, ename, temp1 );
    } // else
    
    // cout << "com1 :" << com1 << " com2 : " << com2 << endl;
    if ( op == 1 && com1 <= com2 )  // >
      correct = false;
    else if ( op == 2 && com1 < com2 )  // >=
      correct = false;
    else if ( op == 3 && com1 >= com2 )  // <
      correct = false;
    else if ( op == 4  && com1 > com2 ) // <=
      correct = false;
    else if ( op == 5 && com1 != com2 )  // =
      correct = false;

    com1 = com2;
    temp = temp->righte;
  } // while
  
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();
  if ( correct ) {
    token3->name = "#t";
    token3->type = TRUE;
  } // if
  else {
    token3->name = "nil";
    token3->type = NILL;       
  } // else
  
  ans->leftt = token3;
  return ans;
    
} // Compnum()

bool Project3::Eval(   Expptr temp ) {
  if ( temp != NULL  ) { 
    if ( temp->rightt != NULL  )
      return false;
    else 
      return Eval( temp->righte );  
  } // if
  
  return true; 
} // Eval()

Expptr Project3::Compstr( Expptr temp, int op, string name ) {
  string com1 = "";
  string com2 = "";
  Expptr temp1 = new Exp();
  bool   correct = true;
  Tokenptr token2 = NULL;
  try {
    temp1 = Runexp( temp, false );      
  } // try
  catch  ( RunError & runerror ) {
    throw runerror;
  } // catch
  
  if ( temp1->leftt != NULL ) {
    token2 = temp1->leftt;
    if ( token2->type == STRING  ) { 
      com1 = token2->name ;
    } // if
    else {
      Expptr error = new Exp();
      AllNULL( error );
      error->leftt = token2;
      throw RunError( 3, name, error ); 
    } // else
      
    temp = temp->righte;       
  } // if 
  else // list is not number
    throw RunError( 3, name, temp1 );
  
  while (  temp != NULL ) {
    try {
      temp1 = Runexp( temp, false );        
    } // try 
    catch  ( RunError & runerror ) {
      throw runerror;
    } // catch

    if ( temp1->leftt != NULL ) {
      token2 = temp1->leftt;
      if ( token2->type == STRING ) { 
        com2 = token2->name;
      } // if
      else {
        Expptr error = new Exp();
        AllNULL( error );
        error->leftt = token2;
        throw RunError( 3, name, error ); 
      } // else       
    } // if 
    else { // list is not number
      string ename = name;
      throw RunError( 3, ename, temp1 );
    } // else
    
    if ( op == 1 && ( com1 < com2 || com1 == com2 ) )  // >
      correct = false;        
    else if ( op == 2 && ( com1 > com2 || com1 == com2 )  )  // <
      correct = false;  
    else if ( op == 3 && com1 != com2  )  // =
      correct = false;  
      
    com1 = com2;
    temp = temp->righte;          
  } // while
  
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();
  if ( correct ) {
    token3->name = "#t";
    token3->type = TRUE;
  } // if
  else {
    token3->name = "nil";
    token3->type = NILL;       
  } // else
  
  ans->leftt = token3;
  return ans;
    
} // Compstr()

Expptr Project3::Not(  Expptr temp ) {
  Expptr temp1 = new Exp();
  bool   correct = true;    
  Tokenptr token2 = NULL;
  try {
    temp1 = Runexp( temp, false );      
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch
  
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();    
  if ( temp1->leftt != NULL ) {
    token2 = temp1->leftt;
    if ( token2->type != NILL  ) { 
      token3->name = "nil";
      token3->type = NILL;
    } // if
    else {
      token3->name = "#t";
      token3->type = TRUE;          
    } // else       
  } // if
  else {
    token3->name = "nil";
    token3->type = NILL;        
  } // else
    
  ans->leftt = token3;
  return ans;   
} // Not()

void Project3::Quote( Expptr temp ) { 
// all operator to be non operator
  if ( temp != NULL  ) {
    if ( temp->leftt != NULL && temp->leftt->type  > NONOPERTOR ) 
      temp->leftt->type = NONOPERTOR;
    else if ( temp->lefte != NULL )
      Quote( temp->lefte );  
    if ( temp->rightt != NULL && temp->rightt->type  > NONOPERTOR ) 
      temp->rightt->type = NONOPERTOR;
    else if ( temp->righte != NULL )
      Quote( temp->righte );    
    
  }  // if
  
} // Quote()

Expptr Project3::OrAnd( Expptr temp, int op ) { // maybe have error
  // op == 1 : and op == 2: or
  Expptr temp1 = new Exp();
  Expptr ans ;
  bool   correct = true;
  if ( op == 2 )
    correct = false;
  Tokenptr token2 = NULL;
  
  while (  temp != NULL ) {
    try {
      temp1 = Runexp( temp, false );
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch
    
    if ( temp1->leftt != NULL && temp1->leftt->type == NILL && op == 1 ) {
      ans = NULL;
      ans = new Exp();
      AllNULL( ans );
      ans->leftt = new Token();
      ans->leftt->name = "nil";
      ans->leftt->type = NILL;
      return ans;
    } // if
    else if ( op == 1 )
      ans = temp1;  
    else if ( temp1->leftt != NULL && temp1->leftt->type != NILL && op == 2 ) {
      if ( !correct )
        ans = temp1;  
      correct = true; 
      return ans;           
    }  // if 

    temp = temp->righte;          
  } // while
  


  if ( !correct ) {
    ans = NULL;
    ans = new Exp();
    AllNULL( ans );
    ans->leftt = new Token();
    ans->leftt->name = "nil";
    ans->leftt->type = NILL;       
  } // if
  
  return ans;
    
} // OrAnd()  

Expptr Project3::List( Expptr temp ) {
  Expptr texp = NULL, ans = new Exp();
  Expptr temp1 = ans;
  if ( temp == NULL ) {
    temp1->leftt = new Token();
    temp1->leftt->type = NILL;
    temp1->leftt->name = "nil";
    return ans;
  } // if
  else if ( temp != NULL ) {
    try {
      texp =  Runexp( temp, false );        
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch
    
    temp1 = temp1->lefte = new Exp();      
    if ( texp->leftt != NULL )
      temp1->leftt = texp->leftt;
    else
      temp1->lefte = texp->lefte; 
    temp = temp->righte;   
  } // if
  else 
    ans->lefte = new Exp();
              
  while (  temp != NULL ) { 
    // deal with ( list (exp) (exp) ( exp ) .... )
    temp1 = temp1->righte = new Exp();      
    try {
      texp =  Runexp( temp, false );        
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch

    if ( texp->leftt != NULL )
      temp1->leftt = texp->leftt;
    else
      temp1->lefte = texp->lefte;  
    temp = temp->righte;   
  } // while
  
  return ans;
  
} // List()

Expptr Project3::Islist( Expptr temp, int op ) {
  Expptr temp1 = new Exp();
  try {
    temp1 = Runexp( temp, false );      
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch
  
  bool correct = false;
  if ( op == 1 && temp1->lefte != NULL ) // list?
    correct = Eval( temp1->lefte );
  else if ( op == 2 && temp1->lefte != NULL )
    correct = true;
  else if   ( op == 3 && temp1->leftt != NULL )  
    correct = true;
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();
  if ( correct ) {
    token3->name = "#t";
    token3->type = TRUE;
  } // if
  else {
    token3->name = "nil";
    token3->type = NILL;       
  } // else
  
  ans->leftt = token3;
  return ans;        
} // Islist()

Expptr  Project3::Isneed( Expptr temp, int com1, int com2  ) {
  Expptr temp1 = new Exp();
  try {
    temp1 = Runexp( temp, false );      
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch

  bool correct = false;
  Tokenptr token1;
  if ( temp1->leftt != NULL ) {
    token1 = temp1->leftt;
    if ( token1->type == com1 || token1->type == com2 )
      correct = true;        
  } // if
  else
    correct = false;  
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();
  if ( correct ) {
    token3->name = "#t";
    token3->type = TRUE;
  } // if
  else {
    token3->name = "nil";
    token3->type = NILL;       
  } // else
  
  ans->leftt = token3;
  return ans;  
} // Isneed()

Expptr Project3::Cons( Expptr temp ) {
  Expptr cons1 = NULL, cons2 = NULL;
  try {
    cons1 = Runexp( temp, false );
    temp = temp->righte ;
    cons2 = Runexp( temp, false );      
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch

  Expptr ans = new Exp();
  AllNULL( ans );
  ans->lefte = new Exp();
  AllNULL( ans->lefte );
  if ( cons1->lefte != NULL ) {
    ans->lefte->lefte = cons1->lefte;
  } // if
  else if ( cons1->leftt != NULL )
    ans->lefte->leftt = cons1->leftt;

  if ( cons2->lefte != NULL ) {
    ans->lefte->righte = cons2->lefte;
  } // if
  else if ( cons2->leftt != NULL ) {
    if ( cons2->leftt->type != NILL )
      ans->lefte->rightt = cons2->leftt;      
  } // if
    
  return ans;      
} // Cons()

bool Project3::Same( Expptr com1, Expptr com2 ) {
  bool equal = false;
  if ( com1 == NULL )
    if ( com2 == NULL  )
      return true;
    else
      return false;  
  else if ( com2 == NULL ) {     
    return false;      
  } // if

  if ( com1->leftt != NULL && com2->leftt != NULL ) {
    if ( com1->leftt->type != com2->leftt->type ) 
      return false;        
    else  if ( com1->leftt->name != com2->leftt->name ) 
      return false;           
    
  } // if
  
  if ( com1->rightt != NULL && com2->rightt != NULL ) {
    if ( com1->rightt->type != com2->rightt->type )
      return false;
    else  if ( com1->rightt->name != com2->rightt->name )
      return false;        
  } // if

  if (  com1->lefte != NULL || com2->lefte != NULL ) {
    equal = Same(  com1->lefte, com2->lefte );
    if ( !equal ) 
      return false;
  } // if
  
  if ( com1->righte != NULL || com2->righte != NULL ) {
    equal = Same(  com1->righte, com2->righte );
    if ( !equal )
      return false;
  } // if
    
  return true; 
} // Same()

Expptr Project3::Eq(  Expptr temp, int op   ) {
  Expptr com1 = NULL, com2 = NULL;
  try {
    com1 = Runexp( temp, false );
    temp = temp->righte;
    com2 = Runexp( temp, false );      
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch    

  bool equal = false;
  if ( op == 1 ) { // eqv?
    if ( com1->leftt != NULL && com2->leftt != NULL ) {
      if ( com1->leftt->type == STRING ) {
        Token* p1 = com1->leftt;
        Token* p2 = com2->leftt;
        if ( p1 == p2 )
          equal = true;
      } // if
      else if ( com1->leftt->type == com2->leftt->type )
        if ( com1->leftt->name == com2->leftt->name )        
          equal = true;
    } // if
    else {
      Exp *p1 = com1->lefte;
      Exp *p2 = com2->lefte;
      if ( p1 == p2 )
        equal = true;
    } // else     
  } // if
  else {
    equal = Same( com1, com2 );
  } // else
  
  Expptr ans = new Exp();
  AllNULL( ans );    
  Tokenptr token3 = new Token();
  if ( equal ) {
    token3->name = "#t";
    token3->type = TRUE;
  } // if
  else {
    token3->name = "nil";
    token3->type = NILL;       
  } // else
  
  ans->leftt = token3;
  return ans;      
} // Eq()

Expptr Project3::Searchs( Expptr temp  ) {
  Tokenptr token1 = temp->leftt;
  for ( int i = 0;  msymbal.size() > i ; i++   ) {
    if ( msymbal[i].symbalname == token1->name ) {
      msymbal[i].symdefine->righte = NULL;
      return msymbal[i].symdefine;        
    } // if    

  } // for
  
  throw RunError( 6, token1->name, NULL );
  return NULL;
} // Searchs()

void Project3::Define( Expptr temp, Expptr head, int argenum  ) {
  Expptr temp1 = NULL;
  temp1 = temp;        
  string name;
  temp = temp->righte;
  if (  temp1->leftt != NULL && temp1->leftt->type == SYMBAL && argenum == 2 ) {
    name = temp1->leftt->name;
    try {
      temp1 = Runexp( temp, false );                
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch           

  } // if
  else if ( temp1->lefte != NULL && argenum > 1 ) {
    temp1 = temp1->lefte;
    Function fun1;
    vector<Symbal> sym;
    int n = mheap.size();
    if ( !Eval( temp1 ) ) {
      Expptr error = new Exp();
      error->lefte = temp1;
      throw  RunError( 1, "", error  );  
    } // if  
    
    if (  temp1->leftt != NULL && temp1->leftt->type == SYMBAL )
      name = temp1->leftt->name;
    else
      throw RunError( 8, "DEFINE", head  );
    temp1 = temp1->righte;  
    while ( temp1 != NULL ) {
      Symbal sym1;
      if (  temp1->leftt != NULL && temp1->leftt->type == SYMBAL )
        sym1.symbalname = temp1->leftt->name;
      else  
        throw RunError( 8, "DEFINE", head  );        
      sym.push_back( sym1 );
      temp1 = temp1->righte;
    } // while   
    
    fun1.locsym = sym;
    fun1.runline = temp;
    mheap.push_back( fun1 );
    Expptr ans = new Exp();
    ans->leftt = new Token();
    ans->leftt->name = name;
    ans->leftt->site = n;
    ans->leftt->type = FUNCTION;      
    temp1 = ans; 
  } // if
  else
    throw RunError( 8, "DEFINE", head  );

  bool find = false;
  for (  int i = 0; msymbal.size() > i && !find ; i++  ) {
    if ( msymbal[i].symbalname == name ) {
      msymbal[i].symdefine = NULL;  
      msymbal[i].symdefine = temp1 ;
      find = true;
    } // if
  } // for
    
  if ( !find ) {
    Symbal symbal;
    symbal.symbalname = name;
    symbal.symdefine = temp1;
    msymbal.push_back( symbal );  
  } // if
    
  cout << name +  " defined\n";
          
    
} // Define()

Expptr Project3::Begin(  Expptr temp ) {
  Expptr ans = NULL;
  
  while ( temp != NULL ) {
    try {
      ans = Runexp( temp, false );        
    } // try
    catch ( RunError & runerror ) {
      if (  temp->righte == NULL || runerror.mtype != 5 || !runerror.mhave )
        throw runerror;  
    } // catch 
    temp = temp->righte;
  } // while
  
  return ans;
  
} // Begin()

Expptr Project3::Cadr(  Expptr temp, int op ) {
  // op == 1 car
  // op == 2 cdr
  Expptr ans = new Exp();
  AllNULL( ans );
  Expptr temp1 = NULL;
  try {
    temp1 = Runexp( temp, false );
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch 
  
  if ( temp1->lefte != NULL ) {
    if ( op == 1 ) { // car
      temp1 = temp1->lefte;
      if ( temp1->lefte != NULL ) // if temp1 = ( ( { exp} )... )
        ans->lefte =  temp1->lefte;
      else if ( temp1->leftt != NULL ) // temp1 = ( exp .... )
        ans->leftt = temp1->leftt;   
    } // if
    else {
      temp1 = temp1->lefte;
      if ( temp1->righte != NULL )
        ans->lefte =  temp1->righte;
      else if ( temp1->rightt != NULL )
        ans->leftt = temp1->rightt; 
      else {
        ans->leftt = new Token();
        ans->leftt->type = NILL;
        ans->leftt->name = "nil";
      } // else        
    } // else 
  } // if
  else { // car cdr parameter need list isn't atom
    string name = "cdr";
    Expptr error = new Exp();
    AllNULL( error );
    error->leftt = temp1->leftt; 
    if ( op == 1 )
      name = "car" ;
    throw RunError( 3, name, error );      
  } // else

  return ans;
} // Cadr()

Expptr Project3::If(  Expptr temp, Expptr head ) { // maybe have error
  Expptr com = NULL, ans = NULL;
  bool tf = true;
  try {
    com = Runexp( temp, false );
  } // try
  catch ( RunError & runerror ) {
    if ( runerror.mtype == 5 && runerror.mhave )
      runerror.mtype = 11;      
    throw runerror;
  } // catch 
      
  try {
    temp = temp->righte;
    if ( com->leftt != NULL && com->leftt->type == NILL )
      tf = false;
    if ( tf )
      ans = Runexp( temp, false );
    else {
      temp = temp->righte;
      if ( temp != NULL )
        ans = Runexp( temp, false );
      else
        throw RunError( 5, "", head );   
    } // else        
  } // try
  catch ( RunError & runerror ) {
    throw runerror;
  } // catch  
  
  return ans;
} // If()



Expptr Project3::Cond2(  Expptr temp, bool &tf, bool least  ) {
  Expptr com = NULL, ans = NULL;
  bool haveans = false;
  if ( temp->lefte != NULL ) {
    try {
      com = Runexp( temp, false );        
    } // try
    catch ( RunError & runerror ) {
      if ( runerror.mtype == 5  && runerror.mhave )
        runerror.mtype = 11;
      throw runerror;
    } // catch        
  } // if      
  else if ( ( temp->leftt != NULL && temp->leftt->name == "else" ) 
            && least ) {
    com = new Exp();
    com->leftt = temp->leftt;
  } // if
  else {
    try {
      com = Runexp( temp, false );         
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch      
  } // else

  if ( com->leftt != NULL && com->leftt->type == NILL )
    return NULL;
  else
    haveans = true;
    
  temp = temp->righte;

  try {
    ans = Begin( temp );      
  } // try    
  catch ( RunError & runerror ) {
    throw runerror;  
  } // catch

  tf = true;
  return ans;  
            
} // Cond2()


Expptr Project3::Cond( Expptr temp, Expptr head ) {
  Expptr temp2 = NULL, temp3 = temp;
  Expptr ans = NULL, tans = NULL;
  bool   tf = false;
  while ( temp3 != NULL ) {
    if ( !Eval( temp3->lefte ) ) {
      Expptr error = new Exp();
      error->lefte = temp3->lefte;
      throw  RunError( 1, "", error  ); 
    } // if   
          
    if ( temp3->arge < 1 )
      throw RunError( 8, "COND", head );

    temp3 = temp3->righte;
  } // while
  
  while ( temp != NULL ) {
    if ( temp->lefte != NULL ) {
      temp2 = temp->lefte; 
      try {
        if ( temp->arge < 1 )
          throw RunError( 8, "COND", head );
        if ( temp->righte != NULL && !tf )
          tans = Cond2(  temp2, tf, false );
        else if ( !tf )
          tans = Cond2(  temp2, tf, true  );          
      } // try
      catch ( RunError & runerror ) {
        throw runerror;
      } // catch          

      if ( ans == NULL && tf )
        ans = tans;                  
    } // if
    else 
      throw RunError( 8, "COND", head );      


    temp = temp->righte;
  } // while
  
  // !tf throw error
  if ( !tf )
    throw RunError( 5, "", head );
  return ans;
  
} // Cond()

Expptr Project3::Let(  Expptr temp, Expptr head, int argenum ) {
  // ( ( ... ) exp ) -> ... is { ( Symbol exp ) }
  //  set function variable and then exp into begin
  Expptr symset;
  vector<Symbal> locsym;
  if ( argenum < 2 ) 
    throw  RunError( 8, "LET", head );

  if ( temp->lefte != NULL )  { // set local symbol
    Expptr checkpoint =  temp->lefte;
    if ( !Eval( checkpoint ) ) {
      Expptr error = new Exp();
      error->lefte = checkpoint;
      throw  RunError( 1, "", error  );  
    } // if    
      
    while ( checkpoint != NULL ) {
      if ( !Eval( checkpoint->lefte ) ) {
        Expptr error = new Exp();
        error->lefte = checkpoint->lefte;
        throw  RunError( 1, "", error  );  
      } // if
              
      if ( checkpoint->lefte != NULL &&  checkpoint->arge == 1 ) {                    
        Expptr check1 = checkpoint->lefte;
        if (  check1->leftt == NULL || check1->leftt->type != SYMBAL ) {
          throw  RunError( 8, "LET", head );                   
        } // if
      } // if
      else {
        string name = "LET" ;
        throw  RunError( 8, name, head );          
      } // else

          
      checkpoint = checkpoint->righte;            
    } // while
    
    
    symset = temp->lefte;
    while ( symset != NULL  ) {
      Symbal locsym1;
      
      if ( symset->lefte != NULL && symset->arge == 1 ) { // ( sym exp )
        Expptr symset1 = symset->lefte;
        if ( symset1->leftt != NULL && symset1->leftt->type == SYMBAL ) // Is symbol
          locsym1.symbalname = symset1->leftt->name;
        else // if not symbol throw error
          throw  RunError( 8, "LET", head );   
        symset1 = symset1->righte ;
        Expptr symline;
        try {
          symline = Runexp( symset1, false );
          locsym1.symdefine = symline;
        } // try
        catch (  RunError & runerror ) {
          runerror.mhave = false;
          throw runerror;
        } // catch 
        locsym.push_back( locsym1 );
      } // if
      else
        throw  RunError( 8, "LET", head );  

      symset = symset->righte;        
    } // while   
  } // if
  else if ( temp->leftt != NULL && temp->leftt->type != NILL  )
    throw  RunError( 8, "LET", head );
  Funsym letsym;
  letsym.islet = true;
  letsym.locsym = locsym;
  mcstack.insert( mcstack.begin(), letsym );
  temp = temp->righte;
  Expptr ans;
  try {
    ans = Begin( temp );
  } // try
  catch (  RunError & runerror ) {
    mcstack.erase( mcstack.begin() );      
    throw runerror;
  } // catch     
  
  mcstack.erase( mcstack.begin() );
  return ans;
  
} // Let()


Expptr Project3::Lambda(   Expptr temp, Expptr head, int argenum  ) {
  // ( lambda ( .... ) {exp} )
  // ... is { symbol } -> one or more than one
  Function fun1;
  Expptr symset;
  vector<Symbal> locsym;    
  int n = mheap.size();

  if ( argenum < 2 )
    throw  RunError( 8, "LAMBDA", head );   

  if ( temp->lefte != NULL )  { // set local symbol ( [stmbol] ) 
    symset = temp->lefte;   
    if ( !Eval( symset ) ) {
      Expptr error = new Exp();
      error->lefte = symset;
      throw  RunError( 1, "", error  );  
    } // if  
    
    while ( symset != NULL  )  {
      Symbal locsym1; 
      if ( symset->leftt != NULL && symset->leftt->type == SYMBAL )
        locsym1.symbalname = symset->leftt->name;
      else
        throw  RunError( 8, "LAMBDA", head );
      symset = symset->righte;
      locsym.push_back( locsym1 );
    } // while   
  } // if
  else if ( temp->leftt != NULL && temp->leftt->type != NILL  )
    throw  RunError( 8, "LAMBDA", head ); 
    
  temp = temp->righte;    
  fun1.runline = temp;
  fun1.locsym = locsym;
  mheap.push_back( fun1 );
  Expptr ans = new Exp();
  ans->leftt = new Token();
  ans->leftt->name = "lambda";
  ans->leftt->site = n;
  ans->leftt->type = FUNCTION;
  return ans; 
  
} // Lambda()


Expptr Project3::RunFun( Expptr temp, Expptr head, Tokenptr token1, int argenum ) {
  // ( < proceed ( symbol or lambda ) > [variable] )
  // get function from mheap look token site
  // 1. check argenum and then sset local symbal
  // 2. run exp ( use begin )

  int n = token1->site;
  Function fun = mheap[n];
  Expptr ans ;
  if ( argenum != fun.locsym.size() )
    throw  RunError( 2, token1->name, NULL );
  vector<Symbal> locsym;
  for ( int i = 0; fun.locsym.size() > i ; i++  ) {
    Symbal locs1;
    locs1.symbalname = fun.locsym[i].symbalname;
    locsym.push_back( locs1 );
  } // for
  
  
  Expptr funline = fun.runline;
  for ( int i = 0; argenum > i && temp != NULL ; i++  ) {

    try {
      locsym[i].symdefine = Runexp( temp, false );
    } // try
    catch (  RunError & runerror ) {
      // Printexp( runerror.merrorline, false );
      if ( runerror.mtype == 5 && runerror.mhave  )
        runerror.mtype = 10;
      throw runerror;
    } // catch
    
    temp = temp->righte;
  } // for
  
  Funsym funsym;
  funsym.islet = false;
  funsym.locsym = locsym ;

  mcstack.insert( mcstack.begin(), funsym  );
  try {
    ans = Begin( funline );
  } // try
  catch (  RunError & runerror ) {
    mcstack.erase( mcstack.begin() );
    throw runerror;
  } // catch     

  mcstack.erase( mcstack.begin() );
  return ans;
      
} // RunFun()

Expptr Project3::Searchlocs( Expptr temp, bool &haveans  ) {
  Tokenptr token1 = temp->leftt;
  if ( mcstack.size() == 0 )
    return NULL;
  for ( int i = 0;  mcstack.size() > i ; i++   ) {
    Funsym  funsym = mcstack[i];
    vector<Symbal> locsym = funsym.locsym;
    for ( int j = locsym.size() - 1;  j >= 0 ; j--    ) {
      if ( locsym[j].symbalname == token1->name ) {
        locsym[j].symdefine->righte = NULL;
        haveans = true;
        return locsym[j].symdefine;        
      } // if    
    } // for
    
    // if islet == FALSE ,that mean this is define function or lambda
    if ( !funsym.islet )
      return NULL;
  } // for
  
  return NULL;
} // Searchlocs()  

Expptr Project3::Runexp( Expptr temp, bool frist ) {
  // runexp and then return ans
  // if have any error throw
  // frist check is atom or exp
  // second check is symbol or not if is symbol find 
  // thrid check is pure list or not
  if ( temp == NULL )
    return NULL;
  bool isatom = true;
  int  argenum = temp->arge;
  Expptr ans = NULL;
  Expptr head = new Exp();
  head->lefte = temp->lefte;
  Tokenptr token1 = NULL;
  if ( !Eval( head->lefte ) ) {
    Expptr error = new Exp();
    error->lefte = head->lefte;
    throw  RunError( 1, "", error  );  
  } // if
    
  if (  temp->lefte != NULL  ) { // ( { exp } )
    temp = temp->lefte;
    isatom = false;
  } // if
  else if ( temp->leftt != NULL && temp->leftt->type != SYMBAL  ) {
    ans = new Exp();
    ans->leftt =  temp->leftt; 
  } // if
  else {
    try {
      bool haveans = false;
      ans = Searchlocs( temp, haveans );
      if ( !haveans )
        ans  = Searchs( temp );          
    } // try
    catch (  RunError & runerror ) {
      throw runerror;
    } // catch
  } // else
  


  if ( isatom  )
    return ans;   
  else if (  temp->leftt != NULL ) {
    if ( temp->leftt->type != SYMBAL )
      token1 = temp->leftt ;
    else {
      // maybe( def a + ) -> ( a 5 6 )
      Expptr temp1;
      bool haveans = false;
      temp1 = Searchlocs( temp, haveans );
      if ( !haveans )        
        temp1 = Searchs( temp );
      if ( temp1->leftt != NULL ) 
        token1 = temp1->leftt ;
      else // no one operation is list
        throw RunError( 4, "", temp1  );                
    } // else

    temp = temp->righte ; 
  } // else if
  else  if (  temp->lefte != NULL )  {
    // ( ( car ( list ( + 1 ) ) ) x y ) or ( ( lambda ....) x y ) 
    Expptr temp1;
    try {
      temp1 = Runexp( temp, false );        
    }
    catch (  RunError & runerror ) {
      runerror.mhave = false;
      throw runerror;
    } // catch
    
    if ( temp1->leftt != NULL  ) {
      token1 = temp1->leftt ;
      temp = temp->righte ;            
    } // if
    else // no one operation is list
      throw RunError( 4, "", temp1  ); 
    // throw error
  } // else if

  
  try {
    if ( token1->type == CONSTRUCTOR ) {
      if ( token1->name == "list" )
        ans = List( temp );
      else if ( token1->name == "cons" && argenum == 2 )
        ans = Cons( temp );
      else
        throw  RunError( 2, token1->name, NULL );           
    } // if
    else if ( token1->type == PARTACCESSOR  ) {
      if ( token1->name == "car" && argenum == 1 )
        ans = Cadr( temp, 1  );
      else if ( token1->name == "cdr" && argenum == 1 )  
        ans = Cadr( temp, 2  ); 
      else
        throw  RunError( 2, token1->name, NULL );                  
    } // if
    else if (  token1->type == EQIV   ) {
      if ( token1->name == "eqv?" && argenum == 2 )
        ans = Eq( temp, 1 );
      else if (   token1->name == "equal?" && argenum == 2  )  
        ans = Eq( temp, 2 );
      else
        throw  RunError( 2, token1->name, NULL );           
    } // if
    else if ( token1->type == DEFINE ) {
      if ( !frist )
        throw RunError( 9, "DEFINE", NULL );
      else if ( argenum < 2 ) 
        throw RunError( 8, "DEFINE", head  ); 
      Define( temp, head, argenum );

    } // if
    else if ( token1->type == QUOTE ) {
      if ( argenum == 1 ) {
        ans = temp;  
        Quote( ans );          
      } // if 
      else
        throw  RunError( 2, token1->name, NULL ); 
    } // if
    else if ( token1->type == BASIC  ) {
      if ( token1->name == "+" &&  argenum >= 2 ) 
        ans = Arith( temp, 1 ) ;        
      else if ( token1->name == "-"  &&  argenum >= 2 )
        ans = Arith( temp, 2 );  
      else if (  token1->name == "*"  &&  argenum >= 2 )
        ans = Arith( temp, 3 );          
      else if (  token1->name == "/"  &&  argenum >= 2 )
        ans =  Arith( temp, 4 ) ;
      else if ( token1->name == "string-append" &&  argenum >= 2 )  
        ans = SAdd( temp );
      else if ( token1->name == ">" &&  argenum >= 2 ) 
        ans =  Compnum( temp, 1, token1->name ); 
      else if ( token1->name == ">=" &&  argenum >= 2 ) 
        ans =  Compnum( temp, 2, token1->name ); 
      else if ( token1->name == "<" &&  argenum >= 2 ) 
        ans =  Compnum( temp, 3, token1->name );   
      else if ( token1->name == "<=" &&  argenum >= 2 ) 
        ans =  Compnum( temp, 4, token1->name );
      else if ( token1->name == "=" &&  argenum >= 2 ) 
        ans =  Compnum( temp, 5, token1->name );    
      else if ( token1->name == "string>?" &&  argenum >= 2 ) 
        ans =  Compstr( temp, 1, token1->name ); 
      else if ( token1->name == "string<?" &&  argenum >= 2 ) 
        ans =  Compstr( temp, 2, token1->name ); 
      else if ( token1->name == "string=?" &&  argenum >= 2 ) 
        ans =  Compstr( temp, 3, token1->name );      
      else if ( token1->name == "not" &&  argenum == 1  ) 
        ans =  Not( temp );
      else if ( token1->name == "and" &&  argenum >= 2  ) 
        ans = OrAnd( temp, 1 );
      else if ( token1->name == "or" &&  argenum >= 2  ) 
        ans = OrAnd( temp, 2 ) ;                                     
      else
        throw  RunError( 2, token1->name, NULL ); 
      
    } // if
    else if (  token1->type == COND   ) {
      if ( token1->name == "if" && ( argenum == 2 || argenum == 3 ) )
        ans = If( temp, head );
      else if ( token1->name == "if" ) 
        throw  RunError( 2, token1->name, NULL ); 
      else if ( token1->name == "cond" && argenum >= 1 )
        ans = Cond( temp, head );
      else {
        throw RunError( 8, "COND", head );          
      } // else

    } // if
    else if ( token1->type == PRIMITIVE  ) {
      if ( argenum != 1 )
        throw  RunError( 2, token1->name, NULL );
      if ( token1->name == "list?" )
        ans = Islist( temp, 1 );
      else if ( token1->name == "pair?" )
        ans = Islist( temp, 2 );
      else if ( token1->name == "atom?" )
        ans = Islist( temp, 3 );
      else if (  token1->name == "null?" )
        ans = Isneed( temp, NILL, 0 );
      else if (  token1->name == "integer?" )
        ans = Isneed( temp, INT, 0 );
      else if (  token1->name == "number?" || token1->name == "real?"  )
        ans = Isneed( temp, INT, FLOAT );
      else if (  token1->name == "string?" )
        ans = Isneed( temp, STRING, 0 );
      else if (  token1->name == "boolean?" )
        ans = Isneed( temp, TRUE, NILL );
      else if (  token1->name == "symbol?" )
        ans = Isneed( temp, SYMBAL, 0 );
    } // if
    else if (  token1->type == BEGIN ) {
      if ( argenum > 0 )
        ans = Begin( temp );
      else
        throw  RunError( 2, token1->name, NULL );           
    } // if
    else if (   token1->type == CLEAN ) {
      if ( !frist ) {
        string errname = "";
        if ( token1->name == "exit" )
          errname = "EXIT";
        else
          errname = "CLEAN-ENVIRONMENT" ; 
        throw RunError( 9, errname, NULL );  
        
      } // if

      if ( token1->name == "exit" && argenum == 0  )
        throw End();
      else if (  token1->name == "clean-environment" && argenum == 0  ) {
        msymbal.clear();
        mheap.clear();
        cout << "environment cleaned" << endl ;
      } // if
      else
        throw  RunError( 2, token1->name, NULL );

    } // if
    else if ( token1->type == FUNCTION  ) {
      if ( token1->name == "let"  )
        ans = Let( temp, head, argenum );
      else
        ans = RunFun( temp, head, token1, argenum );    
    } // if
    else if ( token1->type == SETF  ) {
      ans = Lambda( temp, head, argenum  );
    } // if
    else {
      Expptr error = new Exp();
      error->leftt = token1;
      throw RunError( 4, "", error  );         
    } // else
          
  } // try
  catch ( RunError & runerror ) {
    if ( runerror.mtype != 5 || !runerror.mhave  )
      throw runerror;
      
    if ( token1->type == COND ||  token1->type == BEGIN || 
          token1->type == FUNCTION )
      runerror.merrorline = head;
    else if ( token1->name == "and" || token1->name == "or"  )
      runerror.mtype = 12;
    else if ( token1->name != "define"  )
      runerror.mtype = 10;  

    throw runerror;      
  } // catch 
    
  
  return ans;     
} // Runexp()


void Project3::Startexp() { // do readexp if have error print
  mnowchar = '\0';
  mlinelp = 0;
  mnowline = 1 ;
  mprinttok = false;
  Expptr testline;
  Expptr runover;    
  mlinename = "";
  try {
    testline = Readexp( false );
    mnowcolumn = 0;
    while ( cin.peek() != EOF && Iswhitesp( cin.peek() ) && cin.peek() != '\n' )
      Getchar();

    if ( cin.peek() == ';' ) {
      while (  cin.peek() != EOF && cin.peek() != '\n'  ) // read explain 
        Getchar();
    } // if

    if ( cin.peek() == '\n'  )
      Getchar();
      
    if ( testline != NULL  ) {
      // Printexp( testline ) ;
      runover = Runexp( testline, true ) ;
      Printexp(  runover, false  );
    } // if

  } // try
  catch ( ThrowReadError  &readerror ) {
    mlinename = "error";
    if (  readerror.merrortype != 4  )
      cout << readerror.Error() << endl;
    else {
      mend = true;
      cout << readerror.Error();        
      return;        
    } // else
    
    while ( !mend && mnowchar != '\n' )
      Getchar();   
  } // catch
  catch ( End &end ) {
    mend = true;
    return;      
  } // catch
  catch ( RunError & runerror ) {
    cout << runerror.Error();
    Expptr error = runerror.merrorline;
    if (  runerror.mtype != 4 &&  runerror.mtype != 3 ) {
      Printexp( error, true ) ;        
    } // if
    else {
      Printexp( error, false ) ;
    } // else
  } // catch 

  if ( mend  )  // read EOF and not input
    cout << "\n> ERROR (no more input) : END-OF-FILE encountered" ;


} // Startexp()


// int main() {
//   Project3 test;
//   int  testnum;
//   char ch;
//   test.mend = false;
//   test.mnowline = 1;
//   test.mnowcolumn = 0;
//   cout << "Welcome to OurScheme!\n";
//   cin >> testnum;
//   cout << "this test is " << testnum << endl;
//   ch = cin.get();
//   while ( !test.mend  ) {
//     cout << "\n> ";
//     test.Startexp();
//   } // while

//   cout << "\nThanks for using OurScheme!" ;

// } // main()