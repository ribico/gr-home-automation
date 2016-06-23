U8 gHeatingMixValve_TimerOn = 0;
U16 gHeatingMixValve_TimerCycle = 0;
U16 gHeatingMixValve_TimerCycle_LastVal;

#define HEATINGMIXVALVE_COLD_DIRECTION	1
#define HEATINGMIXVALVE_WARM_DIRECTION	2

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
	/*
#ifdef DEBUG
	Serial.print("STEP MOVE - direction : ");
	Serial.print(direction);
	Serial.print(" - duration_on : ");
	Serial.print(duration_on);
	Serial.print(" - duration_cycle : ");
	Serial.println(duration_cycle);
#endif
*/
	if( direction == 0 || duration_on == 0 || duration_cycle == 0)
	{
		mInput(HVAC_VALVES) = mOutput(HVAC_VALVES);
		Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);
		return;
	}

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

		Souliss_Logic_T1A(memory_map, HVAC_VALVES, &data_changed);
/*
		#ifdef DEBUG
			Serial.print("mInput(HVAC_VALVES): ");
			Serial.println(mInput(HVAC_VALVES), BIN);
		#endif
*/
	}
}

inline void Timer_HeatingMixValve()
{
/*
	#ifdef DEBUG
		Serial.print("gHeatingMixValve_TimerOn : ");
		Serial.print(gHeatingMixValve_TimerOn);
		Serial.print(" - gHeatingMixValve_TimerCycle : ");
		Serial.println(gHeatingMixValve_TimerCycle);
	#endif
*/
	if( gHeatingMixValve_TimerOn > 0)
		gHeatingMixValve_TimerOn--;

	if( gHeatingMixValve_TimerCycle > 0)
		gHeatingMixValve_TimerCycle--;
}


inline void AdjustBoilerToFloorFlowTemperature(float setpoint)
{
	// control the boiler-floor mix valve to keep the setpoint
	// error is used as a timer value for motorized valve
	float error = setpoint - (2*temp_HVAC_Floor_Flow + temp_HVAC_Floor_Return)/3;
/*
#ifdef DEBUG
	Serial.print("HEATING FLOW ERROR: ");
	Serial.println(error);
#endif
*/
	U8 direction = 0;
	U8 duration_on = 0;

	if( error < -1.0 ) // too hot
	{
		direction = HEATINGMIXVALVE_COLD_DIRECTION;
		duration_on = abs(round(error));
	}
	else if( error > 1.0 ) // too cold
	{
		direction = HEATINGMIXVALVE_WARM_DIRECTION;
		duration_on = abs(round(error));
	}

	HeatingMixValve_StepMove(direction, duration_on, HEATING_MIX_VALVE_CYCLE); // cycle of 105 seconds (50x 2110ms)
}


inline void AdjustCollectorToFloorFlowTemperature(float setpoint)
{
	if( !IsTempValid(temp_HVAC_Floor_Flow) ) //&& IsTempValid(temp_HVAC_Floor_Return) )
			return;
			
	// control the collector-floor mix valve to keep the setpoint
	// simple proportional control on return floor temperature
	float error = setpoint - temp_HVAC_Floor_Flow; //(temp_HVAC_Floor_Flow + temp_HVAC_Floor_Return)/2;
	float current_pos = mOutputAsFloat(COLLECTOR_FLOOR_MIX_VALVE_POS);

	if(current_pos - error > COLLECTOR_FLOOR_MIX_VALVE_MAX)
		current_pos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

	else if(current_pos - error < COLLECTOR_FLOOR_MIX_VALVE_MIN)
		current_pos = COLLECTOR_FLOOR_MIX_VALVE_MIN;

	else
		current_pos -= (U8) error;

		ImportAnalog(COLLECTOR_FLOOR_MIX_VALVE_POS, &current_pos);


	#ifdef DEBUG
		Serial.print("Floor Flux Setpoint : ");
		Serial.print(setpoint);
		Serial.print(" - measured : ");
		Serial.print(temp_HVAC_Floor_Flow);
		Serial.print(" - error : ");
		Serial.print(error);
		Serial.print(" - current_pos : ");
		Serial.println(current_pos);
	#endif


}
