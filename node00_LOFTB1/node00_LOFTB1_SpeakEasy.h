#define IsHpLogicManual()			(mOutput(HEATPUMP_MANUAL_MODE) == Souliss_T1n_OnCoil)
#define IsHpLogicAuto()				!IsHpLogicManual()
#define IsHeatMode()				(mOutput(HEATPUMP_COOL) == Souliss_T1n_OffCoil)
#define IsCoolMode()				(mOutput(HEATPUMP_COOL) == Souliss_T1n_OnCoil)

#define IsZoneOpen()				mOutput(HVAC_ZONES)
#define IsHeating()					(IsZoneOpen() && IsHeatMode())
#define IsCooling()					(IsZoneOpen() && IsCoolMode())

#define IsSanitaryWaterInProduction()	(mOutput(HEATPUMP_SANITARY_REQUEST) == Souliss_T1n_OnCoil)
#define IsHotWaterInProduction()		(IsHeatMode() && IsHpCirculationOn())
#define IsCoolWaterInProduction()		(IsCoolMode() && IsHpCirculationOn())

#define IsSanitaryWaterCold()			(temperature_sanitary < SETPOINT_TEMP_SANITARY_WATER_MIN)
#define IsSanitaryWaterHot()			(temperature_sanitary > SETPOINT_TEMP_SANITARY_WATER_MAX)
#define IsHotWaterCold()				(temperature_heating < SETPOINT_TEMP_HEATING_WATER_MIN)
#define IsHotWaterHot()					(temperature_heating > SETPOINT_TEMP_HEATING_WATER_MAX)

#define SanitaryWaterOn()				SetInput(HEATPUMP_SANITARY_REQUEST, Souliss_T1n_OnCmd)	
#define SanitaryWaterOff()				SetInput(HEATPUMP_SANITARY_REQUEST, Souliss_T1n_OffCmd)	

#define IsHpCirculationOn()				(mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil)
#define HpCirculationOn()				if(!IsHpCirculationOn()) SetInput(HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_OnCmd)
#define HpCirculationOff()				if(IsHpCirculationOn()) SetInput(HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_OffCmd)

#define IsHpFlowToBoiler()				(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK)
#define IsHpFlowToCollector()			(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK)
#define SetHpFlowToBoiler()				if(!IsHpFlowToBoiler()) 	SetInput(MAIN_3WAY_VALVE, Souliss_T2n_OpenCmd_SW)
#define SetHpFlowToCollector()	 		if(!IsHpFlowToCollector()) 	SetInput(MAIN_3WAY_VALVE, Souliss_T2n_CloseCmd_SW)

#define IsPumpBoilerToFloorOn()			(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFloorOn()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFancoilOn()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OnCoil)
#define PumpBoilerToFloorOn()			if(!IsPumpBoilerToFloorOn()) 		SetInput(PUMP_BOILER_FLOOR, Souliss_T1n_OnCmd)
#define PumpBoilerToFloorOff()			if(IsPumpBoilerToFloorOn()) 		SetInput(PUMP_BOILER_FLOOR, Souliss_T1n_OffCmd)
#define PumpCollectorToFloorOn()		if(!IsPumpCollectorToFloorOn())		SetInput(PUMP_COLLECTOR_FLOOR, Souliss_T1n_OnCmd)
#define PumpCollectorToFloorOff()		if(IsPumpCollectorToFloorOn())		SetInput(PUMP_COLLECTOR_FLOOR, Souliss_T1n_OffCmd)
#define PumpCollectorToFancoilOn() 		if(!IsPumpCollectorToFancoilOn())	SetInput(PUMP_COLLECTOR_FANCOIL, Souliss_T1n_OnCmd)
#define PumpCollectorToFancoilOff() 	if(IsPumpCollectorToFancoilOn())	SetInput(PUMP_COLLECTOR_FANCOIL, Souliss_T1n_OffCmd)

#define IsHeatingWaterTooHot()				(temperature_floor_flow > SETPOINT_TEMP_HEATING_WATER_MAX)
#define IsHeatingWaterTooCold()				(temperature_floor_flow < SETPOINT_TEMP_HEATING_WATER_MIN)
#define HeatingMixValveOn_ColdDirection()	SetInput(HVAC_VALVES, mOutput(HVAC_VALVES) | HEATING_MIX_VALVE_SWITCH_MASK)
#define HeatingMixValveOn_HotDirection()	SetInput(HVAC_VALVES, mOutput(HVAC_VALVES) | (HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK) )
#define HeatingMixValveOff()				SetInput(HVAC_VALVES, mOutput(HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK & ~HEATING_MIX_VALVE_DIRECTION_MASK)

#define IsHvacOn()						(mOutput(HVAC_MODE) == Souliss_T1n_OnCoil)
