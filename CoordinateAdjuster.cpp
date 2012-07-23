#include "CoordinateAdjuster.hpp"

#include <iostream>

namespace vrplot {

CoordinateAdjuster::CoordinateAdjuster() : mask_(ALL) {
}

double CoordinateAdjuster::x( double in ) const {
  return cx_ * in + ox_;
}

double CoordinateAdjuster::y( double in ) const {
  return cy_ * in + oy_;
}

double CoordinateAdjuster::z( double in ) const {
  return cz_ * in + oz_;
}

void CoordinateAdjuster::setRangeX( double max, double min ) {
  setX( max, min );
  cx_ = 1 / ( maxx_ - minx_ );
  ox_ = - minx_ / ( maxx_ - minx_ );
}

void CoordinateAdjuster::setRangeY( double max, double min ) {
  setY( max, min );
  cy_ = 1 / ( maxy_ - miny_ );
  oy_ = - miny_ / ( maxy_ - miny_ );
}

void CoordinateAdjuster::setRangeZ( double max, double min ) {
  setZ( max, min );
  cz_ = 1 / ( maxz_ - minz_ );
  oz_ = - minz_ / ( maxz_ - minz_ );
}

void CoordinateAdjuster::getRangeX( double *max, double *min ) {
  *max = maxx_;
  *min = minx_;
}

void CoordinateAdjuster::getRangeY( double *max, double *min ) {
  *max = maxy_;
  *min = miny_;
}

void CoordinateAdjuster::getRnageZ( double *max, double *min ) {
  *max = maxz_;
  *min = minz_;
}

void CoordinateAdjuster::setX( double max, double min ) {
  if ( mask_ & MIN_X ) minx_ = min;
  if ( mask_ & MAX_X ) maxx_ = max;
}

void CoordinateAdjuster::setY( double max, double min ) {
  if ( mask_ & MIN_Y ) miny_ = min;
  if ( mask_ & MAX_Y ) maxy_ = max;
}

void CoordinateAdjuster::setZ( double max, double min ) {
  if ( mask_ & MIN_Z ) minz_ = min;
  if ( mask_ & MAX_Z ) maxz_ = max;
}

void CoordinateAdjuster::setMask( unsigned int mask ) {
  mask_ = mask;
}

}
