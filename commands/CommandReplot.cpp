#include "commands/CommandReplot.hpp"
#include "Components.hpp"
#include "volumeGenerator/SimpleVolumeGenerator.hpp"
#include "VolumeRenderer.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

namespace vrplot{
namespace controller {
namespace command {

std::string CommandReplot::getName() const {
  return std::string("replot");
}

bool CommandReplot::execute( std::list< std::string>& args,
Components* components ) const {

  VolumeRenderer *vr = components->getRenderer();
  if ( vr == NULL ) return false;
  
  FileLoader* fl = components->getFileLoader();
  if ( fl == NULL ) return false;

  // TODO : 'resolution' setting(256)
  int resolution = 256;

  // TODO : 'with' option
  volumeGenerator::IVolumeGenerator *vg = components->getVolumeGenerator();
  if ( vg == NULL ) return false;

  // TODO : 'using' option
  std::vector<int> index;
  index.push_back(0);
  index.push_back(1);
  index.push_back(2);
  index.push_back(3);
  index.push_back(4);
  index.push_back(5);
  index.push_back(6);
  
  vg->generate( *fl, index );
  vr->loadVolumeData( resolution, resolution, resolution, vg->volume() );

  return true;
}

std::string CommandReplot::getUsage() const {
  return std::string("Replot a volume using a file which is loaded at last time. Usage: replot");
}

}
}
}
