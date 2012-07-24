#ifndef COMMANDSET_HPP
#define COMMANDSET_HPP

#include "ICommand.hpp"

namespace vrplot {
namespace controller {
namespace command {

class CommandList;

class CommandSet : public ICommand {
public:
  CommandSet();
  
  std::string getName() const ;
  bool execute( int id,
		std::list< std::string >* args,
		Components* components ) const;
  std::string getUsage() const ;

  ~CommandSet();
private:
  CommandList *list_;
};

}
}
}


#endif
