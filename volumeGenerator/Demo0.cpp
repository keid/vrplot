#include "Demo0.hpp"

#include <cmath>

namespace vrplot {
namespace volumeGenerator {

Demo0::Demo0( unsigned int x, unsigned int y, unsigned int z )
  : x_(x), y_(y), z_(z) {
  initialize();
}

void Demo0::generate( const FileLoader &loader,
		      const std::vector< int > &index ) {
  return;
}

void Demo0::initialize () {
  data_ = new ColorType[ size() * 4 ];

  for(int z = 0; z < sizez(); ++z ) {
    for(int y = 0; y < sizey(); ++y ) {
      for(int x = 0; x < sizex(); ++x ) {
	int index  = x + y * sizex() + z * sizex() * sizey();
	index *= 4;

	float dist = std::sqrt( (x-sizex()/2)*(x-sizex()/2)
				+ (y-sizey()/2)*(y-sizey()/2)
				+ (z-sizez()/2)*(z-sizez()/2) );
	unsigned int r = 0;
	unsigned int g = 0;
	unsigned int b = 0;
	unsigned int a = 0;

	float wave = std::sin( dist * 0.1 );
	
	if( wave > 0 ) {
	  r = 255 * std::abs(wave);
	  b = 0;
	} else {
	  r = 0;
	  b = 255 * std::abs(wave);
	}

	if ( (x == sizex()/2) || ( y == sizey()/2 ) ) {
	  a = std::max(r, b) / (dist / 50);
	}
	
	if ( dist < sizex() / 2 ) {
	  a = std::max(r, b) / (dist / 5);
	} else {
	  a = 0;
	}

	if ( x > (sizex()/2) && y > (sizey()/2)  ) {
	  a = 0;
	}

	data_[ index + CH_RED ] = r;
	data_[ index + CH_GREEN ] = g;
	data_[ index + CH_BLUE ] = b;
	data_[ index + CH_ALPHA ] = a;
      }
    }
  }

}

const void* Demo0::volume() const {
  return data_;
}

unsigned int Demo0::size() const {
  return sizex() * sizey() * sizez();
}

unsigned int Demo0::sizex() const {
  return x_;
}

unsigned int Demo0::sizey() const {
  return y_;
}

unsigned int Demo0::sizez() const {
  return z_;
}

unsigned int Demo0::sizeByte() const {
  return size() * sizeof(ColorType) * 4;
}

bool Demo0::good() const {
  return true;
}

Demo0::~Demo0() {
  delete [] data_;
}



}
}
