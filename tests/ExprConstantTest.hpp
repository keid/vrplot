#ifndef EXPRCONSTANTTEST_HPP
#define EXPRCONSTANTTEST_HPP

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

#include "expr/ExprExcept.hpp"
#include "expr/ExprConstant.hpp"

class ExprConstantTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(  ExprConstantTest );
  CPPUNIT_TEST( testNumeric );
  CPPUNIT_TEST_EXCEPTION( testTooManyArgException, vrplot::expr::exception::TooManyArg );
  CPPUNIT_TEST_SUITE_END();
  
private:
  vrplot::expr::ExprConstant *expr;
  
public:
  void setUp();
  void tearDown();

  void testNumeric();
  void testTooManyArgException();
};

#endif
