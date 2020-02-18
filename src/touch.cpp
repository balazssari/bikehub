#include "Arduino.h"
#include "touch.h"

uint16_t * readTouch(uint8_t threshold){
    static uint16_t axes2d[2][2];
    static uint16_t adc[2] = {0};
    for(int i = 0; i < 2; i++){
        pinMode(PA7,OUTPUT);
        pinMode(PA2,OUTPUT);
        digitalWrite(PA7, LOW);
        digitalWrite(PA2, HIGH);
        pinMode(PA3, INPUT);
        pinMode(PB0, INPUT);
        adc[0] = analogRead(PB0);
        pinMode(PA3,OUTPUT);
        pinMode(PB0,OUTPUT);
        digitalWrite(PA3, LOW);
        digitalWrite(PB0, HIGH);
        pinMode(PA7, INPUT);
        pinMode(PA2, INPUT);
        adc[1] = analogRead(PA2);
        axes2d[i][0] = adc[0];
        axes2d[i][1] = adc[1];
    }
    pinMode(PA7,OUTPUT);
    pinMode(PA2,OUTPUT);
    pinMode(PA3,OUTPUT);
    pinMode(PB0,OUTPUT);
    if( (abs(axes2d[0][0]-axes2d[1][0]) > threshold) || (abs(axes2d[0][1]-axes2d[1][1]) > threshold) ){
        adc[0] = 1023;
        adc[1] = 1023;
    }
    return adc;
}