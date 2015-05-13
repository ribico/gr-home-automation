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
#include "grhFancoils.h"
#include "HW_Setup_Mega_2560.h"
#include "NTC.h"

#include "board_LOFTB1_PinMapping.h"
#include "board_LOFTB1_Slots.h"
#include "board_LOFTB1_SpeakEasy.h"

#define TEMP_BOILER_SANITARY_PAD_RESISTANCE 	9800 // measured
#define TEMP_BOILER_HEATING_PAD_RESISTANCE      9800 // measured
#define TEMP_BOILER_BOTTOM_PAD_RESISTANCE       9800 // measured
#define TEMP_FLOOR_FLOW_PAD_RESISTANCE        	9830 // measured
#define TEMP_FLOOR_RETURN_PAD_RESISTANCE      	9820 // measured
#define TEMP_FANCOIL_FLOW_PAD_RESISTANCE      	9830 // measured


#define SETPOINT_SANITARY       42 // °C
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

	if( IsHpLogicAuto() ) 
	{
		// control SANITARY production
		if ( !IsSanitaryWaterInProduction() && (temperature_sanitary < SETPOINT_SANITARY - SETPOINT_DEADBAND) )
		{
			if( !IsHpFlowToBoiler() )
				SetHpFlowToBoiler();

			SanitaryWaterOn();
		}
		
		else if ( IsSanitaryWaterInProduction() && (temperature_sanitary > SETPOINT_SANITARY + SETPOINT_DEADBAND) )
			SanitaryWaterOff();


		// take care of the hot water storage temperature when heating from the boiler
		if( IsHeating() ) 
		{	
			if( !IsPumpBoilerToFloorOn() )
			{
				PumpBoilerToFloorOn();
				PumpCollectorToFloorOff();
				PumpCollectorToFancoilOff();
			}

			// control hot water storage if there's heating requests from any zone
			// otherwise just don't care about the temperature of the storage
			if ( !IsHotWaterInProduction() && (temperature_heating < SETPOINT_HEATING - SETPOINT_DEADBAND) )
			{
				// should produce some hot water here
				SetHpFlowToBoiler();
				HpCirculationOn();
			}
				
			else if ( IsHotWaterInProduction() && (temperature_heating > SETPOINT_HEATING + SETPOINT_DEADBAND) )
				HpCirculationOff();
		}

		else if( IsCooling() )
		{
			// when cooling never use the boiler
			if( !IsSanitaryWaterInProduction() )
				SetHpFlowToCollector();
		}

		
		if( !IsHeating() && !IsCooling() )
			HpCirculationOff();

	

		// adjust heating mix valve position in order to keep the SETPOINT_HEATING flow temperature
		// but only HEATPUMP is set to HEAT (HEATPUMP_COOL == OffCoil) and any zone request is active
		if( IsHeating() && IsPumpBoilerToFloorOn() )
		{
			// at least one zone is open
			// and the heatpump is not set to COOL mode

			if (temperature_floor_flow > SETPOINT_HEATING + SETPOINT_DEADBAND)
			{
				// mix valve on, direction relay off
				MixValveOn_ColdDirection(); 
			}	
			else if (temperature_floor_flow < SETPOINT_HEATING - SETPOINT_DEADBAND)
			{
				// mix valve on, direction relay on
				MixValveOn_HotDirection();
			}
			else
			{
				// mix valve off (hold the position)
				MixValveOff();
			}		
		}
		else
		{
			// heating is off, keep the mix valve off.
			MixValveOff();
		}
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

	SetAsPeerNode(RS485_ADDRESS_GTW1B1,13);
	SetAsPeerNode(RS485_ADDRESS_GTW2B1,14);

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
