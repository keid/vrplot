#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <list>
#include <map>

#include <histedit.h>
#include <pthread.h>

#include "commands/ICommand.hpp"

namespace vrplot {

class Components;

namespace controller {

class Controller {
public:

  Controller( Components* components );

  void invoke();
  
  bool isFinished();

  /*
   * Execute command using command table.
   */
  bool execCommand( std::list< std::string > &cmd );

  void postQuit();
  
  ~Controller();
  
private:
  static const std::string PROMPT_MSG;
  
  EditLine *el_;
  Tokenizer *tok_;
  History *hist_;
  HistEvent *ev_;
  
  bool is_finished_;
  bool is_quit_;
  pthread_t thread_;
   
  std::map< std::string, command::ICommand* > command_table_;

  Components *components_;

  void initialize();
  
  /*
   * Initialize the command table.
   */
  void initializeCommand();
  
  template< typename Command_Type >
  bool addCommand( ) {
    command::ICommand *p;
    
    try {
      p = new Command_Type();
    } catch ( std::bad_alloc &ex ) {
      return false;
    }
    
    command_table_.insert( std::make_pair( p->getName(), p ) );
    return true;
  }
  
  static const char *prompt(EditLine *el);
  static unsigned char completePath(EditLine *el, int ch);

  void pollCommand();

  double getTime();

  static void* launchThread(void *obj);
};

}
}

#endif
