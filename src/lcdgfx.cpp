#include "Arduino.h"
#include "lcdgfx.h"

MCUFRIEND_kbv tft;

void drawUI(){
    tft.fillScreen(BLACK);
}

void drawNumpad(uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth){
    int numHeight = 24;
    int numWidth = 18;
    int bPadding = 4;
    int radius = 8;
    //drawing rectangles:
    // tft.drawRect(x,             y,              bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth,      y,              bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth*2 ,   y,              bwidth,     bheight,    NUMPAD_TABLECOLOR);

    // tft.drawRect(x,             y+bheight,      bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth,      y+bheight,      bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth*2,    y+bheight,      bwidth,     bheight,    NUMPAD_TABLECOLOR);

    // tft.drawRect(x,             y+bheight*2,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth,      y+bheight*2,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth*2,    y+bheight*2,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    
    // tft.drawRect(x,             y+bheight*3,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth,      y+bheight*3,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    // tft.drawRect(x+bwidth*2,    y+bheight*3,    bwidth,     bheight,    NUMPAD_TABLECOLOR);
    //drawing filled rectangles with radius:
    tft.fillRoundRect(x+bPadding,             y+bPadding,              bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth+bPadding,      y+bPadding,              bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth*2+bPadding ,   y+bPadding,              bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);

    tft.fillRoundRect(x+bPadding,             y+bheight+bPadding,      bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth+bPadding,      y+bheight+bPadding,      bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth*2+bPadding,    y+bheight+bPadding,      bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);

    tft.fillRoundRect(x+bPadding,             y+bheight*2+bPadding,    bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth+bPadding,      y+bheight*2+bPadding,    bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth*2+bPadding,    y+bheight*2+bPadding,    bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    
    tft.fillRoundRect(x+bPadding,             y+bheight*3+bPadding,    bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth+bPadding,      y+bheight*3+bPadding,    bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
    tft.fillRoundRect(x+bwidth*2+bPadding,    y+bheight*3+bPadding,    bwidth-bPadding*2,     bheight-bPadding*2,   radius,    NUMPAD_TABLECOLOR);
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
    drawNumpad(220,10,54,54);
}