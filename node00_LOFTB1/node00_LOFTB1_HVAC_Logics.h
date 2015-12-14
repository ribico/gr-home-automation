//--------------------------------------
// USED FOR DHT SENSOR
DHT dht_ext(EXT_DHT22_PIN, DHT22);
DHT dht_loft(LOFT_DHT22_PIN, DHT22);

// DHT PIN1 -> 5V
// DHT PIN2 -> Arduino EXT_DHT22 pin (INPUT_PULLUP)
// DHT PIN3 Not Used
// DHT PIN4 -> GND
//--------------------------------------

float temperature_sanitary, temperature_heating, temperature_bottom;
float temperature_floor_flow, temperature_floor_return, temperature_fancoil_flow;

inline void GetCurrentStatus(U16 phase_fast)
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

inline void ProcessSanitaryWaterRequest(U16 phase_fast)
{
	// control SANITARY production hysteresys in Auto Mode
	if( (IsSanitaryWaterAutoOff() && IsSanitaryWaterCold()) || (IsSanitaryWaterAutoOn() && !IsSanitaryWaterHot()) )
	{
		SetHpFlowToBoiler(); 		// upstream to boiler
		SanitaryWaterAutoOnCmd(); // set to AutoOff automatically after T12 timeout
	}
}

inline void ProcessZoneActivation(U16 phase_fast)
{
	if (IsSanitaryWaterAutoOn())
		return;

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
    if( setpoint_temp == 0 )
      return;

		if( IsHeatMode() )
		{
      if( temp_BED1 != 0 )
      {
  			if( temp_BED1 < min_temp )
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED1);
  			else if( temp_BED1 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED1);
      }

      if( temp_BATH1 != 0 )
      {
  			if( temp_BATH1 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BATH1);
  			else if( temp_BATH1 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH1);
      }

      if( temp_BED2 != 0 )
      {
  			if( temp_BED2 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED2);
  			else if( temp_BED2 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED2);
      }

      if( temp_LIVING != 0 && temp_DINING != 0 )
      {
  			if( (temp_LIVING+temp_DINING)/2.0 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
  			else if( (temp_LIVING+temp_DINING)/2.0 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_LIVING);
      }

      if( temp_BED3 != 0 )
      {
  			if( temp_BED3 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED3);
  			else if( temp_BED3 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED3);
      }

      if( temp_BATH2 != 0 )
      {
  			if( temp_BATH2 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BATH2);
  			else if( temp_BATH2 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH2);
      }

      if( temp_KITCHEN != 0 )
      {
    		if( temp_KITCHEN < min_temp)
    			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_KITCHEN);
    		else if( temp_KITCHEN > max_temp)
    			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_KITCHEN);
      }
		}
		else if( IsCoolMode() )
		{
			// do not cool baths floor
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH1);
			SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BATH2);

      if( temp_BED1 != 0 )
      {
  			if( temp_BED1 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED1);
  			else if( temp_BED1 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED1);
      }

      if( temp_BED2 != 0 )
      {
  			if( temp_BED2 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED2);
  			else if( temp_BED2 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED2);
      }

      if( temp_LIVING != 0 && temp_DINING != 0 )
      {
  			if( (temp_LIVING+temp_DINING)/2.0 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_LIVING);
  			else if( (temp_LIVING+temp_DINING)/2.0 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_LIVING);
      }

      if( temp_BED3 != 0 )
      {
  			if( temp_BED3 > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_BED3);
  			else if( temp_BED3 < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_BED3);
      }

      if( temp_KITCHEN != 0 )
      {
  			if( temp_KITCHEN > max_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | HVAC_MASK_KITCHEN);
  			else if( temp_KITCHEN < min_temp)
  				SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~HVAC_MASK_KITCHEN);
      }
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

		// control hot water storage if there's heating requests from any zone
		if( (!IsStorageWaterInProduction() && IsStorageWaterCold()) || (IsStorageWaterInProduction() && !IsStorageWaterHot()) )
		{
      HpSetpoint2AutoCmd(); 	// fixed HP setpoint 2, do not care about standard HP climatic curves
      SetHpFlowToBoiler();
      HpCirculationAutoOnCmd();
		}

		PumpBoilerToFloorAutoOnCmd();
		AdjustBoilerToFloorFlowTemperature( mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT) );
	}
	else if( IsCooling() ) // cooling at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback

		// produce cold water
		HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
		SetHpFlowToCollector();
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();
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
