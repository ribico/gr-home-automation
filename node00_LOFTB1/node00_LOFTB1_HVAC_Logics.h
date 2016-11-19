//--------------------------------------
// USED FOR DHT SENSOR
DHT dht_ext(EXT_DHT22_PIN, DHT22);
DHT dht_loft(LOFT_DHT22_PIN, DHT22);

// DHT PIN1 -> 5V
// DHT PIN2 -> Arduino EXT_DHT22 pin (INPUT_PULLUP)
// DHT PIN3 Not Used
// DHT PIN4 -> GND
//--------------------------------------


//--------------------------------------
// USED FOR DALLAS TEMP SENSOR
OneWire gOneWire1(DALLAS_WIRE_BUS1_PIN);
DallasTemperature gTempSensors1(&gOneWire1);

OneWire gOneWire2(DALLAS_WIRE_BUS2_PIN); // spare sensor for sanitary water
DallasTemperature gTempSensors2(&gOneWire2);


inline void ReadDallasTemp(DallasTemperature& sensor_group, const DeviceAddress address, float& ret_val, U8 retry = 3)
{
	for(int i=0; i<retry; i++)
	{
		ret_val = sensor_group.getTempC(address);
		if(IsTempValid(ret_val))
			return;
	}
}


U8 gTempBuff[26];

inline void GetCurrentStatus(U16 phase_fast)
{
	// get light sensor
	int analog_val = analogRead(LIGHT_TRANSDUCER_PIN_IN);
	float light_intensity = 40000.0/1024.0 * (1024-analog_val);
	ImportAnalog(LIGHT_SENSOR, &light_intensity);

	float tmp;

	// get the sanitary temp value from spare sensor
	ReadDallasTemp(gTempSensors2, HVAC_BOILER_SANITARY_TEMP_ADDR_2, tmp);
	ImportAnalog(HVAC_BOILER_SANITARY_TEMP, &tmp);


	// read and send external temp & UR to ROW1B1 slots

	tmp = dht_ext.readTemperature();
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff, &tmp);

	tmp = dht_ext.readHumidity();
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+2, &tmp); // 2 bytes offset for UR

	tmp = dht_loft.readTemperature();
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+4, &tmp);

	tmp = dht_loft.readHumidity();
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+6, &tmp); // 2 bytes offset for UR

	ReadDallasTemp(gTempSensors1, HVAC_BOILER_SANITARY_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+8, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_BOILER_HEATING_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+10, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_BOILER_BOTTOM_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+12, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_HEATPUMP_FLOW_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+14, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_HEATPUMP_RETURN_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+16, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_FANCOILS_FLOW_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+18, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_FANCOILS_RETURN_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+20, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_FLOOR_FLOW_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+22, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_FLOOR_RETURN_TEMP_ADDR, tmp);
//	if( IsTempValid(tmp) )
		Souliss_HalfPrecisionFloating(gTempBuff+24, &tmp);

	if(!ReqTyp())
		SendData(IP_ADDRESS_ROW1B1, ROW1B1_EXT_TEMP, gTempBuff, 26); // sending 26 consecutive bytes

	// request temperature for next cycle
	gTempSensors1.requestTemperatures();
	gTempSensors2.requestTemperatures();
}

inline void ProcessSanitaryWaterRequest(U16 phase_fast)
{
	// control SANITARY production hysteresys in Auto Mode
	if( IsTempValid(temp_HVAC_Boiler_Saniary) )
	{
		if( (IsSanitaryWaterAutoOff() && IsSanitaryWaterCold()) || (IsSanitaryWaterAutoOn() && !IsSanitaryWaterHot()) )
		{
			SetHpFlowToBoiler(); 		// upstream to boiler
			SanitaryWaterAutoOnCmd(); // set to AutoOff automatically after T12 timeout

			HpCirculationAutoDelay();	// set HP circulation pump to OFF and set a delay timer for restart
		}
	}
}

inline void FloorZone_HeatingLogics(U8 zone_mask, float current_temp, float setpoint_temp)
{
	if( IsTempValid(current_temp) )
	{
	  if( current_temp < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL )
	    FloorZoneOpen(zone_mask);
	}
}

inline void FloorZone_CoolingLogics(U8 zone_mask, float current_temp, float setpoint_temp)
{
	if( IsTempValid(current_temp) )
	{
	  if( current_temp > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL )
	    FloorZoneOpen(zone_mask);
	}
}


inline void ProcessZonesActivation(U16 phase_fast)
{
	if( IsSanitaryWaterOn() || IsSanitaryWaterAutoOn() )
	{
		if( IsFloorAuto() )
			FloorZones_None();	// close all zones
	}
	else if( IsFloorOn() ) 		// force all zones open
	{
		if( IsHeatMode() )
			FloorZones_All();
		else if( IsCoolMode() )
			FloorZones_AllCooling();
	}
	else if ( IsFloorOff() ) // force all zones Close
	{
		FloorZones_None();
	}
	else if( IsFloorAuto() || IsFloorAutoOn() ) // check zone temperatures to open/close valves
	{
		// AUTO MODE -> activate only needed zones

		float setpoint_temp = mOutputAsFloat(TEMP_AMBIENCE_SETPOINT);
		if ( !IsTempValid(setpoint_temp) )
			return;

		// activate zones according to measured temperature according to setpoint

		if( IsHeatMode() )
		{
			FloorZone_HeatingLogics(HVAC_MASK_BED1, temp_BED1, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BATH1, temp_BATH1, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BED2, temp_BED2, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_LIVING, (temp_LIVING+temp_DINING)/2.0, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BED3, temp_BED3, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BATH2, temp_BATH2, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_KITCHEN, temp_KITCHEN, setpoint_temp);
//			FloorZone_HeatingLogics(HVAC_MASK_LOFT, temp_LOFT, setpoint_temp); // this is a fancoil
		}
		else if( IsCoolMode() )
		{
			// do not cool baths floor
			FloorZone_CoolingLogics(HVAC_MASK_BED1, temp_BED1, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_BED2, temp_BED2, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_LIVING, (temp_LIVING+temp_DINING)/2.0, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_BED3, temp_BED3, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_KITCHEN, temp_KITCHEN, setpoint_temp);
//			FloorZone_CoolingLogics(HVAC_MASK_LOFT, temp_LOFT, setpoint_temp); // this is a fancoil
		}
	}

	// process zones logics both for floor on and off
	Souliss_Logic_T1A(memory_map, HVAC_ZONES, &data_changed);
}

inline void CalculateFloorTempSetpoint(U16 phase_fast)
{
  if( IsHeating() ) // heating request for at least one zone
	{
		float setpoint_floor_water = 30;

		if (temp_EXT != 0)
			setpoint_floor_water = 40.4 - 0.92*temp_EXT; // copied from ROHSS heatpump winter climatic curves
/*
    float temp_amb_sp = mOutputAsFloat(TEMP_AMBIENCE_SETPOINT);
  	float setpoint_floor_water = temp_amb_sp + 2.0; // fixed delta above ambience setpoint
  	setpoint_floor_water += (temp_amb_sp - temp_EXT) / 5.0; // variable with indoor/outdoor delta

  	float temp_ave = temp_BED1;
  	if(temp_BED2 != 0) temp_ave = (temp_ave + temp_BED2)/2;
  	if(temp_BED3 != 0) temp_ave = (temp_ave + temp_BED3)/2;
  	if(temp_BATH1 != 0) temp_ave = (temp_ave + temp_BATH1)/2;
  	if(temp_BATH2 != 0) temp_ave = (temp_ave + temp_BATH2)/2;
  	if(temp_LIVING != 0) temp_ave = (temp_ave + temp_LIVING)/2;
  	if(temp_KITCHEN != 0) temp_ave = (temp_ave + temp_KITCHEN)/2;
    if(temp_DINING != 0) temp_ave = (temp_ave + temp_DINING)/2;

  	if( temp_ave!= 0 )
  		setpoint_floor_water += 2.0*(temp_amb_sp - temp_ave); // variable with indoor delta
*/
  	if( setpoint_floor_water > SETPOINT_TEMP_FLOOR_FLOW_MAX )
  		setpoint_floor_water = SETPOINT_TEMP_FLOOR_FLOW_MAX;

  	ImportAnalog(TEMP_FLOOR_FLOW_SETPOINT, &setpoint_floor_water);
  }
  else if( IsCooling() ) // cooling at least one zone
	{
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

    if( dew_point_MAX < SETPOINT_TEMP_FLOOR_FLOW_MIN )
  		dew_point_MAX = SETPOINT_TEMP_FLOOR_FLOW_MIN;

  	// variable setpoint according to UR and dew point
  	ImportAnalog(TEMP_FLOOR_FLOW_SETPOINT, &dew_point_MAX);
  }
}

inline void ProcessFloorRequest(U16 phase_fast)
{
	if( IsSanitaryWaterAutoOn() )
	{
		// set a delay for next activation of Pump Collector To Floor
		// we don't want to put in circulation the water at the temperature for
		// sanitary to the floor circuit
		//
		PumpCollectorToFloorAutoDelay();
	}
	else if( IsHeating() ) // heating request for at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback

		// direct floor heating from the heatpump
		SetHpFlowToCollector();
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();
		SetCollectorToFloorMixValve_FullOpen();

/*
		// floor heating from the storage
		// control hot water storage if there's heating requests from any zone
		if( IsTempValid(temp_HVAC_Boiler_Heating) && IsTempValid(temp_HVAC_Boiler_Bottom) )
		{
			if( (!IsStorageWaterInProduction() && IsStorageWaterCold()) || (IsStorageWaterInProduction() && !IsStorageWaterHot()) )
			{
	      HpSetpoint2AutoCmd(); 	// fixed HP setpoint 2, do not care about standard HP climatic curves
	      SetHpFlowToBoiler();
	      HpCirculationAutoOnCmd();
			}
		}

		PumpBoilerToFloorAutoOnCmd();

		if( IsTempValid(temp_HVAC_Floor_Flow) && IsTempValid(temp_HVAC_Floor_Return) )
			AdjustBoilerToFloorFlowTemperature( mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT) );
*/
	}
	else if( IsCooling() ) // cooling at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback

		// produce cold water
//		HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
		SetHpFlowToCollector();
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();
		AdjustCollectorToFloorFlowTemperature( mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT) );
	}
}

// based on ambience UR average with hysteresys
//
// return values:
// 3 -> UR above SETPOINT_UR_3
// 2 -> UR above SETPOINT_UR_2
// 1 -> UR above SETPOINT_UR_1
// 0 -> UR below SETPOINT_UR_1
//
inline U8 Fancoils_AmbienceURLevel()
{
	// check UR and eventually activate fancoils
	float UR_AVE = 0.0;
	U8 uSensorCount = 0;

	if( IsURValid(UR_BED1) )
	{
		UR_AVE += UR_BED1;
		uSensorCount++;
	}

	if( IsURValid(UR_BED2) )
	{
		UR_AVE += UR_BED2;
		uSensorCount++;
	}

	if( IsURValid(UR_LIVING) )
	{
		UR_AVE += UR_LIVING;
		uSensorCount++;
	}

	if( IsURValid(UR_BED3) )
	{
		UR_AVE += UR_BED3;
		uSensorCount++;
	}

	if( IsURValid(UR_KITCHEN) )
	{
		UR_AVE += UR_KITCHEN;
		uSensorCount++;
	}

	if( IsURValid(UR_DINING) )
	{
		UR_AVE += UR_DINING;
		uSensorCount++;
	}

	if( uSensorCount > 0 )
		UR_AVE /= uSensorCount;

	if( (IsFancoilsAutoOff() && (UR_AVE > SETPOINT_UR_3)) || (IsFancoilsAutoOn() && (UR_AVE > SETPOINT_UR_3 - SETPOINT_UR_DEADBAND)) )
			return 3;

	if( (IsFancoilsAutoOff() && (UR_AVE > SETPOINT_UR_2)) || (IsFancoilsAutoOn() && (UR_AVE > SETPOINT_UR_2 - SETPOINT_UR_DEADBAND)) )
			return 2;

	if( (IsFancoilsAutoOff() && (UR_AVE > SETPOINT_UR_1)) || (IsFancoilsAutoOn() && (UR_AVE > SETPOINT_UR_1 - SETPOINT_UR_DEADBAND)) )
			return 1;

	return 0;
}

inline void ProcessFancoilsRequest(U16 phase_fast)
{
	if (IsSanitaryWaterAutoOn())
	{
		// set a delay for next activation of Pump Collector To Floor
		// we don't want to put in circulation the water at the temperature for
		// sanitary to the floor circuit
		//
		PumpCollectorToFancoilAutoDelay();
	}
	else if( IsCoolMode() )
	{
		if( IsZoneOpen() ) // at least one floor zone open
			FancoilsAutoOnCmd();

		U8 UR_level = Fancoils_AmbienceURLevel();

		if( UR_level > 0 )
		{
			FancoilsAutoOnCmd();
			HpSetpoint2AutoCmd(); 	// use setpoint2 when UR is too high, floor temp is controlled above the dew point temp
		}

		if( IsFancoilsOn() || IsFancoilsAutoOn() )
		{
			SetHpFlowToCollector();
			HpCirculationAutoOnCmd();
			PumpCollectorToFancoilAutoOnCmd();
			Fancoil_AutoCmd(phase_fast%2, UR_level);
		}
	}
}
