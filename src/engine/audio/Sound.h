/**
 * Sound
 * Danny Van Stemp
 * Wrapper for allegro sample
 * Easy interface to interact with samples
 * 23/11/2018
 **/

#ifndef ENGINE_AUDIO_SOUND_H
#define ENGINE_AUDIO_SOUND_H

#include <allegro5/allegro_audio.h>
#include <string>

struct PlaySoundConfig {
  const float gain = 1.0f;
  const float pan = 0.0f;
  const float speed = 1.0f;
  const bool loop = false;
};

const PlaySoundConfig defaultConfig;

class Sound {
 public:
  Sound();
  explicit Sound(const std::string& path);

  virtual ~Sound();

  void play(const PlaySoundConfig& config = defaultConfig);

  void load(const std::string& path);

  static void setGlobalGain(const float gain);

 private:
  static ALLEGRO_SAMPLE* loadSample(const std::string& file);
  static bool globalGain;

  ALLEGRO_SAMPLE* sample;
};

#endif  // ENGINE_AUDIO_SOUND_H