#ifndef COMMANDSHOW_HPP
#define COMMANDSHOW_HPP

#include "ICommand.hpp"

namespace vrplot {
namespace controller {
namespace command {

class CommandShow : public ICommand {
public:
  std::string getName() const ;
  bool execute( int id,
		std::list< std::string >* args,
		Components* components ) const;
  std::string getUsage() const ;
};

}
}
}

#endif
