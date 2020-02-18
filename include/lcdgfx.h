#include "MCUFRIEND/MCUFRIEND_kbv.h"
//#include "MCUFRIEND/fonts/FreeDefaultFonts.h"
#include "MCUFRIEND/fonts/FreeSerifBoldItalic24pt7b.h"
#include "MCUFRIEND/fonts/FreeSerif18pt7b.h"    //height=22,width=18



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

#define LOCKPAGE_BGCOLOR      BLACK
#define LOCKPAGE_TEXTCOLOR    WHITE
#define LOCKPAGE_TEXTBGCOLOR  BLACK
#define LOCKPAGE_NUMCOLOR     GREEN
#define LOCKPAGE_NUMBGCOLOR   BLACK
#define NUMPAD_TABLECOLOR     BLACK
#define NUMPAD_NUMCOLOR       CYAN
#define NUMPAD_NUMBGCOLOR     BLACK

void drawUI();
void drawNumpad(uint8_t x, uint8_t y, uint8_t bheight, uint8_t bwidth);
void drawWelcome(void);