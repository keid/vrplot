#include "Parameters.hpp"

#include <map>
#include <string>
#include <cfloat>
#include <utility>

namespace vrplot {

const Parameters::ParameterInt Parameters::AUTOSCALE_X_MIN("autoscale.x.min");
const Parameters::ParameterInt Parameters::AUTOSCALE_X_MAX("autoscale.x.max");

const Parameters::ParameterInt Parameters::AUTOSCALE_Y_MIN("autoscale.y.min");
const Parameters::ParameterInt Parameters::AUTOSCALE_Y_MAX("autoscale.y.max");

const Parameters::ParameterInt Parameters::AUTOSCALE_Z_MIN("autoscale.z.min");
const Parameters::ParameterInt Parameters::AUTOSCALE_Z_MAX("autoscale.z.max");

const Parameters::ParameterDouble Parameters::RANGE_X_MIN("range.x.min");
const Parameters::ParameterDouble Parameters::RANGE_X_MAX("range.x.max");
  
const Parameters::ParameterDouble Parameters::RANGE_Y_MIN("range.y.min");
const Parameters::ParameterDouble Parameters::RANGE_Y_MAX("range.y.max");

const Parameters::ParameterDouble Parameters::RANGE_Z_MIN("range.z.min");
const Parameters::ParameterDouble Parameters::RANGE_Z_MAX("range.z.max");

Parameters::Parameters() {
  initialize();
}

void Parameters::initialize() {
  
  setParam( AUTOSCALE_X_MIN, 1 );
  setParam( AUTOSCALE_X_MAX, 1 );
  setParam( AUTOSCALE_Y_MIN, 1 );
  setParam( AUTOSCALE_Y_MAX, 1 );
  setParam( AUTOSCALE_Z_MIN, 1 );
  setParam( AUTOSCALE_Z_MAX, 1 );

  setParam( RANGE_X_MIN, -DBL_MAX );
  setParam( RANGE_X_MAX, DBL_MAX );
  setParam( RANGE_Y_MIN, -DBL_MAX );
  setParam( RANGE_Y_MAX, DBL_MAX );
  setParam( RANGE_Z_MIN, -DBL_MAX );
  setParam( RANGE_Z_MAX, DBL_MAX );
  
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

std::ostream& Parameters::output( std::ostream& ost ) {
  output( ost, getParamTable< int >() );
  output( ost, getParamTable< double >() );
  output( ost, getParamTable< std::string>() );
  return ost;
}

}
