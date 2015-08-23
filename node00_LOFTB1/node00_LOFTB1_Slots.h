#define LIGHT_STAIRS					0
#define LIGHT_LOFT_1            		1
#define LIGHT_LOFT_2            		2
#define LIGHT_TERRACE_1                 3
#define LIGHT_TERRACE_2                 4
#define LIGHT_TERRACE_3                 5
#define LIGHT_TOILET                    6

#define TEMP_BOILER_SANITARY            7
#define TEMP_BOILER_SANITARY_H          8
#define TEMP_BOILER_HEATING             9
#define TEMP_BOILER_HEATING_H   		10
#define TEMP_BOILER_BOTTOM              11
#define TEMP_BOILER_BOTTOM_H    		12

#define FLOOR_MODE 			  			13
#define HEATPUMP_REMOTE_SWITCH          14
#define HEATPUMP_CIRCULATION_PUMP       15
#define HEATPUMP_SANITARY_WATER       16
#define HEATPUMP_COOL                	17

#define TEMP_FLOOR_FLOW 			18      // floor heating system water temp
#define TEMP_FLOOR_FLOW_H 			19
#define TEMP_FLOOR_RETURN  			20      // floor heating system water return temp
#define TEMP_FLOOR_RETURN_H 		21
#define TEMP_FANCOIL_FLOW  			22      // floor heating system water return temp
#define TEMP_FANCOIL_FLOW_H 		23

#define HVAC_ZONES					24
#	define HVAC_MASK_NO_ZONES		0x00
# define HVAC_MASK_ALL_ZONES	0xFF
#	define HVAC_MASK_BED1			0x01
#	define HVAC_MASK_BATH1			0x02
#	define HVAC_MASK_BED2			0x04
#	define HVAC_MASK_LIVING			0x08
#	define HVAC_MASK_BED3			0x10
#	define HVAC_MASK_BATH2			0x20
#	define HVAC_MASK_KITCHEN		0x40
#	define HVAC_MASK_LOFT			0x80

#define HVAC_VALVES 							25
#	define MAIN_3WAY_VALVE_BOILER_MASK			0x01
#	define MAIN_3WAY_VALVE_COLLECTOR_MASK		0x02
#	define HEATING_MIX_VALVE_SWITCH_MASK		0x04
#	define HEATING_MIX_VALVE_DIRECTION_MASK		0x08

#define MAIN_3WAY_VALVE				26

#define PUMP_BOILER_FLOOR 			27
#define PUMP_COLLECTOR_FANCOIL 		28
#define PUMP_COLLECTOR_FLOOR 		29

#define FANCOIL_MODE				30
#define HP_SETPOINT_2				31

#define TEMP_AMBIENCE_SET_POINT		32
#define TEMP_AMBIENCE_SET_POINT_H	33

#define TEMP_SETPOINT_IN	34

inline void DefineTypicals()
{
	Set_SimpleLight(LIGHT_STAIRS);
	Set_SimpleLight(LIGHT_LOFT_1);
	Set_SimpleLight(LIGHT_LOFT_2);
	Set_SimpleLight(LIGHT_TERRACE_1);
	Set_SimpleLight(LIGHT_TERRACE_2);
	Set_SimpleLight(LIGHT_TERRACE_3);
	Set_SimpleLight(LIGHT_TOILET);

	Souliss_SetT12(memory_map, FLOOR_MODE);
	Souliss_SetT12(memory_map, HEATPUMP_REMOTE_SWITCH);
	Souliss_SetT12(memory_map, HEATPUMP_CIRCULATION_PUMP);
	Souliss_SetT12(memory_map, HEATPUMP_SANITARY_WATER);
	Souliss_SetT12(memory_map, HEATPUMP_COOL);

	Set_Temperature(TEMP_BOILER_SANITARY);
	Set_Temperature(TEMP_BOILER_HEATING);
	Set_Temperature(TEMP_BOILER_BOTTOM);
	Set_Temperature(TEMP_FLOOR_FLOW);
	Set_Temperature(TEMP_FLOOR_RETURN);
	Set_Temperature(TEMP_FANCOIL_FLOW);

	Souliss_SetT1A(memory_map, HVAC_ZONES);
	Souliss_SetT1A(memory_map, HVAC_VALVES);

	Souliss_SetT22(memory_map, MAIN_3WAY_VALVE);

	Souliss_SetT12(memory_map, PUMP_BOILER_FLOOR);
	Souliss_SetT12(memory_map, PUMP_COLLECTOR_FANCOIL);
	Souliss_SetT12(memory_map, PUMP_COLLECTOR_FLOOR);

	Set_Temperature(TEMP_AMBIENCE_SET_POINT);

	Souliss_SetT12(memory_map, FANCOIL_MODE);
	Souliss_SetT12(memory_map, HP_SETPOINT_2);

	Souliss_SetT22(memory_map, TEMP_SETPOINT_IN);

	// initialize values
//	SetInput(HEATPUMP_REMOTE_SWITCH, Souliss_T1n_OnCmd);
//	SetInput(LIGHT_LOFT_1, Souliss_T1n_OnCmd);

	float set_point = 19; // winter initial setup
	ImportAnalog(TEMP_AMBIENCE_SET_POINT, &set_point);
}
