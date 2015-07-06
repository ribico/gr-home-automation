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


#define SETPOINT_TEMP_SANITARY_WATER_MIN 	40 // °C
#define SETPOINT_TEMP_SANITARY_WATER_MAX 	45 // °C
#define SETPOINT_TEMP_HEATING_WATER_MIN		26 // °C
#define SETPOINT_TEMP_HEATING_WATER_MAX		30 // °C

#define SETPOINT_TEMP_DEADBAND_SMALL	0.2	// °C

#define SETPOINT_UR_1				60
#define SETPOINT_UR_2				70
#define SETPOINT_UR_3				75

//#define SETPOINT_COOL_TEMP			25
//#define SETPOINT_HEAT_TEMP			19

#define COLLECTOR_FLOOR_MIX_VALVE_MIN	0
#define COLLECTOR_FLOOR_MIX_VALVE_MAX	220
U8 gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;



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

	Souliss_Logic_T12(memory_map, FLOOR_MODE, &data_changed);
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
	Souliss_Logic_T12(memory_map, FANCOIL_MODE, &data_changed);	
	Souliss_Logic_T12(memory_map, HP_SETPOINT_2, &data_changed);	

	Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);
}

inline void ProcessSlowLogics(U16 phase_fast)
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

	// control SANITARY production hysteresys
	if( !IsSanitaryWaterInProduction() && IsSanitaryWaterCold() )
	{
		if( IsHeating() || IsCooling() || 
			!IsPumpCollectorToFloorOff() || !IsPumpCollectorToFancoilOff() || !IsPumpBoilerToFloorOff() ||
			!IsHpFlowToBoiler() )
		{
			// no heating/cooling -> close all zone valves
			mInput(HVAC_ZONES) = HVAC_MASK_NO_ZONES;
			Souliss_Logic_T1A(memory_map, HVAC_ZONES, &data_changed);

			// stop all heating/cooling activities when procucting sanitary water	
			PumpCollectorToFloorOff();
			PumpCollectorToFancoilOff();
			PumpBoilerToFloorOff();
			HeatingMixValveOff();		

			// upstream to boiler
			SetHpFlowToBoiler();
			return;
		}

		SanitaryWaterOn(); // start producing Sanitary Water
		return; // should not exectute follwing code that could modify the HVAC status
	}
	else if ( IsSanitaryWaterInProduction() && !IsSanitaryWaterHot() )
	{
		return; // exit here, following code is for heating/cooling and we are currently producing Sanitary Water
	}

	// if here there's no need to produce Sanitary Water
	SanitaryWaterOff();


	if( IsFloorOff() )
	{
		// no heating/cooling -> close all zone valves
		mInput(HVAC_ZONES) = HVAC_MASK_NO_ZONES;

		// turn all pumps and valves off
		PumpBoilerToFloorOff();
		PumpCollectorToFloorOff();
		HeatingMixValveOff();			
	}
	else if( IsFloorOn() )
	{
		// AUTO MODE -> activate only needed zones

		float setpoint_temp = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT);
		float max_temp = setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL;
		float min_temp = setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL;

		// activate zones according to measured temperature according to setpoint
		mInput(HVAC_ZONES) = mOutput(HVAC_ZONES);

		if( IsHeatMode() )
		{
			if( temp_BED1 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED1);
			else if( temp_BED1 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED1);

			if( temp_BATH1 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BATH1);
			else if( temp_BATH1 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH1);

			if( temp_BED2 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED2);
			else if( temp_BED2 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED2);

			if( temp_LIVING < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
			else if( temp_LIVING > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_LIVING);

			if( temp_BED3 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED3);
			else if( temp_BED3 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED3);

			if( temp_BATH2 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BATH2);
			else if( temp_BATH2 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH2);

			if( temp_KITCHEN < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_KITCHEN);
			else if( temp_KITCHEN > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_KITCHEN);
		}
		else if( IsCoolMode() )
		{
			// do not cool baths floor
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH1);
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH2);

			if( temp_BED1 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED1);
			else if( temp_BED1 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED1);

			if( temp_BED2 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED2);
			else if( temp_BED2 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED2);

			if( temp_LIVING > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
			else if( temp_LIVING < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_LIVING);

			if( temp_BED3 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED3);
			else if( temp_BED3 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED3);

			if( temp_KITCHEN > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_KITCHEN);
			else if( temp_KITCHEN < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_KITCHEN);
		}
	}

	// process zones logics both for floor on and off
	Souliss_Logic_T1A(memory_map, HVAC_ZONES, &data_changed);

	if( IsHeating() ) // heating at least one zone
	{	
		if( IsPumpCollectorToFloorOff() && IsPumpCollectorToFancoilOff() && IsFancoilOff() )
		{
			PumpBoilerToFloorOn();	// using hot water from boiler only, collector pumps off
	
			// adjust heating mix valve position in order to keep the SETPOINT_HEATING flow temperature
			if ( IsHeatingWaterTooHot() )
				HeatingMixValveOn_ColdDirection(); // mix valve on, direction relay off
			
			else if ( IsHeatingWaterTooCold() )
				HeatingMixValveOn_HotDirection(); // mix valve on, direction relay on
			
			else
				HeatingMixValveOff(); // mix valve off (hold the position)
		}
		else
		{
			PumpCollectorToFloorOff();
			PumpCollectorToFancoilOff();
			Fancoil_Off(phase_fast%2);
		}

		// control hot water storage if there's heating requests from any zone
		// otherwise just don't care about the temperature of the storage
		if ( !IsHotWaterInProduction() && IsHotWaterCold() )
		{
			// should produce some hot water here
			if(IsHpFlowToBoiler())
			{
				HpSetpoint1(); // always setpoint 1, explicit call it here.
				HpCirculationOn();
			}
			else
			{
				HpCirculationOff();
				SetHpFlowToBoiler(); // only needed when activating HP Circulation pump
			}
		}
		else if ( IsHotWaterInProduction() && IsHotWaterHot() )
			HpCirculationOff(); // stop producing hot water
	}
	else if( IsCooling() ) // cooling at least one zone
	{
		float UR_MAX = UR_BED1;
		if(UR_MAX < UR_BED2) UR_MAX = UR_BED2;
		if(UR_MAX < UR_LIVING) UR_MAX = UR_LIVING;
		if(UR_MAX < UR_BED3) UR_MAX = UR_BED3;
		if(UR_MAX < UR_KITCHEN) UR_MAX = UR_KITCHEN;
		if(UR_MAX < UR_DINING) UR_MAX = UR_DINING;

		float temp_MAX = temp_BED1;
		if(temp_MAX < temp_BED2) temp_MAX = temp_BED2;
		if(temp_MAX < temp_LIVING) temp_MAX = temp_LIVING;
		if(temp_MAX < temp_BED3) temp_MAX = temp_BED3;
		if(temp_MAX < temp_KITCHEN) temp_MAX = temp_KITCHEN;
		if(temp_MAX < temp_DINING) temp_MAX = temp_DINING;

		float dew_point_MAX = temp_MAX-(100-UR_MAX)/5;
		float setpoint_cooling_water = dew_point_MAX;

		// always use setpoint 2 when cooling
		HpSetpoint2();

		if( IsHpFlowToCollector() && IsPumpBoilerToFloorOff())
		{
			HpCirculationOn();	// Cold water needed
			PumpCollectorToFloorOn();

			// control the collector-floor mix valve to keep the setpoint
			// simple proportional control on return floor temperature
			float error = setpoint_cooling_water - temperature_floor_return;

			if(gCollectorToFloorMixValvePos - error > COLLECTOR_FLOOR_MIX_VALVE_MAX)
				gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

			else if(gCollectorToFloorMixValvePos - error < COLLECTOR_FLOOR_MIX_VALVE_MIN)
				gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MIN;
			
			else
				gCollectorToFloorMixValvePos -= (U8) error;

			analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, gCollectorToFloorMixValvePos);
		}
		else
		{
			HpCirculationOff();
			SetHpFlowToCollector();	// always needed in cooling
			PumpCollectorToFloorOff(); // keep the pump off
			PumpBoilerToFloorOff(); // do not use boiler water
			HeatingMixValveOff();
		}


		if( IsFancoilAuto() )
		{
			// check umidity average to eventually activate fancoils
			float UR_AVE = (UR_BED1+UR_BED2+UR_LIVING+UR_BED3+UR_KITCHEN+UR_DINING) / 6;

			if( UR_AVE > SETPOINT_UR_1 )
			{
				if(IsHpFlowToCollector())
				{
					HpCirculationOn();
					PumpCollectorToFancoilOn();
				}
				else
				{
					HpCirculationOff();
					SetHpFlowToCollector();
				}
			}
			else
				PumpCollectorToFancoilOff();

			// choose the right fancoil fan speed
			if( UR_AVE > SETPOINT_UR_1 && UR_AVE <= SETPOINT_UR_2 )
	 			Fancoil_Speed1(phase_fast%2);

	 		else if( UR_AVE > SETPOINT_UR_2 && UR_AVE <= SETPOINT_UR_3 )
				Fancoil_Speed2(phase_fast%2);

			else if( UR_AVE > SETPOINT_UR_3 )
				Fancoil_Speed3(phase_fast%2);

			else
				Fancoil_Off(phase_fast%2);
		}
		else if( IsFancoilOn() )
		{
			if(IsHpFlowToCollector())
			{
				HpCirculationOn();
				PumpCollectorToFancoilOn();
		 		Fancoil_Speed1(phase_fast%2);
			}
			else
			{
				HpCirculationOff();
				SetHpFlowToCollector();
				Fancoil_Off(phase_fast%2);
			}
		}
		else
		{
			PumpCollectorToFancoilOff();
			Fancoil_Off(phase_fast%2);			
		}
	}
	else
	{
		// not heating and not cooling
		HpSetpoint1(); // simply turn off setpoint 2 relay
		HpCirculationOff();	
		PumpCollectorToFloorOff();
		PumpCollectorToFancoilOff();
		PumpBoilerToFloorOff();
		HeatingMixValveOff();
		Fancoil_Off(phase_fast%2);	
		return;		
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

	LowDigOut(HP_SETPOINT_2_PIN, Souliss_T1n_Coil, HP_SETPOINT_2);
}

inline void ProcessTimers()
{
	Timer_SimpleLight(LIGHT_LOFT_1);
	Timer_SimpleLight(LIGHT_LOFT_2);
	Timer_SimpleLight(LIGHT_TERRACE_1);
	Timer_SimpleLight(LIGHT_TERRACE_2);
	Timer_SimpleLight(LIGHT_TERRACE_3);
	Timer_SimpleLight(LIGHT_TOILET);
	
	Souliss_T12_Timer(memory_map, FLOOR_MODE);
	Souliss_T12_Timer(memory_map, HEATPUMP_REMOTE_SWITCH);
	Souliss_T12_Timer(memory_map, HEATPUMP_CIRCULATION_PUMP);
	Souliss_T12_Timer(memory_map, HEATPUMP_SANITARY_REQUEST);
	Souliss_T12_Timer(memory_map, HEATPUMP_COOL);

	Souliss_T22_Timer(memory_map, MAIN_3WAY_VALVE);

	Souliss_T12_Timer(memory_map, PUMP_BOILER_FLOOR);
	Souliss_T12_Timer(memory_map, PUMP_COLLECTOR_FANCOIL);
	Souliss_T12_Timer(memory_map, PUMP_COLLECTOR_FLOOR);

	Souliss_T12_Timer(memory_map, FANCOIL_MODE);
	Souliss_T12_Timer(memory_map, HP_SETPOINT_2);
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
			ProcessSlowLogics(phase_fast);
		}

		FAST_GatewayComms();	
	}
	
	EXECUTESLOW()
	{	
		UPDATESLOW();
	}
} 
