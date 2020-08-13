/*
 * Comet
 * Allan Legemaate
 * Collidable Comet
 * 11/08/2020
 */
#ifndef ENTITIES_DEBRIS_COMET_H
#define ENTITIES_DEBRIS_COMET_H

#include "Debris.h"

class Comet : public Debris {
 public:
  // Constructor
  Comet(const int x, const int y);

  // Logic override
  virtual void onDestroy(Robot* robot) override;

 private:
  // Sound
  Sound destroy_sound;
};

#endif  // ENTITIES_DEBRIS_COMET_H