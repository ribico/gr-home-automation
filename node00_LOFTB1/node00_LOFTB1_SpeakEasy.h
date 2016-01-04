#define IsFloorOn()					(pOutput(ROW2B1, ROW2B1_HVAC_FLOOR_MODE) == Souliss_T1n_OnCoil)
#define IsFloorAutoOn()			(pOutput(ROW2B1, ROW2B1_HVAC_FLOOR_MODE) == Souliss_T1n_AutoOnCoil)
#define IsFloorAutoOff()		(pOutput(ROW2B1, ROW2B1_HVAC_FLOOR_MODE) == Souliss_T1n_AutoOffCoil)
#define IsFloorOff()				(pOutput(ROW2B1, ROW2B1_HVAC_FLOOR_MODE) == Souliss_T1n_OffCoil)
#define IsFloorAuto()				(pOutput(ROW2B1, ROW2B1_HVAC_FLOOR_MODE) == Souliss_T1n_AutoState)
#define FloorAutoOnCmd()    Send(ROW2B1, ROW2B1_HVAC_FLOOR_MODE, Souliss_T1n_AutoCmd + 4)

#define IsFancoilsOn()				(pOutput(ROW2B1, ROW2B1_HVAC_FANCOIL_MODE) == Souliss_T1n_OnCoil)
#define IsFancoilsOff()				(pOutput(ROW2B1, ROW2B1_HVAC_FANCOIL_MODE) == Souliss_T1n_OffCoil)
#define IsFancoilsAuto()			(pOutput(ROW2B1, ROW2B1_HVAC_FANCOIL_MODE) == Souliss_T1n_AutoState)
#define IsFancoilsAutoOn()		(pOutput(ROW2B1, ROW2B1_HVAC_FANCOIL_MODE) == Souliss_T1n_AutoOnCoil)
#define IsFancoilsAutoOff()		(pOutput(ROW2B1, ROW2B1_HVAC_FANCOIL_MODE) == Souliss_T1n_AutoOffCoil)
#define FancoilsAutoOnCmd()   Send(ROW2B1, ROW2B1_HVAC_FANCOIL_MODE, Souliss_T1n_AutoCmd + 4)

#define IsHeatMode()				(pOutput(ROW2B1, ROW2B1_HEATPUMP_COOL) == Souliss_T1n_OffCoil)
#define IsCoolMode()				(pOutput(ROW2B1, ROW2B1_HEATPUMP_COOL) == Souliss_T1n_OnCoil)

#define IsZoneOpen()				pOutput(ROW2B1, ROW2B1_HVAC_ZONES)
#define IsHeating()					(IsZoneOpen() && IsHeatMode())
#define IsCooling()					(IsZoneOpen() && IsCoolMode())

#define IsSanitaryWaterCold()			  (temperature_sanitary < SETPOINT_TEMP_SANITARY_WATER_MIN)
#define IsSanitaryWaterHot()			  (temperature_sanitary > SETPOINT_TEMP_SANITARY_WATER_MAX)
#define IsSanitaryWaterOn()	        (pOutput(ROW2B1, ROW2B1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_OnCoil)
#define IsSanitaryWaterAuto()	      (pOutput(ROW2B1, ROW2B1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoState)
#define IsSanitaryWaterAutoOn()	    (pOutput(ROW2B1, ROW2B1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoOnCoil)
#define IsSanitaryWaterAutoOff()	  (pOutput(ROW2B1, ROW2B1_HEATPUMP_SANITARY_WATER) == Souliss_T1n_AutoOffCoil)
#define SanitaryWaterAutoOnCmd()    Send(ROW2B1, ROW2B1_HEATPUMP_SANITARY_WATER, Souliss_T1n_AutoCmd + 4 /*cycles*/)

#define IsStorageWaterCold()        (temperature_heating < pOutputAsFloat(ROW2B1, ROW2B1_TEMP_FLOOR_FLOW_SETPOINT))
#define IsStorageWaterHot()					(temperature_bottom > pOutputAsFloat(ROW2B1, ROW2B1_TEMP_FLOOR_FLOW_SETPOINT))
#define IsStorageWaterInProduction()  (IsHpFlowToBoiler() && IsHotWaterInProduction())

#define IsHotWaterInProduction()		(IsHeatMode() && (IsHpCirculationOn() || IsHpCirculationAutoOn()) )
#define IsCoolWaterInProduction()		(IsCoolMode() && (IsHpCirculationOn() || IsHpCirculationAutoOn()) )

#define IsHpCirculationOn()				(pOutput(ROW2B1, ROW2B1_HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil)
#define IsHpCirculationAutoOn()		(pOutput(ROW2B1, ROW2B1_HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_AutoOnCoil)
#define HpCirculationAutoOnCmd()	Send(ROW2B1, ROW2B1_HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_AutoCmd + 4 /*cycles*/)

#define IsHpFlowToBoiler()				(/*(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK) && */(pOutput(ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE) == Souliss_T2n_State_Open))
#define IsHpFlowToCollector()			(/*(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK) &&*/ (pOutput(ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE) == Souliss_T2n_State_Close))
#define Is3WayValveMovingToBoiler()		(pOutput(ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Open)
#define Is3WayValveMovingToCollector()	(pOutput(ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Close)
#define SetHpFlowToBoiler()				if(!IsHpFlowToBoiler() && !Is3WayValveMovingToBoiler()) 	Send(ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE, Souliss_T2n_OpenCmd_SW)
#define SetHpFlowToCollector()	 		if(!IsHpFlowToCollector() && !Is3WayValveMovingToCollector()) 	Send(ROW2B1, ROW2B1_HVAC_MAIN_3WAY_VALVE, Souliss_T2n_CloseCmd_SW)

#define IsPumpBoilerToFloorOn()			(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_BOILER_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpBoilerToFloorOff()		(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_BOILER_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpBoilerToFloorAuto()		(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_BOILER_FLOOR) == Souliss_T1n_AutoState)
#define PumpBoilerToFloorAutoOnCmd()   Send(ROW2B1, ROW2B1_HVAC_PUMP_BOILER_FLOOR, Souliss_T1n_AutoCmd + 4 /*cycles*/)

#define IsPumpCollectorToFloorOn()		(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFloorOff()		(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFloorAuto()		(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR) == Souliss_T1n_AutoState)
#define PumpCollectorToFloorAutoOnCmd()  Send(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FLOOR, Souliss_T1n_AutoCmd + 4)

#define IsPumpCollectorToFancoilOn()	(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFancoilOff()	(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFancoilAuto()	(pOutput(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_AutoState)
#define PumpCollectorToFancoilAutoOnCmd()  Send(ROW2B1, ROW2B1_HVAC_PUMP_COLLECTOR_FANCOIL, Souliss_T1n_AutoCmd + 4)

// mix valve on, direction relay off
#define HeatingMixValveOn_ColdDirection()	Send(ROW2B1, ROW2B1_HVAC_VALVES, pOutput(ROW2B1, ROW2B1_HVAC_VALVES) | HEATING_MIX_VALVE_SWITCH_MASK)
// mix valve on, direction relay on
#define HeatingMixValveOn_WarmDirection()	Send(ROW2B1, ROW2B1_HVAC_VALVES, pOutput(ROW2B1, ROW2B1_HVAC_VALVES) | (HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK) )
#define HeatingMixValveOff()				Send(ROW2B1, ROW2B1_HVAC_VALVES, pOutput(ROW2B1, ROW2B1_HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK & ~HEATING_MIX_VALVE_DIRECTION_MASK)

#define	IsHpSetpoint1()					(pOutput(ROW2B1, ROW2B1_HEATPUMP_SETPOINT_2) == Souliss_T1n_OffCoil)
#define	IsHpSetpoint2()					(pOutput(ROW2B1, ROW2B1_HEATPUMP_SETPOINT_2) == Souliss_T1n_OnCoil)
#define HpSetpoint2AutoCmd()     Send(ROW2B1, ROW2B1_HEATPUMP_SETPOINT_2, Souliss_T1n_AutoCmd + 4)

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

#define temp_HVAC_Boiler_Saniary            pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_SANITARY_TEMP)
#define temp_HVAC_Boiler_Heating            pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_HEATING_TEMP)
#define temp_HVAC_Boiler_Bottom             pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_BOTTOM_TEMP)

#define temp_HVAC_Collector_Flow            pOutputAsFloat(ROW1B1,ROW1B1_HVAC_COLLECTOR_FLOW_TEMP)
#define temp_HVAC_Collector_Return          pOutputAsFloat(ROW1B1,ROW1B1_HVAC_COLLECTOR_RETURN_TEMP)
#define temp_HVAC_Collector_Floor_Flow      pOutputAsFloat(ROW1B1,ROW1B1_HVAC_COLLECTOR_FLOOR_FLOW_TEMP)
#define temp_HVAC_Collector_Floor_Return    pOutputAsFloat(ROW1B1,ROW1B1_HVAC_COLLECTOR_FLOOR_RETURN_TEMP)
#define temp_HVAC_Collector_Fancoil_Flow    pOutputAsFloat(ROW1B1,ROW1B1_HVAC_COLLECTOR_FANCOILS_FLOW_TEMP)
#define temp_HVAC_Collector_Fancoil_Return  pOutputAsFloat(ROW1B1,ROW1B1_HVAC_COLLECTOR_FANCOILS_RETURN_TEMP)

#define temp_HVAC_Boiler_Floor_Flow         pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_FLOOR_FLOW_TEMP)
#define temp_HVAC_Boiler_Floor_Return       pOutputAsFloat(ROW1B1,ROW1B1_HVAC_BOILER_FLOOR_RETURN_TEMP)

#define temp_HVAC_HP_Flow                   pOutputAsFloat(ROW1B1,ROW1B1_HVAC_HP_FLOW_TEMP)
#define temp_HVAC_HP_Return                 pOutputAsFloat(ROW1B1,ROW1B1_HVAC_HP_RETURN_TEMP)

#define temp_HVAC_Floor_Flow                pOutputAsFloat(ROW1B1,ROW1B1_HVAC_FLOOR_FLOW_TEMP)
#define temp_HVAC_Floor_Return              pOutputAsFloat(ROW1B1,ROW1B1_HVAC_FLOOR_RETURN_TEMP)
