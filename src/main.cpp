#include "main.h"

//#define debug
#define VAR_VIEWER_UPDATE_TIME 10
SystemClock_Config

STM32RTC& rtc = STM32RTC::getInstance();

extern MCUFRIEND_kbv tft;
HardwareSerial Serial3(USART3);
VescUart UART;

uint8_t system_state = INIT_MAIN;
#ifdef debug
uint8_t system_state = INIT_MAIN;
#endif
struct dataPackage {
	float avgMotorCurrent;
	float avgInputCurrent;
	float dutyCycleNow;
	long rpm;
	float inpVoltage;
	float ampHours;
	float ampHoursCharged;
	long tachometer;
	long tachometerAbs;
	};
struct batteryData {
    int cellCount;
	float maxVolts;
    float minVolts;
	};
struct SetupData {
    uint8_t pasMagnets; uint8_t wheelCircumference;
    uint8_t batteryCellCount; uint16_t batteryMaxVolts; uint16_t batteryMinVolts;
    uint16_t throttleHandleMax; uint16_t throttleHandleMin;
    uint16_t brakeHandleMax; uint16_t brakeHandleMin;
    uint16_t torqueSensorMax; uint16_t torqueSensorMin;
    uint8_t thModeCount; uint8_t tmMode1_max; uint8_t thMode2_max; uint8_t thMode3_max; uint8_t thMode4_max; uint8_t thMode5_max;
    uint8_t assistLevelCount; uint8_t assistLevel1_max; uint8_t assistLevel2_max; uint8_t assistLevel3_max; uint8_t assistLevel4_max; uint8_t assistLevel5_max;
};
struct batteryData battery;
struct SetupData recData;
struct dataPackage prevPackage;
struct dataPackage Package;
uint8_t assistlevel = 3;
uint8_t prevAssistlevel = 3;
uint8_t batterygaugelevel = 0;
uint8_t prevBatterygaugelevel = 0;
extern float CrankRPM;
float prevCrankRPM;
extern uint32_t capture_interval;
uint32_t previnput_capture;
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


void setup(void);
void loop(void);

//functions related to HW setup:
void lcd_on(void);
void lcd_off(void);
void lcd_setbacklight(uint8_t backlight);
void gpioInit(void);
void drawVarviewer(void);
void varviewerHandler(void);
void drawMain(void);
void print2digits(int number);
void mainHandler(void);
void updateMainscreenChangingElemenets(void);
int fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);

void setup(void) {
    rtc.setClockSource(STM32RTC::LSE_CLOCK);
    rtc.begin();
    rtc.setHours(20);
    rtc.setMinutes(59);
    rtc.setSeconds(0);
    
    Serial3.begin(19200);
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
            system_state = INIT_MAIN;
            }       
        break;
    case INIT_MAIN:
        drawMain();
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
void mainHandler(void){
    screenTime = sysTime - prevscreenTime;
    if(screenTime > VAR_VIEWER_UPDATE_TIME){
        //do updates on the screen one by one
        UART.getVescValues();
        //*** RTC CLOCK UPDATE START ***
        rtctime[0] = rtc.getHours();
        rtctime[1] = rtc.getMinutes(); 

        updateMainscreenChangingElemenets();
        
        prevscreenTime = sysTime;
    }

}
void drawMain(void){
    //*** STILL ELEMENTS ***
    tft.fillScreen(LOCKPAGE_BGCOLOR);
    //draw battery gauge elements
    tft.fillRoundRect(BATTGAUGE_XPOS+10,BATTGAUGE_YPOS-4,20,6,1,GREY);
    tft.fillRoundRect(BATTGAUGE_XPOS,BATTGAUGE_YPOS,40,115,5,GREY);
    tft.fillRoundRect(BATTGAUGE_XPOS+2,BATTGAUGE_YPOS+2,36,111,5,BLACK);
    //draw speed gauge elements
    tft.fillCircle(SPEEDGAUGE_XPOS,SPEEDGAUGE_YPOS,100,BLACK);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,89,8,145,396,BLUEGREY);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,88,15,144,146,GREY);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,88,15,194,196,GREY);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,88,15,244,246,GREY);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,88,15,294,296,GREY);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,88,15,344,346,GREY);
    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS,88,15,394,396,GREY);
    //KM/H
    tft.setFont(&FreeBigFont);
    tft.setCursor(SPEEDGAUGE_XPOS+22,SPEEDGAUGE_YPOS-4);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
    tft.print("KM");
    tft.setCursor(SPEEDGAUGE_XPOS+22,SPEEDGAUGE_YPOS+16);
    tft.print("H");
    //assist level
    tft.setFont(&FreeMono9pt7b);
    tft.setTextColor(WHITE);
    tft.setCursor(330,16);  
    tft.print("ASSIST");
    //E RPM
    tft.setFont(&FreeMono9pt7b);
    tft.setTextColor(WHITE);
    tft.setCursor(350,70);  
    tft.print("ERPM");
    //clock
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(WHITE);
    tft.setCursor(197,23);
    tft.print(":");
    //draw menu button
    tft.fillRoundRect(MENUBUTTON_XPOS,MENUBUTTON_YPOS,40,40,2,BLACK);
    tft.fillRoundRect(MENUBUTTON_XPOS+4,MENUBUTTON_YPOS +6,32,4,2,0xA671);
    tft.fillRoundRect(MENUBUTTON_XPOS+4,MENUBUTTON_YPOS +14,32,4,2,0xA671);
    tft.fillRoundRect(MENUBUTTON_XPOS+4,MENUBUTTON_YPOS +22,32,4,2,0xA671);
    tft.fillRoundRect(MENUBUTTON_XPOS+4,MENUBUTTON_YPOS +30,32,4,2,0xA671);

    tft.fillArcOffsetted(SPEEDGAUGE_XPOS, SPEEDGAUGE_YPOS, 99,6,144,300,0x3391);
}
void print2digits(int number) {
  if (number < 10) {
    tft.print("0"); // print a 0 before if the number is < than 10
  }
  tft.print(number);
}
void updateMainscreenChangingElemenets(void){
       if(rtctime[0] == rtctime[2]){
            //no need to update hours
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(170,25);
            print2digits(rtctime[2]);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(170,25);
            print2digits(rtctime[0]);
            }
        if(rtctime[1] == rtctime[3]){
            //no need to update minutes
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(204,25);
            print2digits(rtctime[3]);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(204,25);
            print2digits(rtctime[1]);
            }
        rtctime[2] = rtctime[0];
        rtctime[3] = rtctime[1];
        //*** RTC CLOCK UPDATE END ***
        //*** INPUT VOLTAGE UPDATE START ***
        if (UART.data.inpVoltage == prevPackage.inpVoltage){
            //no need to update Input Voltage value
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(10,170);  
            tft.print(prevPackage.inpVoltage);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(10,170);  
            tft.print(UART.data.inpVoltage);
            prevPackage.inpVoltage = UART.data.inpVoltage;
        }
        //*** INPUT VOLTAGE UPDATE END ***
        //*** RPM UPDATE START ***
        if (UART.data.rpm == prevPackage.rpm){
            //no need to update RPM value
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(340,94);  
            tft.print(prevPackage.rpm);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(340,94);  
            tft.print(UART.data.rpm);
            prevPackage.rpm = UART.data.rpm;
        }
        //*** RPM UPDATE END ***
        //*** ASSIST LEVEL UPDATE START ***
        if (assistlevel == prevAssistlevel){
            //no need to update assist level
        }
        else{
            tft.setFont(&FreeSansBold18pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(372,46);  
            tft.print(prevAssistlevel);
            tft.setFont(&FreeSansBold18pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(372,46);  
            tft.print(assistlevel);
            prevAssistlevel = assistlevel;
        }
        //*** ASSIST LEVEL UPDATE END ***
        //*** CRANK RPM UPDATE START ***
        if (CrankRPM == prevCrankRPM){
            //no need to update assist level
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(330,126);  
            tft.print(prevCrankRPM);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(330,126);  
            tft.print(CrankRPM);
            prevCrankRPM = CrankRPM;
        }
        //*** CRANK RPM UPDATE END ***
        //*** DEBUG UPDATE START ***
        if (capture_interval == previnput_capture){
            //no need to update assist level
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(330,156);  
            tft.print(previnput_capture);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(330,156);  
            tft.print(capture_interval);
            previnput_capture = capture_interval;
        }
        //*** DEBUG UPDATE END ***
        //*** BATTERY GAUGE UPDATE START ***
        if (batterygaugelevel == prevBatterygaugelevel){
            //no need to update battery gauge
        }
        else{
            if (batterygaugelevel > prevBatterygaugelevel){
                for(int i = prevBatterygaugelevel; i < batterygaugelevel; i++){
                    if(i < 1)           tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, RED);
                    if(i > 0 && i < 3)  tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, YELLOW);
                    if(i > 2 && i < 10) tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, GREEN);
                    prevBatterygaugelevel = batterygaugelevel;
                }
            }
            else if (batterygaugelevel < prevBatterygaugelevel){
                for (int i = prevBatterygaugelevel; i >= batterygaugelevel; i--){
                //remove elements
                tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, BLACK);
                prevBatterygaugelevel = batterygaugelevel;
                }
            }
        }
        //*** BATTERY GAUGE UPDATE END ***    
}
void writeNewToEEPROM(void){
    int eeAddress = 0;
    EEPROM.put(eeAddress, recData);
}
