#define IsFloorOn()					(mOutput(FLOOR_MODE) == Souliss_T1n_OnCoil)
#define IsFloorOff()				(mOutput(FLOOR_MODE) == Souliss_T1n_OffCoil)
//#define IsFloorAuto()				(mOutput(FLOOR_MODE) == Souliss_T1n_AutoState)

#define IsFancoilOn()				(mOutput(FANCOIL_MODE) == Souliss_T1n_OnCoil)
#define IsFancoilOff()				(mOutput(FANCOIL_MODE) == Souliss_T1n_OffCoil)
#define IsFancoilAuto()				(mOutput(FANCOIL_MODE) == Souliss_T1n_AutoState)

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

#define SanitaryWaterOn()				if(!IsSanitaryWaterInProduction()) SetInput(HEATPUMP_SANITARY_REQUEST, Souliss_T1n_OnCmd)	
#define SanitaryWaterOff()				if(IsSanitaryWaterInProduction()) SetInput(HEATPUMP_SANITARY_REQUEST, Souliss_T1n_OffCmd)	

#define IsHpCirculationOn()				(mOutput(HEATPUMP_CIRCULATION_PUMP) == Souliss_T1n_OnCoil)
#define HpCirculationOn()				if(!IsHpCirculationOn()) SetInput(HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_OnCmd)
#define HpCirculationOff()				if(IsHpCirculationOn()) SetInput(HEATPUMP_CIRCULATION_PUMP, Souliss_T1n_OffCmd)

#define IsHpFlowToBoiler()				((mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_BOILER_MASK) && (mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_State_Open))
#define IsHpFlowToCollector()			((mOutput(HVAC_VALVES) & MAIN_3WAY_VALVE_COLLECTOR_MASK) && (mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_State_Close))
#define Is3WayValveMovingToBoiler()		(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Open)
#define Is3WayValveMovingToCollector()	(mOutput(MAIN_3WAY_VALVE) == Souliss_T2n_Coil_Close)
#define SetHpFlowToBoiler()				if(!IsHpFlowToBoiler() && !Is3WayValveMovingToBoiler()) 	SetInput(MAIN_3WAY_VALVE, Souliss_T2n_OpenCmd_SW)
#define SetHpFlowToCollector()	 		if(!IsHpFlowToCollector() && !Is3WayValveMovingToCollector()) 	SetInput(MAIN_3WAY_VALVE, Souliss_T2n_CloseCmd_SW)

#define IsPumpBoilerToFloorOn()			(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpBoilerToFloorOff()		(mOutput(PUMP_BOILER_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFloorOn()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFloorOff()		(mOutput(PUMP_COLLECTOR_FLOOR) == Souliss_T1n_OffCoil)
#define IsPumpCollectorToFancoilOn()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OnCoil)
#define IsPumpCollectorToFancoilOff()	(mOutput(PUMP_COLLECTOR_FANCOIL) == Souliss_T1n_OffCoil)
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

#define	IsHpSetpoint1()					(mOutput(HP_SETPOINT_2) == Souliss_T1n_OffCoil)
#define	IsHpSetpoint2()					(mOutput(HP_SETPOINT_2) == Souliss_T1n_OnCoil)
#define	HpSetpoint1()					if(IsHpSetpoint2()) SetInput(HP_SETPOINT_2, Souliss_T1n_OffCmd)
#define	HpSetpoint2()					if(IsHpSetpoint1()) SetInput(HP_SETPOINT_2, Souliss_T1n_OnCmd)

#define temp_BED1	pOutputAsFloat(9,4)
#define UR_BED1		pOutputAsFloat(9,6)
#define temp_BATH1	pOutputAsFloat(12,3) 
#define UR_BATH1	pOutputAsFloat(12,5) 
#define temp_BED2	pOutputAsFloat(10,5)
#define UR_BED2		pOutputAsFloat(10,7)
#define temp_LIVING	pOutputAsFloat(2,5)
#define UR_LIVING	pOutputAsFloat(2,7)
#define temp_BED3	pOutputAsFloat(5,4)
#define UR_BED3		pOutputAsFloat(5,6)
#define temp_BATH2	pOutputAsFloat(4,5) 
#define UR_BATH2	pOutputAsFloat(4,7) 
#define temp_KITCHEN	pOutputAsFloat(7,3) 
#define UR_KITCHEN	pOutputAsFloat(7,5) 
#define temp_DINING	pOutputAsFloat(3,5) 
#define UR_DINING	pOutputAsFloat(3,7) 