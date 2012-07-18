#ifndef VOLUME_GENERATOR_DEMO0_HPP
#define VOLUME_GENERATOR_DEMO0_HPP

#include <vector>
#include "IVolumeGenerator.hpp"
#include "Fileloader.hpp"

namespace vrplot {
namespace volumeGenerator {

class Demo0 :public IVolumeGenerator {
public:
  Demo0( unsigned int x, unsigned int y, unsigned int z );
  void generate( const FileLoader &loader,
		 const std::vector< int > &index );

  const void *volume() const;
  unsigned int size() const;
  unsigned int sizex() const;
  unsigned int sizey() const;
  unsigned int sizez() const;
  unsigned int sizeByte() const;

  bool good() const;
  ~Demo0();

private:
  const unsigned int x_;
  const unsigned int y_;
  const unsigned int z_;
  ColorType *data_;
  
  void initialize();
};

}
}

#endif
