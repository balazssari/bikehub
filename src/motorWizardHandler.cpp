#include <Arduino.h>
#include <motorWizardHandler.h>
#include "numPadHandler.h"
#include "touch.h"
#include "lcdgfx.h"
#include "systemstates.h"
#include <VescUart.h>

#define MC_CONFIG_DRAW_INTRO                1
#define MC_CONFIG_INTRO                     2
#define MC_CONFIG_DRAW_PAGE_MOTORTYPE       3
#define MC_CONFIG_PAGE_MOTORTYPE            4

uint8_t motorwizard_state = MC_CONFIG_DRAW_INTRO;
extern MCUFRIEND_kbv tft;
extern VescUart UART;
extern uint8_t system_state;

void drawMotorWizard(void){
    switch (motorwizard_state)
    {
    case MC_CONFIG_DRAW_INTRO:
        tft.fillScreen(GREY);
        tft.setTextColor(BLACK);
        tft.setFont(&FreeSansBold12pt7b);
        tft.setCursor(40,20);
        tft.print("Motor configuration wizard");
        tft.setFont(&FreeSmallFont);
        tft.setCursor(20,80);
        tft.print("This wizard will guide you through the motor");
        tft.setCursor(20,92);
        tft.print("setup of the VESC® step by step.");
        tft.setCursor(20,104);
        tft.print("Notice that only the required options for");
        tft.setCursor(20,116);
        tft.print("getting the motor running are shown.");
        tft.setCursor(20,128);
        tft.print("For tweaking the advanced settings, the");
        tft.setCursor(20,140);
        tft.print("configuration pages have to be entered after");
        tft.setCursor(20,152);
        tft.print("finishing this wizard.");
        motorwizard_state = MC_CONFIG_INTRO;
        break;
    case MC_CONFIG_INTRO:
        if (numPadHandler() == 30){
            system_state = INIT_MAIN;        //Back button pressed go back to MAIN
        }
        else if (numPadHandler() == 31){
            motorwizard_state = MC_CONFIG_DRAW_PAGE_MOTORTYPE;
        }
        break;
    case MC_CONFIG_DRAW_PAGE_MOTORTYPE:
        tft.fillScreen(GREY);
        tft.setTextColor(BLACK);
        tft.setFont(&FreeSansBold12pt7b);
        tft.setCursor(80,20);
        tft.print("Choose Motor Type");
        tft.setFont(&FreeSmallFont);
        tft.setCursor(20,80);
        tft.print("motortype");
        motorwizard_state = MC_CONFIG_PAGE_MOTORTYPE;
        break;
    case MC_CONFIG_PAGE_MOTORTYPE:
        
        break;
    default:
        break;
    }
    
}
void motorWizardHandler(void){

}