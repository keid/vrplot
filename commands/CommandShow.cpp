#include "commands/CommandShow.hpp"

#include <iostream>

#include "Components.hpp"
#include "Parameters.hpp"
#include "CommandList.hpp"
#include "CommandParamsAutoscale.hpp"

namespace vrplot{
namespace controller {
namespace command {

CommandShow::CommandShow() : list_(NULL) {
  list_ = new CommandList();
  
  list_->addCommand< CommandParamsAutoscale >( CommandParamsAutoscale::ID_SHOW ); 
  
}

std::string CommandShow::getName() const {
  return std::string("show");
}

bool CommandShow::execute( int id,
			   std::list< std::string>* args,
			   Components* components ) const {
  if ( args->size() == 0 ) {
    std::cout << getUsage() << std::endl;
    return false;
  }

  bool status;
  std::string cmd_name = args->front();
  args->pop_front();
  
  if ( cmd_name == "all" ) {
    list_->execCommandAll( args, components );
    status = true;
  } else {
    status = list_->execCommand( cmd_name, args, components ); 
  }

  return status;
}

std::string CommandShow::getUsage() const {
  std::vector< std::string > com_names;
  std::vector< std::string >::iterator it;
  list_->getCommandName( &com_names );
  //return std::string("Show parameters. Usage: show {all|param_name}");
  std::string result = std::string("Show parameters. Usage: show {all");
  for( it = com_names.begin();
       it != com_names.end();
       ++it ) {
    result += "|" + *it;
  }
  result += "}";
  return result;  
}

CommandShow::~CommandShow() {
  delete list_;
}

}
}
}
