#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <map>
#include <string>
#include <utility>

namespace vrplot {

class Parameters {
public:

  Parameters() ;

  template < typename T >
  T getParam( const std::string param_name ) const {
    typename std::map< std::string, T >::const_iterator it;
    const std::map< std::string, T >& table = getParamTable<T>();
    
    if ( ( it = table.find( param_name )) != table.end() ) {
      return (*it).second;
    } else {
      return T();
    }
  } 
  
  template < typename T >
  Parameters& setParam( const std::string param_name, T param ) {
    typename std::map< std::string, T >::iterator it;
    std::pair< typename std::map< std::string, T >::iterator, bool > result;
    std::map< std::string, T >& table = getParamTable<T>();
    
    result = table.insert( std::make_pair( param_name, param ) );

    if ( result.second == false ) {
      (*(result.first)).second = param;
    }
    
    return *this;
  }

  template < typename T >
  std::map< std::string, T>& getParamTable();
  
private:
  std::map< std::string, std::string > params_str_;
  std::map< std::string, double > params_double_;
  std::map< std::string, int > params_int_;

  void initialize();
};

}

#endif
