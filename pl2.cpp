# include <stdio.h>
# include <stdlib.h> // atoi
# include <iostream> // cin cout endl
# include <string>
# include <vector>
# include <iomanip> // fix
# include <exception> // try catch
# include <sstream> // itos
using namespace std;

 // ===========================
# define STRING 1
# define INT    2
# define FLOAT  3
# define NILL   4
# define TRUE   5
# define SYMBAL 6
# define QUOTE  7
# define NONOPERTOR 8
# define CONSTRUCTOR 9
# define DEFINE 10
# define PARTACCESSOR 11 
// car cdr
# define PRIMITIVE 12
# define BASIC 13
# define EQIV  14
# define BEGIN 15
# define COND  16
# define CLEAN 17 
// clean-enviroment exit

// ==============================

struct Token {
  int type;
  string name;
  int site;
};


typedef Token* Tokenptr;

struct Exp {
  Exp* lefte ;
  Exp* righte ;
  Token* leftt ;
  Token* rightt ;
  int   arge;
};

typedef Exp* Expptr;

struct Symbal {
  string symbalname ;
  Expptr symdefine ;
};

struct Fuction {
  vector<Symbal>  locsym;
  Expptr  runline;      
};

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

class End {
public :
  End() {
  }  // End()
   
};

class  ThrowReadError {
  string mline ;
  string mcolumn ;
  // type 1 : no closing quote
  // type2 = unexpected token no '(' or atom  
  // type3 = unexpected token no ')'
  int merrortype;
  string mnowtoken ; 

public :
  ThrowReadError( int line, int column, int etype, string tokenname ) {
    Itos( mline, line ) ;
    Itos( mcolumn, column  ) ;
    merrortype = etype;  
    mnowtoken = tokenname;
  } // ThrowReadError()

  string  Error() {
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
    else {
    // input is not ')' when parse is ( {exp} . [exp] )
      readerror = "ERROR (unexpected token) : ')' expected when token at Line " + mline ;
      readerror += " Column " + mcolumn + " is >>" +  mnowtoken + "<<";
    } // else 

    return readerror;
  } // Error()

}; // ThrowReadError()


class RunError{
  string mname;
  
public:
  Expptr merrorline;
  int mtype;
  // 1 : error non list
  // 2 : incorrect number of arguments
  // 3 : with incorrect argument type
  // 4 : attempt to apply non-function 
  // 5 : no return value
  // 6 : unbound symbol
  // 7 : division by zero
  // 8 : error format by COND or DEFINE
  // 9 : error level EXIT or CLEAN-ENVIRONMENT or DEFINE  
  RunError( int type, string name, Expptr errorline   ) {
    mtype = type;
    mname = name;
    merrorline = errorline;
  } // RunError()
  
  string Error() {
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
      
    return runerror;     
           
  } // Error()
  
};


class Project2 {
  int    mtokencolumn;
  int    mtokenline;
  string mlinename;
  string mnowtoken;
  int    mnowtype;
  char   mnowchar ; // doesn't process
  vector<Symbal>    msymbal;
  vector<Function>  mcstack;
  vector<Function>  mheap;
  
public:
  bool mend;
  int    mnowline;
  int    mnowcolumn;
  bool Isnum( char com ) { // 數字 
    if ( ( int ) com >= 48 && ( int ) com <= 57 )
      return true;
    return false;  
  } // Isnum()

  bool Iswhitesp(  char com  ) {
    if (  com == ' ' || com == '\t' || com == '\n' || com == '\0' )
      return true; 
    return false;
  } // Iswhitesp()

  bool Ischar( char com ) { // 英文 
    if ( ( ( int ) com >= 65 && ( int ) com <= 90 ) || (  ( int ) com >= 97 && ( int ) com <= 122 ) ) {
      return true;
    } // if

    return false;
  } // Ischar()
  
  bool Isspread( char com  ) {
    // cout << com << endl;
    if (  Iswhitesp( com  )  || com == '\'' || com == '\"' || com == ';' || com == '(' || com == ')' )
      return true;
    return false;  
  } // Isspread()
  
  void Getchar() {
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

  bool IsBasic( string op  ) {
    string basic[16] = { "+", "-", "*", "/", "not", "and", "or", ">", ">=",
      "<", "<=", "=", "string-append", "string>?", "string<?", "string=?" } ;
    for ( int i = 0; 16 > i ; i++  ) {
      if ( basic[i] == op  )
        return true;  
    } // for 
    
    return false;
  } // IsBasic()
  
  bool IsPrimitive( string op  ) {
    string primitive[10] = { "atom?", "pair?", "list?", "null?", "integer?", 
      "real?", "number?", "string?", "boolean?", "symbol?" };
    for ( int a = 0; 10 > a ; a++  ) {
      if ( primitive[a] == op  )
        return true;  
    } // for 
    
    return false;  
  } // IsPrimitive()
  
  void Readsymbal() {

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
    else if ( IsBasic( mnowtoken  )  ) 
      mnowtype =   BASIC;
    else if ( IsPrimitive( mnowtoken )  )
      mnowtype = PRIMITIVE;
    else
      mnowtype = SYMBAL ;

  } // Readsymbal()
  
  void Readnumber(  bool isfloat   ) {

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
  
 
  void Readstring() {
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
      throw  ThrowReadError( previousline, previouscolumn, 1, ""  ) ;
    } // if
    
    // mnowtoken += mnowchar;    
    mnowtype = STRING;
  }  // Readstring()
  
  void Gettoken() {
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

  void AllNULL( Expptr temp  ) {
    temp->lefte = NULL;
    temp->leftt = NULL;
    temp->righte = NULL;
    temp->rightt = NULL;
    temp->arge = 0;
  } // AllNULL()
  

  
  Expptr Readexp( bool haveread ) {
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
      temp2->name = mnowtoken ;
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
        if ( mend ) 
          mtokencolumn++; 
               
        throw  ThrowReadError( mtokenline, mtokencolumn, 3,  mnowtoken  ) ;             
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
      if ( mnowtoken == ""  ) // notthing input but not error
        return NULL;
      throw  ThrowReadError( mtokenline, mtokencolumn, 2, mnowtoken  ) ;
    } // else
 
    return head;
  } // Readexp()

  int mlinelp ;
  bool mprinttok ; 
  void Printwhitesp() {
    int n = mlinelp * 2;
    for ( int i = 0; n > i ; i++  )
      cout << " ";
  } // Printwhitesp()

  void Printtoken(  Tokenptr temp1, bool right  ) { // recoginize token type and then change their name

    if ( right ) { // if righttoken != NiLL cout ". tokenname"
      Printwhitesp() ;
      cout << ".\n" ;
    } // if

    if ( mprinttok ) // char'(' next one doesn't print whitespace 
      Printwhitesp() ; // prevent ". nill " print whitespace
    mprinttok = true;
      
    if ( temp1->type == SYMBAL || temp1->type == NONOPERTOR ) // NONOPERTOR || symbal
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
    else if ( temp1->type > NONOPERTOR  )  
      cout << "#<procedure " +   temp1->name + ">\n" ;

  } // Printtoken()

  void Printexp( Expptr temp ) {
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


  Expptr Arith(  Expptr temp, int op ) {
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
  
  Expptr SAdd( Expptr temp ) { // string-append
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
  
  Expptr Compnum( Expptr temp, int op, string name ) {
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
      token3->name = "#f";
      token3->type = NILL;       
    } // else
    
    ans->leftt = token3;
    return ans;
      
  } // Compnum()
  
  bool Eval(   Expptr temp ) {
    if ( temp != NULL  ) { 
      if ( temp->rightt != NULL  )
        return false;
      else 
        return Eval( temp->righte );  
    } // if
    
    return true; 
  } // Eval()

  Expptr Compstr( Expptr temp, int op, string name ) {
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
      token3->name = "#f";
      token3->type = NILL;       
    } // else
    
    ans->leftt = token3;
    return ans;
      
  } // Compstr()
  
  Expptr Not(  Expptr temp ) {
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
        token3->name = "#f";
        token3->type = NILL;
      } // if
      else {
        token3->name = "#t";
        token3->type = TRUE;          
      } // else       
    } // if
    else {
      token3->name = "#f";
      token3->type = NILL;        
    } // else
     
    ans->leftt = token3;
    return ans;   
  } // Not()
  
  void Quote( Expptr temp ) { 
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
  
  Expptr OrAnd( Expptr temp, int op ) { // maybe have error
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
        ans->leftt->name = "#f";
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
      ans->leftt->name = "#f";
      ans->leftt->type = NILL;       
    } // if
    
    return ans;
      
  } // OrAnd()  
  
  Expptr List( Expptr temp ) {
    Expptr texp = NULL, ans = new Exp();
    Expptr temp1 = ans;
    if ( temp == NULL ) {
      temp1->leftt = new Token();
      temp1->leftt->type = NILL;
      temp1->leftt->name = "#f";
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
  
  Expptr Islist( Expptr temp, int op ) {
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
      token3->name = "#f";
      token3->type = NILL;       
    } // else
    
    ans->leftt = token3;
    return ans;        
  } // Islist()
  
  Expptr  Isneed( Expptr temp, int com1, int com2  ) {
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
      token3->name = "#f";
      token3->type = NILL;       
    } // else
    
    ans->leftt = token3;
    return ans;  
  } // Isneed()
  
  Expptr Cons( Expptr temp ) {
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
  
  bool Same( Expptr com1, Expptr com2 ) {
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
  
  Expptr Eq(  Expptr temp, int op   ) {
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
      token3->name = "#f";
      token3->type = NILL;       
    } // else
    
    ans->leftt = token3;
    return ans;      
  } // Eq()
  
  Expptr Searchs( Expptr temp  ) {
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
  
  void Define( Expptr temp, Expptr head  ) {
    Expptr temp1 = NULL;

    if ( temp->lefte != NULL )
      throw RunError( 8, "DEFINE", head  ); 
    else
      temp1 = temp;        


    string name = temp1->leftt->name;     
    temp = temp->righte;
    if (  temp1->leftt != NULL && temp1->leftt->type == SYMBAL ) { 
      try {
        if ( temp->leftt != NULL )
          temp1 = Runexp( temp, true );
        else  
          temp1 = Runexp( temp, false );                
      } // try
      catch ( RunError & runerror ) {
        throw runerror;
      } // catch           

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
      
    } // if
    else
      throw RunError( 8, "DEFINE", head  ); 
      
  } // Define()
  
  Expptr Begin(  Expptr temp ) {
    Expptr ans = NULL;
    while ( temp != NULL ) {
      try {
        ans = Runexp( temp, false );
        temp = temp->righte;        
      } // try
      catch ( RunError & runerror ) {
        throw runerror;
      } // catch 
    } // while
    
    return ans;
    
  } // Begin()
  
  Expptr Cadr(  Expptr temp, int op ) {
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
        if ( temp1->lefte != NULL )
          ans->lefte =  temp1->lefte;
        else if ( temp1->leftt != NULL )
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
          ans->leftt->name = "#f";
        } // else        
      } // else 
    } // if
    else {
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
  
  Expptr If(  Expptr temp, Expptr head ) { // maybe have error
    Expptr com = NULL, ans = NULL;
    bool tf = true;
    try {
      com = Runexp( temp, false );
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
  

  
  Expptr Cond2(  Expptr temp, bool &tf, bool least  ) {
    Expptr com = NULL, ans = NULL;
    bool haveans = false;
    if ( !Eval( temp ) )
      throw RunError( 8, "COND", NULL );    
    
    if ( temp->lefte != NULL ) {
      try {
        com = Runexp( temp, false );        
      } // try
      catch ( RunError & runerror ) {
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
      haveans = false;
    else
      haveans = true;
      
    temp = temp->righte;
    
    try {
      ans = Begin( temp );      
    } // try    
    catch ( RunError & runerror ) {
      if ( tf || !haveans )
        return NULL;
      else
        throw runerror;  
    } // catch
    
    if ( haveans )
      tf = true;
    return ans;  
             
  } // Cond2()
  
  void Condeval( Expptr temp ) {
    if ( temp != NULL ) {
      if ( temp->lefte != NULL )
        Condeval( temp->lefte );
      if ( temp->rightt != NULL )
        throw RunError( 8, "COND", NULL );
      Condeval( temp->righte );  
    } // if
  } // Condeval()
  
  Expptr Cond( Expptr temp, Expptr head ) {
    Expptr temp2 = NULL, temp3 = temp;
    Expptr ans = NULL, tans = NULL;
    bool   tf = false;
    while ( temp3 != NULL ) {
      if ( temp3->leftt != NULL )
        throw RunError( 8, "COND", NULL  );
      temp3 = temp3->righte;    
    } // while
    
    while ( temp != NULL ) {
      if ( temp->lefte != NULL ) {
        temp2 = temp->lefte; 
        try {
          Condeval( temp2 );
          if ( temp->arge < 1 )
            throw RunError( 8, "COND", NULL );   
          if ( temp->righte != NULL && !tf )
            tans = Cond2(  temp2, tf, false  );
          else if ( !tf )
            tans = Cond2(  temp2, tf, true  );          
        } // try
        catch ( RunError & runerror ) {
          throw runerror;
        } // catch          

        if ( ans == NULL && tf )
          ans = tans;                  
      } // if


      temp = temp->righte;  
    } // while
    
    // !tf throw error
    if ( !tf )
      throw RunError( 5, "", head );
    return ans;
    
  } // Cond()

  Expptr Runexp( Expptr temp, bool frist ) {
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
      ans  = Searchs( temp );  
    } // else
    


    if ( isatom  )
      return ans;   
    else if (  temp->leftt != NULL ) {
      if ( temp->leftt->type != SYMBAL )
        token1 = temp->leftt ;
      else {
        Expptr temp1 = Searchs( temp );  
        if ( temp1->leftt != NULL ) 
          token1 = temp1->leftt ;
        // else error                 
      } // else

      temp = temp->righte ; 
    } // else if
    else  if (  temp->lefte != NULL )  {
      Expptr temp1 = Runexp( temp, false );
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
        if ( argenum == 2 )
          Define( temp, head );
        else
          throw RunError( 8, "DEFINE", head ); 
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
          throw RunError( 8, "COND", NULL );          
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
          cout << "environment cleaned" << endl ;
        } // if
        else
          throw  RunError( 2, token1->name, NULL );         

      } // if
      else {
        Expptr error = new Exp();
        error->leftt = token1;
        throw RunError( 4, "", error  );         
      } // else
            
    } // try
    catch ( RunError & runerror ) {
      throw runerror;
    } // catch 
      
    
    return ans;     
  } // Runexp()


  void Startexp() { // do readexp if have error print
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
        Printexp( testline ) ;        
        runover = Runexp( testline, true ) ;
        Printexp(  runover  );            
      } // if

    } // try
    catch ( ThrowReadError  &readerror ) {
      cout << readerror.Error() << endl;
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
      if ( runerror.mtype != 8 && runerror.mtype != 4  ) {
        Quote( error ) ;
        Printexp( error ) ;        
      } // if
      else if ( runerror.mtype == 4  ) {
        Printexp( error ) ;        
      } // if
      else {
        Quote( testline );
        Printexp(  testline  );           
      } // else                   
    } // catch 

    if ( mend && ( mlinename == "" ) ) // read EOF and have input but only quote
      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
    else if ( mend  )  // read EOF and not input
      cout << "\n> ERROR (no more input) : END-OF-FILE encountered" ;
          

    
    mnowchar = '\0' ;
    mnowline = 1;
  } // Startexp()

}; // Project2


int main() {
  Project2 test;
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



