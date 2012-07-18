#include "commands/CommandPlot.hpp"
#include "Components.hpp"
#include "volumeGenerator/SimpleVolumeGenerator.hpp"
#include "volumeGenerator/Demo0.hpp"
#include "VolumeRenderer.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

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

  VolumeRenderer *vr = components->getRenderer();
  if ( vr == NULL ) return false;
  
  FileLoader* fl = components->getFileLoader();
  if ( fl == NULL ) components->setFileLoader( new FileLoader(), true );

  if ( (fl->load( file_path )) == false ) return false;

  // TODO : 'resolution' setting(256)
  int resolution = 256;

  // TODO : 'with' option
  volumeGenerator::IVolumeGenerator *vg = components->getVolumeGenerator();
  if ( vg == NULL ) {
    vg = new volumeGenerator::SimpleVolumeGenerator( resolution, resolution, resolution );
    //vg = new volumeGenerator::Demo0( resolution, resolution, resolution );
    components->setVolumeGenerator( vg, true );
  }

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

std::string CommandPlot::getUsage() const {
  return std::string("Plot a specified file. Usage: plot 'file'");
}

}
}
}
