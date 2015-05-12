/*********************
LOFTB1 BOARD 
MEGA with Ethernet only acting as GATEWAY

***********************/

//#define DEBUG

#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway_wPersistence.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_Mega_2560.h"
#include "NTC.h"


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
#	define MAIN_3WAY_VALVE_COLLECTOR_MASK	0x02
#	define HEATING_MIX_VALVE_SWITCH_MASK		0x04
#	define HEATING_MIX_VALVE_DIRECTION_MASK		0x08

#define MAIN_3WAY_VALVE				26

#define PUMP_BOILER_FLOOR 			27
#define PUMP_COLLECTOR_FANCOIL 		28
#define PUMP_COLLECTOR_FLOOR 		29


#define TEMP_BOILER_SANITARY_PIN 	A0       // analog pin used
#define TEMP_BOILER_HEATING_PIN 	A1       // analog pin used
#define TEMP_BOILER_BOTTOM_PIN  	A2       // analog pin used
#define TEMP_FLOOR_FLOW_PIN   		A3       // analog pin used
#define TEMP_FLOOR_RETURN_PIN 		A4       // analog pin used
#define TEMP_FANCOIL_FLOW_PIN 		A5       // analog pin used

#define TEMP_BOILER_SANITARY_PAD_RESISTANCE 	9800 // measured
#define TEMP_BOILER_HEATING_PAD_RESISTANCE      9800 // measured
#define TEMP_BOILER_BOTTOM_PAD_RESISTANCE       9800 // measured
#define TEMP_FLOOR_FLOW_PAD_RESISTANCE        	9830 // measured
#define TEMP_FLOOR_RETURN_PAD_RESISTANCE      	9820 // measured
#define TEMP_FANCOIL_FLOW_PAD_RESISTANCE      	9830 // measured


// DEFINE INPUT PIs
#define LIGHT_LOFT_1_PIN                        30
#define LIGHT_LOFT_2_PIN                        31
#define LIGHT_TERRACE_1_PIN                     32
#define LIGHT_TERRACE_2_PIN                     33
#define LIGHT_TERRACE_3_PIN                     34
#define LIGHT_TOILET_PIN                        35
#define MAIN_3WAY_VALVE_BOILER_LIMIT_PIN		9
#define MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN	8

// DEFINE OUTPUT PINs
// ** DO NOT FORGET TO SET pinMode to OUTPUT **
#define PUMP_BOILER_FLOOR_PIN					23
#define PUMP_COLLECTOR_FANCOIL_PIN				24
#define PUMP_COLLECTOR_FLOOR_PIN				25
#define MAIN_3WAY_VALVE_BOILER_PIN				26
#define MAIN_3WAY_VALVE_COLLECTOR_PIN			27
#define HEATING_MIX_VALVE_SWITCH_PIN			28
#define HEATING_MIX_VALVE_DIRECTION_PIN			29
#define HEATPUMP_REMOTE_SWITCH_PIN              36
#define HEATPUMP_CIRCULATION_PUMP_PIN   		37
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
#define LIGHT_LOFT_1_PIN_IN                     48
#define LIGHT_LOFT_2_PIN_IN                     49
#define LIGHT_TERRACE_1_PIN_IN                  50
#define LIGHT_TERRACE_2_PIN_IN                  51
#define LIGHT_TERRACE_3_PIN_IN                  52
#define LIGHT_TOILET_PIN_IN                     53



#define SETPOINT_SANITARY       42 // °C
#define SETPOINT_HEATING        29 // °C
#define SETPOINT_DEADBAND       2


#define HEATPUMP_AUTO 				(mOutput(HEATPUMP_MANUAL_MODE) == Souliss_T1n_OffCoil)
#define HEAT_MODE					(mOutput(HEATPUMP_COOL) == Souliss_T1n_OffCoil)
#define COOL_MODE					(mOutput(HEATPUMP_COOL) == Souliss_T1n_OnCoil)

#define ZONE_REQUEST				mOutput(HVAC_ZONES)
#define HEAT_ZONE_REQUEST			(ZONE_REQUEST && HEAT_MODE)
#define COOL_ZONE_REQUEST			(ZONE_REQUEST && COOL_MODE)

#define ZONE_REQUEST_CMD			mInput(HVAC_ZONES)
#define HEAT_ZONE_REQUEST_CMD		(ZONE_REQUEST_CMD && HEAT_MODE)
#define COOL_ZONE_REQUEST_CMD		(ZONE_REQUEST_CMD && COOL_MODE)
#define SANITARY_REQUEST_CMD 		(mInput(HEATPUMP_SANITARY_REQUEST) == Souliss_T1n_OnCmd)

#define SANITARY_PRODUCTION			(mOutput(HEATPUMP_SANITARY_REQUEST) == Souliss_T1n_OnCoil)
#define HOT_WATER_PRODUCTION		(HEAT_MODE && (mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil))
#define COOL_WATER_PRODUCTION		(COOL_MODE && (mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil))

#define FLOW_TO_BOILER 				(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK)
#define FLOW_TO_COLLECTOR			(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK)
#define SET_FLOW_TO_BOILER 			mInput(MAIN_3WAY_VALVE) = Souliss_T2n_OpenCmd_SW
#define SET_FLOW_TO_COLLECTOR	 	mInput(MAIN_3WAY_VALVE) = Souliss_T2n_CloseCmd_SW



inline void DefinePinMode()
{
	pinMode(LIGHT_LOFT_1_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_LOFT_2_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_1_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_2_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_3_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TOILET_PIN_IN, INPUT_PULLUP);

	pinMode(MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN, INPUT_PULLUP);
	pinMode(MAIN_3WAY_VALVE_BOILER_LIMIT_PIN, INPUT_PULLUP);

	// set all pin to HIGH before switching to OUTPUT mode to avoid false relays activation
	digitalWrite(PUMP_BOILER_FLOOR_PIN, HIGH);
	digitalWrite(PUMP_COLLECTOR_FANCOIL_PIN, HIGH);
	digitalWrite(PUMP_COLLECTOR_FLOOR_PIN, HIGH);
	digitalWrite(MAIN_3WAY_VALVE_BOILER_PIN, HIGH);
	digitalWrite(MAIN_3WAY_VALVE_COLLECTOR_PIN, HIGH);
	digitalWrite(HEATING_MIX_VALVE_SWITCH_PIN, HIGH);
	digitalWrite(HEATING_MIX_VALVE_DIRECTION_PIN, HIGH);
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
	
	pinMode(PUMP_BOILER_FLOOR_PIN, OUTPUT);
	pinMode(PUMP_COLLECTOR_FANCOIL_PIN, OUTPUT);
	pinMode(PUMP_COLLECTOR_FLOOR_PIN, OUTPUT);

	pinMode(MAIN_3WAY_VALVE_BOILER_PIN, OUTPUT);
	pinMode(MAIN_3WAY_VALVE_COLLECTOR_PIN, OUTPUT);
	pinMode(HEATING_MIX_VALVE_SWITCH_PIN, OUTPUT);
	pinMode(HEATING_MIX_VALVE_DIRECTION_PIN, OUTPUT);

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
}

inline void DefineTypicals()
{
	Set_SimpleLight(LIGHT_LOFT_1);
	Set_SimpleLight(LIGHT_LOFT_2);
	Set_SimpleLight(LIGHT_TERRACE_1);
	Set_SimpleLight(LIGHT_TERRACE_2);
	Set_SimpleLight(LIGHT_TERRACE_3);
	Set_SimpleLight(LIGHT_TOILET);

	Souliss_SetT12(memory_map, HEATPUMP_MANUAL_MODE);
	Souliss_SetT12(memory_map, HEATPUMP_REMOTE_SWITCH);
	Souliss_SetT12(memory_map, HEATPUMP_CIRCULATION_PUMP);
	Souliss_SetT12(memory_map, HEATPUMP_SANITARY_REQUEST);
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

	// initialize values
	Souliss_SetInput(memory_map, HEATPUMP_REMOTE_SWITCH, Souliss_T1n_RGBLamp_OnCmd);
	Souliss_SetInput(memory_map, LIGHT_LOFT_1, Souliss_T1n_OnCmd);
}

inline void ReadInputs()
{
	Souliss_LowDigIn(LIGHT_LOFT_1_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_LOFT_1, true);
	Souliss_LowDigIn(LIGHT_LOFT_2_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_LOFT_2, true);
	Souliss_LowDigIn(LIGHT_TERRACE_1_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_1, true);
	Souliss_LowDigIn(LIGHT_TERRACE_2_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_2, true);
	Souliss_LowDigIn(LIGHT_TERRACE_3_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_3, true);
	Souliss_LowDigIn(LIGHT_TOILET_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TOILET, true);

	
	mInput(HVAC_VALVES) = mOutput(HVAC_VALVES);

	if (!digitalRead(MAIN_3WAY_VALVE_BOILER_LIMIT_PIN))
		mInput(HVAC_VALVES) |= MAIN_3WAY_VALVE_BOILER_MASK; // set boiler bit

	else
		mInput(HVAC_VALVES) &= ~MAIN_3WAY_VALVE_BOILER_MASK;	// unset boiler bit

	// use mInput because the T1A Logic has not been called yet
	if (!digitalRead(MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN))
		mInput(HVAC_VALVES) |= MAIN_3WAY_VALVE_COLLECTOR_MASK; // set distribution bit

	else
		mInput(HVAC_VALVES) &= ~MAIN_3WAY_VALVE_COLLECTOR_MASK;	// unset distribution bit

}

inline void ProcessLogics()
{

	// cross logics between slots
	// to be executed before slots standard logics
	if( SANITARY_REQUEST_CMD ) // always performed in auto-mode
		SET_FLOW_TO_BOILER;

	if( HEATPUMP_AUTO ) // not executed in in manual mode
	{ 
		if (HEAT_ZONE_REQUEST && !FLOW_TO_BOILER)
			SET_FLOW_TO_BOILER;

		else if (COOL_ZONE_REQUEST && !FLOW_TO_COLLECTOR)
			SET_FLOW_TO_COLLECTOR;
	}


	// standar slots logics
	Logic_SimpleLight(LIGHT_LOFT_1);
	Logic_SimpleLight(LIGHT_LOFT_2);
	Logic_SimpleLight(LIGHT_TERRACE_1);
	Logic_SimpleLight(LIGHT_TERRACE_2);
	Logic_SimpleLight(LIGHT_TERRACE_3);
	Logic_SimpleLight(LIGHT_TOILET);

	Souliss_Logic_T12(memory_map, HEATPUMP_MANUAL_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_REMOTE_SWITCH, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_CIRCULATION_PUMP, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_SANITARY_REQUEST, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_COOL, &data_changed);

	Logic_Temperature(TEMP_BOILER_SANITARY);
	Logic_Temperature(TEMP_BOILER_HEATING);
	Logic_Temperature(TEMP_BOILER_BOTTOM);
	Logic_Temperature(TEMP_FLOOR_FLOW);
	Logic_Temperature(TEMP_FLOOR_RETURN);
	Logic_Temperature(TEMP_FANCOIL_FLOW);

	Souliss_Logic_T1A(memory_map, HVAC_ZONES, &data_changed);
	Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);

	Souliss_Logic_T22(memory_map, MAIN_3WAY_VALVE, &data_changed, MAIN_3WAY_VALVE_TIMEOUT);

	Souliss_Logic_T12(memory_map, PUMP_BOILER_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FANCOIL, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FLOOR, &data_changed);	
}

inline void ProcessSlowLogics()
{
	float temperature_sanitary = NTC_RawADCToCelsius( analogRead(TEMP_BOILER_SANITARY_PIN),TEMP_BOILER_SANITARY_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_BOILER_SANITARY, &temperature_sanitary);
	
	float temperature_heating = NTC_RawADCToCelsius( analogRead(TEMP_BOILER_HEATING_PIN),TEMP_BOILER_HEATING_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_BOILER_HEATING, &temperature_heating);
	
	float temperature_bottom = NTC_RawADCToCelsius( analogRead(TEMP_BOILER_BOTTOM_PIN),TEMP_BOILER_BOTTOM_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_BOILER_BOTTOM, &temperature_bottom);
	
	float temperature_floor_flow = NTC_RawADCToCelsius( analogRead(TEMP_FLOOR_FLOW_PIN),TEMP_FLOOR_FLOW_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_FLOOR_FLOW, &temperature_floor_flow);
	
	float temperature_floor_return = NTC_RawADCToCelsius( analogRead(TEMP_FLOOR_RETURN_PIN),TEMP_FLOOR_RETURN_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_FLOOR_RETURN, &temperature_floor_return);  

	float temperature_fancoil_flow = NTC_RawADCToCelsius( analogRead(TEMP_FANCOIL_FLOW_PIN),TEMP_FANCOIL_FLOW_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_FANCOIL_FLOW, &temperature_fancoil_flow);


	// retrieve zone temperatures

	float temp_BED1 	= 	pOutputAsFloat(9,4);
	float UR_BED1 		= 	pOutputAsFloat(9,6);
	float temp_BATH1 	=	pOutputAsFloat(12,3); 
	float UR_BATH1 		=	pOutputAsFloat(12,5); 
	float temp_BED2 	= 	pOutputAsFloat(10,5);
	float UR_BED2 		= 	pOutputAsFloat(10,7);
	float temp_LIVING 	= 	pOutputAsFloat(2,5);
	float UR_LIVING		= 	pOutputAsFloat(2,7);
	float temp_BED3 	= 	pOutputAsFloat(5,4);
	float UR_BED3 		= 	pOutputAsFloat(5,6);
	float temp_BATH2 	=	pOutputAsFloat(4,5); 
	float UR_BATH2 		=	pOutputAsFloat(4,7); 
	float temp_KITCHEN 	=	pOutputAsFloat(7,4); 
	float UR_KITCHEN	=	pOutputAsFloat(7,6); 
	float temp_DINING 	=	pOutputAsFloat(3,5); 
	float UR_DINING		=	pOutputAsFloat(3,7); 

/*
Serial.print("temp_BED1: ");
Serial.print(temp_BED1);
Serial.print("\ttemp_BATH1: ");
Serial.print(temp_BATH1);
Serial.print("\ttemp_BED2: ");
Serial.print(temp_BED2);
Serial.print("\ttemp_LIVING: ");
Serial.print(temp_LIVING);
Serial.print("\ttemp_BED3: ");
Serial.print(temp_BED3);
Serial.print("\ttemp_BATH2: ");
Serial.print(temp_BATH2);
Serial.print("\ttemp_KITCHEN: ");
Serial.print(temp_KITCHEN);
Serial.print("\ttemp_DINING: ");
Serial.println(temp_DINING);
*/
	if( HEATPUMP_AUTO ) 
	{
		// control SANITARY production
		if ( !SANITARY_PRODUCTION && (temperature_sanitary < SETPOINT_SANITARY - SETPOINT_DEADBAND) )
			mInput(HEATPUMP_SANITARY_REQUEST) = Souliss_T1n_OnCmd;
		
		else if ( SANITARY_PRODUCTION && (temperature_sanitary > SETPOINT_SANITARY + SETPOINT_DEADBAND) )
			mInput(HEATPUMP_SANITARY_REQUEST) = Souliss_T1n_OffCmd;


		if( HEAT_ZONE_REQUEST )
		{	
			SET_FLOW_TO_BOILER;

			// control hot water storage if there's heating requests from any zone
			// otherwise just don't care about the temperature of the storage
			if ( !HOT_WATER_PRODUCTION && (temperature_heating < SETPOINT_HEATING - SETPOINT_DEADBAND) )
				mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OnCmd;
				
			else if ( HOT_WATER_PRODUCTION && (temperature_heating > SETPOINT_HEATING + SETPOINT_DEADBAND) )
				mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OffCmd;
		}

		else if( COOL_ZONE_REQUEST )
		{
			SET_FLOW_TO_COLLECTOR;
		}

		else if( HOT_WATER_PRODUCTION )
			mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OffCmd;

	}


	// adjust heating mix valve position in order to keep the SETPOINT_HEATING flow temperature
	// but only HEATPUMP is set to HEAT (HEATPUMP_COOL == OffCoil) and any zone request is active
	if( HEAT_ZONE_REQUEST )
	{
		// at least one zone is open
		// and the heatpump is not set to COOL mode

		if (temperature_floor_flow > SETPOINT_HEATING + SETPOINT_DEADBAND)
		{
			// mix valve on, direction relay off
			mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) | HEATING_MIX_VALVE_SWITCH_MASK; 
		}	
		else if (temperature_floor_flow < SETPOINT_HEATING - SETPOINT_DEADBAND)
		{
			// mix valve on, direction relay on
			mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) | (HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK);
		}
		else
		{
			// mix valve off (hold the position)
			mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK;
		}		
	}
	else
	{
		// heating is off, keep the mix valve off.
		mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK;
	}



}


inline void SetOutputs()
{
	LowDigOut(LIGHT_LOFT_1_PIN, Souliss_T1n_Coil, LIGHT_LOFT_1);
	LowDigOut(LIGHT_LOFT_2_PIN, Souliss_T1n_Coil, LIGHT_LOFT_2);
	LowDigOut(LIGHT_TERRACE_1_PIN, Souliss_T1n_Coil, LIGHT_TERRACE_1);
	LowDigOut(LIGHT_TERRACE_2_PIN, Souliss_T1n_Coil, LIGHT_TERRACE_2);
	LowDigOut(LIGHT_TERRACE_3_PIN, Souliss_T1n_Coil, LIGHT_TERRACE_3);
	LowDigOut(LIGHT_TOILET_PIN, Souliss_T1n_Coil, LIGHT_TOILET);

	LowDigOut(HEATPUMP_REMOTE_SWITCH_PIN, Souliss_T1n_Coil, HEATPUMP_REMOTE_SWITCH);
	LowDigOut(HEATPUMP_CIRCULATION_PUMP_PIN, Souliss_T1n_Coil, HEATPUMP_CIRCULATION_PUMP);
	LowDigOut(HEATPUMP_SANITARY_REQUEST_PIN, Souliss_T1n_Coil, HEATPUMP_SANITARY_REQUEST);
	LowDigOut(HEATPUMP_COOL_PIN, Souliss_T1n_Coil, HEATPUMP_COOL);

	// HVAC zone valves
	digitalWrite(ZONE_SWITCH_KITCHEN_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_KITCHEN)	);
	digitalWrite(ZONE_SWITCH_BATH_2_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_BATH2) 	);
	digitalWrite(ZONE_SWITCH_BED_3_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_BED3) 	);
	digitalWrite(ZONE_SWITCH_LIVING_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_LIVING) 	);
	digitalWrite(ZONE_SWITCH_BED_2_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_BED2) 	);
	digitalWrite(ZONE_SWITCH_BATH_1_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_BATH1) 	);
	digitalWrite(ZONE_SWITCH_BED_1_PIN, 	!(mOutput(HVAC_ZONES) & HVAC_MASK_BED1)		);
	digitalWrite(ZONE_SWITCH_LOFT_PIN, 		!(mOutput(HVAC_ZONES) & HVAC_MASK_LOFT)		);

	// heating mix valve on/off and direction
	digitalWrite(HEATING_MIX_VALVE_SWITCH_PIN, 		
		!(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK) );
	digitalWrite(HEATING_MIX_VALVE_DIRECTION_PIN, 
		!(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_DIRECTION_MASK) );

	LowDigOut(MAIN_3WAY_VALVE_BOILER_PIN, Souliss_T2n_Coil_Open, MAIN_3WAY_VALVE);
	LowDigOut(MAIN_3WAY_VALVE_COLLECTOR_PIN, Souliss_T2n_Coil_Close, MAIN_3WAY_VALVE);

	LowDigOut(PUMP_BOILER_FLOOR_PIN, Souliss_T1n_Coil, PUMP_BOILER_FLOOR);	
	LowDigOut(PUMP_COLLECTOR_FANCOIL_PIN, Souliss_T1n_Coil, PUMP_COLLECTOR_FANCOIL);	
	LowDigOut(PUMP_COLLECTOR_FLOOR_PIN, Souliss_T1n_Coil, PUMP_COLLECTOR_FLOOR);	
}

inline void ProcessTimers()
{
	Timer_SimpleLight(LIGHT_LOFT_1);
	Timer_SimpleLight(LIGHT_LOFT_2);
	Timer_SimpleLight(LIGHT_TERRACE_1);
	Timer_SimpleLight(LIGHT_TERRACE_2);
	Timer_SimpleLight(LIGHT_TERRACE_3);
	Timer_SimpleLight(LIGHT_TOILET);
	
	Souliss_T12_Timer(memory_map, HEATPUMP_MANUAL_MODE);
	Souliss_T12_Timer(memory_map, HEATPUMP_REMOTE_SWITCH);
	Souliss_T12_Timer(memory_map, HEATPUMP_CIRCULATION_PUMP);
	Souliss_T12_Timer(memory_map, HEATPUMP_SANITARY_REQUEST);
	Souliss_T12_Timer(memory_map, HEATPUMP_COOL);

	Souliss_T22_Timer(memory_map, MAIN_3WAY_VALVE);

	Souliss_T12_Timer(memory_map, PUMP_BOILER_FLOOR);
	Souliss_T12_Timer(memory_map, PUMP_COLLECTOR_FANCOIL);
	Souliss_T12_Timer(memory_map, PUMP_COLLECTOR_FLOOR);
}


void setup()
{
	DefinePinMode();

	grhOpenSerialOnDebug();
/*	Serial.print("MaCaco_NODES : ");
	Serial.println(MaCaco_NODES);
	Serial.print("MaCaco_SLOT : ");
	Serial.println(MaCaco_SLOT);	
	Serial.print("MaCaco_MEMMAP : ");
	Serial.println(MaCaco_MEMMAP,HEX);*/
	Initialize();
	grhSetIpAddress(IP_ADDRESS_LOFTB1); 
	SetAsGateway(IP_ADDRESS_TESTB1);
	SetAddressingServer();
	grhInitMEGA();

	// LIST OF NODES
	SetAsPeerNode(IP_ADDRESS_ROW1B1,1);
	SetAsPeerNode(RS485_ADDRESS_ROW1B2,2);
	SetAsPeerNode(RS485_ADDRESS_ROW1B3,3);
	SetAsPeerNode(RS485_ADDRESS_ROW1B4,4);


	SetAsPeerNode(IP_ADDRESS_ROW2B1,5);
	SetAsPeerNode(RS485_ADDRESS_ROW2B2,6);
	SetAsPeerNode(RS485_ADDRESS_ROW2B3,7);
	SetAsPeerNode(RS485_ADDRESS_ROW2B4,8);

	SetAsPeerNode(IP_ADDRESS_BED1B1,9);

	SetAsPeerNode(IP_ADDRESS_BED2B1,10);
	SetAsPeerNode(RS485_ADDRESS_BED2B2,11);
	SetAsPeerNode(RS485_ADDRESS_BED2B3,12);

	DefineTypicals();
}


void loop()
{ 
	EXECUTEFAST()
	{						
		UPDATEFAST();
		
		FAST_30ms() 
		{
			ReadInputs();
		} 

		FAST_50ms() 
		{
			ProcessLogics();
			SetOutputs();
		}

		FAST_2110ms()
		{
			ProcessTimers();
			ProcessSlowLogics();
		}

		FAST_GatewayComms();	
	}
	
	EXECUTESLOW()
	{	
		UPDATESLOW();

	}
} 
