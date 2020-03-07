#include <Arduino.h>
#include "pasHandler.h"
#include "lcdgfx.h"
#include <VescUart.h>

#define TRUE                    1
#define FALSE                   0

#define PAS_SENSOR_HAL_PIN      PB2
#define PAS_SENSOR_TORQUE_PIN   PA4
#define PAS_TIMEOUT             1000
#define PAS_MAGNETS             8


extern MCUFRIEND_kbv tft;
extern VescUart UART;

extern uint8_t system_state;
uint16_t pasCounter = 0;
uint32_t input_capture = 0;
uint32_t prev_input_capture = 0;
uint32_t capture_interval = 0;
float CrankRPM = 0;
float currentToVESC = 0;
bool isInputcaptureOld = TRUE;

void pasInit(void){
    pinMode(PA4, INPUT);
    pinMode(PB3, INPUT_PULLUP);
    attachInterrupt(PB3,pasCallback,FALLING);


}
void pasHandler(void){
    if((input_capture + PAS_TIMEOUT) < millis()){
        //not cranking anymore
        CrankRPM = 0;
        isInputcaptureOld = TRUE;
        UART.setCurrent(0.0);
    }
}
void pasCallback(void){
    if(system_state == MAIN){
        pasCounter++;
        if (isInputcaptureOld == FALSE){
            input_capture = millis();
            capture_interval = input_capture - prev_input_capture;
        }
        else if (isInputcaptureOld == TRUE){
            input_capture = millis();
            isInputcaptureOld = FALSE;
        }
        prev_input_capture = input_capture;
        CrankRPM = 60 / ((capture_interval * PAS_MAGNETS) * 0.001);
        if (CrankRPM == (float)infinity()) CrankRPM = 0;
        currentToVESC = CrankRPM / 250;
        if (currentToVESC > 30) currentToVESC = 30;
        if (currentToVESC == (float)infinity()) currentToVESC = 0;
        UART.setCurrent(currentToVESC);
    }
}


