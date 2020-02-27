#include <Arduino.h>
#include "pasHandler.h"
#include "lcdgfx.h"

#define PAS_SENSOR_HAL_PIN      PB2
#define PAS_SENSOR_TORQUE_PIN   PA4
#define PAS_TIMEOUT             1000
#define PAS_MAGNETS             8


extern MCUFRIEND_kbv tft;


extern uint8_t system_state;
uint16_t pasCounter = 0;
uint32_t input_capture = 0;
uint32_t prev_input_capture = 0;
uint32_t capture_interval = 0;
float CrankRPM = 0;
float prevCrankRPM;

void pasInit(void){
    pinMode(PA4, INPUT);
    pinMode(PB3, INPUT_PULLUP);
    attachInterrupt(PB3,pasCallback,FALLING);


}
void pasHandler(void){
    if((input_capture + PAS_TIMEOUT) < millis()){
        //not cranking anymore
        CrankRPM = 0;
    }
}
void pasCallback(void){
    pasCounter++;
    input_capture = millis();
    capture_interval = input_capture - prev_input_capture;
    prev_input_capture = input_capture;
    CrankRPM = 60 / ((capture_interval * PAS_MAGNETS) * 0.001);

    

}


