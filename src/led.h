#ifndef LED_H
#define LED_H

class Led
{
public:
  Led(int pin);
  void update();
  void on();
  void off();

private:
  int _pin;
  bool _ledState;
  unsigned long _previousMillis;
  unsigned long _interval;
};

#endif // LED_H
