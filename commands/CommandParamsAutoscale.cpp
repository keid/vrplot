#include "CommandParamsAutoscale.hpp"

#include <iostream>

#include "Components.hpp"
#include "Parameters.hpp"

namespace vrplot {
namespace controller {
namespace command {

const unsigned int CommandParamsAutoscale::ID_SET = 0x01;
const unsigned int CommandParamsAutoscale::ID_UNSET = 0x02;
const unsigned int CommandParamsAutoscale::ID_SHOW = 0x04;

std::string CommandParamsAutoscale::getName() const {
  return std::string("autoscale");
}

bool CommandParamsAutoscale::execute( int id,
				      std::list< std::string >* args,
				      Components* components ) const {
  Parameters *params = components->getParameters();

  bool x, y, z, min, max;

  x = y = z = min = max = false;
  
  if ( (id & ID_SET) || (id & ID_UNSET) ) {
    if ( args->size() == 0 ) {
      x = y = z = min = max = true;
    } else {
      if ( args->front() == "x" ) {
	x = true;
      } else if ( args->front() == "y" ) {
	y = true;
      } else if ( args->front() == "z" ) {
	z = true;
      }

      args->pop_front();

      if ( !(x | y | z) ) x = y = z = true;

      if ( args->size() == 0 ) {
	min = max = true;
      }
      
      while( args->size() != 0 ) {
	if ( args->front() == "min" ) {
	  min = true;
	} else if ( args->front() == "max" ) {
	  max = true;
	} else {
	  std::cerr << "Invalid option is specified. Choose from {max|min}."
		    << std::endl;
	}
      }
    }
    
    int v = (id & ID_SET) ? 1 : 0;
    
    if ( x & max ) params->setParam( Parameters::AUTOSCALE_X_MAX, v );
    if ( x & min ) params->setParam( Parameters::AUTOSCALE_X_MIN, v );
    
    if ( y & max ) params->setParam( Parameters::AUTOSCALE_Y_MAX, v );
    if ( y & min ) params->setParam( Parameters::AUTOSCALE_Y_MIN, v );
    
    if ( z & max ) params->setParam( Parameters::AUTOSCALE_Z_MAX, v );
    if ( z & min ) params->setParam( Parameters::AUTOSCALE_Z_MIN, v );
    
  } else if ( id & ID_SHOW ) {
    std::cout << " x.max : " << params->getParam( Parameters::AUTOSCALE_X_MAX ) << std::endl;
    std::cout << " x.min : " << params->getParam( Parameters::AUTOSCALE_X_MIN ) << std::endl;
    std::cout << " y.max : " << params->getParam( Parameters::AUTOSCALE_Y_MAX ) << std::endl;
    std::cout << " y.min : " << params->getParam( Parameters::AUTOSCALE_Y_MIN ) << std::endl;
    std::cout << " z.max : " << params->getParam( Parameters::AUTOSCALE_Z_MAX ) << std::endl;
    std::cout << " z.min : " << params->getParam( Parameters::AUTOSCALE_Z_MIN ) << std::endl;
  } else {
    return false;
  }

  return true;
}

std::string CommandParamsAutoscale::getUsage() const {
  return std::string("Set/Show autoscale parameters. DO NOT call this command directly.");
}


}
}
}
