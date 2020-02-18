#include "main.h"


extern MCUFRIEND_kbv tft;


uint8_t system_state = 0;

void setup(void);
void loop(void);

//functions related to HW setup:
void lcd_on(void);
void lcd_off(void);
void lcd_setbacklight(uint8_t backlight);
void gpioInit(void);


void setup(void) {

}

void loop(void) {
    switch (system_state)
    {
    case 0:
        gpioInit();
        lcd_on();
        drawWelcome();
        lcd_setbacklight(100);
        system_state = 1;
        break;
    case 1:
        if (numPadHandler() == 0xFF){
            system_state = 2;
            }       
        break;
    case 2:
        drawUI();
        break;
    
    default:
        break;
    }
    


}

void gpioInit(void){
    pinMode(PB4, OUTPUT);
    pinMode(PB14, OUTPUT);
}
void lcd_on(void){
    digitalWrite(PB4, LOW);
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(1);
}
void lcd_setbacklight(uint8_t backlight){
    if(backlight == 100){
        digitalWrite(PB14, HIGH);
    }
}
void lcd_off(void){
    digitalWrite(PB4, LOW);
}
