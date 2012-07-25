#ifndef VRPLOT_EXPR_EXPRCONSTANT_HPP
#define VRPLOT_EXPR_EXPRCONSTANT_HPP

#include "expr/IExpr.hpp"

namespace vrplot {
namespace expr {

class ExprConstant : public IExpr {
public:
  ExprConstant( double value );
  IExpr& setArgs( unsigned int no, IExpr* arg ) ;
  IExpr& setVariables( const std::map< std::string, double >& args ) ;
  IExpr& clearVariables() ;
  double eval() const ;
  double eval( const std::map< std::string, double >& args ) const ;

private:
  const double value_;
};

}
}

#endif
