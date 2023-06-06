#ifndef BOTON_H
#define BOTON_H

#include <Arduino.h>

struct ButtonConfiguration
{
  int buttonPin;
  int pullupDelay;
};

class ButtonPress
{
public:
  ButtonPress(const ButtonConfiguration& config);
  void setup();
  bool loop();

private:
  ButtonConfiguration _config;
  unsigned long _buttonPressStartTime;
};

#endif  // BOTON_H
