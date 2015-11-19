U8 gHeatingMixValve_TimerOn = 0;
U16 gHeatingMixValve_TimerCycle = 0;
U16 gHeatingMixValve_TimerCycle_LastVal;

#define HEATINGMIXVALVE_COLD_DIRECTION	0
#define HEATINGMIXVALVE_WARM_DIRECTION	1

/*
by calling this function at every cycle it enable the heating mix valve
at the specified direction for a time equals to duration_on
Do not move the valve duting the rest of the cycle (specified with duration_cycle)

Timers are resetted at the end of every cycle or by calling the function with a different
duration_cycle value

@direction specify the direction of movement (HEATINGMIXVALVE_COLD_DIRECTION / HEATINGMIXVALVE_WARM_DIRECTION)
@duration_on specify time of actuation
@duration_cycle specify time of the complete cycle (actuation + delay)

*/
inline void HeatingMixValve_StepMove(U8 direction, U8 duration_on, U16 duration_cycle)
{
	if( gHeatingMixValve_TimerCycle == 0 ||	gHeatingMixValve_TimerCycle_LastVal != duration_cycle )
	{
		// new cycle, set timers and activate the valve
		gHeatingMixValve_TimerOn = duration_on;
		gHeatingMixValve_TimerCycle = duration_cycle;
		gHeatingMixValve_TimerCycle_LastVal = duration_cycle;
	}

	if( gHeatingMixValve_TimerOn == 0 )
		HeatingMixValveOff();

	else if( gHeatingMixValve_TimerOn > 0 )
	{
		if( direction == HEATINGMIXVALVE_COLD_DIRECTION )
			HeatingMixValveOn_ColdDirection();

		else if( direction == HEATINGMIXVALVE_WARM_DIRECTION )
			HeatingMixValveOn_WarmDirection();
	}
}

inline void Timer_HeatingMixValve()
{
	if( gHeatingMixValve_TimerOn > 0)
		gHeatingMixValve_TimerOn--;

	if( gHeatingMixValve_TimerCycle > 0)
		gHeatingMixValve_TimerCycle--;
}


inline void AdjustBoilerToFloorFlowTemperature(float setpoint)
{
	// control the boiler-floor mix valve to keep the setpoint
	// error is used as a timer value for motorized valve
	float error = setpoint - (mOutputAsFloat(TEMP_FLOOR_FLOW)+mOutputAsFloat(TEMP_FLOOR_RETURN))/2;
	Serial.print("HEATING FLOW ERROR: ");
	Serial.println(error);

	if( error < -1.0 ) // too hot
		HeatingMixValve_StepMove(HEATINGMIXVALVE_COLD_DIRECTION, abs(round(error)), HEATING_MIX_VALVE_CYCLE); // cycle of 105 seconds (50x 2110ms)

	else if( error > 1.0 ) // too cold
		HeatingMixValve_StepMove(HEATINGMIXVALVE_WARM_DIRECTION, abs(round(error)), HEATING_MIX_VALVE_CYCLE); // cycle of 105 seconds (50x 2110ms)
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
