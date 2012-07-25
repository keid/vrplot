#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include "ParameterTable.hpp"

namespace vrplot {

class Parameters {
public:
  // parameters for autoscale
  static const ParameterTable::ParameterInt AUTOSCALE_X_MIN;
  static const ParameterTable::ParameterInt AUTOSCALE_X_MAX;

  static const ParameterTable::ParameterInt AUTOSCALE_Y_MIN;
  static const ParameterTable::ParameterInt AUTOSCALE_Y_MAX;

  static const ParameterTable::ParameterInt AUTOSCALE_Z_MIN;
  static const ParameterTable::ParameterInt AUTOSCALE_Z_MAX;

  // parameters for range
  static const ParameterTable::ParameterDouble RANGE_X_MIN;
  static const ParameterTable::ParameterDouble RANGE_X_MAX;
  
  static const ParameterTable::ParameterDouble RANGE_Y_MIN;
  static const ParameterTable::ParameterDouble RANGE_Y_MAX;

  static const ParameterTable::ParameterDouble RANGE_Z_MIN;
  static const ParameterTable::ParameterDouble RANGE_Z_MAX;

  static void registerInitialParameters( ParameterTable* table ) ;
};

}

#endif
