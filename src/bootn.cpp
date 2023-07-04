#include "boton.h"
#include "led.h"
ButtonPress::ButtonPress(const ButtonConfiguration& config)
{
  _config = config;
  _buttonPressStartTime = 0;
}

void ButtonPress::setup()
{
  pinMode(_config.buttonPin, INPUT_PULLUP);
  
}

bool ButtonPress::loop()
{
  int buttonState = digitalRead(_config.buttonPin);
 bool isLongPress = false; 
  if (buttonState == LOW)
  {
    if (_buttonPressStartTime == 0)
    {
      _buttonPressStartTime = millis();
    }
    else
    {
      unsigned long currentMillis = millis();
      unsigned long elapsedTime = currentMillis - _buttonPressStartTime;

      if (elapsedTime >= _config.pullupDelay)
      {
       
        // Acciones a realizar cuando se detecta la presión prolongada del botón durante 5 segundos
        // ...
        isLongPress =true;
       
      }
    }
  }
  else
  {
    _buttonPressStartTime = 0;
  }
    return isLongPress;
  // Resto de la lógica del bucle principal
}
