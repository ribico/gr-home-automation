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

#include "node00_LOFTB1_PinMapping.h"
#include "node00_LOFTB1_Slots.h"
#include "node00_LOFTB1_SpeakEasy.h"

#define TEMP_BOILER_SANITARY_PAD_RESISTANCE 	9800 // measured
#define TEMP_BOILER_HEATING_PAD_RESISTANCE      9800 // measured
#define TEMP_BOILER_BOTTOM_PAD_RESISTANCE       9800 // measured
#define TEMP_FLOOR_FLOW_PAD_RESISTANCE        	9830 // measured
#define TEMP_FLOOR_RETURN_PAD_RESISTANCE      	9820 // measured
#define TEMP_FANCOIL_FLOW_PAD_RESISTANCE      	9830 // measured


#define SETPOINT_TEMP_SANITARY_WATER_MAX 	45 // °C
#define SETPOINT_TEMP_SANITARY_WATER_MIN 	40 // °C
#define SETPOINT_TEMP_HEATING_WATER_MIN		26 // °C
#define SETPOINT_TEMP_HEATING_WATER_MAX		30 // °C

#define SETPOINT_TEMP_DEADBAND_SMALL	0.2	// °C

#define SETPOINT_UR_1				55
#define SETPOINT_UR_2				65
#define SETPOINT_UR_3				75

//#define SETPOINT_COOL_TEMP			25
//#define SETPOINT_HEAT_TEMP			19



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
	Set_SimpleLight(LIGHT_STAIRS);
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

	Set_Temperature(TEMP_AMBIENCE_SET_POINT);

	Souliss_SetT12(memory_map, HVAC_MODE);

	// initialize values
	SetInput(HEATPUMP_REMOTE_SWITCH, Souliss_T1n_OnCmd);
	SetInput(LIGHT_LOFT_1, Souliss_T1n_OnCmd);
	float set_point = 26; TODO("Should come from user interface");
	Souliss_ImportAnalog(memory_map, TEMP_AMBIENCE_SET_POINT, &set_point);

}

inline void ReadInputs()
{
	U8 ret = Souliss_LowDigIn(LIGHT_STAIRS_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_STAIRS, true);
	if( ret != MaCaco_NODATACHANGED)
		RemoteInput(RS485_ADDRESS_ROW1B3, 4, mInput(LIGHT_STAIRS));

	Souliss_LowDigIn(LIGHT_LOFT_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_LOFT_1, true);
	Souliss_LowDigIn(LIGHT_TERRACE_1_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_1, true);
	Souliss_LowDigIn(LIGHT_TERRACE_2_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_2, true);
	Souliss_LowDigIn(LIGHT_TERRACE_3_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TERRACE_3, true);
	Souliss_LowDigIn(LIGHT_TOILET_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_TOILET, true);

	
	mInput(HVAC_VALVES) = mOutput(HVAC_VALVES); 


	if (!digitalRead(MAIN_3WAY_VALVE_BOILER_LIMIT_PIN))
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) | MAIN_3WAY_VALVE_BOILER_MASK); // set boiler bit
	else
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) & ~MAIN_3WAY_VALVE_BOILER_MASK);	// unset boiler bit

	if (!digitalRead(MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN))
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) | MAIN_3WAY_VALVE_COLLECTOR_MASK); // set distribution bit
	else
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) & ~MAIN_3WAY_VALVE_COLLECTOR_MASK);	// unset distribution bit
}


inline void ProcessLogics()
{
	// standar slots logics
	Logic_SimpleLight(LIGHT_STAIRS);
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

	grh_Logic_Temperature(TEMP_BOILER_SANITARY);
	grh_Logic_Temperature(TEMP_BOILER_HEATING);
	grh_Logic_Temperature(TEMP_BOILER_BOTTOM);
	grh_Logic_Temperature(TEMP_FLOOR_FLOW);
	grh_Logic_Temperature(TEMP_FLOOR_RETURN);
	grh_Logic_Temperature(TEMP_FANCOIL_FLOW);
	grh_Logic_Temperature(TEMP_AMBIENCE_SET_POINT);

	Souliss_Logic_T22(memory_map, MAIN_3WAY_VALVE, &data_changed, MAIN_3WAY_VALVE_TIMEOUT);

	Souliss_Logic_T12(memory_map, PUMP_BOILER_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FANCOIL, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FLOOR, &data_changed);	
	Souliss_Logic_T12(memory_map, HVAC_MODE, &data_changed);	

	Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);
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


	if( !IsHpLogicAuto() )
		return; 



	// control SANITARY production hysteresys
	if( !IsSanitaryWaterInProduction() && IsSanitaryWaterRequested() )
		SanitaryWaterOn();
	
	else if ( IsSanitaryWaterInProduction() &&  IsSanitaryWaterNotRequested() )
		SanitaryWaterOff();

	if( IsSanitaryWaterInProduction() )
	{
		SetHpFlowToBoiler();
		return; // exit here, following code is for heating/cooling and we are currently producin Sanitary Water
	}



	if( !IsHvacOn() )
	{
		mInput(HVAC_ZONES) = 0; // no heating/cooling close all zone valves
		return;
	}

	// if here the system is requested to heat or cool the apartment


	// retrieve zone temperatures from remote nodes

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

	float UR_AVE = (UR_BED1+UR_BED2+UR_LIVING+UR_BED3+UR_KITCHEN+UR_DINING) / 6;

	float setpoint_temp = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT);


	mInput(HVAC_ZONES) = mOutput(HVAC_ZONES);

	// activate zones according to setpoint
	if( IsHeatMode() )
	{
		if( temp_BED1 < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED1);
		else if( temp_BED1 > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED1);

		if( temp_BATH1 < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BATH1);
		else if( temp_BATH1 > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH1);

		if( temp_BED2 < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED2);
		else if( temp_BED2 > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED2);

		if( temp_LIVING < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
		else if( temp_LIVING > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_LIVING);

		if( temp_BED3 < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED3);
		else if( temp_BED3 > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED3);

		if( temp_BATH2 < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BATH2);
		else if( temp_BATH2 > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH2);

		if( temp_KITCHEN < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_KITCHEN);
		else if( temp_KITCHEN > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_KITCHEN);

	}
	else if( IsCoolMode() )
	{
		// do not cool baths floor
		SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH1);
		SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH2);

		if( temp_BED1 > setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED1);
		else if( temp_BED1 < setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED1);

		if( temp_BED2 > setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED2);
		else if( temp_BED2 < setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED2);

		if( temp_LIVING > setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
		else if( temp_LIVING < setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_LIVING);

		if( temp_BED3 > setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED3);
		else if( temp_BED3 < setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED3);

		if( temp_KITCHEN < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_KITCHEN);
		else if( temp_KITCHEN > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL)
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_KITCHEN);
	}

	Souliss_Logic_T1A(memory_map, HVAC_ZONES, &data_changed);



	if( IsHeating() ) 
	{	
		PumpBoilerToFloorOn();	// using hot water from boiler only
		PumpCollectorToFloorOff();
		PumpCollectorToFancoilOff();
		FancoilGW1_Off();
		FancoilGW2_Off();

		// control hot water storage if there's heating requests from any zone
		// otherwise just don't care about the temperature of the storage
		if ( !IsHotWaterInProduction() && IsHotWaterRequested() )
		{
			// should produce some hot water here
			SetHpFlowToBoiler(); // only needed when activating HP Circulation pump
			HpCirculationOn();
		}
		else if ( IsHotWaterInProduction() && IsHotWaterNotRequested() )
			HpCirculationOff();


		// adjust heating mix valve position in order to keep the SETPOINT_HEATING flow temperature
		if ( IsHotWaterTooHot() )
			HotMixValveOn_ColdDirection(); // mix valve on, direction relay off
		
		else if ( IsHotWaterTooCold() )
			HotMixValveOn_HotDirection(); // mix valve on, direction relay on
		
		else
			HotMixValveOff(); // mix valve off (hold the position)

	}

	else if( IsCooling() )
	{
		PumpBoilerToFloorOff(); // do not use boiler water
		SetHpFlowToCollector();	// always needed in cooling
		HpCirculationOn();	// Cold water needed
		PumpCollectorToFloorOn();
		HotMixValveOff();

		// check umidity average to eventually activate fancoils

		// commented in order to activate always fancoils when cooling
/*		if( UR_AVE <= SETPOINT_UR_1 )
		{
			PumpCollectorToFancoilOff();
			FancoilGW1_Off();
			FancoilGW2_Off();
		}
		else if( UR_AVE > SETPOINT_UR_1 && UR_AVE <= SETPOINT_UR_2)
*/
		if( UR_AVE <= SETPOINT_UR_2)
		{
			PumpCollectorToFancoilOn();
			FancoilGW1_Speed1();
			FancoilGW2_Speed1();
		}
		else if( UR_AVE > SETPOINT_UR_2 && UR_AVE <= SETPOINT_UR_3)
		{
			PumpCollectorToFancoilOn();
			FancoilGW1_Speed2();
			FancoilGW2_Speed2();
		}
		else if( UR_AVE > SETPOINT_UR_3)
		{
			PumpCollectorToFancoilOn();
			FancoilGW1_Speed3();
			FancoilGW2_Speed3();
		}

	}

	else
	{
		PumpBoilerToFloorOff();
		PumpCollectorToFloorOff();
		PumpCollectorToFancoilOff();
		FancoilGW1_Off();
		FancoilGW2_Off();		
		HpCirculationOff();	
		HotMixValveOff();	
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

	// define static routing table (NOT STRICTLY NEEDED if using PEER_Join in peers)
	RoutingTable(IP_ADDRESS_ROW1B1, RS485_ADDRESS_ROW1SN, 0);
	RoutingTable(IP_ADDRESS_ROW2B1, RS485_ADDRESS_ROW2SN, 1);
	RoutingTable(IP_ADDRESS_BED1B1, RS485_ADDRESS_BED1SN, 2);
	RoutingTable(IP_ADDRESS_BED2B1, RS485_ADDRESS_BED2SN, 3);


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
