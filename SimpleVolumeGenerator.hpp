#ifndef SIMPLEVOLUMEGENERATOR_HPP
#define SIMPLEVOLUMEGENERATOR_HPP

#include <vector>
#include "IVolumeGenerator.hpp"
#include "FileLoader.hpp"

namespace vrplot {

//class FileLoader;

namespace volumeGenerator {

class SimpleVolumeGenerator : public IVolumeGenerator {
  
public:
  SimpleVolumeGenerator( unsigned int x, unsigned int y, unsigned int z );
  void generate( const FileLoader &loader,
		 const std::vector< int > &index );
  const void *volume() const;
  unsigned int size() const;
  unsigned int sizex() const;
  unsigned int sizey() const;
  unsigned int sizez() const;
  unsigned int sizeByte() const;
  unsigned int sizeMin() const;
  unsigned int sizeMax() const;
  bool good() const;
  ~SimpleVolumeGenerator();

private:
  const unsigned int x_;
  const unsigned int y_;
  const unsigned int z_;
  ColorType *data_;

  void initialize();

  void getRange
  (
   const ::vrplot::FileLoader::DataType& min,
   const ::vrplot::FileLoader::DataType& max,
   const std::vector< int > &color_index,
   double *range_max, double *range_min
  );

  void getOffsetAndCoeff
  (
   const ::vrplot::FileLoader::DataType& min,
   const ::vrplot::FileLoader::DataType& max,
   unsigned int index,
   double *offset, double *coeff
  );

  void setVolume( double x, double y, double z,
		  double r, double g, double b,
		  double a);
  
  template< typename T >
  unsigned char clip( T v ) {
    return static_cast<unsigned char>( std::max( static_cast<T>(0), std::min( static_cast<T>(255), v ) ) );
  }

};

}
}

#endif
