#include "Controller.hpp"

#include "config.hpp"

#include <histedit.h>
#include <cstdio>
#include <list>
#include <string>
#include <utility>
#include <dirent.h>
#include <cstring>
#include <sys/time.h>
#include <time.h>

#include "commands/ICommand.hpp"
#include "commands/CommandPlot.hpp"
#include "commands/CommandReplot.hpp"
#include "commands/CommandShow.hpp"
#include "commands/CommandSet.hpp"
#include "commands/CommandUnset.hpp"

#include "Components.hpp"
#include "CommandList.hpp"

namespace vrplot {
namespace controller {

const std::string Controller::PROMPT_MSG = PROG_NAME+"> ";

/* Add new commands in this function */
void Controller::initializeCommand() {
  
  com_list_->addCommand<command::CommandPlot>();
  com_list_->addCommand<command::CommandReplot>();
  com_list_->addCommand<command::CommandShow>();
  com_list_->addCommand<command::CommandSet>();
  com_list_->addCommand<command::CommandUnset>();
  
}

Controller::Controller( Components* components ) : components_(components){
  initialize();
}

bool Controller::execCommand( std::list< std::string > *cmd ) {

  if ( cmd->size() == 0 ) return false;

  bool status;

  const std::string command_name = cmd->front();
  cmd->pop_front();

  if( command_name == "exit" || command_name == "quit" ) {
    postQuit();
    return true;
  }

  if ( com_list_->isRegisterd( command_name ) ) {
    status = com_list_->execCommand( command_name, cmd, components_ );
  } else {
    status = false;
    printf("Unknown command: %s\n", command_name.c_str());
  }

  return status;
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
  if ( pthread_cancel( thread_ ) == 0 ) {
    pthread_join( thread_, NULL );
  }
  
  el_end(el_);
  tok_end(tok_);
  history_end(hist_);
  delete ev_;
}

const char * Controller::prompt(EditLine *el)
{
  return PROMPT_MSG.c_str();
}

unsigned char
Controller::completePath(EditLine *el, int ch)
{
  DIR *dd;
  struct dirent *dp;
  const char* ptr;
  const LineInfo *lf = el_line(el);
  int len;
  int res = CC_ERROR;

  /*                                                                                                                                                 
   * Find the last word                                                                                                                              
   */
  for (ptr = lf->cursor - 1;
       !isspace((unsigned char)*ptr) && ptr > lf->buffer; ptr--)
    continue;
  len = lf->cursor - ++ptr;

  std::string last_word = std::string( ptr, len );
  
  std::string::size_type pos = last_word.find_last_of('/');
  std::string basename;
  std::string dirname;

  if ( pos == std::string::npos ) {
    basename = last_word;
    dirname = ".";
  } else {
    basename = last_word.substr( pos + 1 );
    dirname = last_word.substr( 0, pos );
  }

  if ( basename == "." ) basename = std::string();

  dd = opendir( dirname.c_str() );
  ptr = basename.c_str();
  len = basename.size();
  
  for (dp = readdir(dd); dp != NULL; dp = readdir(dd)) {
    if (len > strlen(dp->d_name))
      continue;
    if (strncmp(dp->d_name, ptr, len) == 0) {
      if (el_insertstr(el, &dp->d_name[len]) == -1)
        res = CC_ERROR;
      else
        res = CC_REFRESH;
      break;
    }
  }

  closedir(dd);
  return res;
}

void Controller::initialize() {
  is_finished_ = false;
  is_quit_ = false;
  
  hist_ = history_init();
  ev_ = new HistEvent();
  
  history(hist_, ev_, H_SETSIZE, 100);
  
  tok_ = tok_init(NULL);

  el_ = el_init(PROG_NAME.c_str(), stdin, stdout, stderr);

  el_set(el_, EL_EDITOR, "emacs");
  el_set(el_, EL_SIGNAL, 1);
  el_set(el_, EL_PROMPT_ESC, prompt, '\1');

  el_set(el_, EL_HIST, history, hist_);

  /* Add a user-defined function        */
  el_set(el_, EL_ADDFN, "ed-complete", "Complete argument", completePath);
  
  /* Bind tab to it             */
  el_set(el_, EL_BIND, "^I", "ed-complete", NULL);
  
  el_source(el_, NULL);

  com_list_ = new command::CommandList();
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

    if ( execCommand( &cmd ) ) {
      // TODO: add error to log file
    }
    
    if ( is_quit_ ) break;
    
    tok_reset(tok_);
    cmd.clear();
  }

  is_finished_ = true;
}

void Controller::postQuit() {
  is_quit_ = true;
}

double Controller::getTime() {
  static struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec
    + static_cast<double>(tv.tv_usec) * 1e-6;
}

void* Controller::launchThread(void *obj) {
  reinterpret_cast<Controller*>(obj) -> pollCommand();
  return NULL;
}

}
}
