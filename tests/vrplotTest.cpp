#include <iostream>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

//#include "ExprConstantTest.hpp"

int main() {
  CppUnit::Test *suite 
    = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
  
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( suite );

  bool wasSucessful = runner.run();

  int errorCode = wasSucessful ? 0 : 1;

  return errorCode;
}
