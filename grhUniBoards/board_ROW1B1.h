
#define LIGHT_LIVING_1			1
#define LIGHT_LIVING_2			2
#define LIGHT_LIVING_3			3		
#define LIGHT_LIVING_4			4

#define SOULISS_GLOBALS

#define SOULISS_DEFINE_TYPICALS Set_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);

#define SOULISS_READ_INPUTS 	LightsGroupIn(IN1, LIGHT_LIVING_1, LIGHT_LIVING_4);

#define SOULISS_PROCESS_LOGICS 	Logic_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);

#define SOULISS_SET_OUTPUTS 	DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_LIVING_1);\
								DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_LIVING_2);\
								DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_LIVING_3);\
								DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_LIVING_4); 

#define SOULISS_PROCESS_TIMERS 	Timer_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);
