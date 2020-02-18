#include "Arduino.h"
#include "numPadHandler.h"
#include "touch.h"
#include "lcdgfx.h"

extern MCUFRIEND_kbv tft;

uint32_t passcode = 12345678;
uint16_t * axes;
uint8_t num = 0;
int ctr = 0;
uint16_t numCtr = 0;
uint8_t numArr[8] = {0};
int isTouchDetected = 0;
int isHoldDetected = 0;
int touchTrig = 0;
uint16_t liftCtr = 0;
int isNumPadOn = 0;
uint32_t output_num = 0;

uint8_t numPadHandler(void){
    axes = readTouch(20);

    if((axes[0] < 700) || (axes[1] < 820)){
        if(isTouchDetected == 0){
            isTouchDetected = 1;
            touchTrig = 1;
        }
        if(isTouchDetected == 1){
            isHoldDetected = 1;
        }
    }
    else if(isTouchDetected == 1 && isHoldDetected == 1){
        liftCtr++;
        if(liftCtr > 100){
            isTouchDetected = 0;
            isHoldDetected = 0;
            liftCtr = 0;
        }

    }
    if(touchTrig == 1){
        ctr++;
        if(isNumPadOn == 1){
        }
        num = numpadTouchHandler(axes,220, 30, 50, 50);
        if(num == 10){  //delete if del button pressed
            if(numCtr == 0){

            }
            else{
                numCtr--;
                tft.setCursor(0+numCtr*18, 100);
                tft.setTextColor(LOCKPAGE_NUMBGCOLOR);
                tft.setTextSize(0);
                tft.print(numArr[numCtr]);
                numArr[numCtr] = 0;
                for(int i = 0; i < numCtr; i++){
                    tft.setTextColor(LOCKPAGE_NUMCOLOR,LOCKPAGE_NUMBGCOLOR);
                    tft.setCursor(0+i*18, 100);
                    tft.print(numArr[i]);
                }
            }
        }
        else if (num == 12){ //ok button pressed, sum numbers
            output_num = numArr[7]*1 + numArr[6]*10 + numArr[5]*100 + numArr[4]*1000 + numArr[3]*10000 + numArr[2]*100000 + numArr[1]*1000000 + numArr[0]*10000000;
            HAL_Delay(1);
            if (output_num == passcode){
                return 0xFF;
            }
            else{
                memset(numArr, 0, sizeof numArr);
                numCtr = 0;
                tft.fillScreen(BLACK);
                tft.setTextColor(RED,BLACK);
                tft.setCursor(150, 100);
                tft.print("Wrong passcode");
                HAL_Delay(3000);
                tft.fillScreen(LOCKPAGE_BGCOLOR);
                tft.setFont(&FreeSerif18pt7b);
                tft.setTextSize(0);
                tft.setTextColor(LOCKPAGE_TEXTCOLOR);
                tft.setCursor(0, (1*22)+(1*2));
                tft.print("Welcome!");
                tft.setCursor(0, (2*22)+(3*2));
                tft.print("Pass:");
                drawNumpad(220,30,50,50);
                isNumPadOn = 1;
            }
        }
        else if (num == 13){ //touch location out of numpad region, do nothing

        }
        else{
            numArr[numCtr] = num;
            numCtr++;
            if(numCtr > 8){
                numCtr = 8;
            }
            else{
                tft.setTextSize(0);
                for(int i = 0; i < numCtr; i++){
                    tft.setTextColor(LOCKPAGE_NUMCOLOR,LOCKPAGE_NUMBGCOLOR);
                    tft.setCursor(0+i*18, 100);
                    tft.print(numArr[i]);
                    }
                }  
        // numpadFeedback(num,100,30,50,50,COLOR);
        }
        // tft.fillRect(0,200,50,30,LOCKPAGE_NUMBGCOLOR);
        // tft.setCursor(0, 200);
        // tft.setTextColor(LOCKPAGE_NUMCOLOR);
        // tft.setTextSize(0);
        // tft.print(ctr);
         touchTrig = 0;
    }
}
uint8_t numpadTouchHandler(uint16_t axes[2], uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth){
    uint8_t ret = 0;
    uint16_t px = 0;
    uint16_t py = 0;
    uint16_t pixelx = map(axes[1],100,900,0,400);
    uint16_t pixely = map(axes[0],100,850,240,0);
    if (pixelx < 0) px = 0;
    if (pixely < 0) py = 0;
    if (pixelx > 400) px = 400;
    if (pixely > 240) px = 240;
    px = pixelx - x;
    py = pixely - y;
    if( (px < bwidth*3) && (py < (y+bheight*4))){
        if(px < bwidth){
            if(py < bheight) ret = 1;
            else if(py < bheight*2) ret = 4;
            else if(py < bheight*3) ret = 7;
            else if(py < bheight*4) ret = 10;
        }
        else if(px > bwidth && px < bwidth*2){
            if(py < bheight) ret = 2;
            else if(py < bheight*2) ret = 5;
            else if(py < bheight*3) ret = 8;
            else if(py < bheight*4) ret = 0;
        }
        else if(px > bwidth*2 && px < bwidth*3){
            if(py < bheight) ret = 3;
            else if(py > bheight && py < bheight*2) ret = 6;
            else if(py > bheight*2 && py < bheight*3) ret = 9;
            else if(py > bheight*3 && py < bheight*4) ret = 12;
        }
    }
    else{
        ret = 13;
    }
    return ret;
}