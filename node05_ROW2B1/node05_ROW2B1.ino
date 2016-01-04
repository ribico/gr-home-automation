
/*********************
BOARD ROW1B1
DINO with Ethernet and RS485 acting as Bridge (ETH->RS485)

***********************/
//#define DEBUG

#include "bconf/DINo_v2_EthernetBridge_RS485.h"
#include "conf/SuperNode.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"
#include "HW_Setup_DINo_v2.h"

#include <DHT.h>
DHT dht(ONE_WIRE_PIN, DHT22);
float th=0;
//--------------------------------------

inline void DefineTypicals()
{
	Set_SimpleLight(ROW2B1_LIGHT_BALCONY2);
	Set_SimpleLight(ROW2B1_LIGHT_BEDROOM3);
	Souliss_SetT22(memory_map, ROW2B1_WINDOW_DINING);

	Set_Temperature(ROW2B1_TEMPERATURE);
	Set_Humidity(ROW2B1_HUMIDITY);
	dht.begin();


	Set_SimpleLight(ROW2B1_LIGHT_LOFT_1);
	Set_SimpleLight(ROW2B1_LIGHT_LOFT_2);
	Set_SimpleLight(ROW2B1_LIGHT_TERRACE_1);
	Set_SimpleLight(ROW2B1_LIGHT_TERRACE_2);
	Set_SimpleLight(ROW2B1_LIGHT_TERRACE_3);
	Set_SimpleLight(ROW2B1_LIGHT_TOILET);

	Set_Light(ROW2B1_LIGHT_SENSOR);

	Souliss_SetT12(memory_map, ROW2B1_HVAC_FLOOR_MODE);
	Souliss_SetT12(memory_map, ROW2B1_HEATPUMP_REMOTE_SWITCH);
	Souliss_SetT12(memory_map, ROW2B1_HEATPUMP_CIRCULATION_PUMP);
	Souliss_SetT12(memory_map, ROW2B1_HEATPUMP_SANITARY_WATER);
	Souliss_SetT12(memory_map, ROW2B1_HEATPUMP_COOL);

	Souliss_SetT1A(memory_map, ROW2B1_HVAC_ZONES);
	Souliss_SetT1A(memory_map, ROW2B1_HVAC_VALVES);

	Souliss_SetT22(memory_map, ROW2B1_HVAC_MAIN_3WAY_VALVE);

	Souliss_SetT12(memory_map, ROW2B1_HVAC_PUMP_BOILER_FLOOR);
	Souliss_SetT12(memory_map, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL);
	Souliss_SetT12(memory_map, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR);

	Set_Temperature(ROW2B1_TEMP_AMBIENCE_SET_POINT);
	Set_Temperature(ROW2B1_TEMP_FLOOR_FLOW_SETPOINT);

	Souliss_SetT12(memory_map, ROW2B1_HVAC_FANCOIL_MODE);
	Souliss_SetT12(memory_map, ROW2B1_HEATPUMP_SETPOINT_2);

	Souliss_SetT22(memory_map, ROW2B1_TEMP_SETPOINT_IN);

	// initialize values
	SetInput(ROW2B1_HEATPUMP_REMOTE_SWITCH, Souliss_T1n_OnCmd);
//	SetInput(LIGHT_LOFT_1, Souliss_T1n_OnCmd);

	Souliss_SetT22(memory_map, ROW2B1_HVAC_HEATING_MIX_VALVE);


	SetInput(ROW2B1_HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd);
	SetInput(ROW2B1_HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_AutoCmd);
	SetInput(ROW2B1_HVAC_PUMP_BOILER_FLOOR, Souliss_T1n_AutoCmd);
	SetInput(ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL, Souliss_T1n_AutoCmd);
	SetInput(ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR, Souliss_T1n_AutoCmd);
//	SetInput(FLOOR_MODE, Souliss_T1n_AutoCmd);
	SetInput(ROW2B1_FANCOIL_MODE, Souliss_T1n_AutoCmd);
	SetInput(ROW2B1_HEATPUMP_SETPOINT_2, Souliss_T1n_AutoCmd);

	float set_point = AMBIENCE_SETPOINT_DEFAULT; // initial setup
	ImportAnalog(ROW2B1_TEMP_AMBIENCE_SET_POINT, &set_point);
}

inline void ReadInputs()
{
	Souliss_DigIn(IN1, Souliss_T1n_ToggleCmd, memory_map, ROW2B1_LIGHT_BALCONY2, true);
	Souliss_DigIn(IN2, Souliss_T1n_ToggleCmd, memory_map, ROW2B1_LIGHT_BEDROOM3, true);
	DigInWindowToggle(IN3, ROW2B1_WINDOW_DINING);
}

inline void ProcessLogics()
{
	Logic_SimpleLight(ROW2B1_LIGHT_BALCONY2);
	Logic_SimpleLight(ROW2B1_LIGHT_BEDROOM3);
	Souliss_Logic_T22(memory_map, ROW2B1_WINDOW_DINING, &data_changed, SHUTTER_SHORT_TIMEOUT);

	grh_Logic_Humidity(ROW2B1_HUMIDITY);
	grh_Logic_Temperature(ROW2B1_TEMPERATURE);


	Souliss_Logic_T12(memory_map, ROW2B1_HVAC_FLOOR_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HEATPUMP_REMOTE_SWITCH, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HEATPUMP_CIRCULATION_PUMP, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HEATPUMP_SANITARY_WATER, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HEATPUMP_COOL, &data_changed);

	grh_Logic_Temperature(ROW2B1_TEMP_AMBIENCE_SET_POINT);
	grh_Logic_Temperature(ROW2B1_TEMP_FLOOR_FLOW_SETPOINT);

	Souliss_Logic_T22(memory_map, ROW2B1_HVAC_MAIN_3WAY_VALVE, &data_changed, MAIN_3WAY_VALVE_TIMEOUT);
	Souliss_Logic_T1A(memory_map, ROW2B1_HVAC_VALVES, &data_changed);

	Souliss_Logic_T12(memory_map, ROW2B1_HVAC_PUMP_BOILER_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HVAC_FANCOIL_MODE, &data_changed);
	Souliss_Logic_T12(memory_map, ROW2B1_HEATPUMP_SETPOINT_2, &data_changed);
}

inline void SetOutputs()
{
	DigOut(RELAY1, Souliss_T1n_Coil, ROW2B1_LIGHT_BALCONY2);
	DigOut(RELAY2, Souliss_T1n_Coil, ROW2B1_LIGHT_BEDROOM3);
	DigOut(RELAY3, Souliss_T2n_Coil_Open,  ROW2B1_WINDOW_DINING);
	DigOut(RELAY4, Souliss_T2n_Coil_Close, ROW2B1_WINDOW_DINING);
}

inline void ProcessTimers()
{
	Timer_SimpleLight(ROW2B1_LIGHT_BALCONY2);
	Timer_SimpleLight(ROW2B1_LIGHT_BEDROOM3);
	Timer_Windows(ROW2B1_WINDOW_DINING);

	th = dht.readHumidity();
	ImportAnalog(ROW2B1_HUMIDITY, &th);

	th = dht.readTemperature();
	ImportAnalog(ROW2B1_TEMPERATURE, &th);

	Timer_SimpleLight(ROW2B1_LIGHT_LOFT_1);
	Timer_SimpleLight(ROW2B1_LIGHT_LOFT_2);
	Timer_SimpleLight(ROW2B1_LIGHT_TERRACE_1);
	Timer_SimpleLight(ROW2B1_LIGHT_TERRACE_2);
	Timer_SimpleLight(ROW2B1_LIGHT_TERRACE_3);
	Timer_SimpleLight(ROW2B1_LIGHT_TOILET);

	Souliss_T12_Timer(memory_map, ROW2B1_HVAC_FLOOR_MODE);
	Souliss_T12_Timer(memory_map, ROW2B1_HEATPUMP_REMOTE_SWITCH);
	Souliss_T12_Timer(memory_map, ROW2B1_HEATPUMP_CIRCULATION_PUMP);
	Souliss_T12_Timer(memory_map, ROW2B1_HEATPUMP_SANITARY_WATER);
	Souliss_T12_Timer(memory_map, ROW2B1_HEATPUMP_COOL);

	Souliss_T22_Timer(memory_map, ROW2B1_HVAC_MAIN_3WAY_VALVE);

	Souliss_T12_Timer(memory_map, ROW2B1_HVAC_PUMP_BOILER_FLOOR);
	Souliss_T12_Timer(memory_map, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL);
	Souliss_T12_Timer(memory_map, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR);

	Souliss_T12_Timer(memory_map, ROW2B1_HVAC_FANCOIL_MODE);
	Souliss_T12_Timer(memory_map, ROW2B1_HEATPUMP_SETPOINT_2);

	Timer_HeatingMixValve();

}




void setup()
{
	grhOpenSerialOnDebug();
//	delay(10000);

	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_ROW2B1);
	grhSetUSARTAddress(RS485_ADDRESS_ROW2B1, 0x0000);

	DefineTypicals();
}

void loop()
{
	EXECUTEFAST() {
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

		grhFastPeerComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
		SLOW_PeerJoin();
	}
}
