#include "commands/CommandPlot.hpp"

#include <iostream>

namespace vrplot{
namespace controller {
namespace command {

std::string CommandPlot::getName() const {
  return std::string("plot");
}

bool CommandPlot::execute( std::list< std::string>& args ) const {
  std::cout << "plot command :" << std::endl;
  for( std::list< std::string>::iterator it = args.begin();
       it != args.end();
       ++it ) {
    std::cout << *it << std::endl;
  }
       
}

std::string CommandPlot::getUsage() const {
  return std::string("Plot a specified file. Usage: plot 'file'");
}

}
}
}
