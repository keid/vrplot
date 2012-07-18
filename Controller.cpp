#include "Controller.hpp"

#include "config.hpp"

#include <histedit.h>
#include <cstdio>
#include <list>
#include <string>
#include <utility>

#include "commands/ICommand.hpp"
#include "commands/CommandPlot.hpp"

namespace vrplot {
namespace controller {

const std::string Controller::PROMPT_MSG = PROG_NAME+"> ";

/* Add new commands in this function */
void Controller::initializeCommand() {
  
  addCommand<command::CommandPlot>();
  
}

Controller::Controller() {
  initialize();
}

bool Controller::execCommand( std::list< std::string > &cmd ) {

  if ( cmd.size() == 0 ) return false;

  const std::string command_name = cmd.front();
  cmd.pop_front();

  if( command_name == "exit" || command_name == "quit" ) {
    return true;
  }

  std::map< std::string, command::ICommand*  >::const_iterator it;
  if ( ( it = command_table_.find( command_name ) ) != command_table_.end() ) {
    const command::ICommand *command = (*it).second;
    command->execute( cmd );
  } else {
    printf("Unknown command: %s\n", command_name.c_str());
  }

  return false;
}

void Controller::invoke() {
  if ( pthread_cancel( thread_ ) == 0 ) {
    pthread_join( thread_, NULL );
  }
  
  pthread_create( &thread_, NULL, &Controller::launchThread, this );
}

bool Controller::isFinished() {
  return is_finished_;
}

Controller::~Controller() {
  el_end(el_);
  tok_end(tok_);
  history_end(hist_);
  delete ev_;
}

const char * Controller::prompt(EditLine *el)
{
  return PROMPT_MSG.c_str();
}

void Controller::initialize() {
  is_finished_ = false;
  
  hist_ = history_init();
  ev_ = new HistEvent();
  
  history(hist_, ev_, H_SETSIZE, 100);
  
  tok_ = tok_init(NULL);

  el_ = el_init(PROG_NAME.c_str(), stdin, stdout, stderr);

  el_set(el_, EL_EDITOR, "emacs");
  el_set(el_, EL_SIGNAL, 1);
  el_set(el_, EL_PROMPT_ESC, prompt, '\1');

  el_set(el_, EL_HIST, history, hist_);

  el_source(el_, NULL);

  initializeCommand();
}

void Controller::pollCommand() {
  const char* buf;
  int num;

  std::list < std::string > cmd;

  while( (buf = el_gets(el_, &num)) != NULL && (num != 0) ) {
    int ac = 0;
    const char **av;
    const LineInfo *li;

    pthread_testcancel();
    
    li = el_line(el_);

    int tok_cond = tok_line(tok_, li, &ac, &av, NULL, NULL);

    if ( tok_cond < 0 ) {
      fprintf(stderr, "Internal error\n");
      continue;
    } else if ( tok_cond > 0 ) {
      fprintf(stderr, "Unmatched quote or backslash\n");
      continue;
    }

    history( hist_, ev_, H_ENTER, buf );

    for( int i=0; i<ac; ++i ) {
      //printf("%s\n", av[i]);
      cmd.push_back( std::string( av[i] ) );
    }

    if ( execCommand( cmd ) ) break;

    tok_reset(tok_);
    cmd.clear();
  }

  is_finished_ = true;
}

void* Controller::launchThread(void *obj) {
  reinterpret_cast<Controller*>(obj) -> pollCommand();
  return NULL;
}

}
}
