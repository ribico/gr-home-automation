/*********************
LOFTB1 BOARD 
MEGA with Ethernet only acting as GATEWAY

***********************/

//#define DEBUG

#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway_wLastin.h"

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

#define TEMP_BOILER_ACS                 7
#define TEMP_BOILER_ACS_H               8
#define TEMP_BOILER_HEATING             9
#define TEMP_BOILER_HEATING_H   		10
#define TEMP_BOILER_BOTTOM              11
#define TEMP_BOILER_BOTTOM_H    		12

#define HEATPUMP_MANUAL_MODE   			13
#define HEATPUMP_REMOTE_SWITCH          14
#define HEATPUMP_CIRCULATION_PUMP       15
#define HEATPUMP_ACS_REQUEST            16
#define HEATPUMP_COOL                	17      

#define TEMP_HEATING_FLOW 			18      // floor heating system water temp
#define TEMP_HEATING_FLOW_H 		19
#define TEMP_HEATING_RETURN  		20      // floor heating system water return temp
#define TEMP_HEATING_RETURN_H 		21

#define HVAC_ZONES					22
#	define HVAC_MASK_BED1			0x01
#	define HVAC_MASK_BATH1			0x02
#	define HVAC_MASK_BED2			0x04
#	define HVAC_MASK_LIVING			0x08
#	define HVAC_MASK_BED3			0x10
#	define HVAC_MASK_BATH2			0x20
#	define HVAC_MASK_KITCHEN		0x40
#	define HVAC_MASK_LOFT			0x80

#define HEATING_MIX_VALVE 						23
#	define HEATING_MIX_VALVE_SWITCH_MASK		0x01
#	define HEATING_MIX_VALVE_DIRECTION_MASK		0x02




#define TEMP_BOILER_ACS_PIN 	0       // analog pin used
#define TEMP_BOILER_HEATING_PIN 1       // analog pin used
#define TEMP_BOILER_BOTTOM_PIN  2       // analog pin used
#define TEMP_HEATING_FLOW_PIN   3       // analog pin used
#define TEMP_HEATING_RETURN_PIN 4       // analog pin used

#define TEMP_BOILER_ACS_PAD_RESISTANCE          9840 // measured
#define TEMP_BOILER_HEATING_PAD_RESISTANCE      9850 // measured
#define TEMP_BOILER_BOTTOM_PAD_RESISTANCE       9930 // measured
#define TEMP_HEATING_FLOW_PAD_RESISTANCE        9800 // measured
#define TEMP_HEATING_RETURN_PAD_RESISTANCE      9870 // measured


// DEFINE OUTPUT PINs
// ** DO NOT FORGET TO SET pinMode to OUTPUT **
#define HEATING_MIX_VALVE_SWITCH_PIN			28
#define HEATING_MIX_VALVE_DIRECTION_PIN			29
#define LIGHT_LOFT_1_PIN                        30
#define LIGHT_LOFT_2_PIN                        31
#define LIGHT_TERRACE_1_PIN                     32
#define LIGHT_TERRACE_2_PIN                     33
#define LIGHT_TERRACE_3_PIN                     34
#define LIGHT_TOILET_PIN                        35
#define HEATPUMP_REMOTE_SWITCH_PIN              36
#define HEATPUMP_CIRCULATION_PUMP_PIN   		37
#define HEATPUMP_ACS_REQUEST_PIN                38
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



#define SETPOINT_ACS            42 // °C
#define SETPOINT_HEATING        29 // °C
#define SETPOINT_DEADBAND       2



inline void DefinePinMode()
{
	pinMode(LIGHT_LOFT_1_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_LOFT_2_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_1_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_2_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TERRACE_3_PIN_IN, INPUT_PULLUP);
	pinMode(LIGHT_TOILET_PIN_IN, INPUT_PULLUP);
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
	digitalWrite(HEATPUMP_ACS_REQUEST_PIN, HIGH);
	digitalWrite(HEATPUMP_COOL_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BED_1_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BED_2_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BED_3_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BATH_1_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_BATH_2_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_KITCHEN_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_LIVING_PIN, HIGH);
	digitalWrite(ZONE_SWITCH_LOFT_PIN, HIGH);
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
	pinMode(HEATPUMP_ACS_REQUEST_PIN, OUTPUT);
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
	Souliss_SetT12(memory_map, HEATPUMP_ACS_REQUEST);
	Souliss_SetT12(memory_map, HEATPUMP_COOL);
	
	Set_Temperature(TEMP_BOILER_ACS);
	Set_Temperature(TEMP_BOILER_HEATING);
	Set_Temperature(TEMP_BOILER_BOTTOM);
	Set_Temperature(TEMP_HEATING_FLOW);
	Set_Temperature(TEMP_HEATING_RETURN);

	Souliss_SetT1A(memory_map, HVAC_ZONES);
	Souliss_SetT1A(memory_map, HEATING_MIX_VALVE);
}

inline void ReadInputs()
{
	Souliss_LowDigIn(LIGHT_LOFT_1_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_LOFT_1, true);
	Souliss_LowDigIn(LIGHT_LOFT_2_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_LOFT_2, true);
	Souliss_LowDigIn(LIGHT_TERRACE_1_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_1, true);
	Souliss_LowDigIn(LIGHT_TERRACE_2_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_2, true);
	Souliss_LowDigIn(LIGHT_TERRACE_3_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_3, true);
	Souliss_LowDigIn(LIGHT_TOILET_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TOILET, true);

}

inline void ProcessLogics()
{
	Logic_SimpleLight(LIGHT_LOFT_1);
	Logic_SimpleLight(LIGHT_LOFT_2);
	Logic_SimpleLight(LIGHT_TERRACE_1);
	Logic_SimpleLight(LIGHT_TERRACE_2);
	Logic_SimpleLight(LIGHT_TERRACE_3);
	Logic_SimpleLight(LIGHT_TOILET);

	Souliss_Logic_T12(memory_map, HEATPUMP_MANUAL_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_REMOTE_SWITCH, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_CIRCULATION_PUMP, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_ACS_REQUEST, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_COOL, &data_changed);

	Logic_Temperature(TEMP_BOILER_ACS);
	Logic_Temperature(TEMP_BOILER_HEATING);
	Logic_Temperature(TEMP_BOILER_BOTTOM);
	Logic_Temperature(TEMP_HEATING_FLOW);
	Logic_Temperature(TEMP_HEATING_RETURN);

	Souliss_Logic_T1A(memory_map, HVAC_ZONES, &data_changed);
	Souliss_Logic_T1A(memory_map, HEATING_MIX_VALVE, &data_changed);

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
	LowDigOut(HEATPUMP_ACS_REQUEST_PIN, Souliss_T1n_Coil, HEATPUMP_ACS_REQUEST);
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
		!(mOutput(HEATING_MIX_VALVE) & HEATING_MIX_VALVE_SWITCH_MASK) );
	digitalWrite(HEATING_MIX_VALVE_DIRECTION_PIN, 
		!(mOutput(HEATING_MIX_VALVE) & HEATING_MIX_VALVE_DIRECTION_MASK) );
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
	Souliss_T12_Timer(memory_map, HEATPUMP_ACS_REQUEST);
	Souliss_T12_Timer(memory_map, HEATPUMP_COOL);

	float temperature_acs = NTC_RawADCToCelsius( analogRead(TEMP_BOILER_ACS_PIN),TEMP_BOILER_ACS_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_BOILER_ACS, &temperature_acs);
	
	float temperature_heating = NTC_RawADCToCelsius( analogRead(TEMP_BOILER_HEATING_PIN),TEMP_BOILER_HEATING_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_BOILER_HEATING, &temperature_heating);
	
	float temperature_bottom = NTC_RawADCToCelsius( analogRead(TEMP_BOILER_BOTTOM_PIN),TEMP_BOILER_BOTTOM_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_BOILER_BOTTOM, &temperature_bottom);
	
	float temperature_heating_flow = NTC_RawADCToCelsius( analogRead(TEMP_HEATING_FLOW_PIN),TEMP_HEATING_FLOW_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_HEATING_FLOW, &temperature_heating_flow);
	
	float temperature_heating_return = NTC_RawADCToCelsius( analogRead(TEMP_HEATING_RETURN_PIN),TEMP_HEATING_RETURN_PAD_RESISTANCE );
	Souliss_ImportAnalog(memory_map, TEMP_HEATING_RETURN, &temperature_heating_return);  


	// retrieve zone temperatures

	float temp_BED1 	= 	pOutputAsFloat(9,5);
	float UR_BED1 		= 	pOutputAsFloat(9,7);
	float temp_BATH1 	=	pOutputAsFloat(12,4); 
	float UR_BATH1 		=	pOutputAsFloat(12,6); 
	float temp_BED2 	= 	pOutputAsFloat(10,6);
	float UR_BED2 		= 	pOutputAsFloat(10,8);
	float temp_LIVING 	= 	pOutputAsFloat(2,6);
	float UR_LIVING		= 	pOutputAsFloat(2,8);
	float temp_BED3 	= 	pOutputAsFloat(5,5);
	float UR_BED3 		= 	pOutputAsFloat(5,7);
	float temp_BATH2 	=	pOutputAsFloat(4,6); 
	float UR_BATH2 		=	pOutputAsFloat(4,8); 
	float temp_KITCHEN 	=	pOutputAsFloat(7,4); 
	float UR_KITCHEN	=	pOutputAsFloat(7,6); 
	float temp_DINING 	=	pOutputAsFloat(3,6); 
	float UR_DINING		=	pOutputAsFloat(3,8); 


	// SLOW LOGICS                
	
	if(mOutput(HEATPUMP_MANUAL_MODE) == Souliss_T1n_OffCoil) 
	{
		// control ACS production
		if (temperature_acs < SETPOINT_ACS - SETPOINT_DEADBAND)
			mInput(HEATPUMP_ACS_REQUEST) = Souliss_T1n_OnCmd;
		
		else if (temperature_acs > SETPOINT_ACS + SETPOINT_DEADBAND)
			mInput(HEATPUMP_ACS_REQUEST) = Souliss_T1n_OffCmd;

		// control water temperature of storage if there's heating requests from any zone
		// otherwise just don't care about the temperature of the storage
		if( mOutput(HVAC_ZONES) && mOutput(HEATPUMP_COOL) == Souliss_T1n_OffCoil )
		{	
			if (temperature_heating < SETPOINT_HEATING - SETPOINT_DEADBAND)
				mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OnCmd;
				
			else if (temperature_heating > SETPOINT_HEATING + SETPOINT_DEADBAND)
				mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OffCmd;
		}
		else
			mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OffCmd;

	}

	// adjust heating mix valve position in order to keep the SETPOINT_HEATING flow temperature
	if( mOutput(HVAC_ZONES) && mOutput(HEATPUMP_COOL) == Souliss_T1n_OffCoil )
	{
		// at least one zone is open
		// and the heatpump is not set to COOL mode

		if (temperature_heating_flow > SETPOINT_HEATING + SETPOINT_DEADBAND)
		{
			// mix valve on, direction relay off
			mInput(HEATING_MIX_VALVE) = HEATING_MIX_VALVE_SWITCH_MASK; 
		}	
		else if (temperature_heating_flow < SETPOINT_HEATING - SETPOINT_DEADBAND)
		{
			// mix valve on, direction relay on
			mInput(HEATING_MIX_VALVE) = HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK;
		}
		else
		{
			// mix valve off (hold the position)
			mInput(HEATING_MIX_VALVE) = 0x0;
		}		
	}
	else
	{
		// heating is off, keep the mix valve off.
		mInput(HEATING_MIX_VALVE) = 0x0;
	}

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
	grhSetIpAddress(IP_ADDRESS_TESTB1); 
	SetAsGateway(IP_ADDRESS_TESTB1);
	TODO(** Modify address to IP_ADDRESS_LOFTB1 **)
	SetAddressingServer();
	grhInitMEGA();

	// LIST OF NODES
	SetAsPeerNode(IP_ADDRESS_TESTB2,1);
	SetAsPeerNode(RS485_ADDRESS_TESTB2,2);

	TODO(SetAsPeerNode);

/*	SetAsPeerNode(RS485_ADDRESS_ROW1B3,3);
	SetAsPeerNode(RS485_ADDRESS_ROW1B4,4);

	SetAsPeerNode(IP_ADDRESS_ROW1B1,1);
	SetAsPeerNode(RS485_ADDRESS_ROW1B2,2);
	SetAsPeerNode(IP_ADDRESS_ROW2B1,5);
	SetAsPeerNode(RS485_ADDRESS_ROW2B2,6);
	SetAsPeerNode(RS485_ADDRESS_ROW2B3,7);
	SetAsPeerNode(RS485_ADDRESS_ROW2B4,8);

	SetAsPeerNode(IP_ADDRESS_BED1B1,9);

	SetAsPeerNode(IP_ADDRESS_BED2B1,10);
	SetAsPeerNode(RS485_ADDRESS_BED2B2,11);
	SetAsPeerNode(RS485_ADDRESS_BED2B3,12);
*/
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
		}

		FAST_GatewayComms();	
	}
	
	EXECUTESLOW()
	{	
		UPDATESLOW();

	}
} 