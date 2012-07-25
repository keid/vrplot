#ifndef PARAMETERSTEST_HPP
#define PARAMETERSTEST_HPP

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

#include "Parameters.hpp"
#include "ParameterTable.hpp"

class ParametersTest : public CppUnit::TestFixture {
  
  CPPUNIT_TEST_SUITE( ParametersTest );
  CPPUNIT_TEST( testParamInt );
  CPPUNIT_TEST( testParamDouble );
  CPPUNIT_TEST( testParamString );
  CPPUNIT_TEST( testParamComplex );
  CPPUNIT_TEST_SUITE_END();

private:
  vrplot::ParameterTable params_;
  
public:
  void setUp();
  void tearDown();

  void testParamInt();
  void testParamDouble();
  void testParamString();

  void testParamComplex();
  
};


#endif
