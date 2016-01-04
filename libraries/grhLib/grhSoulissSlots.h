#define ROW1B1 1
# define ROW1B1_LIGHT_LIVING_1			                    0
# define ROW1B1_LIGHT_LIVING_2			                    1
# define ROW1B1_LIGHT_LIVING_3			                    2
# define ROW1B1_LIGHT_LIVING_4			                    3
# define ROW1B1_LIGHT_LIVING_5			                    4
# define ROW1B1_LIGHT_LIVING_6			                    5
// from node LOFTB1
# define ROW1B1_EXT_TEMP																6
# define ROW1B1_EXT_UR																	8
# define ROW1B1_LOFT_TEMP																10
# define ROW1B1_LOFT_UR																	12
# define ROW1B1_HVAC_BOILER_SANITARY_TEMP 							14
# define ROW1B1_HVAC_BOILER_HEATING_TEMP 								16
#	define ROW1B1_HVAC_BOILER_BOTTOM_TEMP 								18
#	define ROW1B1_HVAC_COLLECTOR_FLOW_TEMP								20
#	define ROW1B1_HVAC_COLLECTOR_RETURN_TEMP							22
#	define ROW1B1_HVAC_COLLECTOR_FLOOR_FLOW_TEMP		 			24
#	define ROW1B1_HVAC_COLLECTOR_FLOOR_RETURN_TEMP  			26
#	define ROW1B1_HVAC_COLLECTOR_FANCOILS_FLOW_TEMP  			28
#	define ROW1B1_HVAC_COLLECTOR_FANCOILS_RETURN_TEMP 		30
#	define ROW1B1_HVAC_BOILER_FLOOR_FLOW_TEMP							32
#	define ROW1B1_HVAC_BOILER_FLOOR_RETURN_TEMP						34
#	define ROW1B1_HVAC_HP_FLOW_TEMP												36
#	define ROW1B1_HVAC_HP_RETURN_TEMP											38
# define ROW1B1_HVAC_FLOOR_FLOW_TEMP										40
# define ROW1B1_HVAC_FLOOR_RETURN_TEMP									42

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
# define ROW1B4_LIGHT_BATH_1		                        3
# define ROW1B4_LIGHT_BATH_2		                        4
# define ROW1B4_TEMPERATURE			                        5
# define ROW1B4_HUMIDITY			                          7

#define ROW2B1 5
# define ROW2B1_LIGHT_BALCONY2		                      1
# define ROW2B1_LIGHT_BEDROOM3		                      2
# define ROW2B1_WINDOW_DINING			                      3
# define ROW2B1_TEMPERATURE			                        4
# define ROW2B1_HUMIDITY				                        6
// from node LOFTB1
# define ROW2B1_LIGHT_LOFT_1            				        8
# define ROW2B1_LIGHT_LOFT_2            				        9
# define ROW2B1_LIGHT_TERRACE_1                         10
# define ROW2B1_LIGHT_TERRACE_2                         11
# define ROW2B1_LIGHT_TERRACE_3                         12
# define ROW2B1_LIGHT_TOILET                            13
# define ROW2B1_LIGHT_SENSOR										        14
# define ROW2B1_LIGHT_SENSOR_H									        15
# define ROW2B1_HEATPUMP_REMOTE_SWITCH                  16
# define ROW2B1_HEATPUMP_CIRCULATION_PUMP               17
# define ROW2B1_HEATPUMP_SANITARY_WATER       	        18
# define ROW2B1_HEATPUMP_COOL                		        19
# define ROW2B1_HEATPUMP_SETPOINT_2				              21
# define ROW2B1_HVAC_FLOOR_MODE 			  							  22
# define ROW2B1_HVAC_ZONES					                    23
#	  define HVAC_MASK_NO_ZONES		              0x00
#	  define HVAC_MASK_ALL_ZONES	              0xFF
#	  define HVAC_MASK_ALL_COOLING_ZONES (HVAC_MASK_BED1|HVAC_MASK_BED2|HVAC_MASK_LIVING|HVAC_MASK_BED3|HVAC_MASK_KITCHEN|HVAC_MASK_LOFT)
#	  define HVAC_MASK_BED1			                0x01
#	  define HVAC_MASK_BATH1			              0x02
#	  define HVAC_MASK_BED2			                0x04
#	  define HVAC_MASK_LIVING			              0x08
#	  define HVAC_MASK_BED3			                0x10
#	  define HVAC_MASK_BATH2			              0x20
#	  define HVAC_MASK_KITCHEN		              0x40
#	  define HVAC_MASK_LOFT			                0x80
# define ROW2B1_HVAC_VALVES 							              24
#	  define MAIN_3WAY_VALVE_BOILER_MASK			  0x01
#	  define MAIN_3WAY_VALVE_COLLECTOR_MASK		  0x02
#	  define HEATING_MIX_VALVE_SWITCH_MASK		  0x04
#	  define HEATING_MIX_VALVE_DIRECTION_MASK		0x08
# define ROW2B1_HVAC_MAIN_3WAY_VALVE				            25
# define ROW2B1_HVAC_PUMP_BOILER_FLOOR 			            26
# define ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL 		        27
# define ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR 		          28
# define ROW2B1_HVAC_FANCOIL_MODE				                29
# define ROW2B1_HVAC_HEATING_MIX_VALVE	                30
# define ROW2B1_TEMP_AMBIENCE_SET_POINT		              31
# define ROW2B1_TEMP_AMBIENCE_SET_POINT_H	              32
# define ROW2B1_TEMP_FLOOR_FLOW_SETPOINT	              33
# define ROW2B1_TEMP_FLOOR_FLOW_SETPOINT_H	            34
# define ROW2B1_TEMP_SETPOINT_IN                        35





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
# define ROW1B4_TEMPERATURE			                        1
# define ROW1B4_HUMIDITY			                          3

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
# define GARDB1_WATERING_ZONE1		                      0
# define GARDB1_WATERING_ZONE2		                      1
# define GARDB1_WATERING_ZONE3		                      2
# define GARDB1_WATERING_ZONE4		                      3
# define GARDB1_LIGHT_NIGHT                             4
# define GARDB1_LIGHT_GARDEN			                      5
