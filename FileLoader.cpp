#include "FileLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace vrplot {

FileLoader::FileLoader () {
  initialize();
}

FileLoader::FileLoader( const std::string& path ) {
  initialize();
  load( path );
}

bool FileLoader::load( const std::string& path ) {
  std::ifstream ifs( path.c_str() );

  if ( !ifs.good() ) return false;

  data_.clear();
  min_.clear();
  max_.clear();
  
  std::string buf;
  while( std::getline( ifs, buf ) ) {
    DataType entry;
    if ( readLine( buf, entry ) ) {
      data_.push_back ( entry );
      updateRange( entry );
    }
  }
  
  return true;
}

bool FileLoader::readLine( const std::string& str, DataType &data ) {
  std::string::size_type valid_pos = str.find_first_not_of(" \t");
  if( valid_pos == std::string::npos ) return false;
  if( str.at( valid_pos ) == '#' ) return false;

  std::string::size_type end_pos = str.find_first_of("#", valid_pos + 1);

  std::string striped_str;

  if ( end_pos == std::string::npos ) {
    striped_str = str.substr(valid_pos);
  } else {
    striped_str = str.substr(valid_pos, end_pos - valid_pos);
  }

  std::istringstream iss( striped_str );
  
  EntryType token;

  data.clear();
  while( !iss.eof() ) {
    iss >> token;
    data.push_back( token );
  }

  return true;
}

const FileLoader::DataType& FileLoader::getMinRange() const {
  return min_;
}

const FileLoader::DataType& FileLoader::getMaxRange() const {
  return max_;
}

FileLoader::DataContainerType::const_iterator FileLoader::begin() const {
  return data_.begin();
}

FileLoader::DataContainerType::const_iterator FileLoader::end() const {
  return data_.end();
}

FileLoader::DataContainerType::size_type FileLoader::size() const {
  return data_.size();
}

void FileLoader::initialize() {
  data_ = DataContainerType();
  min_ = DataType();
  max_ = DataType();
}

void FileLoader::updateRange( const DataType& data ){
  if ( min_.size() < data.size() ) {
    
    for( DataType::size_type i = min_.size(); i < data.size(); ++i ) {
      min_.push_back( data[i] );
      max_.push_back( data[i] );
    }
    
  }

  for( DataType::size_type i=0; i<data.size(); ++i ) {
    min_[i] = std::min( min_[i], data[i] );
    max_[i] = std::max( max_[i], data[i] );
  }
}

FileLoader::~FileLoader() {
}

}
