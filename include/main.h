#include "numPadHandler.h"
#include "lcdgfx.h"
#include "pasHandler.h"
#include "motorWizardHandler.h"
#include "systemStates.h"
#include <VescUart.h>
#include <STM32RTC.h>
#include <EEPROM.h>

#define VAR_VIEWER_UPDATE_TIME 50

class screenElementsClass
{
public:
    uint8_t prevRtcHours;
    uint8_t prevRtcMinutes;
    uint8_t prevBatterygaugelevel;
    uint8_t prevAssistlevel;
    uint32_t prevPeriodMeasured;
    float prevCrankRPM;
    float prevWheelRPM ;
    float prevcurrentToVESC;
    dataPackage prevDataFromVESC;
    
    void print2digits(int number) {
        extern MCUFRIEND_kbv tft;
        if (number < 10) {
            tft.print("0"); // print a 0 before if the number is < than 10
        }
        tft.print(number);
    }
    void updateScreenElements(uint8_t batteryGaugeLevel, uint8_t assistLevel, uint8_t periodMeasured, float crankRPM, float wheelRPM, float currentToVESC, dataPackage dataFromVESC, uint8_t rtcHours, uint8_t rtcMinutes){
        extern MCUFRIEND_kbv tft;
        // RTC CLOCK UPDATE START ///////////////////////////////////////////////////////////////
        if(rtcHours == prevRtcHours){
            //no need to update hours
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(170,25);
            print2digits(prevRtcHours);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(170,25);
            print2digits(rtcHours);
            }
        if(rtcMinutes == prevRtcMinutes){
            //no need to update minutes
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(204,25);
            print2digits(prevRtcMinutes);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(204,25);
            print2digits(rtcMinutes);
            }
        prevRtcHours = rtcHours;
        prevRtcMinutes = rtcMinutes;
        // RTC CLOCK UPDATE END /////////////////////////////////////////////////////////////////
        // INPUT VOLTAGE UPDATE START ///////////////////////////////////////////////////////////
        if (dataFromVESC.inpVoltage == prevDataFromVESC.inpVoltage){
            // no need to update Input Voltage value
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(10,170);  
            tft.print(prevDataFromVESC.inpVoltage);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(10,170);  
            tft.print(dataFromVESC.inpVoltage);
            prevDataFromVESC.inpVoltage = dataFromVESC.inpVoltage;
        }
        // INPUT VOLTAGE UPDATE END /////////////////////////////////////////////////////////////
        // RPM UPDATE START /////////////////////////////////////////////////////////////////////
        if (dataFromVESC.rpm == prevDataFromVESC.rpm){
            // no need to update RPM value
        }
        else{
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(340,94);  
            tft.print(prevDataFromVESC.rpm);
            tft.setFont(&FreeSansBold12pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(340,94);  
            tft.print(dataFromVESC.rpm);
            prevDataFromVESC.rpm = dataFromVESC.rpm;
        }
        // RPM UPDATE END ////////////////////////////////////////////////////////////////////////
        // ASSIST LEVEL UPDATE START /////////////////////////////////////////////////////////////
        if (assistLevel == prevAssistlevel){
            // no need to update assist level
        }
        else{
            tft.setFont(&FreeSansBold18pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(372,46);  
            tft.print(prevAssistlevel);
            tft.setFont(&FreeSansBold18pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(372,46);  
            tft.print(assistLevel);
            prevAssistlevel = assistLevel;
        }
        // ASSIST LEVEL UPDATE END ///////////////////////////////////////////////////////////////

        // DEBUG UPDATE START ////////////////////////////////////////////////////////////////////
        // CRANK RPM UPDATE START ////////////////////////////////////////////////////////////////
        if (crankRPM == prevCrankRPM){
            // no need to update assist level
        }
        else{
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(330,126);  
            tft.print(prevCrankRPM);
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(330,126);  
            tft.print(crankRPM);
            prevCrankRPM = crankRPM;
        }
        // CRANK RPM UPDATE END //////////////////////////////////////////////////////////////////
        if (periodMeasured == prevPeriodMeasured){
            // no need to update assist level
        }
        else{
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(330,146);  
            tft.print(prevPeriodMeasured);
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(330,146);  
            tft.print(periodMeasured);
            prevPeriodMeasured = periodMeasured;
        }
        if (currentToVESC == prevcurrentToVESC){
            // no need to update assist level
        }
        else{
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(330,166);  
            tft.print(prevcurrentToVESC);
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(330,166);  
            tft.print(currentToVESC);
            prevcurrentToVESC = currentToVESC;
        }
        if (wheelRPM == prevWheelRPM){
            // no need to update assist level
        }
        else{
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(LOCKPAGE_BGCOLOR);
            tft.setCursor(330,186);  
            tft.print(prevWheelRPM);
            tft.setFont(&FreeMono9pt7b);
            tft.setTextColor(WHITE);
            tft.setCursor(330,186);  
            tft.print(wheelRPM);
            prevWheelRPM = wheelRPM;
        }
        // DEBUG UPDATE END //////////////////////////////////////////////////////////////////////
        // BATTERY GAUGE UPDATE START ////////////////////////////////////////////////////////////
        if (batteryGaugeLevel == prevBatterygaugelevel){
            // no need to update battery gauge
        }
        else{
            if (batteryGaugeLevel > prevBatterygaugelevel){
                for(int i = prevBatterygaugelevel; i < batteryGaugeLevel; i++){
                    if(i < 1)           tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, RED);
                    if(i > 0 && i < 3)  tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, YELLOW);
                    if(i > 2 && i < 10) tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, GREEN);
                    prevBatterygaugelevel = batteryGaugeLevel;
                }
            }
            else if (batteryGaugeLevel < prevBatterygaugelevel){
                for (int i = prevBatterygaugelevel; i >= batteryGaugeLevel; i--){
                // remove elements
                tft.fillRoundRect(BATTGAUGE_XPOS+4,BATTGAUGE_YPOS+(100-i*12),32,10,2, BLACK);
                prevBatterygaugelevel = batteryGaugeLevel;
                }
            }
        }
        // BATTERY GAUGE UPDATE END //////////////////////////////////////////////////////////////
    }
    void drawMain(void){
        extern MCUFRIEND_kbv tft;
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
};