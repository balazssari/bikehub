#ifndef SYSTEMSTATES_H
#define SYSTEMSTATES_H

#define INIT_SYS            0
#define INIT_LOCKSCREEN     1
#define LOCKSCREEN          2
#define INIT_MAIN           3
#define MAIN                4
#define INIT_MOTORWIZARD    5
#define MOTORWIZARD         6
#define INIT_VAR_VIEWER     99
#define VAR_VIEWER          100

#include <stdint.h>

extern float CrankRPM;
extern float prevCrankRPM;  //used for var viewer
extern uint32_t elapsedTime; 
extern uint32_t sysTime;
extern uint32_t prevsysTime;
extern uint32_t screenTime;
extern uint32_t prevscreenTime;
extern uint32_t input_capture;



struct dataPackage{
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
struct batteryData{
    int cellCount;
	float maxVolts;
    float minVolts;
};
struct SetupData{
    uint8_t pasMagnets; uint8_t wheelCircumference;
    uint8_t batteryCellCount; uint16_t batteryMaxVolts; uint16_t batteryMinVolts;
    uint16_t throttleHandleMax; uint16_t throttleHandleMin;
    uint16_t brakeHandleMax; uint16_t brakeHandleMin;
    uint16_t torqueSensorMax; uint16_t torqueSensorMin;
    uint8_t thModeCount; uint8_t tmMode1_max; uint8_t thMode2_max; uint8_t thMode3_max; uint8_t thMode4_max; uint8_t thMode5_max;
    uint8_t assistLevelCount; uint8_t assistLevel1_max; uint8_t assistLevel2_max; uint8_t assistLevel3_max; uint8_t assistLevel4_max; uint8_t assistLevel5_max;
};

#endif