#include "main.h"
MCUFRIEND_kbv tft;

uint32_t passcode = 12345678;
//variables related to numPadHandler():
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
uint8_t system_state = 0;

void setup(void);
void loop(void);

//functions related to HW setup:
void lcd_on(void);
void lcd_off(void);
void lcd_setbacklight(uint8_t backlight);
void gpioInit(void);
//functions related to graphics:
void drawUI();
void drawNumpad(uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth);
void _drawInputsFeedback(uint16_t xpos, uint16_t ypos, uint16_t thickness, uint16_t height, uint16_t COLOR);
void _drawVeloMetric(uint8_t speed, uint16_t COLOR);
void drawToScreen(void);
void drawWelcome(void);
//functions related to numPadHandler():
uint16_t * readTouch(uint8_t threshold);
uint8_t numpadTouchHandler(uint16_t axes[2], uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth);
void numpadFeedback(uint8_t num,uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth, uint16_t COLOR);
void numPadHandler(void);
//functions related to uiHandler():
void uiHandler(void);


void setup(void) {
    gpioInit();
    lcd_on();
    drawWelcome();
    lcd_setbacklight(100);
}

void loop(void) {
    switch (system_state)
    {
    case 0:
        numPadHandler();
        break;
    case 1:
        uiHandler();
        break;
    
    default:
        break;
    }
    


}
void drawToScreen(void){
    tft.drawPixel(map(axes[1],100,900,0,400),map(axes[0],130,850,240,0),WHITE); //   HAL_Delay(1);
}
void drawUI(){
    _drawInputsFeedback(330,130,25,100,WHITE);
    _drawVeloMetric(26,GREEN);
}
void _drawInputsFeedback(uint16_t xpos, uint16_t ypos, uint16_t thickness, uint16_t height, uint16_t COLOR){
    tft.drawRect(xpos,ypos,thickness,height,COLOR);
    tft.drawRect((xpos+thickness+10),ypos,thickness,height,COLOR);
}
void _drawVeloMetric(uint8_t speed, uint16_t COLOR){
    tft.setTextSize(3);
    tft.setTextColor(COLOR, BLACK);
    tft.setCursor(130, 50);
    tft.println("KPH");
    tft.setFont(&FreeSerifBoldItalic24pt7b);
    tft.setTextSize(0);
    tft.setCursor(200, 170);
    tft.println("26");
}
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
void numPadHandler(void){
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
                tft.fillScreen(BLACK);
                drawUI();
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
void drawNumpad(uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth){
    int numHeight = 24;
    int numWidth = 18;
    
    //drawing rectangles
    tft.drawRect(x,             y,              bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth,      y,              bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth*2 ,   y,              bwidth,     bheight,    NUMPAD_TABLECOLOR);

    tft.drawRect(x,             y+bheight,      bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth,      y+bheight,      bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth*2,    y+bheight,      bwidth,     bheight,    NUMPAD_TABLECOLOR);

    tft.drawRect(x,             y+bheight*2,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth,      y+bheight*2,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth*2,    y+bheight*2,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    
    tft.drawRect(x,             y+bheight*3,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth,      y+bheight*3,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    tft.drawRect(x+bwidth*2,    y+bheight*3,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    //filling up with numbers
    tft.setTextColor(NUMPAD_NUMCOLOR, NUMPAD_NUMBGCOLOR);
    tft.setTextSize(0);
    tft.setCursor(x+(bwidth/2)-numWidth/2, y+(bheight/2)+numHeight/2);
    tft.print("1");
    tft.setCursor(x+bwidth+(bwidth/2)-numWidth/2, y+(bheight/2)+numHeight/2);
    tft.print("2");
    tft.setCursor(x+bwidth*2+(bwidth/2)-numWidth/2, y+(bheight/2)+numHeight/2);
    tft.print("3");
    
    tft.setCursor(x+(bwidth/2)-numWidth/2, y+bheight+(bheight/2)+numHeight/2);
    tft.print("4");
    tft.setCursor(x+bwidth+(bwidth/2)-numWidth/2, y+bheight+(bheight/2)+numHeight/2);
    tft.print("5");
    tft.setCursor(x+bwidth*2+(bwidth/2)-numWidth/2, y+bheight+(bheight/2)+numHeight/2);
    tft.print("6");

    tft.setCursor(x+(bwidth/2)-numWidth/2, y+bheight*2+(bheight/2)+numHeight/2);
    tft.print("7");
    tft.setCursor(x+bwidth+(bwidth/2)-numWidth/2, y+bheight*2+(bheight/2)+numHeight/2);
    tft.print("8");
    tft.setCursor(x+bwidth*2+(bwidth/2)-numWidth/2, y+bheight*2+(bheight/2)+numHeight/2);
    tft.print("9");
    tft.setCursor(x+bwidth+(bwidth/2)-numWidth/2, y+bheight*3+(bheight/2)+numHeight/2);
    tft.print("0");

    tft.setTextSize(0);
    tft.setCursor(x+(bwidth/2)-numWidth, y+bheight*3+(bheight/2)+numHeight/2);
    tft.print("del");

    tft.setCursor(x+bwidth*2+(bwidth/2)-numWidth, y+bheight*3+(bheight/2)+numHeight/2);
    tft.print("ok");

}
void drawWelcome(void){
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
void uiHandler(void){
    drawUI();
}