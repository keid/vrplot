#ifndef COMMANDSHOW_HPP
#define COMMANDSHOW_HPP

#include "ICommand.hpp"

namespace vrplot {
namespace controller {
namespace command {

class CommandList;

class CommandShow : public ICommand {
public:
  CommandShow();
  
  std::string getName() const ;
  bool execute( int id,
		std::list< std::string >* args,
		Components* components ) const;
  std::string getUsage() const ;

  ~CommandShow();
private:
  CommandList *list_;
};

}
}
}

#endif
