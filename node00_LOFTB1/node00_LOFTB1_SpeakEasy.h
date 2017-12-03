#define AUTO_CYCLES_COUNT   8

#define IsFloorOn()					(mOutput(LOFTB1_FLOOR_MODE) == Souliss_T1n_OnCoil)
#define IsFloorAutoOn()			(mOutput(LOFTB1_FLOOR_MODE) == Souliss_T1n_AutoOnCoil)
#define IsFloorAutoOff()		(mOutput(LOFTB1_FLOOR_MODE) == Souliss_T1n_AutoOffCoil)
#define IsFloorOff()				(mOutput(LOFTB1_FLOOR_MODE) == Souliss_T1n_OffCoil)
#define IsFloorAuto()				(mOutput(LOFTB1_FLOOR_MODE) == Souliss_T1n_AutoState)
#define FloorAutoOnCmd()    SetInput(LOFTB1_FLOOR_MODE, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define FloorAutoDelay()    mAuxiliary(LOFTB1_FLOOR_MODE) = HP_CMD_DELAY;

#define IsFancoilsOn()				(mOutput(LOFTB1_FANCOIL_MODE) == Souliss_T1n_OnCoil)
#define IsFancoilsOff()				(mOutput(LOFTB1_FANCOIL_MODE) == Souliss_T1n_OffCoil)
#define IsFancoilsAuto()			(mOutput(LOFTB1_FANCOIL_MODE) == Souliss_T1n_AutoState)
#define IsFancoilsAutoOn()		(mOutput(LOFTB1_FANCOIL_MODE) == Souliss_T1n_AutoOnCoil)
#define IsFancoilsAutoOff()		(mOutput(LOFTB1_FANCOIL_MODE) == Souliss_T1n_AutoOffCoil)
#define FancoilsAutoOnCmd()   SetInput(LOFTB1_FANCOIL_MODE, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define FancoilsAutoDelay()   mAuxiliary(LOFTB1_FANCOIL_MODE) = HP_CMD_DELAY;

#define IsHeatMode()				(mOutput(LOFTB1_HEATPUMP_COOL) == Souliss_T1n_OffCoil)
#define SetHeatMode()               SetInput(LOFTB1_HEATPUMP_COOL, Souliss_T1n_OffCoil)

#define IsCoolMode()				(mOutput(LOFTB1_HEATPUMP_COOL) == Souliss_T1n_OnCoil)
#define SetCoolMode()               SetInput(LOFTB1_HEATPUMP_COOL, Souliss_T1n_OnCoil)

#define IsZoneOpen()				mOutput(LOFTB1_HVAC_ZONES)
#define IsHeating()					(IsZoneOpen() && IsHeatMode())
#define IsCooling()					(IsZoneOpen() && IsCoolMode())

#define IsSanitaryWaterCold()       (temp_HVAC_Boiler_Sanitary < mOutputAsFloat(LOFTB1_HVAC_BOILER_SANITARY_SETPOINT)-SANITARY_WATER_SETPOINT_DB)
#define IsSanitaryWaterHot()        (temp_HVAC_Boiler_Sanitary > mOutputAsFloat(LOFTB1_HVAC_BOILER_SANITARY_SETPOINT)+SANITARY_WATER_SETPOINT_DB)
#define IsSanitaryWaterOn()	        (mOutput(LOFTB1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_OnCoil)
#define IsSanitaryWaterAuto()	    (mOutput(LOFTB1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoState)
#define IsSanitaryWaterAutoOn()	    (mOutput(LOFTB1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoOnCoil)
#define IsSanitaryWaterAutoOff()	(mOutput(LOFTB1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoOffCoil)
#define SanitaryWaterAutoOnCmd()    SetInput(LOFTB1_HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)

#define IsStorageWaterCold()            (temp_HVAC_Boiler_Heating < mOutputAsFloat(LOFTB1_TEMP_FLOOR_FLOW_SETPOINT))
#define IsStorageWaterHot()				(temp_HVAC_Boiler_Bottom > mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT))
#define IsStorageWaterInProduction()    (IsHpFlowToBoiler() && IsHotWaterInProduction())

#define IsHotWaterInProduction()		(IsHeatMode() && (IsHpCirculationOn() || IsHpCirculationAutoOn()) )
#define IsCoolWaterInProduction()		(IsCoolMode() && (IsHpCirculationOn() || IsHpCirculationAutoOn()) )

#define IsHpCirculationOn()				(mOutput(LOFTB1_HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil)
#define IsHpCirculationAutoOn()		    (mOutput(LOFTB1_HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_AutoOnCoil)
#define HpCirculationAutoOnCmd()	    SetInput(LOFTB1_HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define HpCirculationAutoDelay()        mAuxiliary(LOFTB1_HEATPUMP_CIRCULATION_PUMP) = HP_CMD_DELAY;

#define IsHpFlowToBoiler()				(/*(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK) && */(mOutput(LOFTB1_MAIN_3WAY_VALVE) == Souliss_T2n_State_Open))
#define IsHpFlowToCollector()			(/*(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK) &&*/ (mOutput(LOFTB1_MAIN_3WAY_VALVE) == Souliss_T2n_State_Close))
#define Is3WayValveMovingToBoiler()		(mOutput(LOFTB1_MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Open)
#define Is3WayValveMovingToCollector()	(mOutput(LOFTB1_MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Close)
#define SetHpFlowToBoiler()				if(!IsHpFlowToBoiler() && !Is3WayValveMovingToBoiler()) 	SetInput(LOFTB1_MAIN_3WAY_VALVE, Souliss_T2n_OpenCmd_SW)
#define SetHpFlowToCollector()	 		if(!IsHpFlowToCollector() && !Is3WayValveMovingToCollector()) 	SetInput(LOFTB1_MAIN_3WAY_VALVE, Souliss_T2n_CloseCmd_SW)

#define IsPumpBoilerToFloorOn()			(mOutput(LOFTB1_PUMP_BOILER_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpBoilerToFloorAutoOn()		(mOutput(LOFTB1_PUMP_BOILER_FLOOR) == Souliss_T1n_AutoOnCoil)
#define IsPumpBoilerToFloorOff()		(mOutput(LOFTB1_PUMP_BOILER_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpBoilerToFloorAuto()		(mOutput(LOFTB1_PUMP_BOILER_FLOOR) == Souliss_T1n_AutoState)
#define PumpBoilerToFloorAutoOnCmd()   SetInput(LOFTB1_PUMP_BOILER_FLOOR, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)

#define IsPumpCollectorToFloorOn()		(mOutput(LOFTB1_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFloorAutoOn()	(mOutput(LOFTB1_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_AutoOnCoil)
#define IsPumpCollectorToFloorOff()		(mOutput(LOFTB1_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFloorAuto()	(mOutput(LOFTB1_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_AutoState)
#define PumpCollectorToFloorAutoOnCmd()  SetInput(LOFTB1_PUMP_COLLECTOR_FLOOR, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define PumpCollectorToFloorAutoDelay()  mAuxiliary(LOFTB1_PUMP_COLLECTOR_FLOOR) = HP_CMD_DELAY;

#define IsPumpCollectorToFancoilOn()	(mOutput(LOFTB1_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFancoilAutoOn()	(mOutput(LOFTB1_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_AutoOnCoil)
#define IsPumpCollectorToFancoilOff()	(mOutput(LOFTB1_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFancoilAuto()	(mOutput(LOFTB1_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_AutoState)
#define PumpCollectorToFancoilAutoOnCmd()  SetInput(LOFTB1_PUMP_COLLECTOR_FANCOIL, Souliss_T1n_AutoCmd + 4)
#define PumpCollectorToFancoilAutoDelay()  mAuxiliary(LOFTB1_PUMP_COLLECTOR_FANCOIL) = HP_CMD_DELAY;

// mix valve on, direction relay off
#define HeatingMixValveOn_ColdDirection()	SetInput(LOFTB1_HVAC_VALVES, mOutput(LOFTB1_HVAC_VALVES) | HEATING_MIX_VALVE_SWITCH_MASK)
// mix valve on, direction relay on
#define HeatingMixValveOn_WarmDirection()	SetInput(LOFTB1_HVAC_VALVES, mOutput(LOFTB1_HVAC_VALVES) | (HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK) )
#define HeatingMixValveOff()				SetInput(LOFTB1_HVAC_VALVES, mOutput(LOFTB1_HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK & ~HEATING_MIX_VALVE_DIRECTION_MASK)

#define SetCollectorToFloorMixValve(pos)         analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, pos)
#define SetCollectorToFloorMixValve_FullOpen()   analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, COLLECTOR_FLOOR_MIX_VALVE_MAX)
#define SetCollectorToFloorMixValve_FullClose()  analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, COLLECTOR_FLOOR_MIX_VALVE_MIN)

#define IsHpSetpoint2Auto()	            (mOutput(LOFTB1_HP_SETPOINT_2) == Souliss_T1n_AutoState)
#define	IsHpSetpoint1()					(mOutput(LOFTB1_HP_SETPOINT_2) == Souliss_T1n_OffCoil)
#define	IsHpSetpoint2()					(mOutput(LOFTB1_HP_SETPOINT_2) == Souliss_T1n_OnCoil)
#define HpSetpoint2AutoCmd()    SetInput(LOFTB1_HP_SETPOINT_2, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)

#define FloorZones_None()        mInput(LOFTB1_HVAC_ZONES) = HVAC_MASK_NO_ZONES
#define FloorZones_All()         mInput(LOFTB1_HVAC_ZONES) = HVAC_MASK_ALL_ZONES
#define FloorZones_AllCooling()  mInput(LOFTB1_HVAC_ZONES) = HVAC_MASK_ALL_COOLING_ZONES
#define FloorZoneOpen(mask)      SetInput(LOFTB1_HVAC_ZONES, mInput(LOFTB1_HVAC_ZONES) | mask)
#define FloorZoneClose(mask)     SetInput(LOFTB1_HVAC_ZONES, mInput(LOFTB1_HVAC_ZONES) & ~mask)

#define temp_BED1	    pOutputAsFloat(BED1B1,BED1B1_TEMPERATURE)
#define UR_BED1		    pOutputAsFloat(BED1B1,BED1B1_HUMIDITY)
#define temp_BATH1	  pOutputAsFloat(BED2B3,BED2B3_TEMPERATURE)
#define UR_BATH1	    pOutputAsFloat(BED2B3,BED2B3_HUMIDITY)
#define temp_BED2	    pOutputAsFloat(BED2B1,BED2B1_TEMPERATURE)
#define UR_BED2		    pOutputAsFloat(BED2B1,BED2B1_HUMIDITY)
#define temp_LIVING	  pOutputAsFloat(ROW1B2,ROW1B2_TEMPERATURE)
#define UR_LIVING	    pOutputAsFloat(ROW1B2,ROW1B2_HUMIDITY)
#define temp_BED3	    pOutputAsFloat(ROW2B1,ROW2B1_TEMPERATURE)
#define UR_BED3		    pOutputAsFloat(ROW2B1,ROW2B1_HUMIDITY)
#define temp_BATH2	  pOutputAsFloat(ROW1B4,ROW1B4_TEMPERATURE)
#define UR_BATH2	    pOutputAsFloat(ROW1B4,ROW1B4_HUMIDITY)
#define temp_KITCHEN  pOutputAsFloat(ROW2B3,ROW2B3_TEMPERATURE)
#define UR_KITCHEN	  pOutputAsFloat(ROW2B3,ROW2B3_HUMIDITY)
#define temp_DINING	  pOutputAsFloat(ROW1B3,ROW1B3_TEMPERATURE)
#define UR_DINING	    pOutputAsFloat(ROW1B3,ROW1B3_HUMIDITY)

#define temp_EXT      pOutputAsFloat(ROW1B1,ROW1B1_EXT_TEMP)
#define UR_EXT        pOutputAsFloat(ROW1B1,ROW1B1_EXT_UR)
#define temp_LOFT     pOutputAsFloat(ROW1B1,ROW1B1_LOFT_TEMP)
#define UR_LOFT       pOutputAsFloat(ROW1B1,ROW1B1_LOFT_UR)

#define temp_HVAC_Boiler_Sanitary           (IsTempValid(pOutputAsFloat(LOFTB2,LOFTB2_HVAC_SANITARY_WATER_TEMP)) ? pOutputAsFloat(LOFTB2,LOFTB2_HVAC_SANITARY_WATER_TEMP) : mOutputAsFloat(LOFTB1_HVAC_BOILER_SANITARY_TEMP))
#define temp_HVAC_Boiler_Heating            pOutputAsFloat(LOFTB2,LOFTB2_HVAC_BOILER_HEATING_TEMP)
#define temp_HVAC_HEATPUMP_Flow             pOutputAsFloat(LOFTB2,LOFTB2_HVAC_FLOOR_FLOW_TEMP)
#define temp_HVAC_HEATPUMP_Return           pOutputAsFloat(LOFTB2,LOFTB2_HVAC_FLOOR_RETURN_TEMP)

#define temp_HVAC_Floor_Flow                pOutputAsFloat(LOFTB2,LOFTB2_HVAC_FLOOR_FLOW_TEMP)
#define temp_HVAC_Floor_Return              pOutputAsFloat(LOFTB2,LOFTB2_HVAC_FLOOR_RETURN_TEMP)
#define temp_HVAC_Fancoil_Flow              pOutputAsFloat(LOFTB2,LOFTB2_HVAC_FANCOILS_FLOW_TEMP)
#define temp_HVAC_Fancoil_Return            pOutputAsFloat(LOFTB2,LOFTB2_HVAC_FANCOILS_RETURN_TEMP)

#define IsTempValid(temp)                   ( (temp != 0.0) && (temp > -15.0) && (temp < 65.0) )
#define IsURValid(ur)                       ( (ur != 0.0) && (ur < 99.0) )

#define IsNight()                           ( mOutputAsFloat(LOFTB1_LIGHT_SENSOR) < LUX_VALUE_NIGHT )
#define IsSunShining()                      ( mOutputAsFloat(LOFTB1_LIGHT_SENSOR) > LUX_VALUE_SUN )

#define FacadeLightsAutoOnCmd()             ( RemoteInput(IP_ADDRESS_GARDB1, GARDB1_LIGHT_GARDEN, Souliss_T1n_AutoCmd + 16) )

#define Is_HVAC_FullAuto_On()				(mOutput(LOFTB1_HVAC_FULL_AUTO) == Souliss_T1n_OnCoil)
#define Is_HVAC_FullAuto_Off()				(mOutput(LOFTB1_HVAC_FULL_AUTO) == Souliss_T1n_OffCoil)

#define Is_HVAC_ValvesAuto_On()				(mOutput(LOFTB1_HVAC_VALVES_AUTO) == Souliss_T1n_OnCoil)
#define Is_HVAC_ValvesAuto_Off()			(mOutput(LOFTB1_HVAC_VALVES_AUTO) == Souliss_T1n_OffCoil)
