#include "SimpleVolumeGenerator.hpp"

#include <iostream>
#include <cstring>
#include <cfloat>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace vrplot {
namespace volumeGenerator {

SimpleVolumeGenerator::
SimpleVolumeGenerator( unsigned int x, unsigned int y, unsigned int z )
  : x_(x), y_(y), z_(z), data_(NULL) {
  data_ = new ColorType[ size() * 4 ];
  initialize();
}

void SimpleVolumeGenerator::generate( const FileLoader &loader,
				      const std::vector< int > &index ) {
  if( index.size() < 4 ) {
    throw std::runtime_error("SimpleVolumeGenerator::4 indeces is needed at least.");
  }

  int n_index = index.size();

  std::vector< int > index_coordinate;
  index_coordinate.push_back( index.at(0) ); // x
  index_coordinate.push_back( index.at(1) ); // y
  index_coordinate.push_back( index.at(2) ); // z
  
  std::vector< int > index_color;
  index_color.push_back( index.at( std::min(3, n_index-1) ) ); // red
  index_color.push_back( index.at( std::min(4, n_index-1) ) ); // green
  index_color.push_back( index.at( std::min(5, n_index-1) ) ); // blue
  index_color.push_back( index.at( std::min(6, n_index-1) ) ); // alpha
  
  int n_unspecified
    = std::count_if( index_color.begin(), index_color.end(), std::bind2nd( std::less<int>(), 0 ));
  
  int is_invalid
    = std::count_if( index_color.begin(), index_color.end(), std::bind2nd( std::greater<int>(), index.size()-1 ));

  if( n_unspecified > 2 ) {
    throw std::runtime_error("At least one color index has to be valid.");
  } else if ( is_invalid ) {
    throw std::runtime_error("Too large index is specified.");
  }

  /////////////////
  
  double coordinate_min = DBL_MAX;
  double coordinate_max = -DBL_MAX;
  
  getRange( loader.getMinRange(), loader.getMaxRange(), index_coordinate,
	    &coordinate_max, &coordinate_min );

  double coordinate_offset = coordinate_min;
  double coordinate_coeff = sizeMax() / ( coordinate_max - coordinate_min );
  
  if ( coordinate_coeff >= 1 ) {
    coordinate_coeff = 1;
  }

  double color_min = DBL_MAX;
  double color_max = -DBL_MAX;

  getRange( loader.getMinRange(), loader.getMaxRange(), index_color,
	    &color_max, &color_min );

  double color_offset = color_min;
  double color_coeff = 255 / ( color_max - color_min );

  ////////////////

  for( FileLoader::DataContainerType::const_iterator it = loader.begin();
       it != loader.end();
       ++it ) {
    const FileLoader::DataType &data = *it;

    int index;
    double x, y, z;
    double r, g, b, a;
    
    index = index_coordinate.at( 0 );
    x = coordinate_coeff * (data.at( index ) + coordinate_offset);
    
    index = index_coordinate.at( 1 );
    y = coordinate_coeff * (data.at( index ) + coordinate_offset);

    index = index_coordinate.at( 2 );
    z = coordinate_coeff * (data.at( index ) + coordinate_offset);
    
    index = index_color.at( 0 );
    if ( index >= 0 && index < data.size() ) {
      r = color_coeff * ( data.at( index ) + color_offset );
    } else {
      r = 0.0;
    }

    index = index_color.at( 1 );
    if ( index >= 0 && index < data.size() ) {
      g = color_coeff * ( data.at( index ) + color_offset );
    } else {
      g = 0.0;
    }

    index = index_color.at( 2 );
    if ( index >= 0 && index < data.size() ) {
      b = color_coeff * ( data.at( index ) + color_offset );
    } else {
      b = 0.0;
    }

    index = index_color.at( 3 );
    if ( index >= 0 && index < data.size() ) {
      a = color_coeff * ( data.at( index ) + color_offset );
    } else {
      a = std::max( r, std::max( g, b ) );
    }

    setVolume( x, y, z, r, g, b, a );
    
  }
  
}

const void* SimpleVolumeGenerator::volume() const {
  return data_;
}

unsigned int SimpleVolumeGenerator::size() const {
  return sizex() * sizey() * sizez();
}

unsigned int SimpleVolumeGenerator::sizex() const {
  return x_;
}

unsigned int SimpleVolumeGenerator::sizey() const {
  return y_;
}

unsigned int SimpleVolumeGenerator::sizez() const {
  return z_;
}

unsigned int SimpleVolumeGenerator::sizeByte() const {
  return size() * sizeof(ColorType) * 4;
}

unsigned int SimpleVolumeGenerator::sizeMin() const {
  return std::min( sizex(), std::min( sizey(), sizez() ) );
}

unsigned int SimpleVolumeGenerator::sizeMax() const {
  return std::max( sizex(), std::max( sizey(), sizez() ) );
}


bool SimpleVolumeGenerator::good() const {
  return true;
}

void SimpleVolumeGenerator::initialize() {
  std::memset( data_, 0, sizeByte() );
}

void SimpleVolumeGenerator::getRange
  (
   const ::vrplot::FileLoader::DataType& min,
   const ::vrplot::FileLoader::DataType& max,
   const std::vector< int > &indeces,
   double *range_max, double *range_min
  ) {

  for( std::vector<int>::const_iterator it = indeces.begin();
       it != indeces.end();	
       ++it ) {
    int index = *it;
    if ( index < 0 ) continue;

    *range_max = std::max( *range_max, max.at(index) );
    *range_min = std::min( *range_min, min.at(index) );
    
  }
  
}

void SimpleVolumeGenerator::getOffsetAndCoeff
(
 const ::vrplot::FileLoader::DataType& min,
 const ::vrplot::FileLoader::DataType& max,
 unsigned int index,
 double *offset, double *coeff
) {
  *offset = min.at( index );
  *coeff = 255 / ( max.at( index ) - *offset );
}

void SimpleVolumeGenerator::setVolume( double x, double y, double z,
		double r, double g, double b,
		double a) {
  int ix = clip(static_cast<int>(x));
  int iy = clip(static_cast<int>(y));
  int iz = clip(static_cast<int>(z));
  
  int index = ix + sizex() * iy + sizex() * sizey() * iz;

  ColorType *v = data_;

  *(v + 4*index + CH_RED) = clip( *(v + 4*index + CH_RED) + r );
  *(v + 4*index + CH_GREEN) = clip( *(v + 4*index + CH_GREEN) + g );
  *(v + 4*index + CH_BLUE) = clip( *(v + 4*index + CH_BLUE) + b );
  *(v + 4*index + CH_ALPHA) = clip( *(v + 4*index + CH_ALPHA) + a );
}

SimpleVolumeGenerator::
~SimpleVolumeGenerator() {
  delete [] data_;
}

}
}
