#include "tests/ExprGeneratorTest.hpp"
#include "expr/ExprConstant.hpp"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cmath>
#include <string>
#include <map>
#include <utility>

CPPUNIT_TEST_SUITE_REGISTRATION(ExprGeneratorTest);

void ExprGeneratorTest::setUp() {
}

void ExprGeneratorTest::tearDown() {
}

void ExprGeneratorTest::testConstant() {
  vrplot::expr::IExpr *expr = NULL;
  std::string expr_str;

  expr_str = "1238";
  expr = vrplot::expr::ExprGenerator::generate( expr_str );
  CPPUNIT_ASSERT( expr != NULL );
  CPPUNIT_ASSERT( expr->eval() == 1238 );
  
  expr_str = "-999";
  expr = vrplot::expr::ExprGenerator::generate( expr_str );
  CPPUNIT_ASSERT( expr != NULL );
  CPPUNIT_ASSERT( expr->eval() == -999 );
  
  expr_str = "3.14";
  expr = vrplot::expr::ExprGenerator::generate( expr_str );
  CPPUNIT_ASSERT( expr != NULL );
  CPPUNIT_ASSERT( expr->eval() == 3.14 );
  
  expr_str = "-3.14";
  expr = vrplot::expr::ExprGenerator::generate( expr_str );
  CPPUNIT_ASSERT( expr != NULL );
  CPPUNIT_ASSERT( expr->eval() == -3.14 );
}

void ExprGeneratorTest::testVariable() {
  vrplot::expr::IExpr *expr = NULL;
  std::string expr_str = "x";

  std::map< std::string, double > data;

  data.insert( std::make_pair( "x", 2.73 ) );

  expr = vrplot::expr::ExprGenerator::generate( expr_str );
  CPPUNIT_ASSERT( expr != NULL );
  CPPUNIT_ASSERT( expr->eval(data) == 2.73 );

  expr_str = "x*y";
  expr = vrplot::expr::ExprGenerator::generate( expr_str );
  CPPUNIT_ASSERT( expr != NULL );

  data.insert( std::make_pair( "y", 3.14 ) );
  CPPUNIT_ASSERT( expr->eval(data) == (2.73*3.14) );
}

void ExprGeneratorTest::testFunction() {
  vrplot::expr::IExpr *expr = NULL;
  CPPUNIT_ASSERT( expr != NULL );
}

void ExprGeneratorTest::testSyntaxError() {
}
