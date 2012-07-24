#ifndef COMMANDLIST_HPP
#define COMMANDLIST_HPP

#include <map>
#include <utility>
#include <string>
#include <list>
#include <vector>

#include "commands/ICommand.hpp"

namespace vrplot {

class Components;

namespace controller {
namespace command {

class CommandList {
private:
  typedef struct {
    ICommand *command;
    int id;
  } Command;

public:
  bool execCommand( const std::string& cmd_name,
		    std::list< std::string > *args,
		    Components *components);
  
  void execCommandAll( std::list< std::string > *args,
		       Components *components);

  void getCommandName( std::vector< std::string > *array );
  
  bool isRegisterd( const std::string& name );
  
  template< typename Command_Type >
  CommandList& addCommand() {
    ICommand *p = new Command_Type();

    return addCommand( p, p->getName(), 0 );
  }

  template< typename Command_Type >
  CommandList& addCommand( int id ) {
    ICommand *p = new Command_Type();
    return addCommand( p, p->getName(), id );
  }

  template< typename Command_Type >
  CommandList& addCommand( const std::string& name, int id ) {
    ICommand *p = new Command_Type();
    return addCommand( p, name, id );
  }
  
  CommandList& clear();

  ~CommandList();
private:
  std::map< std::string, Command > command_table_;

  CommandList& addCommand( ICommand *command, const std::string& name, int arg );
};

}
}
}

#endif
