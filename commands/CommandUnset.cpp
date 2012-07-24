#include "commands/CommandUnset.hpp"

#include <iostream>

#include "Components.hpp"
#include "Parameters.hpp"
#include "CommandList.hpp"
#include "CommandParamsAutoscale.hpp"

namespace vrplot{
namespace controller {
namespace command {

CommandUnset::CommandUnset() : list_(NULL) {
  list_ = new CommandList();
  
  list_->addCommand< CommandParamsAutoscale >( CommandParamsAutoscale::ID_UNSET ); 
  
}

std::string CommandUnset::getName() const {
  return std::string("unset");
}

bool CommandUnset::execute( int id,
			   std::list< std::string>* args,
			   Components* components ) const {
  if ( args->size() == 0 ) {
    std::cout << getUsage() << std::endl;
    return false;
  }

  bool status;
  std::string cmd_name = args->front();
  args->pop_front();
	    
  status = list_->execCommand( cmd_name, args, components ); 


  return status;
}

std::string CommandUnset::getUsage() const {
  std::vector< std::string > com_names;
  std::vector< std::string >::iterator it;
  list_->getCommandName( &com_names );
  //return std::string("Unset parameters. Usage: show {all|param_name}");
  std::string result = std::string("Unset parameters. Usage: set {");
  for( it = com_names.begin();
       it != com_names.end();
       ++it ) {
    
    if ( it != com_names.begin() ) result += "|";
    result += *it;
  }
  result += "} {params}";
  return result;  
}

CommandUnset::~CommandUnset() {
  delete list_;
}

}
}
}
