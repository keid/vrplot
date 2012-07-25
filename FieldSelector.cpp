#include "FieldSelector.hpp"

#include <climits>

namespace vrplot {

FieldSelector::FieldSelector() {
}

FieldSelector::FieldSelector( const std::string& arg ) {
  setArg( arg );
}

FieldSelector::EntryType 
FieldSelector::getField( const DataType& data, int n ) const {
  if ( static_cast<int>(data.size()) < n ) {
    return 0;
  } else {
    return data.at( getFieldIndex( n ) );
  }
}

int FieldSelector::getFieldIndex( int n ) const {
  return n; 
}

int FieldSelector::getFieldNum() const {
  return INT_MAX;
}

void FieldSelector::setArg( const std::string& arg) {
}
  

}
