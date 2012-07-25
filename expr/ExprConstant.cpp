#include "expr/ExprConstant.hpp"

#include <string>

#include "expr/ExprExcept.hpp"

namespace vrplot {
namespace expr {

ExprConstant::ExprConstant( double value ) : value_(value) {
  
}

IExpr& ExprConstant::setArgs( unsigned int no, IExpr* arg ) {
  throw exception::TooManyArg
    ( 
     std::string( "Constant value doesn't need any arguments" )
    );
  return *this;
}

IExpr& ExprConstant::setVariables( const std::map< std::string, double >& args ) {
  return *this;
}

IExpr& ExprConstant::clearVariables()  {
  return *this;
}

double ExprConstant::eval() const  {
  return value_;
}

double ExprConstant::eval( const std::map< std::string, double >& args ) const {
  return value_;
}


}
}
