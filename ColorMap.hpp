#ifndef VRPLOT_COLORMAP_HPP
#define VRPLOT_COLORMAP_HPP

#include <map>
#include "Color.hpp"

namespace vrplot {

class ColorMap {
public:
  ColorMap();
  void setColor( double v, const Color& c );
  void setOutOfRangeColorMax( const Color& c );
  void setOutOfRangeColorMin( const Color& c );

  Color getColor( double v ) const;
  void getColor( double v, double *r, double *g, double *b, double *a) const;

  void clear();
  
private:
  std::map< double, Color > color_map_;
  Color c_max_;
  Color c_min_;

  void findRawColors
  ( double v,
    std::map< double, Color>::const_iterator *it_lower,
    std::map< double, Color>::const_iterator *it_upper
  ) const;
		      
};

}

#endif
