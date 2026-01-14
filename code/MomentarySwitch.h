#ifndef MomentarySwitch_h
#define MomentarySwitch_h

#include "Arduino.h"

class MomentarySwitch {
  public:
  	MomentarySwitch(int readPin, int debounceDelay = 50);
    void begin();
  	bool update();
  private:
  	bool state;
    int readPin;
  	int debounceDelay;
  	unsigned long lastAction;
};


#endif