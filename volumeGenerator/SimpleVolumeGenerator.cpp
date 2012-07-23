#include "SimpleVolumeGenerator.hpp"

#include <iostream>
#include <cstring>
#include <cfloat>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <functional>

#include "VolumeData.hpp"
#include "FieldSelector.hpp"
#include "CoordinateAdjuster.hpp"
#include "ColorMap.hpp"
#include "Color.hpp"

namespace vrplot {
namespace volumeGenerator {

SimpleVolumeGenerator::
SimpleVolumeGenerator( unsigned int x, unsigned int y, unsigned int z )
  : volume_(NULL) {

  volume_ = new VolumeData( x, y, z );
}

void SimpleVolumeGenerator::generate( const FileLoader &loader,
				      const FieldSelector &selector,
				      CoordinateAdjuster &adjuster,
				      const ColorMap &colormap
) {
  
  if( selector.getFieldNum() < 4 ) {
    throw std::runtime_error("SimpleVolumeGenerator::4 indeces is needed at least.");
  }
  
  volume_->clear();

  // setting coordinates range
  const FileLoader::DataType &max_range = loader.getMaxRange();
  const FileLoader::DataType &min_range = loader.getMinRange();

  const double min_x = selector.getField( min_range, 0);
  const double min_y = selector.getField( min_range, 1);
  const double min_z = selector.getField( min_range, 2);
  
  const double dist_x = selector.getField( max_range, 0) - min_x;
  const double dist_y = selector.getField( max_range, 1) - min_y;
  const double dist_z = selector.getField( max_range, 2) - min_z;
  const double dist = std::max( dist_x, std::max( dist_y, dist_z ) );

  const double max_x = min_x + dist;
  const double max_y = min_y + dist;
  const double max_z = min_z + dist;

  adjuster.setRangeX( max_x, min_x );
  adjuster.setRangeY( max_y, min_y );
  adjuster.setRangeZ( max_z, min_z );

  // setting volume data
  for( FileLoader::DataContainerType::const_iterator it = loader.begin();
       it != loader.end();
       ++it ) {
    const FileLoader::DataType &data = *it;

    const double raw_x = selector.getField( data, 0 );
    const double raw_y = selector.getField( data, 1 );
    const double raw_z = selector.getField( data, 2 );
    
    const double x = adjuster.x(raw_x) * ( getVolume()->sizex() );
    const double y = adjuster.y(raw_y) * ( getVolume()->sizey() );
    const double z = adjuster.z(raw_z) * ( getVolume()->sizez() );
    
    double r, g, b, a;
    colormap.getColor( adjuster.x(raw_x), &r, &g, &b, &a );
    
    setVolumeElement
      ( 
       x, y, z,
       r * 255, g * 255, b * 255, a * 255);
  }
  
}

const VolumeData* SimpleVolumeGenerator::getVolume() const {
  return volume_;
}

bool SimpleVolumeGenerator::good() const {
  return true;
}

void SimpleVolumeGenerator::setVolumeElement( double x, double y, double z,
					      double r, double g, double b,
					      double a) {
  int ix = static_cast<int>(x + 0.5);
  int iy = static_cast<int>(y + 0.5);
  int iz = static_cast<int>(z + 0.5);

  try {
    volume_->element( ix, iy, iz, VolumeData::CH_RED ) += r;
    volume_->element( ix, iy, iz, VolumeData::CH_GREEN ) += g;
    volume_->element( ix, iy, iz, VolumeData::CH_BLUE ) += b;
    volume_->element( ix, iy, iz, VolumeData::CH_ALPHA ) += a;
  } catch ( const std::out_of_range & ex ) {
    return ;
  }
}

SimpleVolumeGenerator::
~SimpleVolumeGenerator() {
  delete volume_;
}

}
}
