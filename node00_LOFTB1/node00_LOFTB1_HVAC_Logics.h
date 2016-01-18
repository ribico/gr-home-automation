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


inline void ReadDallasTemp(DallasTemperature& sensor_group, const DeviceAddress address, float& ret_val, U8 retry = 3)
{
	for(int i=0; i<retry; i++)
	{
		ret_val = sensor_group.getTempC(address);
		if(IsTempValid(ret_val))
			return;
	}
}

inline void GetCurrentStatus(U16 phase_fast)
{
	// read and send external temp & UR to ROW1B1 slots
	float tmp;
	U8 buff[8];

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



	ReadDallasTemp(gTempSensors1, HVAC_BOILER_SANITARY_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_BOILER_HEATING_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff+2, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_BOILER_BOTTOM_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff+4, &tmp);

	SendData(IP_ADDRESS_ROW1B1, ROW1B1_HVAC_BOILER_SANITARY_TEMP, buff, 6);



	ReadDallasTemp(gTempSensors1, HVAC_FLOOR_FLOW_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_FLOOR_RETURN_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff+2, &tmp);

	SendData(IP_ADDRESS_ROW1B1, ROW1B1_HVAC_HEATPUMP_FLOW_TEMP, buff, 4);



	ReadDallasTemp(gTempSensors1, HVAC_FANCOILS_FLOW_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_FANCOILS_RETURN_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff+2, &tmp);



	ReadDallasTemp(gTempSensors1, HVAC_HEATPUMP_FLOW_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff+4, &tmp);

	ReadDallasTemp(gTempSensors1, HVAC_HEATPUMP_RETURN_TEMP_ADDR, tmp);
	Souliss_HalfPrecisionFloating(buff+6, &tmp);

	SendData(IP_ADDRESS_ROW1B1, ROW1B1_HVAC_FANCOILS_FLOW_TEMP, buff, 8);



	// request temperature for next cycle
	gTempSensors1.requestTemperatures();
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
		}
	}
}

inline void FloorZone_HeatingLogics(U8 zone_mask, float current_temp, float setpoint_temp)
{
	if( IsTempValid(current_temp) )
	{
	  if( current_temp < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL )
	    FloorZoneOpen(zone_mask);
	  else if( current_temp > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL )
	    FloorZoneClose(zone_mask);
	}
}

inline void FloorZone_CoolingLogics(U8 zone_mask, float current_temp, float setpoint_temp)
{
	if( IsTempValid(current_temp) )
	{
	  if( current_temp < setpoint_temp - SETPOINT_TEMP_DEADBAND_SMALL )
	    FloorZoneClose(zone_mask);
	  else if( current_temp > setpoint_temp + SETPOINT_TEMP_DEADBAND_SMALL )
	    FloorZoneOpen(zone_mask);
	}
}


inline void ProcessZonesActivation(U16 phase_fast)
{
	if (IsSanitaryWaterAutoOn())
	{
		FloorZones_None();
		return;
	}

	if( IsFloorOn() ) 		// force all zones open
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

		float setpoint_temp = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT);

		// activate zones according to measured temperature according to setpoint
		mInput(HVAC_ZONES) = mOutput(HVAC_ZONES);
    if( setpoint_temp == 0 )
      return;

		if( IsHeatMode() )
		{
			FloorZone_HeatingLogics(HVAC_MASK_BED1, temp_BED1, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BATH1, temp_BATH1, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BED2, temp_BED2, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_LIVING, (temp_LIVING+temp_DINING)/2.0, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BED3, temp_BED3, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BATH2, temp_BATH2, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_KITCHEN, temp_KITCHEN, setpoint_temp);
		}
		else if( IsCoolMode() )
		{
			// do not cool baths floor
			FloorZoneClose(HVAC_MASK_BATH1);
			FloorZoneClose(HVAC_MASK_BATH2);

			FloorZone_CoolingLogics(HVAC_MASK_BED1, temp_BED1, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_BED2, temp_BED2, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_LIVING, (temp_LIVING+temp_DINING)/2.0, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_BED3, temp_BED3, setpoint_temp);
			FloorZone_CoolingLogics(HVAC_MASK_KITCHEN, temp_KITCHEN, setpoint_temp);
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
			40.4 - 0.92*temp_EXT; // copied from ROHSS heatpump winter climatic curves
/*
    float temp_amb_sp = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT);
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
	if (IsSanitaryWaterAutoOn())
		return;

	if( IsHeating() ) // heating request for at least one zone
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
		HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
		SetHpFlowToCollector();
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();

		if( IsTempValid(temp_HVAC_Floor_Flow) && IsTempValid(temp_HVAC_Floor_Return) )
			AdjustCollectorToFloorFlowTemperature( mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT) );
	}
}

inline void ProcessFancoilsRequest(U16 phase_fast)
{
	if (IsSanitaryWaterAutoOn())
		return;

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
