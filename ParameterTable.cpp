#include "ParameterTable.hpp"

#include <map>
#include <string>
#include <cfloat>
#include <utility>

namespace vrplot {

ParameterTable::ParameterTable() {
  initialize();
}

void ParameterTable::initialize() {
}

template < >
std::map< std::string, std::string> & ParameterTable::getParamTable() {
  return params_str_;
}

template < >
std::map< std::string, double> & ParameterTable::getParamTable() {
  return params_double_;
}

template < >
std::map< std::string, int> & ParameterTable::getParamTable() {
  return params_int_;
}  

template < >
const std::map< std::string, std::string> & ParameterTable::getParamTable() const{
  return params_str_;
}

template < >
const std::map< std::string, double> & ParameterTable::getParamTable() const {
  return params_double_;
}

template < >
const std::map< std::string, int> & ParameterTable::getParamTable() const {
  return params_int_;
}  

std::ostream& ParameterTable::output( std::ostream& ost ) {
  output( ost, getParamTable< int >() );
  output( ost, getParamTable< double >() );
  output( ost, getParamTable< std::string>() );
  return ost;
}

}
