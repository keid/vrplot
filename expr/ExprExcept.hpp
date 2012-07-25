#ifndef EXPREXCEPT_HPP
#define EXPREXCEPT_HPP

#include <stdexcept>
#include <string>

namespace vrplot {
namespace expr {
namespace exception {

class UnknownVar : public std::invalid_argument {
public:
  explicit UnknownVar( const std::string& what_arg )
    : invalid_argument( what_arg ) {}
};

class TooFewArg : public std::invalid_argument {
public:
  explicit TooFewArg( const std::string& what_arg )
    : invalid_argument( what_arg ) {}
};

class TooManyArg : public std::invalid_argument {
public:
  explicit TooManyArg( const std::string& what_arg )
    : invalid_argument( what_arg ) {}
};

}
}
}

#endif
