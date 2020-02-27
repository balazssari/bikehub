#include "Arduino.h"
#include "touch.h"

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
uint16_t fullscreenTouchHandler(uint16_t axes[2]){
    uint16_t ret = 0;
    uint16_t px = 0;
    uint16_t py = 0;
    uint16_t pixelx = map(axes[1],100,900,0,400);
    uint16_t pixely = map(axes[0],100,850,240,0);
    if (pixelx < 0) px = 0;
    if (pixely < 0) py = 0;
    if (pixelx > 400) px = 400;
    if (pixely > 240) px = 240;
    px = pixelx;
    py = pixely;

        if(px < 40){
                 if(py < 40)  ret = 101;
            else if(py < 80)  ret = 201;
            else if(py < 120) ret = 301;
            else if(py < 160) ret = 401;
            else if(py < 200) ret = 501;
            else if(py < 240) ret = 601;
        }
        else if(px > 40 && px < 80){
                 if(py < 40)  ret = 102;
            else if(py < 80)  ret = 202;
            else if(py < 120) ret = 302;
            else if(py < 160) ret = 402;
            else if(py < 200) ret = 502;
            else if(py < 240) ret = 602;
        }
        else if(px > 80 && px < 120){
                 if(py < 40)  ret = 103;
            else if(py < 80)  ret = 203;
            else if(py < 120) ret = 303;
            else if(py < 160) ret = 403;
            else if(py < 200) ret = 503;
            else if(py < 240) ret = 603;
        }
        else if(px > 120 && px < 160){
                 if(py < 40)  ret = 104;
            else if(py < 80)  ret = 204;
            else if(py < 120) ret = 304;
            else if(py < 160) ret = 404;
            else if(py < 200) ret = 504;
            else if(py < 240) ret = 604;
        }
        else if(px > 160 && px < 200){
                 if(py < 40)  ret = 105;
            else if(py < 80)  ret = 205;
            else if(py < 120) ret = 305;
            else if(py < 160) ret = 405;
            else if(py < 200) ret = 505;
            else if(py < 240) ret = 605;
        }
        else if(px > 200 && px < 240){
                 if(py < 40)  ret = 106;
            else if(py < 80)  ret = 206;
            else if(py < 120) ret = 306;
            else if(py < 160) ret = 406;
            else if(py < 200) ret = 506;
            else if(py < 240) ret = 606;
        }
        else if(px > 240 && px < 280){
                 if(py < 40)  ret = 107;
            else if(py < 80)  ret = 207;
            else if(py < 120) ret = 307;
            else if(py < 160) ret = 407;
            else if(py < 200) ret = 507;
            else if(py < 240) ret = 607;
        }
        else if(px > 280 && px < 320){
                 if(py < 40)  ret = 108;
            else if(py < 80)  ret = 208;
            else if(py < 120) ret = 308;
            else if(py < 160) ret = 408;
            else if(py < 200) ret = 508;
            else if(py < 240) ret = 608;
        }
        else if(px > 320 && px < 360){
                 if(py < 40)  ret = 109;
            else if(py < 80)  ret = 209;
            else if(py < 120) ret = 309;
            else if(py < 160) ret = 409;
            else if(py < 200) ret = 509;
            else if(py < 240) ret = 609;
        }
        else if(px > 360 && px < 40){
                 if(py < 40)  ret = 110;
            else if(py < 80)  ret = 210;
            else if(py < 120) ret = 310;
            else if(py < 160) ret = 410;
            else if(py < 200) ret = 510;
            else if(py < 240) ret = 610;
        }
    return ret;
}