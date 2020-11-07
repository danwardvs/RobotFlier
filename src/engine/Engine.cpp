#include "Engine.h"

// Allegro includes
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

// Scenes
#include "../scenes/Game.h"
#include "../scenes/Init.h"
#include "../scenes/Intro.h"
#include "../scenes/Menu.h"

// Locator provisions
#include "Locator.h"
#include "assets/AssetManager.h"
#include "audio/DefaultAudioService.h"
#include "common/Exceptions.h"
#include "input/Input.h"
#include "logging/DebugLogger.h"
#include "settings/SettingManager.h"

// Update ticks per second
const float UPDATES_PER_SECOND = 60;

// Exit helper
void showDialog(const std::string& title, const std::string& message = "") {
  al_show_native_message_box(nullptr, title.c_str(), title.c_str(),
                             message.c_str(), nullptr,
                             ALLEGRO_MESSAGEBOX_ERROR);
}

// Setup engine
Engine::Engine()
    : event_queue(nullptr),
      timer(nullptr),
      closing(false),
      current_scene(nullptr) {}

// Start your engine!
void Engine::start(const ProgramScene scene_id) {
  try {
    // Set the current scene ID
    Scene::nextScene = scene_id;

    // Setup game
    setup();

    // Loop
    while (!closing) {
      update();
    }
  } catch (FileIOException& e) {
    showDialog("File Error", e.what());
  } catch (std::runtime_error& e) {
    showDialog("Runtime Error", e.what());
  } catch (...) {
    showDialog("Unknown Error", "An unknown error has occured :(");
  }
}

// Change game screen
void Engine::changeScene() {
  // If the scene needs not to be changed
  if (Scene::nextScene == SCENE_NULL) {
    return;
  }

  // Delete the current scene
  if (Scene::nextScene != SCENE_EXIT) {
    delete current_scene;
  }

  // Change the scene
  switch (Scene::nextScene) {
    case SCENE_INIT:
      current_scene = new Init();
      break;

    case SCENE_INTRO:
      current_scene = new Intro();
      break;

    case SCENE_MENU:
      current_scene = new Menu();
      break;

    case SCENE_GAME:
      current_scene = new Game();
      break;

    case SCENE_EXIT:
    default:
      closing = true;
      break;
  }

  // Change the current scene ID
  Scene::sceneId = Scene::nextScene;

  // NULL the next scene ID
  Scene::nextScene = SCENE_NULL;
}

// Sets up game
void Engine::setup() {
  // Init allegro 5
  if (!al_init()) {
    throw InitException("Could not allegro");
  }

  // Setup window
  Locator::provideWindow<Window>();
  Locator::getWindow().setWindowSize(1600, 1200);
  Locator::getWindow().setBufferSize(800, 600);
  Locator::getWindow().setMode(DISPLAY_MODE::WINDOWED);
  Locator::getWindow().setTitle("Loading");

  // Input
  if (!al_install_keyboard()) {
    throw InitException("Could not init keyboard");
  }
  if (!al_install_mouse()) {
    throw InitException("Could not init mouse");
  }
  if (!al_install_joystick()) {
    throw InitException("Could not init joystick");
  }

  // Fonts
  if (!al_init_font_addon()) {
    throw InitException("Could not init font addon");
  }
  if (!al_init_ttf_addon()) {
    throw InitException("Could not init ttf addon");
  }

  // Graphics
  if (!al_init_image_addon()) {
    throw InitException("Could not init image addon");
  }
  if (!al_init_primitives_addon()) {
    throw InitException("Could not init primitives addon");
  }

  // Audio
  if (!al_install_audio()) {
    throw InitException("Could not init audio addon");
  }
  if (!al_init_acodec_addon()) {
    throw InitException("Could not init acodec addon");
  }
  al_reserve_samples(20);

  // Events
  event_queue = al_create_event_queue();

  Locator::getWindow().registerEventSource(event_queue);
  timer = al_create_timer(1.0 / UPDATES_PER_SECOND);
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_start_timer(timer);

  // Window title
  Locator::getWindow().setTitle("Robot Flier");
  Locator::getWindow().setIcon("images/icon.png");

  // Setup service locator
  Locator::provideAudio<DefaultAudioService>();

  // Setup asset manager
  Locator::provideAssetManager<AssetManager>();

  // Setup setting manager
  Locator::provideSettings<SettingManager>();

  // Setup logger
  Locator::provideLogger<DebugLogger>();

  // Setup input
  Locator::provideInput<Input>();
  Locator::getInput().registerEvents(event_queue);
}

// Universal update
void Engine::update() {
  // Event checking
  ALLEGRO_EVENT ev;
  al_wait_for_event(event_queue, &ev);

  // Update timer
  if (ev.type == ALLEGRO_EVENT_TIMER) {
    // Change scene (if needed)
    changeScene();

    // Update scene
    if (current_scene) {
      current_scene->update();
      current_scene->updateInternal();
    }
  }
  // Exit
  else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
    closing = true;
  }
  // Scaling
  else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
    Locator::getWindow().resize(ev.display.width, ev.display.height);
  }

  // Process input
  Locator::getInput().processEvent(ev);

  // Queue empty? Lets draw
  if (al_is_event_queue_empty(event_queue)) {
    if (current_scene) {
      Locator::getWindow().draw(current_scene);
    }
  }
}