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

class CoordinateAdjuster;
class ColorMap;

class ParameterTable;

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

  CoordinateAdjuster* getCoordinateAdjuster();
  void setCoordinateAdjuster( CoordinateAdjuster* adjuster, bool is_delete );

  ColorMap* getColorMap();
  void setColorMap( ColorMap* colormap, bool is_delete );

  ParameterTable* getParameterTable();
  
  ~Components();
  
private:
  VolumeRenderer* renderer_;
  controller::Controller* controller_;
  FileLoader* loader_;
  volumeGenerator::IVolumeGenerator* generator_;
  CoordinateAdjuster* adjuster_;
  ColorMap *colormap_;
  ParameterTable *params_;
};

}

#endif
