#include "FieldSelectorTest.hpp"

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cmath>
#include <string>
#include <map>
#include <utility>
#include <vector>

#include "FileLoader.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(FieldSelectorTest);

void FieldSelectorTest::setUp() {
}

void FieldSelectorTest::tearDown() {
}

void FieldSelectorTest::testInitialCondition() {  
  for ( int i=0; i<128; ++i ) {
    CPPUNIT_ASSERT( selector.getFieldIndex( i ) == i );
  }

  vrplot::FileLoader::DataType data;
  data.push_back( 1.1 );
  data.push_back( 2.2 );
  data.push_back( 3.3 );
  data.push_back( 4.4 );

  for ( int i=0; i<4; ++i ) {
    CPPUNIT_ASSERT( selector.getField( data, i) == data.at(i) );
  }
 
}

void FieldSelectorTest::testStandardFeature() {
  vrplot::FileLoader::DataType data;
  data.push_back( 1.1 );
  data.push_back( 2.2 );
  data.push_back( 3.3 );
  data.push_back( 4.4 );

  vrplot::FileLoader::DataType data2;
  data2.push_back( 3.3 );
  data2.push_back( 2.2 );
  data2.push_back( 1.1 );
  data2.push_back( 4.4 );

  selector.setArg( "3:2:1:4" );

  CPPUNIT_ASSERT( selector.getFieldNum() == 4 );

  CPPUNIT_ASSERT( selector.getFieldIndex( 0 ) == 2 );
  CPPUNIT_ASSERT( selector.getFieldIndex( 1 ) == 1 );
  CPPUNIT_ASSERT( selector.getFieldIndex( 2 ) == 0 );
  CPPUNIT_ASSERT( selector.getFieldIndex( 3 ) == 3 );
  
  for ( int i=0; i<4; ++i ) {
    CPPUNIT_ASSERT( selector.getField( data, i) == data2.at(i) );
  }

}

