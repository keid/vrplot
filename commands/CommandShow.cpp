#include "commands/CommandShow.hpp"

#include <iostream>

#include "Components.hpp"
#include "Parameters.hpp"

namespace vrplot{
namespace controller {
namespace command {

std::string CommandShow::getName() const {
  return std::string("show");
}

bool CommandShow::execute( int id,
			   std::list< std::string>* args,
			   Components* components ) const {
  components->getParameters()->output( std::cout );
  return true;
}

std::string CommandShow::getUsage() const {
  return std::string("Show parameters. Usage: show");
}

}
}
}
