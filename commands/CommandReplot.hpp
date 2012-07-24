#ifndef VRPLOT_CONTROLLER_COMMAND_COMMANDREPLOT_HPP
#define VRPLOT_CONTROLLER_COMMAND_COMMANDREPLOT_HPP

#include "ICommand.hpp"

namespace vrplot {

class Parameters;

namespace controller {
namespace command {

class CommandReplot : public ICommand {
public:
  std::string getName() const ;
  bool execute( int id,
		std::list< std::string >* args,
		Components* components ) const;
  std::string getUsage() const ;

private:
  unsigned int getAutoscaleMask( const Parameters *params ) const ;
};

}
}
}

#endif
