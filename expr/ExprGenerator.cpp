#include "expr/ExprGenerator.hpp"

#include <string>
#include <sstream>

#include "expr/ExprConstant.hpp"

namespace vrplot {
namespace expr {

IExpr* ExprGenerator::generate( const std::string& expr ) {
  static std::istringstream iss;
  double value;

  iss.str(std::string(""));
  iss.clear();

  iss.str( expr );
  iss >> value;

  return new ExprConstant( value );
}

}
}
