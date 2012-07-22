#ifndef COORDINATEADJUSTER_HPP
#define COORDINATEADJUSTER_HPP

namespace vrplot {

class CoordinateAdjuster {
  
public:
  double x( double in ) const;
  double y( double in ) const;
  double z( double in ) const;

  void setRangeX( double max, double min );
  void setRangeY( double max, double min );
  void setRangeZ( double max, double min );

  void getRangeX( double *max, double *min );
  void getRangeY( double *max, double *min );
  void getRnageZ( double *max, double *min );

private:
  // coeffients
  double cx_;
  double cy_;
  double cz_;

  // offsets
  double ox_;
  double oy_;
  double oz_;

  double minx_;
  double miny_;
  double minz_;

  double maxx_;
  double maxy_;
  double maxz_;

  void setX( double max, double min );
  void setY( double max, double min );
  void setZ( double max, double min );
  
};

}

#endif
