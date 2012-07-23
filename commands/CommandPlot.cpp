#include "commands/CommandPlot.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

#include "Components.hpp"
#include "volumeGenerator/SimpleVolumeGenerator.hpp"
#include "VolumeRenderer.hpp"
#include "FieldSelector.hpp"
#include "CoordinateAdjuster.hpp"
#include "Controller.hpp"

namespace vrplot{
namespace controller {
namespace command {

std::string CommandPlot::getName() const {
  return std::string("plot");
}

bool CommandPlot::execute( std::list< std::string>& args,
Components* components ) const {

  if ( args.size() < 1 ) return false;

  const std::string file_path = args.front();
  
  FileLoader* fl = components->getFileLoader();
  if ( fl == NULL ) components->setFileLoader( new FileLoader(), true );

  if ( (fl->load( file_path )) == false ) return false;

  // TODO : 'resolution' setting(256)
  int resolution = 256;

  // TODO : 'with' option
  volumeGenerator::IVolumeGenerator *vg = components->getVolumeGenerator();
  if ( vg == NULL ) {
    vg = new volumeGenerator::SimpleVolumeGenerator( resolution, resolution, resolution );
    components->setVolumeGenerator( vg, true );
  }

  args.front() = "replot";

  return components->getController()->execCommand( args );
}

std::string CommandPlot::getUsage() const {
  return std::string("Plot a specified file. Usage: plot 'file'");
}

}
}
}
