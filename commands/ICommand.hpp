#ifndef VRPLOT_CONTROLLER_COMMAND_ICOMMAND_HPP
#define VRPLOT_CONTROLLER_COMMAND_ICOMMAND_HPP

#include <string>
#include <list>

namespace vrplot {

class Components;

namespace controller {
namespace command {

class ICommand {
public:
  /*
   * Return a name of this command. The name will be used for 
   * invoke the command from prompt.
   */
  virtual std::string getName() const = 0;

  /*
   * Execute a command using args. Return true if the command is
   * finished successfully, or return false if failed.
   */
  virtual bool execute( std::list< std::string >& args,
			Components* components) const = 0;
  
  /*
   * Return a message which describes a usage of a command. The
   * message will be printed as a part of help.
   */
  virtual std::string getUsage() const = 0;
  
  virtual ~ICommand(){}
};

}
}
}



#endif
