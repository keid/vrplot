#ifndef VOLUMEDATA_HPP
#define VOLUMEDATA_HPP

namespace vrplot {

class VolumeData {
public:
  
  typedef unsigned int ch_type;
  typedef unsigned int size_type;
  typedef unsigned char element_type;

  static const unsigned int CH_RED = 0;
  static const unsigned int CH_GREEN = 1;
  static const unsigned int CH_BLUE = 2;
  static const unsigned int CH_ALPHA = 3;
  
  
  VolumeData( );
  VolumeData( size_type s );
  VolumeData( size_type x, size_type y, size_type z );
  
  element_type& element(int x, int y, int z, ch_type ch);
  const element_type& element(int x, int y, int z, ch_type ch) const;

  const element_type* getVolume() const;
  
  void clear();
  
  size_type sizex() const;
  size_type sizey() const;
  size_type sizez() const;
  size_type sizeByte() const;

  VolumeData& operator=( const VolumeData& rhl );

  ~VolumeData();
  
private:
  element_type* data_;
  size_type sx_;
  size_type sy_;
  size_type sz_;

  void initialize();

  int getIndex( int x, int y, int z, ch_type ch ) const;

  bool isValidIndex( int x, int y, int z ) const;
  
};

}

#endif
