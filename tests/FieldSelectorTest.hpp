#ifndef VRPLOT_TEST_FIELDSELECTORTEST_HPP
#define VRPLOT_TEST_FIELDSELECTORTEST_HPP

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

#include "FieldSelector.hpp"

class FieldSelectorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(  FieldSelectorTest );
  CPPUNIT_TEST( testInitialCondition );
  CPPUNIT_TEST( testStandardFeature );
  CPPUNIT_TEST_SUITE_END();


private:
  vrplot::FieldSelector selector;
  
public:
  void setUp();
  void tearDown();

  void testInitialCondition();
  void testStandardFeature();
  
};

#endif
