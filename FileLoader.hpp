#ifndef FILELOADER_HPP
#define FILELOADER_HPP

#include <string>
#include <vector>

namespace vrplot {

class FileLoader {
public:
  typedef double EntryType;
  typedef std::vector< EntryType > DataType;
  typedef std::vector< DataType > DataContainerType;
  
  FileLoader();
  FileLoader( const std::string& path );

  bool load( const std::string& path );

  DataContainerType::const_iterator begin() const;
  DataContainerType::const_iterator end() const;
  DataContainerType::size_type size() const;

  const DataType& getMinRange() const;
  const DataType& getMaxRange() const;

  ~FileLoader();

private:
  DataContainerType data_;
  DataType min_;
  DataType max_;
  
  bool readLine( const std::string& str, DataType &data );

  void initialize();

  void updateRange( const DataType& data );

};

}

#endif
