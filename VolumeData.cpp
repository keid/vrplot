#include "VolumeData.hpp"

#include <cstring>
#include <stdexcept>

namespace vrplot {

VolumeData::VolumeData( ) : data_(NULL), sx_(0), sy_(0), sz_(0) {}

VolumeData::VolumeData( size_type s ) : data_(NULL), sx_(s), sy_(s), sz_(s) {
  initialize();
}

VolumeData::VolumeData( size_type x, size_type y, size_type z) :
  data_(NULL), sx_(x), sy_(y), sz_(z) {
  initialize();
}

VolumeData::element_type& 
VolumeData::element( int x, int y, int z, ch_type ch) {
  if ( data_ == NULL) {
    throw std::runtime_error("Memory space is not allocated.");
  }

  if ( !isValidIndex( x, y, z ) ) {
    throw std::out_of_range("");
  }

  return data_[ getIndex(x, y, z, ch) ];
}

const VolumeData::element_type&
VolumeData::element( int x, int y, int z, ch_type ch) const {
  if ( data_ == NULL) {
    throw std::runtime_error("Memory space is not allocated.");
  }

  if ( !isValidIndex( x, y, z ) ) {
    throw std::out_of_range("");
  }

  return data_[ getIndex(x, y, z, ch) ];
}

const VolumeData::element_type* VolumeData::getVolume() const {
  return data_;
}

void VolumeData::clear() {
  memset( data_, 0, sizeByte() );
}

VolumeData::size_type VolumeData::sizex() const {
  return sx_;
}

VolumeData::size_type VolumeData::sizey() const {
  return sy_;
}

VolumeData::size_type VolumeData::sizez() const {
  return sz_;
}

VolumeData::size_type VolumeData::sizeByte() const {
  return sizex() * sizey() * sizez() * 4;
}

VolumeData& VolumeData::operator=( const VolumeData& rhl ) {
  if ( (sizex() != rhl.sizex()) 
       || (sizey() != rhl.sizey())
       || (sizez() != rhl.sizez()) ) {

    sx_ = rhl.sizex();
    sy_ = rhl.sizey();
    sz_ = rhl.sizez();
    
    initialize();
  }
  
  memmove( data_, rhl.getVolume(), sizeByte() );

  return *this;
}

void VolumeData::initialize() {
  delete [] data_;
  data_ = new element_type[ sizeByte() ];
}

int VolumeData::getIndex( int x, int y, int z, ch_type ch ) const {
  return 4 * (x + y * sizex() + z * sizex() * sizey() ) + ch;
}

bool VolumeData::isValidIndex( int x, int y, int z ) const {
  if ( x < 0 ) return false;
  if ( x >= static_cast<int>(sizex()) ) return false;
  if ( y < 0 ) return false;
  if ( y >= static_cast<int>(sizey()) ) return false;
  if ( z < 0 ) return false;
  if ( z >= static_cast<int>(sizez()) ) return false;

  return true;
}

VolumeData::~VolumeData() {
  delete [] data_;
}

}
