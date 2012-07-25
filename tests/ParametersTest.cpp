#include "tests/ParametersTest.hpp"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cmath>
#include <string>
#include <map>
#include <utility>

CPPUNIT_TEST_SUITE_REGISTRATION( ParametersTest );

void ParametersTest::setUp() {}
void ParametersTest::tearDown() {}

void ParametersTest::testParamInt() {
  vrplot::Parameters::ParameterInt pi0;
  vrplot::Parameters::ParameterInt pi1;

  pi0 = std::string( "param0" );
  pi1 = std::string( "param1" );

  CPPUNIT_ASSERT( params_.getParam( pi0 ) == 0 );
  CPPUNIT_ASSERT( params_.getParam( pi1 ) == 0 );
  
  for(int i=0; i<128; ++i ) {
    params_.setParam( pi0, i );
    CPPUNIT_ASSERT( params_.getParam( pi0 ) == i );
    CPPUNIT_ASSERT( params_.getParam( pi1 ) == 0 );
  }
  
  pi1 = pi0;
  
  CPPUNIT_ASSERT( params_.getParam( pi0 ) == params_.getParam( pi1) );
  
}

void ParametersTest::testParamDouble() {
  vrplot::Parameters::ParameterDouble pd0;
  vrplot::Parameters::ParameterDouble pd1;
  
  pd0 = std::string( "param0" );
  pd1 = std::string( "param1" );

  CPPUNIT_ASSERT( params_.getParam( pd0 ) == 0 );
  CPPUNIT_ASSERT( params_.getParam( pd1 ) == 0 );
  
  for(int i=0; i<128; ++i ) {
    params_.setParam( pd0, i/128.0 );
    CPPUNIT_ASSERT( params_.getParam( pd0 ) == i/128.0 );
    CPPUNIT_ASSERT( params_.getParam( pd1 ) == 0 );
  }
  
  pd1 = pd0;
  
  CPPUNIT_ASSERT( params_.getParam( pd0 ) == params_.getParam( pd1) );

}

void ParametersTest::testParamString() {
  
  vrplot::Parameters::ParameterString ps0;
  vrplot::Parameters::ParameterString ps1;
  
  ps0 = std::string( "param0" );
  ps1 = std::string( "param1" );
  
  CPPUNIT_ASSERT( params_.getParam( ps0 ) == std::string() );
  CPPUNIT_ASSERT( params_.getParam( ps1 ) == std::string() );

  std::string value = "hogehoge";
  
  params_.setParam( ps0, value );
  CPPUNIT_ASSERT( params_.getParam( ps0 ) == value );
  CPPUNIT_ASSERT( params_.getParam( ps1 ) == std::string() );
  
  ps1 = ps0;

  CPPUNIT_ASSERT( params_.getParam( ps0 ) == params_.getParam( ps1 ) );
}

void ParametersTest::testParamComplex() {
  vrplot::Parameters::ParameterInt param_i;
  vrplot::Parameters::ParameterDouble param_d;
  vrplot::Parameters::ParameterString param_s;

  param_i = std::string("testParami");
  param_d = param_i;
  param_s = param_i;

  params_.setParam( param_i, 10 );
  CPPUNIT_ASSERT( params_.getParam( param_i ) != params_.getParam( param_d ) );

  params_.setParam( param_d, M_PI );
  CPPUNIT_ASSERT( params_.getParam( param_i ) != params_.getParam( param_d ) );

  CPPUNIT_ASSERT( params_.getParam( param_s ) == std::string() );  
}
