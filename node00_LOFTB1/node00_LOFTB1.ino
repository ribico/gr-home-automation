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
#include "node00_LOFTB1_ControlMixValves.h"

#include <math.h>

#define TEMP_BOILER_SANITARY_PAD_RESISTANCE 	9800 // measured
#define TEMP_BOILER_HEATING_PAD_RESISTANCE      9800 // measured
#define TEMP_BOILER_BOTTOM_PAD_RESISTANCE       9800 // measured
#define TEMP_FLOOR_FLOW_PAD_RESISTANCE        	9830 // measured
#define TEMP_FLOOR_RETURN_PAD_RESISTANCE      	9820 // measured
#define TEMP_FANCOIL_FLOW_PAD_RESISTANCE      	9830 // measured


#define SETPOINT_TEMP_SANITARY_WATER_MIN 	36
#define SETPOINT_TEMP_SANITARY_WATER_MAX 	43

#define SETPOINT_TEMP_DEADBAND_SMALL	0.2	// °C

#define SETPOINT_UR_1				60
#define SETPOINT_UR_2				70
#define SETPOINT_UR_3				75
#define SETPOINT_UR_DEADBAND	3


//--------------------------------------
// USED FOR DHT SENSOR
#include <DHT.h>
DHT dht_ext(EXT_DHT22_PIN, DHT22);
DHT dht_loft(LOFT_DHT22_PIN, DHT22);

// DHT PIN1 -> 5V
// DHT PIN2 -> Arduino EXT_DHT22 pin (INPUT_PULLUP)
// DHT PIN3 Not Used
// DHT PIN4 -> GND
//--------------------------------------


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
	Souliss_Logic_T12(memory_map, HEATPUMP_SANITARY_WATER, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_COOL, &data_changed);

	grh_Logic_Temperature(TEMP_BOILER_SANITARY);
	grh_Logic_Temperature(TEMP_BOILER_HEATING);
	grh_Logic_Temperature(TEMP_BOILER_BOTTOM);
	grh_Logic_Temperature(TEMP_FLOOR_FLOW);
	grh_Logic_Temperature(TEMP_FLOOR_RETURN);
	grh_Logic_Temperature(TEMP_FANCOIL_FLOW);
	grh_Logic_Temperature(TEMP_AMBIENCE_SET_POINT);
	grh_Logic_Temperature(TEMP_FLOOR_FLOW_SETPOINT);

	Souliss_Logic_T22(memory_map, MAIN_3WAY_VALVE, &data_changed, MAIN_3WAY_VALVE_TIMEOUT);

	Souliss_Logic_T12(memory_map, PUMP_BOILER_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FANCOIL, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, FANCOIL_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, HP_SETPOINT_2, &data_changed);

	Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);

	// use a T21 for increasing/decreasing ambience temp setpoint
	float ambience_setpoint = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT);
	if(mInput(TEMP_SETPOINT_IN) == Souliss_T2n_OpenCmd_SW)
	{
		mInput(TEMP_SETPOINT_IN) = Souliss_T2n_RstCmd;
		ambience_setpoint += 0.5;
		ImportAnalog(TEMP_AMBIENCE_SET_POINT, &ambience_setpoint);
	}
	if(mInput(TEMP_SETPOINT_IN) == Souliss_T2n_CloseCmd_SW)
	{
		mInput(TEMP_SETPOINT_IN) = Souliss_T2n_RstCmd;
		ambience_setpoint -= 0.5;
		ImportAnalog(TEMP_AMBIENCE_SET_POINT, &ambience_setpoint);
	}
}

float temperature_sanitary, temperature_heating, temperature_bottom;
float temperature_floor_flow, temperature_floor_return, temperature_fancoil_flow;

inline void ProcessSlowLogics1(U16 phase_fast)
{
	temperature_sanitary = NTC10k_ToCelsius( TEMP_BOILER_SANITARY_PIN, TEMP_BOILER_SANITARY_PAD_RESISTANCE );
	ImportAnalog(TEMP_BOILER_SANITARY, &temperature_sanitary);

	temperature_heating = NTC10k_ToCelsius( TEMP_BOILER_HEATING_PIN, TEMP_BOILER_HEATING_PAD_RESISTANCE );
	ImportAnalog(TEMP_BOILER_HEATING, &temperature_heating);

	temperature_bottom = NTC10k_ToCelsius( TEMP_BOILER_BOTTOM_PIN, TEMP_BOILER_BOTTOM_PAD_RESISTANCE );
	ImportAnalog(TEMP_BOILER_BOTTOM, &temperature_bottom);

	temperature_floor_flow = NTC10k_ToCelsius( TEMP_FLOOR_FLOW_PIN, TEMP_FLOOR_FLOW_PAD_RESISTANCE );
	ImportAnalog(TEMP_FLOOR_FLOW, &temperature_floor_flow);

	temperature_floor_return = NTC10k_ToCelsius( TEMP_FLOOR_RETURN_PIN, TEMP_FLOOR_RETURN_PAD_RESISTANCE );
	ImportAnalog(TEMP_FLOOR_RETURN, &temperature_floor_return);

	temperature_fancoil_flow = NTC10k_ToCelsius( TEMP_FANCOIL_FLOW_PIN, TEMP_FANCOIL_FLOW_PAD_RESISTANCE );
	ImportAnalog(TEMP_FANCOIL_FLOW, &temperature_fancoil_flow);


	// read and send external temp & UR to ROW1B1 slots
	float tmp;
	U8 buff[4]; // two bytes for temp and two bytes for UR

	tmp = dht_ext.readTemperature();
	Souliss_HalfPrecisionFloating(buff, &tmp);

	tmp = dht_ext.readHumidity();
	Souliss_HalfPrecisionFloating(buff+2, &tmp); // 2 bytes offset for UR

	SendData(IP_ADDRESS_ROW1B1, ROW1B1_EXT_TEMP, buff, 4); // sending 4 consecutive bytes (2 temp + 2 UR)

	tmp = dht_loft.readTemperature();
	Souliss_HalfPrecisionFloating(buff, &tmp);

	tmp = dht_loft.readHumidity();
	Souliss_HalfPrecisionFloating(buff+2, &tmp); // 2 bytes offset for UR

	SendData(IP_ADDRESS_ROW1B1, ROW1B1_LOFT_TEMP, buff, 4); // sending 4 consecutive bytes (2 temp + 2 UR)
}

inline void ProcessSlowLogics2(U16 phase_fast)
{
	// control SANITARY production hysteresys in Auto Mode
	if( (IsSanitaryWaterAutoOff() && IsSanitaryWaterCold()) || (IsSanitaryWaterAutoOn() && !IsSanitaryWaterHot()) )
	{
		SetHpFlowToBoiler(); 		// upstream to boiler
		SanitaryWaterAutoOnCmd(); // set to AutoOff automatically after T12 timeout

		return; // should not execute following code that could modify the HVAC status
	}


	if( IsFloorOn() ) 		// force all zones open
	{
		if( IsHeatMode() )
			mInput(HVAC_ZONES) = HVAC_MASK_ALL_ZONES;
		else if( IsCoolMode() )
			mInput(HVAC_ZONES) = HVAC_MASK_ALL_COOLING_ZONES;
	}
	else if ( IsFloorOff() ) // force all zones Close
	{
		mInput(HVAC_ZONES) = HVAC_MASK_NO_ZONES;
	}
	else if( IsFloorAuto() || IsFloorAutoOn() ) // check zone temperatures to open/close valves
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

			if( (temp_LIVING+temp_DINING)/2.0 < min_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
			else if( (temp_LIVING+temp_DINING)/2.0 > max_temp)
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

			if( (temp_LIVING+temp_DINING)/2.0 > max_temp)
				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
			else if( (temp_LIVING+temp_DINING)/2.0 < min_temp)
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
}

inline void ProcessSlowLogics3(U16 phase_fast)
{
	if( IsHeating() ) // heating request for at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback

		// control hot water storage if there's heating requests from any zone
		if( (!IsStorageWaterInProduction() && IsStorageWaterCold()) || (IsStorageWaterInProduction() && !IsStorageWaterHot()) )
		{
			SetHpFlowToBoiler();
			HpCirculationAutoOnCmd();
		}
		else
		{
			PumpBoilerToFloorAutoOnCmd();
			AdjustFloorFlowTemperature_HEATING();
		}
	}
	else if( IsCooling() ) // cooling at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback
		// produce cold water
		HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
		SetHpFlowToCollector();
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();
		AdjustFloorFlowTemperature_COOLING();
	}
}

inline void ProcessSlowLogics4(U16 phase_fast)
{
	if( IsCoolMode() )
	{
		// check the max UR to eventually activate fancoils
		float UR_MAX = UR_BED1;
		UR_MAX = max(UR_MAX, UR_BED2);
		UR_MAX = max(UR_MAX, UR_LIVING);
		UR_MAX = max(UR_MAX, UR_BED3);
		UR_MAX = max(UR_MAX, UR_KITCHEN);
		UR_MAX = max(UR_MAX, UR_DINING);

		// fancoils hysteresys in auto mode
		if( (IsFancoilsAutoOff() && (UR_MAX > SETPOINT_UR_1)) || (IsFancoilsAutoOn() && (UR_MAX > SETPOINT_UR_1 - SETPOINT_UR_DEADBAND)) )
				FancoilsAutoOnCmd();

		if( IsFancoilsOn() || IsFancoilsAutoOn() )
		{
			// produce cold water
			HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
			SetHpFlowToCollector();
			HpCirculationAutoOnCmd();
			PumpCollectorToFancoilAutoOnCmd();
			Fancoil_AutoCmd(phase_fast%2); TODO("adjust fancoils speed with UR hysteresys");
		}
	}
}


inline void SetOutputs()
{
	nLowDigOut(LIGHT_LOFT_1_PIN, Souliss_T1n_OnCoil, LIGHT_LOFT_1);
	nLowDigOut(LIGHT_LOFT_2_PIN, Souliss_T1n_OnCoil, LIGHT_LOFT_2);
	nLowDigOut(LIGHT_TERRACE_1_PIN, Souliss_T1n_OnCoil, LIGHT_TERRACE_1);
	nLowDigOut(LIGHT_TERRACE_2_PIN, Souliss_T1n_OnCoil, LIGHT_TERRACE_2);
	nLowDigOut(LIGHT_TERRACE_3_PIN, Souliss_T1n_OnCoil, LIGHT_TERRACE_3);
	nLowDigOut(LIGHT_TOILET_PIN, Souliss_T1n_OnCoil, LIGHT_TOILET);

	nLowDigOut(HEATPUMP_REMOTE_SWITCH_PIN, Souliss_T1n_OnCoil, HEATPUMP_REMOTE_SWITCH);
	nLowDigOut(HEATPUMP_CIRCULATION_PUMP_PIN, Souliss_T1n_OnCoil, HEATPUMP_CIRCULATION_PUMP);
	nLowDigOut(HEATPUMP_SANITARY_REQUEST_PIN, Souliss_T1n_OnCoil, HEATPUMP_SANITARY_WATER);
	nLowDigOut(HEATPUMP_COOL_PIN, Souliss_T1n_OnCoil, HEATPUMP_COOL);

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

	nLowDigOut(PUMP_BOILER_FLOOR_PIN, Souliss_T1n_OnCoil, PUMP_BOILER_FLOOR);
	nLowDigOut(PUMP_COLLECTOR_FANCOIL_PIN, Souliss_T1n_OnCoil, PUMP_COLLECTOR_FANCOIL);
	nLowDigOut(PUMP_COLLECTOR_FLOOR_PIN, Souliss_T1n_OnCoil, PUMP_COLLECTOR_FLOOR);

	nLowDigOut(HP_SETPOINT_2_PIN, Souliss_T1n_OnCoil, HP_SETPOINT_2);
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
	Souliss_T12_Timer(memory_map, HEATPUMP_SANITARY_WATER);
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
			ReadInputs();

		SHIFT_50ms(1)
			ProcessLogics();

		SHIFT_50ms(2)
			SetOutputs();


		SHIFT_2110ms(1)
			ProcessSlowLogics1(phase_fast);

		SHIFT_2110ms(2)
			ProcessSlowLogics2(phase_fast);

		SHIFT_2110ms(3)
			ProcessSlowLogics3(phase_fast);

		SHIFT_2110ms(4)
			ProcessSlowLogics4(phase_fast);

		SHIFT_2110ms(5)
			ProcessTimers();

		FAST_GatewayComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
	}
}
