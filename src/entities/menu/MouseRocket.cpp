#include "MouseRocket.h"

#include <afk/color/Color.h>
#include <afk/entities/ParticleEmitter.h>
#include <afk/random/RandomGenerator.h>
#include <afk/services/Services.h>

MouseRocket::MouseRocket(afk::Scene& scene) : Sprite(scene, "mouse", 0, 0, 20) {
  mouse_rocket_up = false;

  mouse_y = scene.input.mouseY();

  // Add particle emitter
  afk::ParticleEmitter& emitter =
      scene.addObj<afk::ParticleEmitter>(scene, 0, 0, 0);
  emitter_id = emitter.getId();

  // Create "realistic" particles
  for (int i = 0; i < 100; i++) {
    afk::Particle particle(scene, 0, 0, 0, afk::ParticleType::SQUARE);
    particle.setSize(2);

    if (scene.config.get<bool>("christmas", false)) {
      int red_or_green = afk::Random::randomInt(0, 1) * 255;
      particle.setColor(afk::color::rgb(red_or_green, 255 - red_or_green, 0));
    } else {
      particle.setColor(afk::color::rgb(255, 255, 0),
                        afk::color::rgb(128, 0, 0));
    }

    particle.setVelocity(afk::Random::randomFloat(-5.0, 5.0), 100.0);
    particle.setLifespan(afk::Random::randomInt(30, 80));
    emitter.addParticle(particle);
  }
}

void MouseRocket::update(Uint32 delta) {
  // Set position of sprite
  setPosition(scene.input.mouseX() - width / 2, scene.input.mouseY());

  scene.get<afk::ParticleEmitter>(emitter_id)
      .setPosition(scene.input.mouseX(), scene.input.mouseY() + height * 0.8);

  // Just went up, flip textures
  if (y < mouse_y && !mouse_rocket_up) {
    setTexture("mouse_rocket");
    mouse_rocket_up = true;
  } else if (y >= mouse_y && mouse_rocket_up) {
    setTexture("mouse");
    mouse_rocket_up = false;
  }

  // Set old y position
  mouse_y = y;
}
