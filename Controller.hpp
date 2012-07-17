#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <vector>

#include <histedit.h>

#include <pthread.h>

namespace vrplot {

class Controller {
public:

  Controller();

  void invoke();

  bool isFinished();
  
  ~Controller();
  
private:
  EditLine *el_;
  Tokenizer *tok_;
  History *hist_;
  HistEvent *ev_;

  bool is_finished_;

  pthread_t thread_;

  static const std::string PROMPT_MSG;

  void initialize();

  bool execCommand( const std::vector< std::string > &cmd );
  static const char *prompt(EditLine *el);

  void pollCommand();

  static void* launchThread(void *obj);
  
};

}

#endif
