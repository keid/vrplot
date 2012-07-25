#include "tests/ExprConstantTest.hpp"
#include "expr/ExprConstant.hpp"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cmath>
#include <string>
#include <map>
#include <utility>

CPPUNIT_TEST_SUITE_REGISTRATION(ExprConstantTest);

void ExprConstantTest::setUp() {
  expr = new vrplot::expr::ExprConstant( M_PI );
}

void ExprConstantTest::tearDown() {
  delete expr;
}

void ExprConstantTest::testNumeric() {
  std::map< std::string, double> data;

  CPPUNIT_ASSERT( expr->eval() == M_PI );
  CPPUNIT_ASSERT( static_cast<double>(*expr) == M_PI );
  CPPUNIT_ASSERT( static_cast<double>(expr->clearVariables()) == M_PI );
  CPPUNIT_ASSERT( expr->eval( data ) == M_PI );

  data.insert( std::make_pair("x", M_PI) );
  
  CPPUNIT_ASSERT( expr->eval( data ) == M_PI );
  
}

void ExprConstantTest::testTooManyArgException() {
  expr->setArgs( 0, NULL );
}
