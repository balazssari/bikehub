#include <Arduino.h>
#include "pasHandler.h"
#include "lcdgfx.h"
#include <VescUart.h>

#define TRUE                    1
#define FALSE                   0

#define PAS_SENSOR_HAL_PIN      PB3
#define PAS_SENSOR_TORQUE_PIN   PA4
#define PAS_TIMEOUT             1000
#define PAS_MAGNETS             8

#define pin                     PB3

uint32_t channel;
volatile uint32_t FrequencyMeasured, LastCapture = 0, CurrentCapture, PeriodMeasured;
uint32_t input_freq = 0;
volatile uint32_t rolloverCompareCount = 0;
HardwareTimer *PasTimer;
HardwareTimer *vescUpdateTimer;

extern MCUFRIEND_kbv tft;
extern VescUart UART;
float currentDecreaseRate = 2;
extern uint8_t system_state;
uint16_t pasCounter = 0;
uint32_t input_capture = 0;
uint32_t prev_input_capture = 0;
uint32_t capture_interval = 0;
float CrankRPM = 0;
float currentToVESC = 0;
bool isInputcaptureOld = TRUE;
bool newValueAvailable = FALSE;
uint8_t pedalStopCtr = 0;



void pasInit(void){
    pinMode(PA4, INPUT);
    pinMode(PAS_SENSOR_HAL_PIN, INPUT_PULLUP);
    setup_inputcapture();
    setup_vescUpdateTimer();
    //attachInterrupt(PB3,pasCallback,FALLING);
}
void setup_vescUpdateTimer(void){
    TIM_TypeDef *Instance = TIM1;
    vescUpdateTimer = new HardwareTimer(Instance);
    vescUpdateTimer->setOverflow(20, HERTZ_FORMAT); // 10 Hz
    vescUpdateTimer->attachInterrupt(vescUpdateCallback);
    vescUpdateTimer->resume();
}
void vescUpdateCallback(HardwareTimer*){
    if ( newValueAvailable == TRUE ){
      newValueAvailable = FALSE;
      pedalStopCtr = 0;
      CrankRPM = 60.0 / ((PeriodMeasured * PAS_MAGNETS) * 0.0001);
      currentToVESC = CrankRPM / 10;
      //Send Current to VESC
      UART.setCurrent(currentToVESC);
    }
    else if ( newValueAvailable == FALSE ) {
      pedalStopCtr++;
      if (pedalStopCtr > 10){
        if(currentToVESC  > currentDecreaseRate){
            currentToVESC = currentToVESC - currentDecreaseRate;
            UART.setCurrent(currentToVESC);
          }
        if (currentToVESC <= currentDecreaseRate){
            currentToVESC = 0.0;
            UART.setCurrent(currentToVESC);
            }
        }
    }
}
void setup_inputcapture(void)
{
  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PAS_SENSOR_HAL_PIN), PinMap_PWM);
  channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PAS_SENSOR_HAL_PIN), PinMap_PWM));

  // Instantiate HardwareTimer object. Thanks to 'new' instantiation, HardwareTimer is not destructed when setup() function is finished.
  PasTimer = new HardwareTimer(Instance);

  // Configure rising edge detection to measure frequency
  PasTimer->setMode(channel, TIMER_INPUT_CAPTURE_FALLING, PAS_SENSOR_HAL_PIN);

  // With a PrescalerFactor = 1, the minimum frequency value to measure is : TIM counter clock / CCR MAX
  //  = (SystemCoreClock) / 65535
  // Example on Nucleo_L476RG with systemClock at 80MHz, the minimum frequency is around 1,2 khz
  // To reduce minimum frequency, it is possible to increase prescaler. But this is at a cost of precision.
  // The maximum frequency depends on processing of the interruption and thus depend on board used
  // Example on Nucleo_L476RG with systemClock at 80MHz the interruption processing is around 4,5 microseconds and thus Max frequency is around 220kHz
  uint32_t PrescalerFactor = 7200-1;
  PasTimer->setPrescaleFactor(PrescalerFactor);
  PasTimer->setOverflow(0x10000-1); // Max Period value to have the largest possible time to detect rising edge and avoid timer rollover
  PasTimer->attachInterrupt(channel, InputCapture_IT_callback);
  PasTimer->attachInterrupt(Rollover_IT_callback);
  PasTimer->resume();

  // Compute this scale factor only once
  input_freq = PasTimer->getTimerClkFreq() / PasTimer->getPrescaleFactor();
  HAL_Delay(1);
}
void InputCapture_IT_callback(HardwareTimer*)
{
  CurrentCapture = PasTimer->getCaptureCompare(channel);
  /* frequency computation */
  if (CurrentCapture > LastCapture) {
    FrequencyMeasured = input_freq / (CurrentCapture - LastCapture);
    PeriodMeasured = (CurrentCapture - LastCapture);
  }
  else if (CurrentCapture <= LastCapture) {
    /* 0x1000 is max overflow value */
    FrequencyMeasured = input_freq / ((0x10000-1) + CurrentCapture - LastCapture);
    PeriodMeasured = ((0x10000-1) + CurrentCapture - LastCapture);
  }
  newValueAvailable = TRUE;
  LastCapture = CurrentCapture;
  rolloverCompareCount = 0;
}

void Rollover_IT_callback(HardwareTimer*)
{
  rolloverCompareCount++;

  if (rolloverCompareCount > 1)
  {
    FrequencyMeasured = 0;
  }

}


