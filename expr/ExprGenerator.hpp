#ifndef EXPRGENERATOR_HPP
#define EXPRGENERATOR_HPP

#include <string>

#include "expr/IExpr.hpp"

namespace vrplot {
namespace expr {

class ExprGenerator {
public:
  
  static IExpr *generate( const std::string& expr );
  
private:
  ExprGenerator();
};

}
}

#endif
