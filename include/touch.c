#include <stdint.h>
#include <Arduino.h>
#include <touch.h>


uint16_t * readTouch(){
    //Resistive touch  |X- |X+ |Y- |Y+ |
    //STM32 pin        |PA7|PA2|PA3|PB0|
    static uint16_t xyaxis[2];
    pinMode(PA7,OUTPUT);
    pinMode(PA2,OUTPUT);
    digitalWrite(PA7, LOW);
    digitalWrite(PA2, HIGH);
    pinMode(PA3, INPUT);
    pinMode(PB0, INPUT);
    // uint16_t temp[10] = {0};
    // for(int i = 0; i < 10; i++){
    //     temp[i] = analogRead(PB0);
    // }
    xyaxis[0] = analogRead(PB0);
    pinMode(PA3,OUTPUT);
    pinMode(PB0,OUTPUT);
    digitalWrite(PA3, LOW);
    digitalWrite(PB0, HIGH);
    pinMode(PA7, INPUT);
    pinMode(PA2, INPUT);
    // for(int i = 0; i < 10; i++){
    //     temp[i] = analogRead(PA2);
    // }
    xyaxis[1] = analogRead(PA2);
    pinMode(PA7,OUTPUT);
    pinMode(PA2,OUTPUT);
    pinMode(PA3,OUTPUT);
    pinMode(PB0,OUTPUT);
    return xyaxis;
}
uint16_t bubbleFilter(uint16_t a[], uint16_t size){
    uint16_t i;
    uint16_t j;
    uint16_t tmp;
    for (i=size-1; 0<i; --i) {
        for (j=0; j < i; ++j) {
            if (a[j]>a[j+1]) {
                tmp=a[j];
                a[j]=a[j+1];
                a[j+1]=tmp;
            }
        }
    }
    uint64_t sum = 0;
    for(i=size-3; 2<i; --i){
        sum += a[i];
    }
    return sum/(size-3);
}   