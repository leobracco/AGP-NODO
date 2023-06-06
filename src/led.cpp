#include "Arduino.h"
#include "led.h"
Led::Led(int pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  _ledState = LOW;
  _previousMillis = 0;
  _interval = 500; // Intervalo de titilado en milisegundos
}

void Led::update()
{
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _interval)
  {
    _previousMillis = currentMillis;
    _ledState = !_ledState;
    digitalWrite(_pin, _ledState);
  }
}

void Led::on()
{
  digitalWrite(_pin, LOW);
}

void Led::off()
{
  digitalWrite(_pin, HIGH);
}