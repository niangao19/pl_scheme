#ifndef PL1_H
#define PL1_H
# include <string>
# include <vector>
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
# define PARTACCESSOR 11 // car cdr
# define PRIMITIVEPREDICATE 12
# define BASIC 13
# define EQIV  14
# define BEGIN 15
# define COND  16
# define CLEAN 17

struct Token {
  int type;
  std::string name;
};


typedef Token* Tokenptr;

struct Exp {
  Exp* lefte ;
  Exp* righte ;
  Token* leftt ;
  Token* rightt ;
};

typedef Exp* Expptr;

struct Symbal {
  std::string symbalname ;
  Expptr symdefine ;
};

void Itos( std::string & temp, int num   );

void Ftos( std::string & temp, float num   );

class  ReadError {
  std::string mline ;
  std::string mcolumn ;
  // type 1 : no closing quote, type2 = unexpected token no '(' or atom , type3 = unexpected token no ')'
  int merrortype;
  std::string mnowtoken ; 

public :
  ReadError( int line, int column, int etype, std::string tokenname );

  std::string  Error();

}; // ReadError()


class Project1 {

  int    mtokencolumn;
  int    mtokenline;
  std::string mlinename;
  std::string mnowtoken;
  int    mnowtype;
  char   mnowchar ; // doesn't process
  std::vector<Token>  msymbal;
public:
  bool mend;
  int    mnowline;
  int    mnowcolumn;
  bool Isnum( char com ) ;

  bool Iswhitesp(  char com  );

  bool Ischar( char com );
  
  bool Isspread( char com  ) ;
  
  void Getchar() ;
  
  void Readsymbal() ;
  
  void Readnumber(  bool isfloat   );
 
  void Readstring();
  
  void Gettoken();	

  void AllNULL( Expptr temp  );
  

  
  Expptr Readexp( bool haveread );

  int mlinelp ;
  bool mprinttok ; 
  void Printwhitesp();

  void Printtoken(  Tokenptr temp1, bool right  );

  void Printexp( Expptr temp );

  bool Isend(  Expptr temp );

  void Startexp();

}; // Project1
#endif