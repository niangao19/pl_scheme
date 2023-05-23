#ifndef PL3_H
#define PL3_H
# include <string>
# include <vector>
// =========================== define operator 
# define STRING 1
# define INT    2
// {0-9}
# define FLOAT  3
// [0-9]*.{0-9}
# define NILL   4
// nil or #f or ()
# define TRUE   5
// #t or t
# define SYMBAL 6
// expect operator
# define QUOTE  8
// 'or quote
# define NONOPERTOR 7
# define CONSTRUCTOR 9
// list cons
# define DEFINE 10
// define
# define PARTACCESSOR 11 
// car cdr
# define PRIMITIVE 12
// atom? list? ....etc
# define BASIC 13
// + - * / ....etc
# define EQIV  14
// eqv? equal?
# define BEGIN 15
// begin
# define COND  16
// if cond
# define CLEAN 17 
//  (clean-enviornment) exit
# define FUNCTION 18
// let lambda or define function
# define SETF 19
// lambda
// ==============================define operator end

struct Token {
  int type;
  std::string name;
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
  std::string symbalname ;
  Expptr symdefine ;
};

struct Function {
  std::vector<Symbal>  locsym;
  Expptr  runline;
};

struct Funsym {
  bool islet;
  std::vector<Symbal>  locsym;  
};
// =====================================struct
void Itos( std::string & temp, int num   );

void Ftos( std::string & temp, float num   );

class End {
public :
  End() {
  }  // End()
   
};

class  ThrowReadError {
  std::string mline ;
  std::string mcolumn ;
  // type 1 : no closing quote
  // type2 = unexpected token no '(' or atom  
  // type3 = unexpected token no ')'
  // type4 =  no more input
  std::string mnowtoken ; 

public :
  int merrortype;  
  ThrowReadError( int line, int column, int etype, std::string tokenname ); 

  std::string  Error() ;

}; // ThrowReadError()

class RunError{
  std::string mname;
  
public:
  Expptr merrorline;
  int mtype;
  bool mhave;
  // 1 : error non list
  // 2 : incorrect number of arguments
  // 3 : with incorrect argument type
  // 4 : attempt to apply non-function 
  // 5 : no return value
  // 6 : unbound symbol
  // 7 : division by zero
  // 8 : error format by COND or DEFINE
  // 9 : error level EXIT or CLEAN-ENVIRONMENT or DEFINE  
  // 10 : unbound parameter
  // 11 : unbound test-condition
  // 12 : unbound condition
  RunError( int type, std::string name, Expptr errorline   );
  
  std::string Error();
  
};

class Project3 {


  std::vector<Symbal>    msymbal; // global varible
  std::vector< Funsym >  mcstack; // location varible
  std::vector<Function>  mheap; // program counter
  
public:
  std::string mlinename;
  std::string mnowtoken;
  int    mnowtype;
  char   mnowchar ; // doesn't process
  int    mtokencolumn;
  int    mtokenline;
  bool mend;
  int    mnowline;
  int    mnowcolumn;
  bool Isnum( char com ) ;

  bool Iswhitesp(  char com  );

  bool Ischar( char com ) ;
  
  bool Isspread( char com  ) ;
  
  void Getchar() ;

  bool IsBasic( std::string op  ) ;
  
  bool IsPrimitive( std::string op  ) ;

  
  void Readsymbal() ;
  
  void Readnumber(  bool isfloat   ) ;
  
 
  void Readstring() ;
  
  void Gettoken() ;	

  void AllNULL( Expptr temp  ) ;



  Expptr Readexp( bool haveread ) ;

  int mlinelp ;
  bool mprinttok ; 
  void Printwhitesp() ;

  void Printtoken(  Tokenptr temp1, bool right, bool error  ) ;

  void Printexp( Expptr temp, bool error );

  Expptr Arith(  Expptr temp, int op ) ;
  
  Expptr SAdd( Expptr temp );
  
  Expptr Compnum( Expptr temp, int op, std::string name ) ;
  
  bool Eval(   Expptr temp ) ;

  Expptr Compstr( Expptr temp, int op, std::string name ) ;
  
  Expptr Not(  Expptr temp );
  
  void Quote( Expptr temp ) ;
  
  Expptr OrAnd( Expptr temp, int op );
  
  Expptr List( Expptr temp );
  
  Expptr Islist( Expptr temp, int op );
  
  Expptr  Isneed( Expptr temp, int com1, int com2  );
  
  Expptr Cons( Expptr temp );
  
  bool Same( Expptr com1, Expptr com2 );
  
  Expptr Eq(  Expptr temp, int op   ) ;
  
  Expptr Searchs( Expptr temp  ) ;
  
  void Define( Expptr temp, Expptr head, int argenum  );
  
  Expptr Begin(  Expptr temp );
  
  Expptr Cadr(  Expptr temp, int op );
  
  Expptr If(  Expptr temp, Expptr head ) ;

  Expptr Cond2(  Expptr temp, bool &tf, bool least  );
  

  Expptr Cond( Expptr temp, Expptr head ) ;
  
  Expptr Let(  Expptr temp, Expptr head, int argenum ) ;
  
  
  Expptr Lambda(   Expptr temp, Expptr head, int argenum  );
  
  
  Expptr RunFun( Expptr temp, Expptr head, Tokenptr token1, int argenum );

  Expptr Searchlocs( Expptr temp, bool &haveans  );
  
  Expptr Runexp( Expptr temp, bool frist );


  void Startexp();

}; // Project3
#endif
