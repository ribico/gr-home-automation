
#define SANITARY_WATER_SETPOINT_DEFAULT   44.0
#define SANITARY_WATER_SETPOINT_DB        2.0

#define SETPOINT_TEMP_DEADBAND_SMALL	0.2	// °C

#define SETPOINT_UR_1				65.0
#define SETPOINT_UR_2				70.0
#define SETPOINT_UR_3				75.0
#define SETPOINT_UR_DEADBAND	3.0

#define AMBIENCE_SETPOINT_DEFAULT_HEAT	19.0
#define AMBIENCE_SETPOINT_DEFAULT_COOL	25.5
#define AMBIENCE_SETPOINT_DELTA_BATHS   1.0
#define AMBIENCE_SETPOINT_DELTA_FULLAUTO    1.0

#define TEMP_FLOOR_FLOW_SETPOINT_DEFAULT   20.0
#define SETPOINT_TEMP_FLOOR_FLOW_MIN 17.0
#define SETPOINT_TEMP_FLOOR_FLOW_MAX 32.0

#define HEATING_MIX_VALVE_CYCLE 50
#define HEATING_MIX_VALVE_LONG_CYCLE 3000

#define MAIN_3WAY_VALVE_TIMEOUT		(Souliss_T2n_Timer_Off + 5)

#define HP_CMD_DELAY  30 // cycles delay after sanitary water production

#define COLLECTOR_FLOOR_MIX_VALVE_MIN	0
#define COLLECTOR_FLOOR_MIX_VALVE_MAX	200


#define NTC10K_A1_PAD_RESISTANCE   	9800 // measured

#define LUX_VALUE_NIGHT     25000 // below this value set the night mode
#define LUX_VALUE_SUN       30000 // above this value the sun is shining
