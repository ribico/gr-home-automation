#define LOFTB1 0
#   define LOFTB1_LIGHT_STAIRS										0
#   define LOFTB1_LIGHT_LOFT_1            				1
#   define LOFTB1_LIGHT_LOFT_2            				2
#   define LOFTB1_LIGHT_TERRACE_1                 3
#   define LOFTB1_LIGHT_TERRACE_2                 4
#   define LOFTB1_LIGHT_TERRACE_3                 5
#   define LOFTB1_LIGHT_TOILET                    6

#   define LOFTB1_FLOOR_MODE 			  							7
#   define LOFTB1_HEATPUMP_REMOTE_SWITCH          8
#   define LOFTB1_HEATPUMP_CIRCULATION_PUMP       9
#   define LOFTB1_HEATPUMP_SANITARY_WATER       	10
#   define LOFTB1_HEATPUMP_COOL                		11

#   define LOFTB1_HVAC_ZONES											12
#	    define HVAC_MASK_NO_ZONES							0x00
#	    define HVAC_MASK_ALL_ZONES						0xFF
#	    define HVAC_MASK_ALL_COOLING_ZONES (HVAC_MASK_BED1|HVAC_MASK_BED2|HVAC_MASK_LIVING|HVAC_MASK_BED3|HVAC_MASK_KITCHEN|HVAC_MASK_LOFT)
#	    define HVAC_MASK_BED1									0x01
#	    define HVAC_MASK_BATH1								0x02
#	    define HVAC_MASK_BED2									0x04
#	    define HVAC_MASK_LIVING								0x08
#	    define HVAC_MASK_BED3									0x10
#	    define HVAC_MASK_BATH2								0x20
#	    define HVAC_MASK_KITCHEN							0x40
#	    define HVAC_MASK_LOFT									0x80

#   define LOFTB1_HVAC_VALVES 												13
#	    define MAIN_3WAY_VALVE_BOILER_MASK				0x01
#	    define MAIN_3WAY_VALVE_COLLECTOR_MASK			0x02
#	    define HEATING_MIX_VALVE_SWITCH_MASK			0x04
#	    define HEATING_MIX_VALVE_DIRECTION_MASK		0x08

#   define LOFTB1_MAIN_3WAY_VALVE								14

#   define LOFTB1_PUMP_BOILER_FLOOR 						15
#   define LOFTB1_PUMP_COLLECTOR_FANCOIL 				16
#   define LOFTB1_PUMP_COLLECTOR_FLOOR 					17

#   define LOFTB1_FANCOIL_MODE									18
#   define LOFTB1_HP_SETPOINT_2									19

#   define LOFTB1_TEMP_AMBIENCE_SETPOINT				20
#   define LOFTB1_TEMP_AMBIENCE_SETPOINT_H			21

#   define LOFTB1_TEMP_FLOOR_FLOW_SETPOINT			22
#   define LOFTB1_TEMP_FLOOR_FLOW_SETPOINT_H		23

#   define LOFTB1_HEATING_MIX_VALVE							24

#   define LOFTB1_LIGHT_SENSOR									25
#   define LOFTB1_LIGHT_SENSOR_H								26

#   define LOFTB1_HVAC_FULL_AUTO					27
#   define XXXXX_SPARE				                28

#   define LOFTB1_HVAC_BOILER_SANITARY_TEMP			29
#   define LOFTB1_HVAC_BOILER_SANITARY_TEMP_H		30

#   define LOFTB1_HVAC_BOILER_SANITARY_SETPOINT			31
#   define LOFTB1_HVAC_BOILER_SANITARY_SETPOINT_H		32

#   define LOFTB1_COLLECTOR_FLOOR_MIX_VALVE_POS					34
#   define LOFTB1_COLLECTOR_FLOOR_MIX_VALVE_POS_H				35

#   define LOFTB1_HVAC_BOILER_HEATING_TEMP			36
#   define LOFTB1_HVAC_BOILER_HEATING_TEMP_H		37
// MAX SLOT 41 (VNET_PAYLOAD-MACACO_HEADER)

#define ROW1B1 1
# define ROW1B1_LIGHT_LIVING_1			                    0
# define ROW1B1_LIGHT_LIVING_2			                    1
# define ROW1B1_LIGHT_LIVING_3			                    2
# define ROW1B1_LIGHT_LIVING_4			                    3
# define ROW1B1_LIGHT_LIVING_5			                    4
# define ROW1B1_LIGHT_LIVING_6			                    5
# define ROW1B1_EXT_TEMP																6
# define ROW1B1_EXT_UR																	8
# define ROW1B1_LOFT_TEMP																10
# define ROW1B1_LOFT_UR																	12
# define ROW1B1_HVAC_BOILER_SANITARY_TEMP 							    14
# define ROW1B1_HVAC_BOILER_HEATING_TEMP 								16
# define ROW1B1_HVAC_BOILER_BOTTOM_TEMP 								18
# define ROW1B1_HVAC_HEATPUMP_FLOW_TEMP									20
# define ROW1B1_HVAC_HEATPUMP_RETURN_TEMP								22
# define ROW1B1_HVAC_FANCOILS_FLOW_TEMP  								24
# define ROW1B1_HVAC_FANCOILS_RETURN_TEMP 							    26
# define ROW1B1_HVAC_FLOOR_FLOW_TEMP									28
# define ROW1B1_HVAC_FLOOR_RETURN_TEMP									30

#define ROW1B2 2
# define ROW1B2_LIGHT_KITCHEN_1		                      1
# define ROW1B2_LIGHT_KITCHEN_2		                      2
# define ROW1B2_LIGHT_KITCHEN_3		                      3
# define ROW1B2_TEMPERATURE			                        5
# define ROW1B2_HUMIDITY			                          7

#define ROW1B3 3
# define ROW1B3_LIGHT_DINING_1		                      1
# define ROW1B3_LIGHT_DINING_2		                      2
# define ROW1B3_LIGHT_DINING_3		                      3
# define ROW1B3_LIGHT_STAIRS		                        4
# define ROW1B3_TEMPERATURE			                        5
# define ROW1B3_HUMIDITY			                          7

#define ROW1B4 4
# define ROW1B4_LIGHT_ENTRANCE_1	                      1
# define ROW1B4_LIGHT_ENTRANCE_2	                      2
# define ROW1B4_LIGHT_BATH2_1		                        3
# define ROW1B4_LIGHT_BATH2_2		                        4
# define ROW1B4_TEMPERATURE			                        5
# define ROW1B4_HUMIDITY			                          7

#define ROW2B1 5
# define ROW2B1_LIGHT_BALCONY2		                      1
# define ROW2B1_LIGHT_BEDROOM3		                      2
# define ROW2B1_WINDOW_DINING			                      3
# define ROW2B1_TEMPERATURE			                        4
# define ROW2B1_HUMIDITY				                        6

#define ROW2B2 6
# define ROW2B2_WINDOW_LIVING		                        1
# define ROW2B2_WINDOW_KITCHEN		                      2
# define ROW2B2_TEMPERATURE			                        3
# define ROW2B2_HUMIDITY			                          5

#define ROW2B3 7
# define ROW2B3_WINDOW_BED3		                          1
# define ROW2B3_WINDOW_BATH2	                          2
# define ROW2B3_TEMPERATURE		                        	3
# define ROW2B3_HUMIDITY			                          5

#define ROW2B4 8
# define ROW2B4_WATERING_ZONE6                          0
# define ROW2B4_TEMPERATURE			                        1
# define ROW2B4_HUMIDITY			                          3
# define ROW2B4_KITCHEN_POWER                           5
# define ROW2B4_LIGHT_EXT_DOOR                          6
# define ROW2B4_LIGHT_AQUARIUM                          7

#define BED1B1 9
# define BED1B1_LIGHT_BEDROOM1		                      1
# define BED1B1_LIGHT_BALCONY1		                      2
# define BED1B1_WINDOW_BEDROOM1		                      3
# define BED1B1_TEMPERATURE			                        4
# define BED1B1_HUMIDITY			                          6
# define BED1B1_REMOTE_LIGHT_GW1	                      8

#define BED2B1 10
# define BED2B1_LIGHT_BATH1_1		                        1
# define BED2B1_LIGHT_BATH1_2		                        2
# define BED2B1_LIGHT_BATH1_3		                        3
# define BED2B1_LIGHT_LIVING		                        4
# define BED2B1_TEMPERATURE			                        5
# define BED2B1_HUMIDITY			                          7
# define BED2B1_REMOTE_LIGHT_GW1	                      9

#define BED2B2 11
# define BED2B2_LIGHT_BEDROOM2_1			                  1
# define BED2B2_LIGHT_BEDROOM2_2			                  2
# define BED2B2_LIGHT_BEDROOM2_3			                  3
# define BED2B2_LIGHT_BEDROOM2_4			                  4
# define BED2B2_TEMPERATURE			                        5
# define BED2B2_HUMIDITY			                          7

#define BED2B3 12
# define BED2B3_WINDOW_BATH1		                        1
# define BED2B3_WINDOW_BEDROOM2		                      2
# define BED2B3_TEMPERATURE			                        3
# define BED2B3_HUMIDITY			                          5

#define GTW1B1 13
# define GTW1B1_FAN_SPEED1			                        1
# define GTW1B1_FAN_SPEED2			                        2
# define GTW1B1_FAN_SPEED3			                        3
# define GTW1B1_LIGHT_GW			                          4

#define GTW2B1 14
# define GTW2B1_FAN_SPEED1			                        1
# define GTW2B1_FAN_SPEED2			                        2
# define GTW2B1_FAN_SPEED3			                        3
# define GTW2B1_LIGHT_GW			                          4

#define GARDB1 15
# define GARDB1_LIGHT_GARDEN                            0
# define GARDB1_WATERING_ZONE1		                      1
# define GARDB1_WATERING_ZONE2		                      2
# define GARDB1_WATERING_ZONE3		                      3
# define GARDB1_WATERING_ZONE4		                      4
# define GARDB1_WATERING_ZONE5                          5
# define GARDB1_TOTAL_POWER                             6
# define GARDB1_TOTAL_POWER_H                           7

#define LYTB1 16
# define LYTB1_RGB_LIGHT                               0
# define LYTB1_AQUARIUM_LIGHT_DURATION                4
# define LYTB1_AQUARIUM_LIGHT_DURATION_H              5
# define LYTB1_WATERING_ZONE1_DURATION                  6
# define LYTB1_WATERING_ZONE1_DURATION_H                7
# define LYTB1_WATERING_ZONE2_DURATION                  8
# define LYTB1_WATERING_ZONE2_DURATION_H                9
# define LYTB1_WATERING_ZONE3_DURATION                  10
# define LYTB1_WATERING_ZONE3_DURATION_H                11
# define LYTB1_WATERING_ZONE4_DURATION                  12
# define LYTB1_WATERING_ZONE4_DURATION_H                13
# define LYTB1_WATERING_ZONE5_DURATION                  14
# define LYTB1_WATERING_ZONE5_DURATION_H                15
# define LYTB1_WATERING_ZONE6_DURATION                  16
# define LYTB1_WATERING_ZONE6_DURATION_H                17
# define LYTB1_SANITARY_WATER_PRODUCTION_DURATION       18
# define LYTB1_SANITARY_WATER_PRODUCTION_DURATION_H     19

#define LOFTB2 17
# define LOFTB2_HVAC_SPARE_SANITARY_WATER_TEMP			0
# define LOFTB2_HVAC_SOLAR_EXT_TEMP						2
# define LOFTB2_HVAC_SOLAR_INT_TEMP					    4
# define LOFTB2_HVAC_SOLAR_HEAT_EXC_TEMP			    6

