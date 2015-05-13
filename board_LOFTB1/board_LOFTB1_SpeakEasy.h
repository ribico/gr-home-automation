#define IsHpLogicAuto()				(mOutput(HEATPUMP_MANUAL_MODE) == Souliss_T1n_OffCoil)
#define IsHeatMode()				(mOutput(HEATPUMP_COOL) == Souliss_T1n_OffCoil)
#define IsCoolMode()				(mOutput(HEATPUMP_COOL) == Souliss_T1n_OnCoil)

#define IsZoneOpen()				mOutput(HVAC_ZONES)
#define IsHeating()					(IsZoneOpen() && IsHeatMode())
#define IsCooling()					(IsZoneOpen() && IsCoolMode())

#define IsSanitaryWaterInProduction()	(mOutput(HEATPUMP_SANITARY_REQUEST) == Souliss_T1n_OnCoil)
#define IsHotWaterInProduction()		(IsHeatMode() && (mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil))
#define IsCoolWaterInProduction()		(IsCoolMode() && (mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil))

#define SanitaryWaterOn()				(mInput(HEATPUMP_SANITARY_REQUEST) = Souliss_T1n_OnCmd)	
#define SanitaryWaterOff()				(mInput(HEATPUMP_SANITARY_REQUEST) = Souliss_T1n_OffCmd)	

#define IsHpCirculationOn()				(mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil)
#define HpCirculationOn()				(mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OnCmd)
#define HpCirculationOff()				(mInput(HEATPUMP_CIRCULATION_PUMP) = Souliss_T1n_OffCmd)

#define IsHpFlowToBoiler()				(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK)
#define IsHpFlowToCollector()			(mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK)
#define SetHpFlowToBoiler()				mInput(MAIN_3WAY_VALVE) = Souliss_T2n_OpenCmd_SW
#define SetHpFlowToCollector()	 		mInput(MAIN_3WAY_VALVE) = Souliss_T2n_CloseCmd_SW

#define IsPumpBoilerToFloorOn()			(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFloorOn()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFancoilOn()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OnCoil)
#define PumpBoilerToFloorOn()			(mInput(PUMP_BOILER_FLOOR) = Souliss_T1n_OnCmd)
#define PumpBoilerToFloorOff()			(mInput(PUMP_BOILER_FLOOR) = Souliss_T1n_OffCmd)
#define PumpCollectorToFloorOn()		(mInput(PUMP_COLLECTOR_FLOOR) = Souliss_T1n_OnCmd)
#define PumpCollectorToFloorOff()		(mInput(PUMP_COLLECTOR_FLOOR) = Souliss_T1n_OffCmd)
#define PumpCollectorToFancoilOn() 		(mInput(PUMP_COLLECTOR_FANCOIL) = Souliss_T1n_OnCmd)
#define PumpCollectorToFancoilOff() 	(mInput(PUMP_COLLECTOR_FANCOIL) = Souliss_T1n_OffCmd)

#define MixValveOn_ColdDirection()		(mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) | HEATING_MIX_VALVE_SWITCH_MASK)
#define MixValveOn_HotDirection()		(mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) | (HEATING_MIX_VALVE_SWITCH_MASK | HEATING_MIX_VALVE_DIRECTION_MASK) )
#define MixValveOff()					(mInput(HVAC_VALVES) = mOutput(HVAC_VALVES) & ~HEATING_MIX_VALVE_SWITCH_MASK)
