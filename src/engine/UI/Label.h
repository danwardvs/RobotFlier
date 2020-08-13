/**
 * Label
 * Danny Van Stemp and Allan Legemaate
 * UI Button
 * 25/11/2018
 **/

#ifndef ENGINE_UI_LABEL_H
#define ENGINE_UI_LABEL_H

#include "UIElement.h"

class Label : public UIElement {
 public:
  // Constructor
  Label();

  // Detailed constructor
  Label(const int x,
        const int y,
        std::string text,
        std::string id,
        ALLEGRO_FONT* font);

  // Dtor
  inline virtual ~Label(){};

  // Draw
  virtual void draw() override;

  // Update
  virtual void update() override;
};

#endif  // ENGINE_UI_LABEL_H