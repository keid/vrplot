#ifndef IVOLUMEGENERATOR_HPP
#define IVOLUMEGENERATOR_HPP

#include <vector>

namespace vrplot {

class FileLoader;
class VolumeData;
class FieldSelector;
class CoordinateAdjuster;

namespace volumeGenerator {

class IVolumeGenerator {
public:
  virtual void generate( const FileLoader &loader,
			 const FieldSelector &selector,
			 CoordinateAdjuster &adjuster
  ) = 0;
  virtual const VolumeData* getVolume() const = 0;
  virtual bool good() const = 0;
  virtual ~IVolumeGenerator(){};
};

}
}

#endif
