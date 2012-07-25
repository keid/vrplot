#include "Parameters.hpp"

#include <map>
#include <string>
#include <cfloat>
#include <utility>

namespace vrplot {

const ParameterTable::ParameterInt Parameters::AUTOSCALE_X_MIN("autoscale.x.min");
const ParameterTable::ParameterInt Parameters::AUTOSCALE_X_MAX("autoscale.x.max");

const ParameterTable::ParameterInt Parameters::AUTOSCALE_Y_MIN("autoscale.y.min");
const ParameterTable::ParameterInt Parameters::AUTOSCALE_Y_MAX("autoscale.y.max");

const ParameterTable::ParameterInt Parameters::AUTOSCALE_Z_MIN("autoscale.z.min");
const ParameterTable::ParameterInt Parameters::AUTOSCALE_Z_MAX("autoscale.z.max");

const ParameterTable::ParameterDouble Parameters::RANGE_X_MIN("range.x.min");
const ParameterTable::ParameterDouble Parameters::RANGE_X_MAX("range.x.max");
  
const ParameterTable::ParameterDouble Parameters::RANGE_Y_MIN("range.y.min");
const ParameterTable::ParameterDouble Parameters::RANGE_Y_MAX("range.y.max");

const ParameterTable::ParameterDouble Parameters::RANGE_Z_MIN("range.z.min");
const ParameterTable::ParameterDouble Parameters::RANGE_Z_MAX("range.z.max");

void Parameters::registerInitialParameters( ParameterTable* table )  {
  
  table->setParam( AUTOSCALE_X_MIN, 1 );
  table->setParam( AUTOSCALE_X_MAX, 1 );
  table->setParam( AUTOSCALE_Y_MIN, 1 );
  table->setParam( AUTOSCALE_Y_MAX, 1 );
  table->setParam( AUTOSCALE_Z_MIN, 1 );
  table->setParam( AUTOSCALE_Z_MAX, 1 );

  table->setParam( RANGE_X_MIN, -DBL_MAX );
  table->setParam( RANGE_X_MAX, DBL_MAX );
  table->setParam( RANGE_Y_MIN, -DBL_MAX );
  table->setParam( RANGE_Y_MAX, DBL_MAX );
  table->setParam( RANGE_Z_MIN, -DBL_MAX );
  table->setParam( RANGE_Z_MAX, DBL_MAX );
  
}

}
