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

#define HEATPUMP_MANUAL_MODE   			13
#define HEATPUMP_REMOTE_SWITCH          14
#define HEATPUMP_CIRCULATION_PUMP       15
#define HEATPUMP_SANITARY_REQUEST       16
#define HEATPUMP_COOL                	17      

#define TEMP_FLOOR_FLOW 			18      // floor heating system water temp
#define TEMP_FLOOR_FLOW_H 			19
#define TEMP_FLOOR_RETURN  			20      // floor heating system water return temp
#define TEMP_FLOOR_RETURN_H 		21
#define TEMP_FANCOIL_FLOW  			22      // floor heating system water return temp
#define TEMP_FANCOIL_FLOW_H 		23

#define HVAC_ZONES					24
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