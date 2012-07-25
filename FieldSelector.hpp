#ifndef FIELDSELECTOR_HPP
#define FIELDSELECTOR_HPP

#include <string>
#include "FileLoader.hpp"

namespace vrplot {

class FieldSelector {

private:
  typedef FileLoader::EntryType EntryType;
  typedef FileLoader::DataType DataType;
  
public:
  FieldSelector();
  FieldSelector( const std::string& arg );

  EntryType getField( const DataType& data, int n ) const;
  int getFieldIndex( int n ) const;
  int getFieldNum() const;

  void setArg( const std::string& arg);
  
};

}

#endif
