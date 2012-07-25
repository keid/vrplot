#include "commands/CommandReplot.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

#include "Components.hpp"
#include "volumeGenerator/SimpleVolumeGenerator.hpp"
#include "VolumeRenderer.hpp"
#include "FieldSelector.hpp"
#include "VolumeData.hpp"
#include "CoordinateAdjuster.hpp"
#include "ColorMap.hpp"
#include "Parameters.hpp"
#include "ParameterTable.hpp"

namespace vrplot{
namespace controller {
namespace command {

std::string CommandReplot::getName() const {
  return std::string("replot");
}

bool CommandReplot::execute( int id,
			     std::list< std::string>* args,
			     Components* components ) const {

  VolumeRenderer *vr = components->getRenderer();
  if ( vr == NULL ) return false;
  
  FileLoader* fl = components->getFileLoader();
  if ( fl == NULL ) return false;

  ParameterTable* params = components->getParameterTable();

  // TODO : 'resolution' setting(256)
  int resolution = 256;

  volumeGenerator::IVolumeGenerator *vg = components->getVolumeGenerator();
  if ( vg == NULL ) return false;

  // TODO : 'using' option
  FieldSelector selector;

  // TODO : 'set range'
  //CoordinateAdjuster adjuster;
  CoordinateAdjuster* adjuster = components->getCoordinateAdjuster();
  adjuster->setMask( getAutoscaleMask( params ) );

  ColorMap* colormap = components->getColorMap();
  
  vg->generate( *fl, selector, *adjuster, *colormap);
  vr->loadVolumeData( resolution, resolution, resolution, vg->getVolume()->getVolume() );

  return true;
}

std::string CommandReplot::getUsage() const {
  return std::string("Replot a volume using a file which is loaded at last time. Usage: replot");
}

unsigned int CommandReplot::getAutoscaleMask( const ParameterTable *params ) const  {
  unsigned int flag = 0x00;

  if ( params->getParam( Parameters::AUTOSCALE_X_MAX ) != 0 ) 
    flag |= CoordinateAdjuster::MAX_X;

  if ( params->getParam( Parameters::AUTOSCALE_X_MIN ) != 0 ) 
    flag |= CoordinateAdjuster::MIN_X;

  if ( params->getParam( Parameters::AUTOSCALE_Y_MAX ) != 0 ) 
    flag |= CoordinateAdjuster::MAX_Y;

  if ( params->getParam( Parameters::AUTOSCALE_Y_MIN ) != 0 ) 
    flag |= CoordinateAdjuster::MIN_Y;

  if ( params->getParam( Parameters::AUTOSCALE_Z_MAX ) != 0 ) 
    flag |= CoordinateAdjuster::MAX_Z;

  if ( params->getParam( Parameters::AUTOSCALE_Z_MIN ) != 0 ) 
    flag |= CoordinateAdjuster::MIN_Z;

  return flag;

}

}
}
}
