

// DEFINE INPUT PINs
#define LIGHT_STAIRS_PIN_IN                     48
#define LIGHT_LOFT_PIN_IN                     	49
#define LIGHT_TERRACE_1_PIN_IN                  50
#define LIGHT_TERRACE_2_PIN_IN                  51
#define LIGHT_TERRACE_3_PIN_IN                  52
#define LIGHT_TOILET_PIN_IN                     53

#define TEMP_BOILER_SANITARY_PIN 	A0       // analog pin used
#define TEMP_BOILER_HEATING_PIN 	A1       // analog pin used
#define TEMP_BOILER_BOTTOM_PIN  	A2       // analog pin used
#define TEMP_FLOOR_FLOW_PIN   		A3       // analog pin used
#define TEMP_FLOOR_RETURN_PIN 		A4       // analog pin used
#define TEMP_FANCOIL_FLOW_PIN 		A5       // analog pin used

#define MAIN_3WAY_VALVE_BOILER_LIMIT_PIN			9
#define MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN		8

#define EXT_DHT22_PIN													7
#define LOFT_DHT22_PIN												6

#define COLLECTOR_FLOOR_MIX_VALVE_PIN					13

// DEFINE OUTPUT PINs
// ** DO NOT FORGET TO SET pinMode to OUTPUT **
#define HP_SETPOINT_2_PIN						22

#define PUMP_BOILER_FLOOR_PIN					23
#define PUMP_COLLECTOR_FANCOIL_PIN				24
#define PUMP_COLLECTOR_FLOOR_PIN				25
#define MAIN_3WAY_VALVE_BOILER_PIN				26
#define MAIN_3WAY_VALVE_COLLECTOR_PIN			27
#define HEATING_MIX_VALVE_HOT_PIN			28
#define HEATING_MIX_VALVE_COLD_PIN			29

#define LIGHT_LOFT_1_PIN                        30
#define LIGHT_LOFT_2_PIN                        31
#define LIGHT_TERRACE_1_PIN                     32
#define LIGHT_TERRACE_2_PIN                     33
#define LIGHT_TERRACE_3_PIN                     34
#define LIGHT_TOILET_PIN                        35

#define HEATPUMP_REMOTE_SWITCH_PIN              36
#define HEATPUMP_CIRCULATION_PUMP_PIN   				37
#define HEATPUMP_SANITARY_REQUEST_PIN           38
#define HEATPUMP_COOL_PIN                       39
#define ZONE_SWITCH_BED_1_PIN                   40
#define ZONE_SWITCH_BED_2_PIN                   41
#define ZONE_SWITCH_BED_3_PIN                   42
#define ZONE_SWITCH_BATH_1_PIN                  43
#define ZONE_SWITCH_BATH_2_PIN                  44
#define ZONE_SWITCH_KITCHEN_PIN                 45
#define ZONE_SWITCH_LIVING_PIN                  46
#define ZONE_SWITCH_LOFT_PIN                    47



inline void DefinePinMode()
{
	pinMode(LIGHT_STAIRS_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_LOFT_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_1_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_2_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_3_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TOILET_PIN_IN, INPUT_PULLUP);

	pinMode(MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN, INPUT_PULLUP);
	pinMode(MAIN_3WAY_VALVE_BOILER_LIMIT_PIN, INPUT_PULLUP);

	// set all pin to HIGH before switching to OUTPUT mode to avoid false relays activation
	digitalWrite(HP_SETPOINT_2_PIN, HIGH);
	digitalWrite(PUMP_BOILER_FLOOR_PIN, HIGH);
	digitalWrite(PUMP_COLLECTOR_FANCOIL_PIN, HIGH);
	digitalWrite(PUMP_COLLECTOR_FLOOR_PIN, HIGH);
	digitalWrite(MAIN_3WAY_VALVE_BOILER_PIN, HIGH);
	digitalWrite(MAIN_3WAY_VALVE_COLLECTOR_PIN, HIGH);
	digitalWrite(HEATING_MIX_VALVE_HOT_PIN, HIGH);
	digitalWrite(HEATING_MIX_VALVE_COLD_PIN, HIGH);
	digitalWrite(LIGHT_LOFT_1_PIN, HIGH);
	digitalWrite(LIGHT_LOFT_2_PIN, HIGH);
	digitalWrite(LIGHT_TERRACE_1_PIN, HIGH);
	digitalWrite(LIGHT_TERRACE_2_PIN, HIGH);
	digitalWrite(LIGHT_TERRACE_3_PIN, HIGH);
	digitalWrite(LIGHT_TOILET_PIN, HIGH);
	digitalWrite(HEATPUMP_REMOTE_SWITCH_PIN, HIGH);
	digitalWrite(HEATPUMP_CIRCULATION_PUMP_PIN, HIGH);
	digitalWrite(HEATPUMP_SANITARY_REQUEST_PIN, HIGH);
	digitalWrite(HEATPUMP_COOL_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BED_1_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BED_2_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BED_3_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BATH_1_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BATH_2_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_KITCHEN_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_LIVING_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_LOFT_PIN, HIGH);

	pinMode(HP_SETPOINT_2_PIN, OUTPUT);

	pinMode(PUMP_BOILER_FLOOR_PIN, OUTPUT);
	pinMode(PUMP_COLLECTOR_FANCOIL_PIN, OUTPUT);
	pinMode(PUMP_COLLECTOR_FLOOR_PIN, OUTPUT);

	pinMode(MAIN_3WAY_VALVE_BOILER_PIN, OUTPUT);
	pinMode(MAIN_3WAY_VALVE_COLLECTOR_PIN, OUTPUT);
	pinMode(HEATING_MIX_VALVE_HOT_PIN, OUTPUT);
	pinMode(HEATING_MIX_VALVE_COLD_PIN, OUTPUT);

	pinMode(LIGHT_LOFT_1_PIN, OUTPUT);
	pinMode(LIGHT_LOFT_2_PIN, OUTPUT);
	pinMode(LIGHT_TERRACE_1_PIN, OUTPUT);
	pinMode(LIGHT_TERRACE_2_PIN, OUTPUT);
	pinMode(LIGHT_TERRACE_3_PIN, OUTPUT);
	pinMode(LIGHT_TOILET_PIN, OUTPUT);

	pinMode(HEATPUMP_REMOTE_SWITCH_PIN, OUTPUT);
	pinMode(HEATPUMP_CIRCULATION_PUMP_PIN, OUTPUT);
	pinMode(HEATPUMP_SANITARY_REQUEST_PIN, OUTPUT);
	pinMode(HEATPUMP_COOL_PIN, OUTPUT);

	pinMode(ZONE_SWITCH_BED_1_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_BED_2_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_BED_3_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_BATH_1_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_BATH_2_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_KITCHEN_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_LIVING_PIN, OUTPUT);
	pinMode(ZONE_SWITCH_LOFT_PIN, OUTPUT);

	pinMode(EXT_DHT22_PIN, INPUT_PULLUP);
	pinMode(LOFT_DHT22_PIN, INPUT_PULLUP);
}
