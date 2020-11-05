#include "Font.h"

#include "../common/Tools.h"

// Ctor
Font::Font() : font(nullptr), font_size(0) {}

Font::Font(const std::string& path, const int size)
    : font(nullptr), font_size(size) {
  load(path, size);
}

// Dtor
Font::~Font() {
  if (font) {
    // al_destroy_font(font);
  }
}

/**
 * Load Font from file
 * @param path, path to file
 */
void Font::load(const std::string& path, const int size) {
  font = loadFont(path, size);
}

/**
 * Return line height for given text
 */
int Font::getHeight() {
  if (!font) {
    return 0;
  }

  return al_get_font_line_height(font);
}

/**
 * Get Size of font
 */
int Font::getSize() {
  return font_size;
}

/**
 * Return width given text
 */
int Font::getWidth(const std::string& text) {
  if (!font) {
    return 0;
  }

  return al_get_text_width(font, text.c_str());
}

/**
 * Draw Font to screen
 * @param x, x position to draw to
 * @param y, y position to draw to
 * @param flags, config flags passed to al_draw_bitmap
 * @param text, text to draw
 */
void Font::draw(const int x,
                const int y,
                const std::string& text,
                const ALLEGRO_COLOR colour,
                const int flags) {
  if (!font) {
    return;
  }

  al_draw_text(font, colour, x, y, flags, text.c_str());
}

/**
 * Load font from file
 * @param file, path to image
 */
ALLEGRO_FONT* Font::loadFont(const std::string& file, const int size) {
  // Attempt to load
  ALLEGRO_FONT* temp_font = al_load_font(file.c_str(), size, 0);

  if (!temp_font) {
    tools::abort_on_error(
        "There was an error loading " + file + "... \nSorry...",
        "Loading Error");
  }

  return temp_font;
}