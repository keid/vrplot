#include "Parameters.hpp"

#include <map>
#include <string>
#include <cfloat>

namespace vrplot {

Parameters::Parameters() {
  initialize();
}

void Parameters::initialize() {
  
  setParam( "autoscale.x.min", 1 );
  setParam( "autoscale.x.max", 1 );
  setParam( "autoscale.y.min", 1 );
  setParam( "autoscale.y.max", 1 );
  setParam( "autoscale.z.min", 1 );
  setParam( "autoscale.z.max", 1 );

  setParam( "range.x.min", -DBL_MAX );
  setParam( "range.x.max", DBL_MAX );
  setParam( "range.y.min", -DBL_MAX );
  setParam( "range.y.max", DBL_MAX );
  setParam( "range.z.min", -DBL_MAX );
  setParam( "range.z.max", DBL_MAX );
  
}

template < >
std::map< std::string, std::string> & Parameters::getParamTable() {
  return params_str_;
}

template < >
std::map< std::string, double> & Parameters::getParamTable() {
  return params_double_;
}

template < >
std::map< std::string, int> & Parameters::getParamTable() {
  return params_int_;
}  

}
