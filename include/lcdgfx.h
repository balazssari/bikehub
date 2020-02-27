#include "MCUFRIEND/MCUFRIEND_kbv.h"
#include "MCUFRIEND/fonts/FreeDefaultFonts.h"
#include "MCUFRIEND/fonts/FreeSerifBoldItalic24pt7b.h"
#include "MCUFRIEND/fonts/FreeSerif18pt7b.h"    //height=22,width=18
#include "MCUFRIEND/fonts/FreeMono9pt7b.h"
#include "MCUFRIEND/fonts/FreeMonoOblique18pt7b.h"
#include "MCUFRIEND/fonts/FreeSansBold18pt7b.h"
#include "MCUFRIEND/fonts/FreeSansBold12pt7b.h"

// #include "Fonts/FreeSans9pt7b.h"
// #include "Fonts/FreeSans12pt7b.h"
// #include "Fonts/FreeSerif12pt7b.h"

//element positions on MAIN screen
#define SPEEDGAUGE_XPOS     200
#define SPEEDGAUGE_YPOS     140
#define MENUBUTTON_XPOS     340
#define MENUBUTTON_YPOS     190
#define BATTGAUGE_XPOS       10
#define BATTGAUGE_YPOS       10

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PINK    0xF2FF
#define GREY    0xBDD7
#define BLUEGREY 0x10C5

#define LOCKPAGE_BGCOLOR      0x0842
#define LOCKPAGE_TEXTCOLOR    WHITE
#define LOCKPAGE_TEXTBGCOLOR  BLACK
#define LOCKPAGE_NUMCOLOR     GREEN
#define LOCKPAGE_NUMBGCOLOR   0x0842
#define NUMPAD_TABLECOLOR     BLUEGREY
#define NUMPAD_NUMCOLOR       WHITE
#define NUMPAD_NUMBGCOLOR     BLACK

#define VARV_CRANKRPM_ROW     1
#define VARV_COLUMN_1         180
#define VARV_COLUMN_2         350

void drawNumpad(uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth);
void drawWelcome(void);
void setFontSerif18(void);
void setFontMono9(void);
void fillScreen(uint16_t COLOR);