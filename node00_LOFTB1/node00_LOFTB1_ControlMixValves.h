U8 gHeatingMixValve_TimerOn = 0;
U8 gHeatingMixValve_TimerCycle = 0;

// Move the heating mix valve of one step asynchronously from the rest of code
// @direction specify the direction of movement (Souliss_T2n_Coil_Open - HOT / Souliss_T2n_Coil_Close - COLD)
// @duration_on specify time of actuation
// @duration_cycle specify time of the complete cycle (actuation + delay)
//

inline void HeatingMixValve_StepMove(U8 direction, U8 duration_on, U8 duration_cycle)
{
	if( gHeatingMixValve_TimerCycle == 0 )
	{
		gHeatingMixValve_TimerOn = duration_on;
		gHeatingMixValve_TimerCycle = duration_cycle;
		SetInput(HEATING_MIX_VALVE, direction);
	}
}

inline void Timer_HeatingMixValveCycle()
{
	if( gHeatingMixValve_TimerCycle > 0 )
		gHeatingMixValve_TimerCycle--;
}


inline void AdjustBoilerToFloorFlowTemperature(float setpoint)
{
	// control the boiler-floor mix valve to keep the setpoint
	// error is used as a timer value for motorized valve
	float error = setpoint - (mOutputAsFloat(TEMP_FLOOR_FLOW)+mOutputAsFloat(TEMP_FLOOR_RETURN))/2;

	if( error < -1.0 ) // too hot
		HeatingMixValve_StepMove(Souliss_T2n_CloseCmd_SW, abs(round(error)), HEATINGMIXVALVE_PWM_CYCLE);

	else if( error > 1.0 ) // too cold
		HeatingMixValve_StepMove(Souliss_T2n_OpenCmd_SW, abs(round(error)), HEATINGMIXVALVE_PWM_CYCLE);
}

#define COLLECTOR_FLOOR_MIX_VALVE_MIN	0
#define COLLECTOR_FLOOR_MIX_VALVE_MAX	200
U8 gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

inline void AdjustCollectorToFloorFlowTemperature(float setpoint)
{
	// control the collector-floor mix valve to keep the setpoint
	// simple proportional control on return floor temperature
	float error = setpoint - (mOutputAsFloat(TEMP_FLOOR_FLOW)+mOutputAsFloat(TEMP_FLOOR_RETURN))/2;

	if(gCollectorToFloorMixValvePos - error > COLLECTOR_FLOOR_MIX_VALVE_MAX)
		gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

	else if(gCollectorToFloorMixValvePos - error < COLLECTOR_FLOOR_MIX_VALVE_MIN)
		gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MIN;

	else
		gCollectorToFloorMixValvePos -= (U8) error;

	analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, gCollectorToFloorMixValvePos);
}
