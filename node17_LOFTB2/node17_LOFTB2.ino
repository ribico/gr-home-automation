/*********************
BOARD ROW1B2
DINO with ETH only acting as Peer

***********************/

//#define DEBUG
#include "SoulissFramework.h"

#include "bconf/DINo_v2.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "grhSoulissSlots.h"
#include "HW_Setup_DINo_v2.h"

#include "OneWire.h"
#include "DallasTemperature.h"

//--------------------------------------
// USED FOR DALLAS TEMP SENSOR
OneWire gOneWire1(ONE_WIRE_PIN);
DallasTemperature gTempSensors1(&gOneWire1);

#define DALLAS_TEMP_RESOLUTION  11

#define DALLAS_1_ADDRESS  { 0x28, 0x12, 0x0F, 0x29, 0x07, 0x00, 0x00, 0xDD }
#define DALLAS_2_ADDRESS  { 0x28, 0xA1, 0x29, 0x2A, 0x07, 0x00, 0x00, 0x6F }
#define DALLAS_3_ADDRESS  { 0x28, 0x4F, 0x7F, 0x29, 0x07, 0x00, 0x00, 0xD4 }
#define DALLAS_4_ADDRESS  { 0x28, 0x9F, 0x1E, 0x29, 0x07, 0x00, 0x00, 0xCC }
#define DALLAS_5_ADDRESS  { 0x28, 0x57, 0xDC, 0x29, 0x07, 0x00, 0x00, 0x53 }
#define DALLAS_6_ADDRESS  { 0x28, 0xBB, 0xFC, 0x28, 0x07, 0x00, 0x00, 0x70 }
#define DALLAS_7_ADDRESS  { 0x28, 0x42, 0xF0, 0x28, 0x07, 0x00, 0x00, 0xB5 }
#define DALLAS_8_ADDRESS  { 0x28, 0xE1, 0x06, 0x29, 0x07, 0x00, 0x00, 0x05 }
#define DALLAS_9_ADDRESS  { 0x28, 0x2D, 0x35, 0x2A, 0x07, 0x00, 0x00, 0xA5 }
#define DALLAS_10_ADDRESS  { 0x28, 0xBA, 0xA2, 0x2A, 0x07, 0x00, 0x00, 0x77 }
#define DALLAS_11_ADDRESS  { 0x28, 0xEB, 0x32, 0x03, 0x07, 0x00, 0x00, 0x3A }
#define DALLAS_12_ADDRESS  { 0x28, 0x81, 0x22, 0x17, 0x07, 0x00, 0x00, 0x7C }
#define DALLAS_13_ADDRESS  { 0x28, 0xA3, 0xFE, 0x50, 0x07, 0x00, 0x00, 0xBD }
#define DALLAS_14_ADDRESS  { 0x28, 0xC8, 0xC8, 0x51, 0x07, 0x00, 0x00, 0x11 }

const DeviceAddress HVAC_BOILER_SANITARY_TEMP_ADDR = DALLAS_13_ADDRESS;
const DeviceAddress HVAC_BOILER_HEATING_TEMP_ADDR = DALLAS_14_ADDRESS;
const DeviceAddress HVAC_HEATPUMP_FLOW_TEMP_ADDR = DALLAS_5_ADDRESS;
const DeviceAddress HVAC_HEATPUMP_RETURN_TEMP_ADDR = DALLAS_10_ADDRESS;
const DeviceAddress HVAC_FANCOILS_FLOW_TEMP_ADDR = DALLAS_6_ADDRESS;
const DeviceAddress HVAC_FANCOILS_RETURN_TEMP_ADDR = DALLAS_9_ADDRESS;
const DeviceAddress HVAC_FLOOR_FLOW_TEMP_ADDR = DALLAS_4_ADDRESS;
const DeviceAddress HVAC_FLOOR_RETURN_TEMP_ADDR = DALLAS_11_ADDRESS;


#define IsTempValid(temp)                   ( (temp != DEVICE_DISCONNECTED_C) && (temp != 0.0) && (temp > -15.0) && (temp < 65.0) )

inline void ReadDallasTemp(DallasTemperature& sensor_group, const DeviceAddress address, float& ret_val, U8 retry = 3)
{
	for(int i=0; i<retry; i++)
	{
		ret_val = sensor_group.getTempC(address);
		if(IsTempValid(ret_val))
			return;
	}
}



inline void DefineTypicals()
{
	Set_Temperature(LOFTB2_HVAC_SANITARY_WATER_TEMP);
	Set_Temperature(LOFTB2_HVAC_BOILER_HEATING_TEMP);
	Set_Temperature(LOFTB2_HVAC_SOLAR_EXT_TEMP);
	Set_Temperature(LOFTB2_HVAC_SOLAR_INT_TEMP);
	Set_Temperature(LOFTB2_HVAC_SOLAR_HEAT_EXC_TEMP);
	Set_Temperature(LOFTB2_HVAC_HEATPUMP_FLOW_TEMP);
	Set_Temperature(LOFTB2_HVAC_HEATPUMP_RETURN_TEMP);
	Set_Temperature(LOFTB2_HVAC_FLOOR_FLOW_TEMP);
	Set_Temperature(LOFTB2_HVAC_FLOOR_RETURN_TEMP);
	Set_Temperature(LOFTB2_HVAC_FANCOILS_FLOW_TEMP);
	Set_Temperature(LOFTB2_HVAC_FANCOILS_RETURN_TEMP);
}

inline void ReadInputs()
{
	// input only coming from other boards
}

inline void ProcessLogics()
{
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SANITARY_WATER_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_BOILER_HEATING_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SOLAR_EXT_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SOLAR_INT_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_SOLAR_HEAT_EXC_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_HEATPUMP_FLOW_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_HEATPUMP_RETURN_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_FLOOR_FLOW_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_FLOOR_RETURN_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_FANCOILS_FLOW_TEMP);
	grh_Logic_HVAC_Temperature(LOFTB2_HVAC_FANCOILS_RETURN_TEMP);
}

inline void SetOutputs()
{

}

inline void ProcessTimers()
{

}


void setup()
{
	grhOpenSerialOnDebug();
	grhDINo_v2_HW_Setup();

	grhSetIpAddress(IP_ADDRESS_LOFTB2);

	DefineTypicals();

	gTempSensors1.begin();
	gTempSensors1.setResolution(DALLAS_TEMP_RESOLUTION);
	gTempSensors1.setWaitForConversion(true); // asynchronous temp conversions
	gTempSensors1.requestTemperatures();
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
			float tmp;
			ReadDallasTemp(gTempSensors1, HVAC_BOILER_SANITARY_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_SANITARY_WATER_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_SANITARY_WATER_TEMP, &tmp);

			ReadDallasTemp(gTempSensors1, HVAC_BOILER_HEATING_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_BOILER_HEATING_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_BOILER_HEATING_TEMP, &tmp);
		
			ReadDallasTemp(gTempSensors1, HVAC_HEATPUMP_FLOW_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_HEATPUMP_FLOW_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_HEATPUMP_FLOW_TEMP, &tmp);

			ReadDallasTemp(gTempSensors1, HVAC_HEATPUMP_RETURN_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_HEATPUMP_RETURN_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_HEATPUMP_RETURN_TEMP, &tmp);

			ReadDallasTemp(gTempSensors1, HVAC_FANCOILS_FLOW_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_FANCOILS_FLOW_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_FANCOILS_FLOW_TEMP, &tmp);

			ReadDallasTemp(gTempSensors1, HVAC_FANCOILS_RETURN_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_FANCOILS_RETURN_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_FANCOILS_RETURN_TEMP, &tmp);

			ReadDallasTemp(gTempSensors1, HVAC_FLOOR_FLOW_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_FLOOR_FLOW_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_FLOOR_FLOW_TEMP, &tmp);

			ReadDallasTemp(gTempSensors1, HVAC_FLOOR_RETURN_TEMP_ADDR, tmp);
			if( IsTempValid(tmp) )
				tmp = grh_W_Average(Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + LOFTB2_HVAC_FLOOR_RETURN_TEMP), tmp);
			ImportAnalog(LOFTB2_HVAC_FLOOR_RETURN_TEMP, &tmp);

			gTempSensors1.requestTemperatures();

			ProcessTimers();
		}

		grhFastPeerComms();
	}

	EXECUTESLOW()
	{
		UPDATESLOW();
		grh_SLOW_PeerJoin();
	}
}
