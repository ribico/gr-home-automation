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
#include "grhSoulissSlots.h"
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
	RemoteLowDigIn(LIGHT_STAIRS_PIN_IN, Souliss_T1n_ToggleCmd, RS485_ADDRESS_ROW1B3, ROW1B3_LIGHT_STAIRS);

	RemoteLowDigIn(LIGHT_LOFT_PIN_IN, Souliss_T1n_ToggleCmd, IP_ADDRESS_ROW2B1, ROW2B1_LIGHT_LOFT_1);
	RemoteLowDigIn(LIGHT_TERRACE_1_PIN_IN, Souliss_T1n_ToggleCmd, IP_ADDRESS_ROW2B1, ROW2B1_LIGHT_TERRACE_1);
	RemoteLowDigIn(LIGHT_TERRACE_2_PIN_IN, Souliss_T1n_ToggleCmd, IP_ADDRESS_ROW2B1, ROW2B1_LIGHT_TERRACE_2);
	RemoteLowDigIn(LIGHT_TERRACE_3_PIN_IN, Souliss_T1n_ToggleCmd, IP_ADDRESS_ROW2B1, ROW2B1_LIGHT_TERRACE_3);
	RemoteLowDigIn(LIGHT_TOILET_PIN_IN, Souliss_T1n_ToggleCmd, IP_ADDRESS_ROW2B1, ROW2B1_LIGHT_TOILET);


/*
	mInput(HVAC_VALVES) = mOutput(HVAC_VALVES);


	if (!digitalRead(MAIN_3WAY_VALVE_BOILER_LIMIT_PIN))
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) | MAIN_3WAY_VALVE_BOILER_MASK); // set boiler bit
	else
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) & ~MAIN_3WAY_VALVE_BOILER_MASK);	// unset boiler bit

	if (!digitalRead(MAIN_3WAY_VALVE_COLLECTOR_LIMIT_PIN))
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) | MAIN_3WAY_VALVE_COLLECTOR_MASK); // set distribution bit
	else
		SetInput(HVAC_VALVES, mInput(HVAC_VALVES) & ~MAIN_3WAY_VALVE_COLLECTOR_MASK);	// unset distribution bit
*/
}

inline void ReadSlowInputs()
{
/*
	int analog_val = analogRead(LIGHT_TRANSDUCER_PIN_IN);
	float light_intensity = 40000.0/5.0 * (1024-analog_val);
	ImportAnalog(LIGHT_SENSOR, &light_intensity);
*/
}



inline void ProcessLogics()
{
/*
	// use a T21 for increasing/decreasing ambience temp setpoint
	float ambience_setpoint = pOutputAsFloat(ROW2B1, ROW2B1_TEMP_AMBIENCE_SET_POINT);
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
*/
}



inline void SetOutputs()
{
	pnLowDigOut(LIGHT_LOFT_1_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_LIGHT_LOFT_1);
	pnLowDigOut(LIGHT_LOFT_2_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_LIGHT_LOFT_2);
	pnLowDigOut(LIGHT_TERRACE_1_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_LIGHT_TERRACE_1);
	pnLowDigOut(LIGHT_TERRACE_2_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_LIGHT_TERRACE_2);
	pnLowDigOut(LIGHT_TERRACE_3_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_LIGHT_TERRACE_3);
	pnLowDigOut(LIGHT_TOILET_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_LIGHT_TOILET);

	pnLowDigOut(HEATPUMP_REMOTE_SWITCH_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HEATPUMP_REMOTE_SWITCH);
	pnLowDigOut(HEATPUMP_CIRCULATION_PUMP_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HEATPUMP_CIRCULATION_PUMP);
	pnLowDigOut(HEATPUMP_SANITARY_REQUEST_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HEATPUMP_SANITARY_WATER);
	pnLowDigOut(HEATPUMP_COOL_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HEATPUMP_COOL);

	// HVAC zone valves
	digitalWrite(ZONE_SWITCH_KITCHEN_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_KITCHEN)	);
	digitalWrite(ZONE_SWITCH_BATH_2_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_BATH2) 	);
	digitalWrite(ZONE_SWITCH_BED_3_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_BED3) 	);
	digitalWrite(ZONE_SWITCH_LIVING_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_LIVING) 	);
	digitalWrite(ZONE_SWITCH_BED_2_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_BED2) 	);
	digitalWrite(ZONE_SWITCH_BATH_1_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_BATH1) 	);
	digitalWrite(ZONE_SWITCH_BED_1_PIN, 	!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_BED1)		);
	digitalWrite(ZONE_SWITCH_LOFT_PIN, 		!(pOutput(ROW2B1, ROW2B1_HVAC_ZONES) & HVAC_MASK_LOFT)		);

	#ifdef DEBUG
		if(pOutput(ROW2B1, ROW2B1_HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK)
		{
			Serial.print("SetOutputs - pOutput(ROW2B1, ROW2B1_HVAC_VALVES): ");
			Serial.println(pOutput(ROW2B1, ROW2B1_HVAC_VALVES), BIN);
			Serial.print("Move valve: ");
			Serial.print(pOutput(ROW2B1, ROW2B1_HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK);
			Serial.print(" Direction ");
			Serial.println(pOutput(ROW2B1, ROW2B1_HVAC_VALVES) & HEATING_MIX_VALVE_DIRECTION_MASK);
		}
	#endif
	// heating mix valve on/off and direction
	digitalWrite(HEATING_MIX_VALVE_SWITCH_PIN,
		!(pOutput(ROW2B1, ROW2B1_HVAC_VALVES) & HEATING_MIX_VALVE_SWITCH_MASK) );
	digitalWrite(HEATING_MIX_VALVE_DIRECTION_PIN,
		!(pOutput(ROW2B1, ROW2B1_HVAC_VALVES) & HEATING_MIX_VALVE_DIRECTION_MASK) );

	pLowDigOut(MAIN_3WAY_VALVE_BOILER_PIN, Souliss_T2n_Coil_Open, ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE);
	pLowDigOut(MAIN_3WAY_VALVE_COLLECTOR_PIN, Souliss_T2n_Coil_Close, ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE);

	pnLowDigOut(PUMP_BOILER_FLOOR_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HVAC_PUMP_BOILER_FLOOR);
	pnLowDigOut(PUMP_COLLECTOR_FANCOIL_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL);
	pnLowDigOut(PUMP_COLLECTOR_FLOOR_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR);

	pnLowDigOut(HP_SETPOINT_2_PIN, Souliss_T1n_OnCoil, ROW2B1, ROW2B1_HEATPUMP_SETPOINT_2);
}


inline void ProcessTimers()
{
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
	gTempSensors.begin();

  DeviceAddress tempDeviceAddress;
	gTempSensors.getAddress(tempDeviceAddress, 0);
  gTempSensors.setResolution(tempDeviceAddress, DALLAS_TEMP_RESOLUTION);

  gTempSensors.setWaitForConversion(false); // asynchronous temp conversions
  gTempSensors.requestTemperatures();

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
			GetCurrentStatus(phase_fast);

		SHIFT_2110ms(2)
			ProcessSanitaryWaterRequest(phase_fast);

		SHIFT_2110ms(3)
			ProcessZoneActivation(phase_fast);

		SHIFT_2110ms(4)
			CalculateFloorTempSetpoint(phase_fast);

		SHIFT_2110ms(5)
			ProcessFloorRequest(phase_fast);

		SHIFT_2110ms(6)
			ProcessFancoilsRequest(phase_fast);

		SHIFT_2110ms(7)
			ReadSlowInputs();

		SHIFT_2110ms(8)
			ProcessTimers();

		FAST_GatewayComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
	}
}
