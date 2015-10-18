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


#define SETPOINT_TEMP_SANITARY_WATER_MIN 	30 // °C
#define SETPOINT_TEMP_SANITARY_WATER_MAX 	42 // °C
#define SETPOINT_TEMP_HEATING_WATER_MIN		26 // °C
#define SETPOINT_TEMP_HEATING_WATER_MAX		30 // °C

#define SETPOINT_TEMP_DEADBAND_SMALL	0.2	// °C

#define SETPOINT_UR_1				60
#define SETPOINT_UR_2				70
#define SETPOINT_UR_3				75
#define SETPOINT_UR_DEADBAND	3

//#define SETPOINT_COOL_TEMP			25
//#define SETPOINT_HEAT_TEMP			19

#define COLLECTOR_FLOOR_MIX_VALVE_MIN	0
#define COLLECTOR_FLOOR_MIX_VALVE_MAX	200
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
	Souliss_Logic_T12(memory_map, HEATPUMP_SANITARY_WATER, &data_changed);
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

inline void ProcessSlowLogics(U16 phase_fast)
{
	float temperature_sanitary = NTC10k_ToCelsius( TEMP_BOILER_SANITARY_PIN, TEMP_BOILER_SANITARY_PAD_RESISTANCE );
	ImportAnalog(TEMP_BOILER_SANITARY, &temperature_sanitary);

	float temperature_heating = NTC10k_ToCelsius( TEMP_BOILER_HEATING_PIN, TEMP_BOILER_HEATING_PAD_RESISTANCE );
	ImportAnalog(TEMP_BOILER_HEATING, &temperature_heating);

	float temperature_bottom = NTC10k_ToCelsius( TEMP_BOILER_BOTTOM_PIN, TEMP_BOILER_BOTTOM_PAD_RESISTANCE );
	ImportAnalog(TEMP_BOILER_BOTTOM, &temperature_bottom);

	float temperature_floor_flow = NTC10k_ToCelsius( TEMP_FLOOR_FLOW_PIN, TEMP_FLOOR_FLOW_PAD_RESISTANCE );
	ImportAnalog(TEMP_FLOOR_FLOW, &temperature_floor_flow);

	float temperature_floor_return = NTC10k_ToCelsius( TEMP_FLOOR_RETURN_PIN, TEMP_FLOOR_RETURN_PAD_RESISTANCE );
	ImportAnalog(TEMP_FLOOR_RETURN, &temperature_floor_return);

	float temperature_fancoil_flow = NTC10k_ToCelsius( TEMP_FANCOIL_FLOW_PIN, TEMP_FANCOIL_FLOW_PAD_RESISTANCE );
	ImportAnalog(TEMP_FANCOIL_FLOW, &temperature_fancoil_flow);

	// control SANITARY production hysteresys in Auto Mode
	if( (IsSanitaryWaterAutoOff() && IsSanitaryWaterCold()) || (IsSanitaryWaterAutoOn() && !IsSanitaryWaterHot()) )
	{
		SetHpFlowToBoiler(); 		// upstream to boiler
		SanitaryWaterAutoOnCmd(); // set to AutoOff automatically after T12 timeout

		return; // should not exectute following code that could modify the HVAC status
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
	else if( IsFloorAuto() ) // check zone temperatures to open/close valves
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


	if( IsHeating() ) // heating request for at least one zone
	{
		PumpBoilerToFloorAutoOnCmd();

		// the upstream floor temperature shall be adjusted 3 degrees above the ambiance setpoint
		// + a correction factor proportional to the temp error
		float fFloorTemp = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT) + 3 + (mOutputAsFloat(TEMP_AMBIENCE_SET_POINT)-temp_DINING);

		// adjust heating mix valve position in order to keep the SETPOINT_HEATING flow temperature
		if ( IsHeatingWaterTooHot(fFloorTemp) )
			HeatingMixValveOn_ColdDirection(); // mix valve on, direction relay off

		else if ( IsHeatingWaterTooCold(fFloorTemp) )
			HeatingMixValveOn_HotDirection(); // mix valve on, direction relay on

		else
			HeatingMixValveOff(); // mix valve off (hold the position)

		// control hot water storage if there's heating requests from any zone
		// otherwise just don't care about the temperature of the storage
		if( (!IsStorageWaterInProduction() && IsStorageWaterCold()) || (IsStorageWaterInProduction() && !IsStorageWaterHot()) )
		{
			SetHpFlowToBoiler();
			HpCirculationAutoOnCmd();
		}
	}
	else if( IsCooling() ) // cooling at least one zone
	{
		// produce cold water
		HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
		SetHpFlowToCollector();
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();

		// check the dew point to reduce floor water temperature
		float dew_point_BED1 = temp_BED1-(100-UR_BED1)/5;
		float dew_point_BED2 = temp_BED2-(100-UR_BED2)/5;
		float dew_point_LIVING = temp_LIVING-(100-UR_LIVING)/5;
		float dew_point_BED3 = temp_BED3-(100-UR_BED3)/5;
		float dew_point_KITCHEN = temp_KITCHEN-(100-UR_KITCHEN)/5;
		float dew_point_DINING = temp_DINING-(100-UR_DINING)/5;

		float dew_point_MAX = dew_point_BED1;
		dew_point_MAX = max(dew_point_MAX, dew_point_BED2);
		dew_point_MAX = max(dew_point_MAX, dew_point_LIVING);
		dew_point_MAX = max(dew_point_MAX, dew_point_BED3);
		dew_point_MAX = max(dew_point_MAX, dew_point_KITCHEN);
		dew_point_MAX = max(dew_point_MAX, dew_point_DINING);

		float setpoint_cooling_water = dew_point_MAX;

		// control the collector-floor mix valve to keep the setpoint
		// simple proportional control on return floor temperature
		float error = setpoint_cooling_water - (temperature_floor_flow + temperature_floor_return)/2;

		if(gCollectorToFloorMixValvePos - error > COLLECTOR_FLOOR_MIX_VALVE_MAX)
			gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

		else if(gCollectorToFloorMixValvePos - error < COLLECTOR_FLOOR_MIX_VALVE_MIN)
			gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MIN;

		else
			gCollectorToFloorMixValvePos -= (U8) error;

		analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, gCollectorToFloorMixValvePos);
	}

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
			ProcessSlowLogics(phase_fast);
			ProcessTimers();
		}

		FAST_GatewayComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
	}
}
