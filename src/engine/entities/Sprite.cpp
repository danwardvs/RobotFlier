#include "Sprite.h"

#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "../../constants/globals.h"
#include "../scene/Scene.h"

// Constructor
Sprite::Sprite(Scene& scene, const float x, const float y, const int z)
    : GameObject(scene, x, y, z), visible(true) {}

// Constructor
Sprite::Sprite(Scene& scene,
               const std::string texture,
               const float x,
               const float y,
               const int z)
    : Sprite(scene, x, y, z) {
  setTexture(texture);
}

// Destructor
Sprite::~Sprite() {}

void Sprite::setVisible(const bool visible) {
  this->visible = visible;
}

void Sprite::setTexture(const std::string& texture) {
  this->texture = scene.getAsset().getImage(texture);
  this->width = this->texture.getWidth();
  this->height = this->texture.getHeight();
}

// Draw
void Sprite::draw() {
  // Don't show if not visible
  if (!visible) {
    return;
  }

  // Draw image
  texture.drawScaled(x, y, width, height);

  // Draw particles
  if (scene.getSettings().get<int>("particleType", 0) != 3) {
    for (unsigned int i = 0; i < parts.size(); i++) {
      parts.at(i).draw();
    }
  }

  // Draw bounding box
  if (scene.getSettings().get<bool>("debug", false)) {
    al_draw_rectangle(x, y, x + width, y + height, al_map_rgb(88, 88, 88), 1);
  }
}
