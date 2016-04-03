#define LIGHT_STAIRS										0
#define LIGHT_LOFT_1            				1
#define LIGHT_LOFT_2            				2
#define LIGHT_TERRACE_1                 3
#define LIGHT_TERRACE_2                 4
#define LIGHT_TERRACE_3                 5
#define LIGHT_TOILET                    6

#define FLOOR_MODE 			  							7
#define HEATPUMP_REMOTE_SWITCH          8
#define HEATPUMP_CIRCULATION_PUMP       9
#define HEATPUMP_SANITARY_WATER       	10
#define HEATPUMP_COOL                		11

#define HVAC_ZONES											12
#	define HVAC_MASK_NO_ZONES							0x00
#	define HVAC_MASK_ALL_ZONES						0xFF
#	define HVAC_MASK_ALL_COOLING_ZONES (HVAC_MASK_BED1|HVAC_MASK_BED2|HVAC_MASK_LIVING|HVAC_MASK_BED3|HVAC_MASK_KITCHEN|HVAC_MASK_LOFT)
#	define HVAC_MASK_BED1									0x01
#	define HVAC_MASK_BATH1								0x02
#	define HVAC_MASK_BED2									0x04
#	define HVAC_MASK_LIVING								0x08
#	define HVAC_MASK_BED3									0x10
#	define HVAC_MASK_BATH2								0x20
#	define HVAC_MASK_KITCHEN							0x40
#	define HVAC_MASK_LOFT									0x80

#define HVAC_VALVES 												13
#	define MAIN_3WAY_VALVE_BOILER_MASK				0x01
#	define MAIN_3WAY_VALVE_COLLECTOR_MASK			0x02
#	define HEATING_MIX_VALVE_SWITCH_MASK			0x04
#	define HEATING_MIX_VALVE_DIRECTION_MASK		0x08

#define MAIN_3WAY_VALVE								14

#define PUMP_BOILER_FLOOR 						15
#define PUMP_COLLECTOR_FANCOIL 				16
#define PUMP_COLLECTOR_FLOOR 					17

#define FANCOIL_MODE									18
#define HP_SETPOINT_2									19

#define TEMP_AMBIENCE_SET_POINT				20
#define TEMP_AMBIENCE_SET_POINT_H			21
#define TEMP_SETPOINT_IN							22

#define TEMP_FLOOR_FLOW_SETPOINT			23
#define TEMP_FLOOR_FLOW_SETPOINT_H		24

#define HEATING_MIX_VALVE							25

#define LIGHT_SENSOR									26
#define LIGHT_SENSOR_H								27

#define HVAC_BOILER_SANITARY_TEMP			28
#define HVAC_BOILER_SANITARY_TEMP_H		29


/////////////////////////////////////////////////////////////
#include "grhSoulissSlots.h"

inline void DefineTypicals()
{
	Set_SimpleLight(LIGHT_STAIRS);
	Set_SimpleLight(LIGHT_LOFT_1);
	Set_SimpleLight(LIGHT_LOFT_2);
	Set_SimpleLight(LIGHT_TERRACE_1);
	Set_SimpleLight(LIGHT_TERRACE_2);
	Set_SimpleLight(LIGHT_TERRACE_3);
	Set_SimpleLight(LIGHT_TOILET);

	Set_Light(LIGHT_SENSOR);

	Souliss_SetT12(memory_map, FLOOR_MODE);
	Souliss_SetT12(memory_map, HEATPUMP_REMOTE_SWITCH);
	Souliss_SetT12(memory_map, HEATPUMP_CIRCULATION_PUMP);
	Souliss_SetT12(memory_map, HEATPUMP_SANITARY_WATER);
	Souliss_SetT12(memory_map, HEATPUMP_COOL);

	Souliss_SetT1A(memory_map, HVAC_ZONES);
	Souliss_SetT1A(memory_map, HVAC_VALVES);

	Souliss_SetT22(memory_map, MAIN_3WAY_VALVE);

	Souliss_SetT12(memory_map, PUMP_BOILER_FLOOR);
	Souliss_SetT12(memory_map, PUMP_COLLECTOR_FANCOIL);
	Souliss_SetT12(memory_map, PUMP_COLLECTOR_FLOOR);

	Set_Temperature(TEMP_AMBIENCE_SET_POINT);
	Set_Temperature(TEMP_FLOOR_FLOW_SETPOINT);
	Set_Temperature(HVAC_BOILER_SANITARY_TEMP);

	Souliss_SetT12(memory_map, FANCOIL_MODE);
	Souliss_SetT12(memory_map, HP_SETPOINT_2);

	Souliss_SetT22(memory_map, TEMP_SETPOINT_IN);

	// initialize values
	SetInput(HEATPUMP_REMOTE_SWITCH, Souliss_T1n_OnCmd);
//	SetInput(LIGHT_LOFT_1, Souliss_T1n_OnCmd);

	Souliss_SetT22(memory_map, HEATING_MIX_VALVE);


	SetInput(HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd);
	SetInput(HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_AutoCmd);
	SetInput(PUMP_BOILER_FLOOR, Souliss_T1n_AutoCmd);
	SetInput(PUMP_COLLECTOR_FANCOIL, Souliss_T1n_AutoCmd);
	SetInput(PUMP_COLLECTOR_FLOOR, Souliss_T1n_AutoCmd);
//	SetInput(FLOOR_MODE, Souliss_T1n_AutoCmd);
	SetInput(FANCOIL_MODE, Souliss_T1n_AutoCmd);
	SetInput(HP_SETPOINT_2, Souliss_T1n_AutoCmd);

	float set_point = AMBIENCE_SETPOINT_DEFAULT; // initial setup
	ImportAnalog(TEMP_AMBIENCE_SET_POINT, &set_point);
}
