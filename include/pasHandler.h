#include "Arduino.h"
#include "systemStates.h"

void pasInit(void);
void InputCapture_IT_callback(HardwareTimer*);
void Rollover_IT_callback(HardwareTimer*);
void setup_inputcapture(void);
void setup_vescUpdateTimer(void);
void vescUpdateCallback(HardwareTimer*);
