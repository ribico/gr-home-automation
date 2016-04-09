/*********************
LOFTB1 BOARD
MEGA with Ethernet only acting as GATEWAY

***********************/

//#define DEBUG
//#define DEBUG_SOULISS

#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway_wPersistence.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include <math.h>
#include <DHT.h>

#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhFancoils.h"
#include "HW_Setup_Mega_2560.h"
#include "NTC.h"

#include "OneWire.h"
#include "DallasTemperature.h"


#include "node00_LOFTB1_PinMapping.h"
#include "node00_LOFTB1_Parameters.h"
#include "node00_LOFTB1_Slots.h"
#include "node00_LOFTB1_SpeakEasy.h"
#include "node00_LOFTB1_ControlMixValves.h"
#include "node00_LOFTB1_HVAC_Logics.h"


inline void ReadInputs()
{
	U8 ret = Souliss_LowDigIn(LIGHT_STAIRS_PIN_IN, Souliss_T1n_ToggleCmd, memory_map, LIGHT_STAIRS, true);
	if( ret != MaCaco_NODATACHANGED)
		RemoteInput(RS485_ADDRESS_ROW1B3, ROW1B3_LIGHT_STAIRS, mInput(LIGHT_STAIRS));

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

	Souliss_SetT62(memory_map, HVAC_BOILER_SANITARY_SETPOINT);
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

	Logic_Light(LIGHT_SENSOR);

	Souliss_Logic_T12(memory_map, FLOOR_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_REMOTE_SWITCH, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_CIRCULATION_PUMP, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_SANITARY_WATER, &data_changed);
	Souliss_Logic_T12(memory_map, HEATPUMP_COOL, &data_changed);

	grh_Logic_Temperature(TEMP_AMBIENCE_SET_POINT);
	grh_Logic_Temperature(TEMP_FLOOR_FLOW_SETPOINT);
	grh_Logic_Temperature(HVAC_BOILER_SANITARY_TEMP);

	Souliss_Logic_T22(memory_map, MAIN_3WAY_VALVE, &data_changed, MAIN_3WAY_VALVE_TIMEOUT);
	Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);

	Souliss_Logic_T12(memory_map, PUMP_BOILER_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FANCOIL, &data_changed);
	Souliss_Logic_T12(memory_map, PUMP_COLLECTOR_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, FANCOIL_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, HP_SETPOINT_2, &data_changed);

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

	Souliss_Logic_T62(memory_map, HVAC_BOILER_SANITARY_SETPOINT, 0.1, &data_changed);
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

	#ifdef DEBUG
		if(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK)
		{
			Serial.print("SetOutputs - mOutput(HVAC_VALVES): ");
			Serial.println(mOutput(HVAC_VALVES), BIN);
			Serial.print("Move valve: ");
			Serial.print(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK);
			Serial.print(" Direction ");
			Serial.println(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_DIRECTION_MASK);
		}
	#endif
	// heating mix valve on/off and direction
	digitalWrite(HEATING_MIX_VALVE_SWITCH_PIN,
		!(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK) );
	digitalWrite(HEATING_MIX_VALVE_DIRECTION_PIN,
		!(mOutput(HVAC_VALVES) & HEATING_MIX_VALVE_DIRECTION_MASK) );

		if(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Open)
		{
			dWrite(MAIN_3WAY_VALVE_BOILER_PIN, LOW);
			dWrite(MAIN_3WAY_VALVE_COLLECTOR_PIN, HIGH);
		}
		else if(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Close)
		{
			dWrite(MAIN_3WAY_VALVE_BOILER_PIN, LOW);
			dWrite(MAIN_3WAY_VALVE_COLLECTOR_PIN, LOW);
		}
		else
		{
			dWrite(MAIN_3WAY_VALVE_BOILER_PIN, HIGH);
			dWrite(MAIN_3WAY_VALVE_COLLECTOR_PIN, HIGH);
		}

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

	Timer_HeatingMixValve();
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

	// Initialize dallas temperature sensors
	gTempSensors1.begin();
  gTempSensors1.setResolution(DALLAS_TEMP_RESOLUTION);
  gTempSensors1.setWaitForConversion(true); // asynchronous temp conversions
  gTempSensors1.requestTemperatures();

	gTempSensors2.begin();
  gTempSensors2.setResolution(DALLAS_TEMP_RESOLUTION);
  gTempSensors2.setWaitForConversion(true); // asynchronous temp conversions
  gTempSensors2.requestTemperatures();

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

	SetAsPeerNode(IP_ADDRESS_GARDB1,15);

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
			GetCurrentStatus(phase_fast);

		SHIFT_2110ms(2)
			ProcessSanitaryWaterRequest(phase_fast);

		SHIFT_2110ms(3)
			ProcessZonesActivation(phase_fast);

		SHIFT_2110ms(4)
			CalculateFloorTempSetpoint(phase_fast);

		SHIFT_2110ms(5)
			ProcessFloorRequest(phase_fast);

		SHIFT_2110ms(6)
			ProcessFancoilsRequest(phase_fast);

		SHIFT_2110ms(7)
			ProcessTimers();

		SHIFT_2110ms(8)
		{
			// logics to turn on night lights when lux is low
			float lux = mOutputAsFloat(LIGHT_SENSOR);
			if(lux < NIGHT_LIGHT_LUX_LIMIT)
				RemoteInput(IP_ADDRESS_GARDB1, GARDB1_LIGHT_NIGHT, Souliss_T1n_AutoCmd + 10);
		}

		FAST_GatewayComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
	}
}
