#include "Components.hpp"

#include "VolumeRenderer.hpp"
#include "Controller.hpp"
#include "FileLoader.hpp"
#include "IVolumeGenerator.hpp"

namespace vrplot {

Components::Components() 
  : renderer_(NULL), controller_(NULL), loader_(NULL), generator_(NULL)
{
  setRenderer( new vrplot::VolumeRenderer( ), true);
  getRenderer()->loadShaderSource( "simple.vert", "simple.frag" );

  setFileLoader( new vrplot::FileLoader(), true);
  
  setController( new vrplot::controller::Controller( this ), true);
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

Components::~Components() {
  delete renderer_;
  delete controller_;
  delete loader_;
  delete generator_;
}


}
