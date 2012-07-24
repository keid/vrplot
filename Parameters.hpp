#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <map>
#include <algorithm>
#include <string>
#include <utility>
#include <ostream>

namespace vrplot {

class Parameters {
private:
  template < typename T >
  class ParameterEntry  {
  public:
    ParameterEntry( const std::string& str ) : str_(str) { }

    ParameterEntry() : str_("") { }

    ParameterEntry& operator=( const std::string& str ) {
      str_ = str;
    }

    operator std::string() const {
      return str_;
    }

  private:
    std::string str_;
    
  };

public:

  typedef ParameterEntry<int> ParameterInt;
  typedef ParameterEntry<double> ParameterDouble;
  typedef ParameterEntry<std::string> ParameterString;
  
  // parameters for autoscale
  static const ParameterInt AUTOSCALE_X_MIN;
  static const ParameterInt AUTOSCALE_X_MAX;

  static const ParameterInt AUTOSCALE_Y_MIN;
  static const ParameterInt AUTOSCALE_Y_MAX;

  static const ParameterInt AUTOSCALE_Z_MIN;
  static const ParameterInt AUTOSCALE_Z_MAX;

  // parameters for range
  static const ParameterDouble RANGE_X_MIN;
  static const ParameterDouble RANGE_X_MAX;
  
  static const ParameterDouble RANGE_Y_MIN;
  static const ParameterDouble RANGE_Y_MAX;

  static const ParameterDouble RANGE_Z_MIN;
  static const ParameterDouble RANGE_Z_MAX;

  Parameters() ;

  template < typename T >
  T getParam( const ParameterEntry<T>& name ) const {
    typename std::map< std::string, T >::const_iterator it;
    const std::map< std::string, T >& table = getParamTable<T>();
    
    if ( ( it = table.find( name )) != table.end() ) {
      return (*it).second;
    } else {
      return T();
    }
  } 
  
  template < typename T >
  Parameters& setParam( const ParameterEntry<T>& name, T param ) {
    typename std::map< std::string, T >::iterator it;
    std::pair< typename std::map< std::string, T >::iterator, bool > result;
    std::map< std::string, T >& table = getParamTable<T>();
    
    result = table.insert( std::make_pair( static_cast<std::string>(name), param ) );

    if ( result.second == false ) {
      (*(result.first)).second = param;
    }
    
    return *this;
  }

  template < typename T >
  std::map< std::string, T>& getParamTable();

  template < typename T >
  const std::map< std::string, T>& getParamTable() const;

  std::ostream& output( std::ostream& ost );
  
private:
  std::map< std::string, std::string > params_str_;
  std::map< std::string, double > params_double_;
  std::map< std::string, int > params_int_;

  template < typename T >
  class OutputFunc {
  public:
    OutputFunc( std::ostream& ost ) : ost_(ost) {}
    void operator()( const std::pair< const std::string, T > &pair) {
      ost_ << pair.first << ":" << pair.second << std::endl;
    }
  private:
    std::ostream& ost_;
  };

  template < typename T >
  std::ostream& output( std::ostream& ost, const std::map< std::string, T >& table ) {
    std::for_each( table.begin(), table.end(), OutputFunc<T>(ost) );
    return ost;
  }

  void initialize();
};

}

#endif
