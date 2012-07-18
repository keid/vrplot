#ifndef VRPLOT_COMPONENTS_HPP
#define VRPLOT_COMPONENTS_HPP

namespace vrplot {

class VolumeRenderer;

namespace controller {
class Controller;
}

class FileLoader;

namespace volumeGenerator {
class IVolumeGenerator;
}

class Components {

public:
  Components();

  VolumeRenderer* getRenderer();
  void setRenderer( VolumeRenderer* renderer, bool is_delete );

  controller::Controller* getController();
  void setController( controller::Controller* controller, bool is_delete );

  FileLoader* getFileLoader();
  void setFileLoader( FileLoader* loader, bool is_delete);

  volumeGenerator::IVolumeGenerator* getVolumeGenerator();
  void setVolumeGenerator( volumeGenerator::IVolumeGenerator* generator, bool is_delete );

  ~Components();
  
private:
  VolumeRenderer* renderer_;
  controller::Controller* controller_;
  FileLoader* loader_;
  volumeGenerator::IVolumeGenerator* generator_;
};

}

#endif
