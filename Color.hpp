#ifndef VRPLOT_COLOR_HPP
#define VRPLOT_COLOR_HPP

namespace vrplot {

class Color {

public:
  static const double R_MAX;
  static const double G_MAX;
  static const double B_MAX;
  static const double H_MAX;
  static const double S_MAX;
  static const double V_MAX;
  static const double ALPHA_MAX;

  static const unsigned int MODE_RGB;
  static const unsigned int MODE_HSV;

  Color();

  Color( unsigned int mode, double v0, double v1, double v2, double alpha);
  
  void setRGB( double r, double g, double b );
  void getRGB( double *r, double *g, double *b ) const;

  void setR( double r );
  void setG( double g );
  void setB( double b );

  double getR() const;
  double getG() const;
  double getB() const;
  
  void setHSV( double h, double s, double v );
  void getHSV( double *h, double *s, double *v ) const;

  void setH( double h);
  void setS( double s);
  void setV( double v);

  double getH() const;
  double getS() const;
  double getV() const;

  void setAlpha( double a );
  double getAlpha() const;

  static void convertRGBtoHSV( double r, double g, double b, double *h, double *s, double *v ) ;
  static void convertHSVtoRGB( double h, double s, double v, double *r, double *g, double *b ) ;
  
  Color& operator=( const Color& rhl );

private:
  double r_;
  double g_;
  double b_;
  
  double h_;
  double s_;
  double v_;

  double a_;

};

}

#endif
