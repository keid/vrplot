#ifndef VRPLOT_TSET_EXPRGENERATORTEST_HPP
#define VRPLOT_TSET_EXPRGENERATORTEST_HPP

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

#include "expr/ExprExcept.hpp"
#include "expr/ExprGenerator.hpp"

class ExprGeneratorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(  ExprGeneratorTest );
  CPPUNIT_TEST( testConstant );
  CPPUNIT_TEST( testVariable );
  CPPUNIT_TEST( testFunction );
  CPPUNIT_TEST_EXCEPTION( testSyntaxError, vrplot::expr::exception::SyntaxError );
  CPPUNIT_TEST_SUITE_END();
  
public:
  void setUp();
  void tearDown();

  void testConstant();
  void testVariable();
  void testFunction();
  void testSyntaxError();
  
};




#endif
