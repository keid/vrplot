#ifndef VRPLOT_EXPR_IEXPR_HPP
#define VRPLOT_EXPR_IEXPR_HPP

#include <string>
#include <map>

namespace vrplot {
namespace expr {

class IExpr {
public:
  virtual IExpr& setArgs( unsigned int no, IExpr* arg ) = 0;
  virtual IExpr& setVariables( const std::map< std::string, double >& args ) = 0;
  virtual IExpr& clearVariables() = 0;
  virtual double eval() const = 0;
  virtual double eval( const std::map< std::string, double >& args ) const = 0;

  operator double() { return eval(); }

  virtual ~IExpr(){}
};

}
}

#endif
