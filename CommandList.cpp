#include "CommandList.hpp"

#include "Components.hpp"

#include <stdexcept>

namespace vrplot {
namespace controller {
namespace command {

bool CommandList::execCommand( const std::string& cmd_name,
			       std::list< std::string > *args,
			       Components *components) {
  std::map< std::string, Command >::iterator it;

  bool status;
  if ( (it = command_table_.find( cmd_name )) != command_table_.end() ) {
    const command::ICommand *command = (*it).second.command;
    int id = (*it).second.id;
    status = command->execute( id, args, components );
  } else {
    status = false;
  }

  return false;
}
  
void CommandList::execCommandAll( std::list< std::string > *args,
				  Components *components) {
  std::map< std::string, Command >::iterator it;
  std::list< std::string > args_tmp;
  for( it = command_table_.begin();
       it != command_table_.end();
       ++it ) {
    const command::ICommand *command = (*it).second.command;
    int id = (*it).second.id;
    args_tmp = *args;
    command->execute( id, &args_tmp, components );
  }
}

void CommandList::getCommandName( std::vector< std::string > *array ) {
  std::map< std::string, Command >::iterator it;
  std::list< std::string > args_tmp;

  if ( array == NULL ) return;
  
  for( it = command_table_.begin();
       it != command_table_.end();
       ++it ) {
    const command::ICommand *command = (*it).second.command;
    array->push_back( command->getName() );
  }
}

CommandList& CommandList::addCommand( ICommand *command, const std::string& name, int id ) {
  Command com;
  std::pair< typename std::map< std::string, Command >::iterator, bool > result;
  com.command = command;
  com.id = id;
  result = command_table_.insert( std::make_pair( name, com ) );

  if ( result.second == false ) {
    throw std::runtime_error("Command name conflictiong.");
  }

  return *this;
}

bool CommandList::isRegisterd( const std::string& name ) {
  std::map< std::string, Command >::iterator it;
  if ( command_table_.find( name ) != command_table_.end() ) {
    return true;
  } else {
    return false;
  }
}

CommandList& CommandList::clear() {
  command_table_.clear();
  return *this;
}

CommandList::~CommandList() {
  std::map< std::string, Command >::iterator it;
  for( it = command_table_.begin();
       it != command_table_.end();
       ++it ) {
    command::ICommand *command = (*it).second.command;
    delete command;
   }
}


}
}
}
