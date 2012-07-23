#ifndef SIMPLEVOLUMEGENERATOR_HPP
#define SIMPLEVOLUMEGENERATOR_HPP

#include <vector>
#include "IVolumeGenerator.hpp"
#include "FileLoader.hpp"

namespace vrplot {

namespace volumeGenerator {

class SimpleVolumeGenerator : public IVolumeGenerator {
  
public:
  SimpleVolumeGenerator( unsigned int x, unsigned int y, unsigned int z );
  
  void generate( const FileLoader &loader,
		 const FieldSelector &selector,
		 CoordinateAdjuster &adjuster,
		 const ColorMap &colormap
  );
  const VolumeData *getVolume() const;
  bool good() const;
  ~SimpleVolumeGenerator();

private:
  VolumeData *volume_;

  void setVolumeElement( double x, double y, double z,
			 double r, double g, double b,
			 double a);
  
};

}
}

#endif
