#ifndef IndicatorLED_h
#define IndicatorLED_h

#include "Arduino.h"

class IndicatorLED {
  public:
  	IndicatorLED(int ledPin, int toggleDuration);
    void begin();
  	void activate();
  	void deactivate();
  	void update();
  private:
  	bool state;
  	int ledPin;
  	int toggleDuration;
  	unsigned long lastAction;
};

#endif