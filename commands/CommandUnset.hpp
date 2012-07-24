#ifndef COMMANDUNSET_HPP
#define COMMANDUNSET_HPP

#include "ICommand.hpp"

namespace vrplot {
namespace controller {
namespace command {

class CommandList;

class CommandUnset : public ICommand {
public:
  CommandUnset();
  
  std::string getName() const ;
  bool execute( int id,
		std::list< std::string >* args,
		Components* components ) const;
  std::string getUsage() const ;

  ~CommandUnset();
private:
  CommandList *list_;
};

}
}
}


#endif
