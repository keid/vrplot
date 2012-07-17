#ifndef IVOLUMEGENERATOR_HPP
#define IVOLUMEGENERATOR_HPP

#include <vector>

namespace vrplot {

class FileLoader;

namespace volumeGenerator {

class IVolumeGenerator {
protected:
  static const unsigned int CH_RED = 0;
  static const unsigned int CH_GREEN = 1;
  static const unsigned int CH_BLUE = 2;
  static const unsigned int CH_ALPHA = 3;

public:

  typedef unsigned char ColorType;
  
  virtual void generate( const FileLoader &loader,
			 const std::vector< int > &index ) = 0;
  virtual const void* volume() const = 0;
  virtual unsigned int size() const = 0;
  virtual unsigned int sizex() const = 0;
  virtual unsigned int sizey() const = 0;
  virtual unsigned int sizez() const = 0;
  virtual unsigned int sizeByte() const = 0;
  virtual bool good() const = 0;
  virtual ~IVolumeGenerator(){};
};

}
}

#endif
