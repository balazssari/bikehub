#define INIT_SYS            0
#define INIT_LOCKSCREEN     1
#define LOCKSCREEN          2
#define INIT_MAIN           3
#define MAIN                4
#define INIT_VAR_VIEWER     99
#define VAR_VIEWER          100


extern float CrankRPM;
extern float prevCrankRPM;  //used for var viewer
extern uint32_t elapsedTime; 
extern uint32_t sysTime;
extern uint32_t prevsysTime;
extern uint32_t screenTime;
extern uint32_t prevscreenTime;
extern uint32_t input_capture;