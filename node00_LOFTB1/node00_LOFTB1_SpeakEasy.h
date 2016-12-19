#define AUTO_CYCLES_COUNT   4

#define IsFloorOn()					(mOutput(FLOOR_MODE) == Souliss_T1n_OnCoil)
#define IsFloorAutoOn()			(mOutput(FLOOR_MODE) == Souliss_T1n_AutoOnCoil)
#define IsFloorAutoOff()		(mOutput(FLOOR_MODE) == Souliss_T1n_AutoOffCoil)
#define IsFloorOff()				(mOutput(FLOOR_MODE) == Souliss_T1n_OffCoil)
#define IsFloorAuto()				(mOutput(FLOOR_MODE) == Souliss_T1n_AutoState)
#define FloorAutoOnCmd()    SetInput(FLOOR_MODE, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define FloorAutoDelay()    mAuxiliary(FLOOR_MODE) = HP_CMD_DELAY;

#define IsFancoilsOn()				(mOutput(FANCOIL_MODE) == Souliss_T1n_OnCoil)
#define IsFancoilsOff()				(mOutput(FANCOIL_MODE) == Souliss_T1n_OffCoil)
#define IsFancoilsAuto()			(mOutput(FANCOIL_MODE) == Souliss_T1n_AutoState)
#define IsFancoilsAutoOn()		(mOutput(FANCOIL_MODE) == Souliss_T1n_AutoOnCoil)
#define IsFancoilsAutoOff()		(mOutput(FANCOIL_MODE) == Souliss_T1n_AutoOffCoil)
#define FancoilsAutoOnCmd()   SetInput(FANCOIL_MODE, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define FancoilsAutoDelay()   mAuxiliary(FANCOIL_MODE) = HP_CMD_DELAY;

#define IsHeatMode()				(mOutput(HEATPUMP_COOL) == Souliss_T1n_OffCoil)
#define IsCoolMode()				(mOutput(HEATPUMP_COOL) == Souliss_T1n_OnCoil)

#define IsZoneOpen()				mOutput(HVAC_ZONES)
#define IsHeating()					(IsZoneOpen() && IsHeatMode())
#define IsCooling()					(IsZoneOpen() && IsCoolMode())

#define IsSanitaryWaterCold()			  (temp_HVAC_Boiler_Saniary < mOutputAsFloat(HVAC_BOILER_SANITARY_SETPOINT)-SANITARY_WATER_SETPOINT_DB)
#define IsSanitaryWaterHot()			  (temp_HVAC_Boiler_Saniary > mOutputAsFloat(HVAC_BOILER_SANITARY_SETPOINT)+SANITARY_WATER_SETPOINT_DB)
#define IsSanitaryWaterOn()	        (mOutput(HEATPUMP_SANITARY_WATER) == Souliss_T1n_OnCoil)
#define IsSanitaryWaterAuto()	      (mOutput(HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoState)
#define IsSanitaryWaterAutoOn()	    (mOutput(HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoOnCoil)
#define IsSanitaryWaterAutoOff()	  (mOutput(HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoOffCoil)
#define SanitaryWaterAutoOnCmd()    SetInput(HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)

#define IsStorageWaterCold()        (temp_HVAC_Boiler_Heating < mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT))
#define IsStorageWaterHot()					(temp_HVAC_Boiler_Bottom > mOutputAsFloat(TEMP_FLOOR_FLOW_SETPOINT))
#define IsStorageWaterInProduction()  (IsHpFlowToBoiler() && IsHotWaterInProduction())

#define IsHotWaterInProduction()		(IsHeatMode() && (IsHpCirculationOn() || IsHpCirculationAutoOn()) )
#define IsCoolWaterInProduction()		(IsCoolMode() && (IsHpCirculationOn() || IsHpCirculationAutoOn()) )

#define IsHpCirculationOn()				(mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil)
#define IsHpCirculationAutoOn()		(mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_AutoOnCoil)
#define HpCirculationAutoOnCmd()	SetInput(HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define HpCirculationAutoDelay()  mAuxiliary(HEATPUMP_CIRCULATION_PUMP) = HP_CMD_DELAY;

#define IsHpFlowToBoiler()				(/*(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK) && */(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_State_Open))
#define IsHpFlowToCollector()			(/*(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK) &&*/ (mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_State_Close))
#define Is3WayValveMovingToBoiler()		(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Open)
#define Is3WayValveMovingToCollector()	(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Close)
#define SetHpFlowToBoiler()				if(!IsHpFlowToBoiler() && !Is3WayValveMovingToBoiler()) 	SetInput(MAIN_3WAY_VALVE, Souliss_T2n_OpenCmd_SW)
#define SetHpFlowToCollector()	 		if(!IsHpFlowToCollector() && !Is3WayValveMovingToCollector()) 	SetInput(MAIN_3WAY_VALVE, Souliss_T2n_CloseCmd_SW)

#define IsPumpBoilerToFloorOn()			(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpBoilerToFloorOff()		(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpBoilerToFloorAuto()		(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_AutoState)
#define PumpBoilerToFloorAutoOnCmd()   SetInput(PUMP_BOILER_FLOOR, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)

#define IsPumpCollectorToFloorOn()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFloorOff()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFloorAuto()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_AutoState)
#define PumpCollectorToFloorAutoOnCmd()  SetInput(PUMP_COLLECTOR_FLOOR, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)
#define PumpCollectorToFloorAutoDelay()  mAuxiliary(PUMP_COLLECTOR_FLOOR) = HP_CMD_DELAY;

#define IsPumpCollectorToFancoilOn()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFancoilOff()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFancoilAuto()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_AutoState)
#define PumpCollectorToFancoilAutoOnCmd()  SetInput(PUMP_COLLECTOR_FANCOIL, Souliss_T1n_AutoCmd + 4)
#define PumpCollectorToFancoilAutoDelay()  mAuxiliary(PUMP_COLLECTOR_FANCOIL) = HP_CMD_DELAY;

// mix valve on, direction relay off
#define HeatingMixValveOn_ColdDirection()	SetInput(HVAC_VALVES, mOutput(HVAC_VALVES) | HEATING_MIX_VALVE_SWITCH_MASK)
// mix valve on, direction relay on
#define HeatingMixValveOn_WarmDirection()	SetInput(HVAC_VALVES, mOutput(HVAC_VALVES) | (HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK) )
#define HeatingMixValveOff()				SetInput(HVAC_VALVES, mOutput(HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK & ~HEATING_MIX_VALVE_DIRECTION_MASK)

#define SetCollectorToFloorMixValve(pos)         analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, pos)
#define SetCollectorToFloorMixValve_FullOpen()   analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, COLLECTOR_FLOOR_MIX_VALVE_MAX)
#define SetCollectorToFloorMixValve_FullClose()  analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, COLLECTOR_FLOOR_MIX_VALVE_MIN)

#define	IsHpSetpoint1()					(mOutput(HP_SETPOINT_2) == Souliss_T1n_OffCoil)
#define	IsHpSetpoint2()					(mOutput(HP_SETPOINT_2) == Souliss_T1n_OnCoil)
#define HpSetpoint2AutoCmd()    SetInput(HP_SETPOINT_2, Souliss_T1n_AutoCmd + AUTO_CYCLES_COUNT)

#define FloorZones_None()        mInput(HVAC_ZONES) = HVAC_MASK_NO_ZONES
#define FloorZones_All()         mInput(HVAC_ZONES) = HVAC_MASK_ALL_ZONES
#define FloorZones_AllCooling()  mInput(HVAC_ZONES) = HVAC_MASK_ALL_COOLING_ZONES
#define FloorZoneOpen(mask)      SetInput(HVAC_ZONES, mInput(HVAC_ZONES) | mask)
#define FloorZoneClose(mask)     SetInput(HVAC_ZONES, mInput(HVAC_ZONES) & ~mask)

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

//#define temp_HVAC_Boiler_Saniary            pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_SANITARY_TEMP)
#define temp_HVAC_Boiler_Saniary            mOutputAsFloat(HVAC_BOILER_SANITARY_TEMP)
#define temp_HVAC_Boiler_Heating            pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_HEATING_TEMP)
#define temp_HVAC_Boiler_Bottom             pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_BOTTOM_TEMP)

#define temp_HVAC_HEATPUMP_Flow             pOutputAsFloat(ROW1B1,ROW1B1_HVAC_HEATPUMP_FLOW_TEMP)
#define temp_HVAC_HEATPUMP_Return           pOutputAsFloat(ROW1B1,ROW1B1_HVAC_HEATPUMP_RETURN_TEMP)

#define temp_HVAC_Fancoil_Flow              pOutputAsFloat(ROW1B1,ROW1B1_HVAC_FANCOILS_FLOW_TEMP)
#define temp_HVAC_Fancoil_Return            pOutputAsFloat(ROW1B1,ROW1B1_HVAC_FANCOILS_RETURN_TEMP)
#define temp_HVAC_Floor_Flow                pOutputAsFloat(ROW1B1,ROW1B1_HVAC_FLOOR_FLOW_TEMP)
#define temp_HVAC_Floor_Return              pOutputAsFloat(ROW1B1,ROW1B1_HVAC_FLOOR_RETURN_TEMP)

#define IsTempValid(temp)                   ( (temp != DEVICE_DISCONNECTED_C) && (temp != 0.0) && (temp > -15.0) && (temp < 45.0) )
#define IsURValid(ur)                       ( (ur != 0.0) && (ur < 99.0) )
