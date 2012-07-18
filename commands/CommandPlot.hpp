#ifndef VRPLOT_CONTROLLER_COMMAND_COMMANDPLOT_HPP
#define VRPLOT_CONTROLLER_COMMAND_COMMANDPLOT_HPP

#include "ICommand.hpp"

namespace vrplot {
namespace controller {
namespace command {

class CommandPlot : public ICommand {
public:
  std::string getName() const ;
  bool execute( std::list< std::string >& args,
		Components* components ) const;
  std::string getUsage() const ;
};

}
}
}

#endif
