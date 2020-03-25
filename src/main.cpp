#include "main.h"

//#define debug

//Global variables
#ifndef debug
uint8_t system_state = INIT_SYS;
#elif debug
uint8_t system_state = INIT_MAIN;
#endif

batteryData battery;
SetupData recData;
dataPackage packageVESC;
uint8_t assistlevel = 3;
uint8_t batterygaugelevel = 0;
extern float CrankRPM;
extern uint32_t capture_interval;
uint8_t motorMagnetPoles = 23;
float wheelRPM = 0;
extern float currentToVESC;
extern uint32_t PeriodMeasured;
uint32_t elapsedTime = 0; 
uint32_t sysTime = 0;
uint32_t prevsysTime = 0;
uint32_t screenTime = 0;
uint32_t prevscreenTime = 0;
uint8_t varviewerStep = 0;
uint8_t rtctime[4];
#ifdef debug
uint8_t dummyupdate = 0;
uint8_t reachedTop = 0;
#endif

// Class Instances
screenElementsClass screenElements;
STM32RTC& rtc = STM32RTC::getInstance();
extern MCUFRIEND_kbv tft;
HardwareSerial Serial3(USART3);
VescUart UART;

// Function Prototypes
void setup(void);
void loop(void);
void lcd_on(void);
void lcd_off(void);
void lcd_setbacklight(uint8_t backlight);
void gpioInit(void);
void drawVarviewer(void);
void varviewerHandler(void);
void mainHandler(void);


void setup(void) {
    rtc.setClockSource(STM32RTC::LSE_CLOCK);
    rtc.begin();
    rtc.setHours(20);
    rtc.setMinutes(59);
    rtc.setSeconds(0);
    
    Serial3.begin(115200);
    UART.setSerialPort(&Serial3);

    gpioInit();
    lcd_on();
    lcd_setbacklight(100);
}

void loop(void) {
    sysTime = millis();
    #ifdef debug
    if(sysTime > dummyupdate * 1000){
        if(reachedTop == 0){
            batterygaugelevel+=1;
        }
        else if (reachedTop == 1){
            batterygaugelevel--;
        }
        if(batterygaugelevel == 0) reachedTop = 0;
        if(batterygaugelevel == 9){
            reachedTop = 1;
            batterygaugelevel = 8;
        }
        
        dummyupdate++;
        }
    #endif
    //pasHandler();
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
            system_state = INIT_MAIN;
            }       
        break;
    case INIT_MAIN:
        screenElements.drawMain();
        pasInit();
        system_state = MAIN;
        break;
    case MAIN:
        mainHandler();
        break;
    case INIT_MOTORWIZARD:
        drawMotorWizard();
        break;
    case MOTORWIZARD:
        motorWizardHandler();
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
void mainHandler(void){
    screenTime = sysTime - prevscreenTime;
    if(screenTime > VAR_VIEWER_UPDATE_TIME){
        UART.getVescValues();
        // át kellene adni a UART.data-t egy másik struktúrának
        memcpy(&packageVESC, &UART.data, sizeof(UART.data));
        wheelRPM = (float)UART.data.rpm / motorMagnetPoles;

        screenElements.updateScreenElements(batterygaugelevel, assistlevel, PeriodMeasured, CrankRPM, wheelRPM, currentToVESC, packageVESC, rtc.getHours(), rtc.getMinutes());
        
        prevscreenTime = sysTime;
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
    setFontMono9();
    tft.fillRect(0,0,400,40, GREY);
    tft.setTextColor(BLACK);
    tft.setTextSize(1);
    tft.setCursor(2, (20)+(1*2));
    tft.print("Variable viewer");
}
void varviewerHandler(void){

    screenTime = sysTime - prevscreenTime;
    if(screenTime > VAR_VIEWER_UPDATE_TIME){
        float readCrankRPM = CrankRPM;
 
        prevscreenTime = sysTime;
    }

}
void writeNewToEEPROM(void){
    int eeAddress = 0;
    EEPROM.put(eeAddress, recData);
}
