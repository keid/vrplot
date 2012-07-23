#include "Components.hpp"

#include "VolumeRenderer.hpp"
#include "Controller.hpp"
#include "FileLoader.hpp"
#include "IVolumeGenerator.hpp"
#include "CoordinateAdjuster.hpp"
#include "Color.hpp"
#include "ColorMap.hpp"
#include "Parameters.hpp"

namespace vrplot {

Components::Components() 
  : renderer_(NULL), controller_(NULL), loader_(NULL), generator_(NULL),
    adjuster_(NULL), colormap_(NULL), params_(NULL)
{
  setRenderer( new vrplot::VolumeRenderer( ), true);
  getRenderer()->loadShaderSource( "simple.vert", "simple.frag" );

  setFileLoader( new vrplot::FileLoader(), true);
  
  setController( new vrplot::controller::Controller( this ), true);

  setCoordinateAdjuster( new CoordinateAdjuster(), true );

  setColorMap( new ColorMap(), true );
  /*
    getColorMap()->setColor( 0.0, Color( Color::MODE_HSV, 300.0, 1.0, 1.0, 1.0) );
  getColorMap()->setColor( 0.5, Color( Color::MODE_HSV, 200.0, 1.0, 1.0, 1.0) );
  getColorMap()->setColor( 1.0, Color( Color::MODE_HSV, 0.0, 1.0, 1.0, 1.0) );
  */
  for ( int i=0; i<150; ++i ) {
    getColorMap()->setColor( i/150.0, Color( Color::MODE_HSV, i+210, 1.0, 1.0, 0.5 + i / 150) );
  }

  params_ = new Parameters();
  
}

VolumeRenderer* Components::getRenderer() {
  return renderer_;
}

void Components::setRenderer( VolumeRenderer* renderer, bool is_delete ) {
  if( renderer_ != renderer ) {
    if ( is_delete ) delete renderer_;
    renderer_ = renderer;
  }
}

controller::Controller* Components::getController() {
  return controller_;
}

void Components::setController( controller::Controller* controller, bool is_delete ) {
  if ( controller_ != controller ) {
    if ( is_delete ) delete controller_;
    controller_ = controller;
  }
}

FileLoader* Components::getFileLoader() {
  return loader_;
}

void Components::setFileLoader( FileLoader* loader, bool is_delete ) {
  if ( loader_ != loader ) {
    if ( is_delete ) delete loader_;
    loader_ = loader;
  }
}

volumeGenerator::IVolumeGenerator* Components::getVolumeGenerator() {
  return generator_;
}

void Components::setVolumeGenerator( volumeGenerator::IVolumeGenerator* generator, bool is_delete ) {
  if ( generator_ != generator ) {
    if ( is_delete ) delete generator_;
    generator_ = generator;
  }
}

CoordinateAdjuster* Components::getCoordinateAdjuster() {
  return adjuster_;
}

void Components::setCoordinateAdjuster( CoordinateAdjuster* adjuster, bool is_delete ) {
  if ( adjuster_ != adjuster ) {
    if ( is_delete ) delete adjuster_;
    adjuster_ = adjuster;
  }
}

ColorMap* Components::getColorMap() {
  return colormap_;
}

void Components::setColorMap( ColorMap* colormap, bool is_delete ) {
  if ( colormap_ != colormap ) {
    if ( is_delete ) delete colormap_;
    colormap_ = colormap;
  }
}

Components::~Components() {
  delete renderer_;
  delete controller_;
  delete loader_;
  delete generator_;
  delete adjuster_;
  delete colormap_;
  delete params_;
}


}
