#include "ColorMap.hpp"
#include <utility>

namespace vrplot {

ColorMap::ColorMap() : color_map_() {
  clear();
}

void ColorMap::setColor( double v, const Color& c ) {
  color_map_.insert( std::make_pair(v, c) );
}

void ColorMap::setOutOfRangeColorMax( const Color& c ) {
  c_max_ = c;
}

void ColorMap::setOutOfRangeColorMin( const Color& c ) {
  c_min_ = c;
}

Color ColorMap::getColor( double v ) const {
  std::map< double, Color>::const_iterator it_lower;
  std::map< double, Color>::const_iterator it_upper;
  findRawColors( v, &it_lower, &it_upper); 
  if (it_lower == color_map_.end() ) return c_min_;
  if (it_upper == color_map_.end() ) return c_max_;
  
  double dist = (*it_upper).first - (*it_lower).first;
  double cfu = (v - (*it_lower).first) / dist;
  double cfl = ((*it_upper).first - v) / dist;
  const Color& cu = (*it_lower).second;
  const Color& cl = (*it_upper).second;
  Color c;
  c.setRGB( cfu*cu.getR() + cfl*cl.getR(),
	    cfu*cu.getG() + cfl*cl.getG(),
	    cfu*cu.getB() + cfl*cl.getB() );
  c.setAlpha( cfu*cu.getAlpha() + cfl*cl.getAlpha() );
  return c;
}

void ColorMap::getColor( double v, double *r, double *g, double *b, double *a) const {
  std::map< double, Color>::const_iterator it_lower;
  std::map< double, Color>::const_iterator it_upper;
  findRawColors( v, &it_lower, &it_upper); 
  if (it_lower == color_map_.end() ) {
    *r = c_min_.getR();
    *g = c_min_.getG();
    *b = c_min_.getB();
    *a = c_min_.getAlpha();
  }
  
  if (it_upper == color_map_.end() ) {
    *r = c_max_.getR();
    *g = c_max_.getG();
    *b = c_max_.getB();
    *a = c_max_.getAlpha();
  }
  
  double dist = (*it_upper).first - (*it_lower).first;
  double cfu = (v - (*it_lower).first) / dist;
  double cfl = ((*it_upper).first - v) / dist;
  const Color& cl = (*it_lower).second;
  const Color& cu = (*it_upper).second;

  *r = cfu*cu.getR() + cfl*cl.getR();
  *g = cfu*cu.getG() + cfl*cl.getG();
  *b = cfu*cu.getB() + cfl*cl.getB();
  *a = cfu*cu.getAlpha() + cfl*cl.getAlpha();
}

void ColorMap::clear() {
  color_map_.clear();
  c_max_.setAlpha( 0.0 );
  c_min_.setAlpha( 0.0 );
}

void ColorMap::findRawColors
  ( double v,
    std::map< double, Color>::const_iterator *it_lower,
    std::map< double, Color>::const_iterator *it_upper
  ) const {
  *it_upper = color_map_.upper_bound( v );

  if (*it_upper == color_map_.end() ) return;
  if (*it_upper == color_map_.begin() ) {
    *it_lower = color_map_.end();
    return;
  }

  *it_lower = *it_upper;
  --(*it_lower);
}


}
