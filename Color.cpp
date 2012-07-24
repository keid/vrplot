#include "Color.hpp"
#include <algorithm>
#include <cmath>

namespace vrplot {

const double Color::R_MAX = 1.0;
const double Color::G_MAX = 1.0;
const double Color::B_MAX = 1.0;
const double Color::H_MAX = 360.0;
const double Color::S_MAX = 1.0;
const double Color::V_MAX = 1.0;
const double Color::ALPHA_MAX = 1.0;

const unsigned int Color::MODE_RGB = 0;
const unsigned int Color::MODE_HSV = 1;

Color::Color() {
}

Color::Color( unsigned int mode, double v0, double v1, double v2, double alpha) {
  if ( mode == MODE_RGB ) {
    setRGB( v0, v1, v2 );
  } else if ( mode == MODE_HSV ) {
    setHSV( v0, v1, v2 );
  }
  setAlpha( alpha );
}

void Color::setRGB( double r, double g, double b ) {
  r_ = std::max( 0.0, std::min( R_MAX, r ) );
  g_ = std::max( 0.0, std::min( G_MAX, g ) );
  b_ = std::max( 0.0, std::min( B_MAX, b ) );
  convertRGBtoHSV( r, g, b, &h_, &s_, &v_ );
}

void Color::setRGBA( double r, double g, double b, double a ) {
  r_ = std::max( 0.0, std::min( R_MAX, r ) );
  g_ = std::max( 0.0, std::min( G_MAX, g ) );
  b_ = std::max( 0.0, std::min( B_MAX, b ) );
  convertRGBtoHSV( r, g, b, &h_, &s_, &v_ );
  setAlpha( a );
}

void Color::getRGB( double *r, double *g, double *b ) const {
  *r = r_;
  *g = g_;
  *b = b_;
}

void Color::getRGBA( double *r, double *g, double *b, double *a) const {
  getRGB( r, g, b );
  *a = getAlpha();
}

void Color::setR( double r ) {
  r_ = std::max( 0.0, std::min( R_MAX, r ) );
  convertRGBtoHSV( r_, g_, b_, &h_, &s_, &v_ );
}

void Color::setG( double g ) {
  g_ = std::max( 0.0, std::min( G_MAX, g ) );
  convertRGBtoHSV( r_, g_, b_, &h_, &s_, &v_ );
}

void Color::setB( double b ) {
  b_ = std::max( 0.0, std::min( B_MAX, b ) );
  convertRGBtoHSV( r_, g_, b_, &h_, &s_, &v_ );
}

double Color::getR() const {
  return r_;
}

double Color::getG() const {
  return g_;
}

double Color::getB() const {
  return b_;
}

void Color::setHSV( double h, double s, double v ) {
  h_ = std::max( 0.0, std::min( H_MAX, h) );
  s_ = std::max( 0.0, std::min( S_MAX, s) );
  v_ = std::max( 0.0, std::min( V_MAX, v) );
  convertHSVtoRGB( h, s, v, &r_, &g_, &b_ );
}

void Color::getHSV( double *h, double *s, double *v ) const {
  *h = h_;
  *s = s_;
  *v = v_;
}

void Color::setH( double h) {
  h_ = std::max( 0.0, std::min( H_MAX, h) );
  convertHSVtoRGB( h_, s_, v_, &r_, &g_, &b_ );
}

void Color::setS( double s) {
  s_ = std::max( 0.0, std::min( S_MAX, s) );
  convertHSVtoRGB( h_, s_, v_, &r_, &g_, &b_ );
}

void Color::setV( double v) {
  v_ = std::max( 0.0, std::min( V_MAX, v) );
  convertHSVtoRGB( h_, s_, v_, &r_, &g_, &b_ );
}

double Color::getH() const {
  return h_;
}

double Color::getS() const {
  return s_;
}

double Color::getV() const {
  return v_;
}

void Color::setAlpha( double a ) {
  a_ = std::max( 0.0, std::min( ALPHA_MAX, a) );
}

double Color::getAlpha() const {
  return a_;
}

Color& Color::operator=( const Color& rhl ) {
  rhl.getRGB( &r_, &g_, &b_ );
  rhl.getHSV( &h_, &s_, &v_ );
  a_ = rhl.getAlpha();
  return *this;
}

void Color::convertRGBtoHSV( double r, double g, double b, double *h, double *s, double *v ) {
  double rgb_max;
  double rgb_min;

  double EPS = 10e-10;

  if ( ( r > g ) && ( r > b ) ) {
    rgb_max = r;
    rgb_min = std::min( g, b );
    if ( rgb_max - rgb_min < EPS ) {
      *h = 0;
    } else {
      *h = 60 * (g - b)/(rgb_max - rgb_min);
    }
  } else if ( ( g > r ) && ( g > b ) ) {
    rgb_max = g;
    rgb_min = std::min( r, b );
    if ( rgb_max - rgb_min < EPS ) {
      *h = 0;
    } else {
      *h = 60 * (b - r)/(rgb_max - rgb_min) + 120;
    }
  } else {
    rgb_max = b;
    rgb_min = std::min( r, g );
    if ( rgb_max - rgb_min < EPS ) {
      *h = 0;
    } else {
      *h = 60 * (r - g)/(rgb_max - rgb_min) + 240;
    }
  }

  while ( *h < 0 ) *h += 360;
  while ( *h > 360 ) *h -= 360;
  
  *s = (rgb_max - rgb_min) / rgb_max;
  *v = rgb_max;
}

void Color::convertHSVtoRGB( double h, double s, double v, double *r, double *g, double *b ) {
  double hi = std::floor(h/60);
  double f = h/60 - hi;
  double p = v * (1 - s);
  double q = v * (1 - f * s);
  double t = v * (1 - ( 1 - f ) * s);
  
  if ( hi == 0 ) {
    *r = v;
    *g = t;
    *b = p;
  } else if ( hi == 1 ) {
    *r = q;
    *g = v;
    *b = p;
  } else if ( hi == 2 ) {
    *r = p;
    *g = v;
    *b = t;
  } else if ( hi == 3 ) {
    *r = p;
    *g = q;
    *b = v;
  } else if ( hi == 4 ) {
    *r = t;
    *g = p;
    *b = v;
  } else {
    *r = v;
    *g = p;
    *b = q;
  }
}

}
