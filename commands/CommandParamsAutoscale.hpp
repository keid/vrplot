#ifndef COMMANDPARAMSAUTOSCALE_HPP
#define COMMANDPARAMSAUTOSCALE_HPP

#include "ICommand.hpp"

namespace vrplot {
namespace controller {
namespace command {

class CommandParamsAutoscale : public ICommand {
public:
  static const unsigned int ID_SET;
  static const unsigned int ID_UNSET;
  static const unsigned int ID_SHOW;

  std::string getName() const;
  bool execute( int id,
		std::list< std::string >* args,
		Components* components ) const;
  std::string getUsage() const;
};

}
}
}

#endif
