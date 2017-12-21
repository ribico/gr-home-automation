#include "grhSoulissSlots.h"

inline void DefineTypicals()
{
	Set_SimpleLight(LOFTB1_LIGHT_STAIRS);
	Set_SimpleLight(LOFTB1_LIGHT_LOFT_1);
	Set_SimpleLight(LOFTB1_LIGHT_LOFT_2);
	Set_SimpleLight(LOFTB1_LIGHT_TERRACE_1);
	Set_SimpleLight(LOFTB1_LIGHT_TERRACE_2);
	Set_SimpleLight(LOFTB1_LIGHT_TERRACE_3);
	Set_SimpleLight(LOFTB1_LIGHT_TOILET);

	Set_Light(LOFTB1_LIGHT_SENSOR);

	Souliss_SetT12(memory_map, LOFTB1_FLOOR_MODE);
	Souliss_SetT12(memory_map, LOFTB1_HEATPUMP_REMOTE_SWITCH);
	Souliss_SetT12(memory_map, LOFTB1_HEATPUMP_CIRCULATION_PUMP);
	Souliss_SetT12(memory_map, LOFTB1_HEATPUMP_SANITARY_WATER);
	Souliss_SetT12(memory_map, LOFTB1_HEATPUMP_COOL);

	Souliss_SetT1A(memory_map, LOFTB1_HVAC_ZONES);
	Souliss_SetT1A(memory_map, LOFTB1_HVAC_VALVES);

	Souliss_SetT22(memory_map, LOFTB1_MAIN_3WAY_VALVE);

	Souliss_SetT12(memory_map, LOFTB1_PUMP_BOILER_FLOOR);
	Souliss_SetT12(memory_map, LOFTB1_PUMP_COLLECTOR_FANCOIL);
	Souliss_SetT12(memory_map, LOFTB1_PUMP_COLLECTOR_FLOOR);

	Set_Temperature_Setpoint(LOFTB1_TEMP_AMBIENCE_SETPOINT);
	Set_Temperature_Setpoint(LOFTB1_TEMP_FLOOR_FLOW_SETPOINT);
	Set_Temperature_Setpoint(LOFTB1_HVAC_BOILER_SANITARY_SETPOINT);

	Set_Temperature(LOFTB1_HVAC_BOILER_SANITARY_TEMP);
	Set_Temperature(LOFTB1_HVAC_BOILER_HEATING_TEMP);

	Souliss_SetT12(memory_map, LOFTB1_FANCOIL_MODE);
	Souliss_SetT12(memory_map, LOFTB1_HP_SETPOINT_2);

//	Souliss_SetT22(memory_map, LOFTB1_TEMP_SETPOINT_IN);

	Set_Analog_Setpoint(LOFTB1_COLLECTOR_FLOOR_MIX_VALVE_POS);

	// initialize values
	SetInput(LOFTB1_HEATPUMP_REMOTE_SWITCH, Souliss_T1n_OnCmd);
//	SetInput(LOFTB1_LIGHT_LOFT_1, Souliss_T1n_OnCmd);

	Souliss_SetT22(memory_map, LOFTB1_HEATING_MIX_VALVE);

	Souliss_SetT11(memory_map, LOFTB1_HVAC_FULL_AUTO);
	Souliss_SetT11(memory_map, LOFTB1_HVAC_VALVES_AUTO);

	SetInput(LOFTB1_HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_PUMP_BOILER_FLOOR, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_PUMP_COLLECTOR_FANCOIL, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_PUMP_COLLECTOR_FLOOR, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_FLOOR_MODE, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_FANCOIL_MODE, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_HP_SETPOINT_2, Souliss_T1n_AutoCmd);
	SetInput(LOFTB1_HVAC_VALVES_AUTO, Souliss_T1n_OnCmd); // start with all valves auto mode

	float set_point = AMBIENCE_SETPOINT_DEFAULT_HEAT; // initial setup
	ImportAnalog(LOFTB1_TEMP_AMBIENCE_SETPOINT, &set_point);

	set_point = TEMP_FLOOR_FLOW_SETPOINT_DEFAULT; // initial setup
	ImportAnalog(LOFTB1_TEMP_FLOOR_FLOW_SETPOINT, &set_point);

	set_point = SANITARY_WATER_SETPOINT_DEFAULT; // initial setup
	ImportAnalog(LOFTB1_HVAC_BOILER_SANITARY_SETPOINT, &set_point);

	set_point = COLLECTOR_FLOOR_MIX_VALVE_MAX;
	ImportAnalog(LOFTB1_COLLECTOR_FLOOR_MIX_VALVE_POS, &set_point);
}
