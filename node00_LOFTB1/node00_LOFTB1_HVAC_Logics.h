//--------------------------------------
// USED FOR DHT SENSOR
DHT dht_ext(EXT_DHT22_PIN, DHT22);
DHT dht_loft(LOFT_DHT22_PIN, DHT22);

// DHT PIN1 -> 5V
// DHT PIN2 -> Arduino EXT_DHT22 pin (INPUT_PULLUP)
// DHT PIN3 Not Used
// DHT PIN4 -> GND
//--------------------------------------


U8 gTempBuff[26];

// from here: http://forum.arduino.cc/index.php?topic=43605.0
float GetPlatinumRTD(float R,float R0) { 
   float A=3.9083E-3; 
   float B=-5.775E-7; 
   float T; 
   
   R=R/R0; 
   
   //T = (0.0-A + sqrt((A*A) - 4.0 * B * (1.0 - R))) / 2.0 * B; 
   T=0.0-A; 
   T+=sqrt((A*A) - 4.0 * B * (1.0 - R)); 
   T/=(2.0 * B); 
   
   if(T>0&&T<200) { 
     return T; 
   } 
   else { 
     //T=  (0.0-A - sqrt((A*A) - 4.0 * B * (1.0 - R))) / 2.0 * B; 
     T=0.0-A; 
     T-=sqrt((A*A) - 4.0 * B * (1.0 - R)); 
     T/=(2.0 * B); 
     return T; 
   } 
} 

inline float ReadTempFromSolarSystem(U8 pin)
{
	int analog_val = analogRead(pin);
	float voltage = analog_val * 0.0049;
	float resistance = voltage/0.001; // solar system is exciting RTD at constant 1mA
	return GetPlatinumRTD(resistance, 1000);
}

inline void GetCurrentStatus(U16 phase_fast)
{
	// get light sensor
	int analog_val = analogRead(LIGHT_TRANSDUCER_PIN_IN);
	float current_intensity = 40000.0/1024.0 * (1024-analog_val);
	float light_intensity = grh_W_Average(mOutputAsFloat(LOFTB1_LIGHT_SENSOR), current_intensity);
	ImportAnalog(LOFTB1_LIGHT_SENSOR, &light_intensity);

	float tmp;


// get temperatures from PT1000 in the solar system
	tmp = ReadTempFromSolarSystem(TEMP_SOLAR_EXT_PIN_IN);	
	Serial.print("EXT : ");
	Serial.print(tmp);
	tmp = grh_W_Average(pOutputAsFloat(LOFTB2, LOFTB2_HVAC_SOLAR_EXT_TEMP), tmp);
	Souliss_HalfPrecisionFloating(gTempBuff, &tmp);
	
	tmp = ReadTempFromSolarSystem(TEMP_SOLAR_INT_PIN_IN);	
	Serial.print(" INT : ");
	Serial.print(tmp);
	tmp = grh_W_Average(pOutputAsFloat(LOFTB2, LOFTB2_HVAC_SOLAR_INT_TEMP), tmp);
	Souliss_HalfPrecisionFloating(gTempBuff+2, &tmp);

	tmp = ReadTempFromSolarSystem(TEMP_SOLAR_HEAT_EXC_PIN_IN);	
	Serial.print(" H EXC : ");
	Serial.print(tmp);
	tmp = grh_W_Average(pOutputAsFloat(LOFTB2, LOFTB2_HVAC_SOLAR_HEAT_EXC_TEMP), tmp);
	Souliss_HalfPrecisionFloating(gTempBuff+4, &tmp);
/*
	tmp = NTC10k_ToCelsius( TEMP_BOILER_BOTTOM_NTC10K_PIN_IN, NTC10K_A1_PAD_RESISTANCE );
	Serial.print(" BOTTOM : ");
	Serial.print(tmp);
	tmp = grh_W_Average(pOutputAsFloat(LOFTB2, LOFTB2_HVAC_BOILER_BOTTOM_TEMP), tmp);
	Souliss_HalfPrecisionFloating(gTempBuff, &tmp);
*/
	if(!ReqTyp())
		SendData(IP_ADDRESS_LOFTB2, LOFTB2_HVAC_SOLAR_EXT_TEMP, gTempBuff, 6); // sending 6 consecutive bytes


	tmp = ReadTempFromSolarSystem(TEMP_SANITARY_WATER_PIN_IN);
//	Serial.print(" SANITARY : ");
//	Serial.print(tmp);
	tmp = grh_W_Average(mOutputAsFloat(LOFTB1_HVAC_BOILER_SANITARY_TEMP), tmp);
	ImportAnalog(LOFTB1_HVAC_BOILER_SANITARY_TEMP, &tmp);

	tmp = ReadTempFromSolarSystem(TEMP_BOILER_HEATING_PIN_IN);
//	Serial.print(" HEATING : ");
//	Serial.println(tmp);
	tmp = grh_W_Average(mOutputAsFloat(LOFTB1_HVAC_BOILER_HEATING_TEMP), tmp);
	ImportAnalog(LOFTB1_HVAC_BOILER_HEATING_TEMP, &tmp);

	// get the sanitary temp value from spare sensor
/*
	ReadDallasTemp(gTempSensors2, HVAC_BOILER_SANITARY_TEMP_ADDR_2, tmp);
	
	if( IsTempValid(tmp) )
		tmp = grh_W_Average(mOutputAsFloat(LOFTB1_HVAC_BOILER_SANITARY_TEMP), tmp);
	ImportAnalog(LOFTB1_HVAC_BOILER_SANITARY_TEMP, &tmp);
*/

	// read and send external temp & UR to ROW1B1 slots

	tmp = dht_ext.readTemperature();
	if( IsTempValid(tmp) )
		tmp = grh_W_Average(temp_EXT, tmp);
	Souliss_HalfPrecisionFloating(gTempBuff, &tmp);

	tmp = dht_ext.readHumidity();
	if( IsTempValid(tmp) )
		tmp = grh_W_Average(UR_EXT, tmp);
	Souliss_HalfPrecisionFloating(gTempBuff+2, &tmp); // 2 bytes offset for UR

	tmp = dht_loft.readTemperature();
	if( IsTempValid(tmp) )
		tmp = grh_W_Average(temp_LOFT, tmp);
	Souliss_HalfPrecisionFloating(gTempBuff+4, &tmp);

	tmp = dht_loft.readHumidity();
	if( IsTempValid(tmp) )
		tmp = grh_W_Average(UR_LOFT, tmp);
	Souliss_HalfPrecisionFloating(gTempBuff+6, &tmp); // 2 bytes offset for UR

	if(!ReqTyp())
		SendData(IP_ADDRESS_ROW1B1, ROW1B1_EXT_TEMP, gTempBuff, 8); // sending 8 consecutive bytes
}

inline void ProcessSanitaryWaterRequest(U16 phase_fast)
{
	// control SANITARY production hysteresys in Auto Mode
	if( IsTempValid(temp_HVAC_Boiler_Sanitary) )
	{
		if( (IsSanitaryWaterAutoOff() && IsSanitaryWaterCold()) || (IsSanitaryWaterAutoOn() && !IsSanitaryWaterHot()) )
		{
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

		float setpoint_temp = mOutputAsFloat(LOFTB1_TEMP_AMBIENCE_SETPOINT);
		if ( !IsTempValid(setpoint_temp) )
			return;

		// activate zones according to measured temperature according to setpoint

		if( IsHeatMode() )
		{
			FloorZone_HeatingLogics(HVAC_MASK_BED1, temp_BED1, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BATH1, temp_BATH1, setpoint_temp+AMBIENCE_SETPOINT_DELTA_BATHS);
			FloorZone_HeatingLogics(HVAC_MASK_BED2, temp_BED2, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_LIVING, (temp_LIVING+temp_DINING)/2.0, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BED3, temp_BED3, setpoint_temp);
			FloorZone_HeatingLogics(HVAC_MASK_BATH2, temp_BATH2, setpoint_temp+AMBIENCE_SETPOINT_DELTA_BATHS);
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
	Souliss_Logic_T1A(memory_map, LOFTB1_HVAC_ZONES, &data_changed);
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

  	ImportAnalog(LOFTB1_TEMP_FLOOR_FLOW_SETPOINT, &setpoint_floor_water);
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
  	ImportAnalog(LOFTB1_TEMP_FLOOR_FLOW_SETPOINT, &dew_point_MAX);
  }
}

inline void ProcessFloorRequest(U16 phase_fast)
{
	if ( IsSanitaryWaterOn() )
	{
		// nothing to do here
		// just do not process floor request
	}
	else if( IsSanitaryWaterAutoOn())
	{
		// set a delay for next activation of Pump Collector To Floor
		// we don't want to put in circulation the water at the temperature for
		// sanitary to the floor circuit
		//
		PumpCollectorToFloorAutoDelay();
		HpCirculationAutoDelay();

		// do not process floor request
	}
	else if( IsHeating() ) // heating request for at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback

		// if sun is shining the heatpump should be used to use the photovoltaic production
		// while solar panels will accumulate hot water in the storage for later
		// (this should be rewritten considering the power echange to the grid and not on the sun presence)
		// or
		// if storage water temperature is too cold the heatpump production should be used 
		// (regardless the sun presence) 
		if( IsSunShining() || (IsTempValid(temp_HVAC_Boiler_Heating) && temp_HVAC_Boiler_Heating < 30) )
		{
			// storage too coold -> direct floor heating from the heatpump
//			SetHpFlowToCollector(); // DONE BY ProcessHVACValvesAuto
			HpCirculationAutoOnCmd();
			PumpCollectorToFloorAutoOnCmd();
			SetCollectorToFloorMixValve_FullOpen();
		}
		else
		{
			// storage warm enough for heating
			PumpBoilerToFloorAutoOnCmd();
//			if( IsTempValid(temp_HVAC_Floor_Flow) && IsTempValid(temp_HVAC_Floor_Return) )
//				AdjustBoilerToFloorFlowTemperature( mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT) );
		}
	}
	else if( IsCooling() ) // cooling at least one zone
	{
		FloorAutoOnCmd(); // only for user interface feedback

		// produce cold water
//		HpSetpoint2AutoCmd(); 	// always use setpoint2 when cooling, floor temp is controlled above the dew point temp
//		SetHpFlowToCollector(); // DONE BY ProcessHVACValvesAuto
		HpCirculationAutoOnCmd();
		PumpCollectorToFloorAutoOnCmd();
		AdjustCollectorToFloorFlowTemperature( mOutputAsFloat(LOFTB1_TEMP_FLOOR_FLOW_SETPOINT) );
	}
	else
	{
		// set a delay for next activation of Pump Collector To Floor
		// this prevents false activations
		PumpCollectorToFloorAutoDelay();
		HpCirculationAutoDelay();
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
	if ( IsSanitaryWaterOn() )
	{
		// nothing to do here
		// just do not process fancoils request
	}
	else if (IsSanitaryWaterAutoOn())
	{
		// set a delay for next activation of Pump Collector To Floor
		// we don't want to put in circulation the water at the temperature for
		// sanitary to the floor circuit
		//
		PumpCollectorToFancoilAutoDelay();

		// do not process fancoils request
	}
	else if( IsHeating() ) // heating request for at least one zone
	{
			// do nothing, do not disturb manual settings
	}
	else if( IsCooling() )
	{
		FancoilsAutoOnCmd();

		U8 UR_level = Fancoils_AmbienceURLevel();

		if( UR_level > 0 )
		{
			FancoilsAutoOnCmd();
			HpSetpoint2AutoCmd(); 	// use setpoint2 when UR is too high, floor temp is controlled above the dew point temp
		}

		if( IsFancoilsOn() || IsFancoilsAutoOn() )
		{
//			SetHpFlowToCollector(); // DONE BY ProcessHVACValvesAuto
			HpCirculationAutoOnCmd();
			PumpCollectorToFancoilAutoOnCmd();
			Fancoil_AutoCmd(phase_fast%2, UR_level);
		}
	}
	else
	{
		// set a delay for next activation of Pump Collector To Floor
		// this prevents false activations
		PumpCollectorToFancoilAutoDelay();
		FancoilsAutoDelay(); // delay to prevent false activations
	}
}

inline void ProcessHVACValvesAuto(U16 phase_fast)
{
	if( Is_HVAC_ValvesAuto_Off() )
		return;

//	Serial.print("ProcessHVACValvesAuto ");

	// 3WAY VALVE (HP FLOW to collector or boiler)
	if( IsSanitaryWaterOn() || IsSanitaryWaterAutoOn() )
	{
//		Serial.print("Sanitary Water ");
		SetHpFlowToBoiler(); 		// upstream to boiler
	}
	else if( IsPumpBoilerToFloorOn() || IsPumpBoilerToFloorAutoOn() )
	{
//		Serial.print("Boiler To Floor ");
		SetHpFlowToBoiler(); 		// close path from collector to boiler
	}
	else
	{
//		Serial.print("Collector To Floor ");
		SetHpFlowToCollector();
	}	
//	Serial.println();
	// BOILER MIX VALVE REGULATION ???
}

inline void ProcessHpSetpoint2Auto(U16 phase_fast)
{
	if( !IsHpSetpoint2Auto() )
		return;

// this is setting the Setpoint2 based on light sensor..
// it should be based on power exchance with the public network
// to take into consideration also local loads
//	if( mOutputAsFloat(LOFTB1_LIGHT_SENSOR) > LUX_VALUE_SUN + 2000) // strong sun
//		HpSetpoint2AutoCmd();
}


#define IsAmbienceTempTooCool(ambience_temp)	( ambience_temp < AMBIENCE_SETPOINT_DEFAULT_HEAT-SETPOINT_TEMP_DEADBAND_SMALL )
#define IsAmbienceTempTooWarm(ambience_temp)	( ambience_temp > AMBIENCE_SETPOINT_DEFAULT_COOL+SETPOINT_TEMP_DEADBAND_SMALL )
#define IsAmbienceTempOK(ambience_temp)		( !IsAmbienceTempTooCool(ambience_temp) && !IsAmbienceTempTooWarm(ambience_temp) )

// Modify the ambience temperature setpoint and HEAT/COOL mode according to measured ambience temperature
// and presence of sun (production of photovoltaic system)
void ProcessFullAutoLogics()
{
	if( Is_HVAC_FullAuto_On() )
	{
		float ambience_temp = temp_LIVING;
		if( !IsTempValid(ambience_temp) )
			ambience_temp = temp_DINING;

		if( !IsTempValid(ambience_temp) )
			return; // two sensors broken ?? -> do nothing
	
		if( IsHeating() || IsAmbienceTempTooCool(ambience_temp) ) // < 18.8°C
		{
			SetHeatMode();

			float ambience_set_point = AMBIENCE_SETPOINT_DEFAULT_HEAT;

			// ambience setpoint is changed according to the presence of sun
			if(IsSunShining())
			{
				ambience_set_point += AMBIENCE_SETPOINT_DELTA_FULLAUTO;
				HpSetpoint2AutoCmd(); // during winter sunlight is too short => heat with setpoint2
			}
			else
				ambience_set_point -= AMBIENCE_SETPOINT_DELTA_FULLAUTO;				

			ImportAnalog(LOFTB1_TEMP_AMBIENCE_SETPOINT, &ambience_set_point);
			return;
		}

		if( IsCooling() || IsAmbienceTempTooWarm(ambience_temp) ) // > 25.7°C
		{
			SetCoolMode();

			float ambience_set_point = AMBIENCE_SETPOINT_DEFAULT_COOL;
			// ambience setpoint is changed according to the presence of sun

			if(IsSunShining())
				ambience_set_point -= AMBIENCE_SETPOINT_DELTA_FULLAUTO;
			else
				ambience_set_point += AMBIENCE_SETPOINT_DELTA_FULLAUTO;				

			ImportAnalog(LOFTB1_TEMP_AMBIENCE_SETPOINT, &ambience_set_point);
			return;
		}

	}
}