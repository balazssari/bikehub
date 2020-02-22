#include "main.h"

#define debug
#define VAR_VIEWER_UPDATE_TIME 1000

extern MCUFRIEND_kbv tft;
HardwareSerial Serial3(USART3);
VescUart UART;

uint8_t system_state = INIT_SYS;


uint32_t elapsedTime = 0; 
uint32_t sysTime = 0;
uint32_t prevsysTime = 0;
uint32_t screenTime = 0;
uint32_t prevscreenTime = 0;

void setup(void);
void loop(void);

//functions related to HW setup:
void lcd_on(void);
void lcd_off(void);
void lcd_setbacklight(uint8_t backlight);
void gpioInit(void);
void drawVarviewer(void);
void varviewerHandler(void);

void setup(void) {
    pasInit();
    Serial3.begin(19200);
    UART.setSerialPort(&Serial3);
}

void loop(void) {
    sysTime = millis();
    pasHandler();
    switch (system_state)
    {
    case INIT_SYS:
        gpioInit();
        lcd_on();
        system_state = INIT_LOCKSCREEN;
        #ifdef debug
        system_state = INIT_VAR_VIEWER;
        #endif
        break;
    case INIT_LOCKSCREEN:
        drawWelcome();
        lcd_setbacklight(100);
        system_state = LOCKSCREEN;
        break;
    case LOCKSCREEN:
        if (numPadHandler() == 0xFF){
            system_state = MAIN;
            }       
        break;
    case MAIN:
        drawUI();
        break;
    case INIT_VAR_VIEWER:
        tft.fillScreen(LOCKPAGE_BGCOLOR);
        lcd_setbacklight(100);
        drawVarviewer();
        system_state = VAR_VIEWER;
        break;
    case VAR_VIEWER:
        varviewerHandler();
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

    digitalWrite(PB4, HIGH);
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
void drawVarviewer(void){
    tft.setFont(&FreeSerif18pt7b);
    tft.setTextSize(0);
    tft.setCursor(0, (VARV_CRANKRPM_ROW*22)+(VARV_CRANKRPM_ROW*2));
    tft.setTextColor(GREEN);
    tft.print("CrankRPM");
}
void varviewerHandler(void){
    screenTime = sysTime - prevscreenTime;
    if(screenTime > VAR_VIEWER_UPDATE_TIME){
        float readCrankRPM = CrankRPM;
        if(CrankRPM == prevCrankRPM){

        }
        else{
            tft.setFont(&FreeSerif18pt7b);
            tft.setTextSize(0);
            tft.setCursor(VARV_COLUMN_1, (VARV_CRANKRPM_ROW*22)+(VARV_CRANKRPM_ROW*2));
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.print(prevCrankRPM);
            tft.setTextColor(LOCKPAGE_TEXTCOLOR);
            tft.setCursor(VARV_COLUMN_1, (VARV_CRANKRPM_ROW*22)+(VARV_CRANKRPM_ROW*2));
            tft.print(readCrankRPM);
            prevCrankRPM = readCrankRPM;
            }

        if ( UART.getVescValues() ) {
            tft.setCursor(VARV_COLUMN_1, (2*22)+(2*2));
            tft.print(UART.data.rpm);
            tft.setCursor(VARV_COLUMN_1, (3*22)+(3*2));
            tft.print(UART.data.inpVoltage);
            tft.setCursor(VARV_COLUMN_1, (4*22)+(4*2));
            tft.print(UART.data.ampHours);
            tft.setCursor(VARV_COLUMN_1, (5*22)+(5*2));
            tft.print(UART.data.tachometerAbs);
            }
        else{
            tft.setCursor(0, (2*22)+(2*2));
            tft.print("Failed to get VESC data");
            }
        prevscreenTime = sysTime;
    }

}