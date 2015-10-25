U8 gHeatingMixValve_TimerOn = 0;
U8 gHeatingMixValve_TimerCycle = 0;

#define HEATINGMIXVALVE_COLD_DIRECTION	0
#define HEATINGMIXVALVE_WARM_DIRECTION	1

// Move the heating mix valve of one step asynchronously from the rest of code
// @direction specify the direction of movement (HEATINGMIXVALVE_COLD_DIRECTION / HEATINGMIXVALVE_WARM_DIRECTION)
// @duration_on specify time of actuation
// @duration_cycle specify time of the complete cycle (actuation + delay)
//
inline void HeatingMixValve_StepMove(U8 direction, U8 duration_on, U8 duration_cycle)
{
	if( gHeatingMixValve_TimerCycle == 0 )
	{
		// new cycle, set timers and activate the valve
		gHeatingMixValve_TimerOn = duration_on;
		gHeatingMixValve_TimerCycle = duration_cycle;

		if( direction == HEATINGMIXVALVE_COLD_DIRECTION )
			HeatingMixValveOn_ColdDirection();

		else if( direction == HEATINGMIXVALVE_WARM_DIRECTION )
			HeatingMixValveOn_WarmDirection();
	}

	if( gHeatingMixValve_TimerOn == 0 )
		HeatingMixValveOff();

	if( gHeatingMixValve_TimerOn > 0)
		gHeatingMixValve_TimerOn--;

	if( gHeatingMixValve_TimerCycle > 0)
		gHeatingMixValve_TimerCycle--;
}


inline float FloorFlow_HEATING_Setpoint()
{
	float temp_min = temp_BED1;
	if(temp_BED2 != 0) temp_min = min(temp_min, temp_BED2);
	if(temp_BED3 != 0) temp_min = min(temp_min, temp_BED3);
	if(temp_BATH1 != 0) temp_min = min(temp_min, temp_BATH1);
	if(temp_BATH2 != 0) temp_min = min(temp_min, temp_BATH2);
	if(temp_LIVING != 0) temp_min = min(temp_min, temp_LIVING);
	if(temp_KITCHEN != 0) temp_min = min(temp_min, temp_KITCHEN);
	if(temp_DINING != 0) temp_min = min(temp_min, temp_DINING);

	float setpoint_floor_water = mOutputAsFloat(TEMP_AMBIENCE_SET_POINT) + 3.0 // fixed delta above ambience setpoint
															+ (mOutputAsFloat(TEMP_AMBIENCE_SET_POINT) - temp_min); // variable delta at startup
	ImportAnalog(TEMP_FLOOR_FLOW_SETPOINT, &setpoint_floor_water);

	return setpoint_floor_water;
}

inline float FloorFlow_COOLING_Setpoint()
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

	// variable setpoint according to UR and dew point
	ImportAnalog(TEMP_FLOOR_FLOW_SETPOINT, &dew_point_MAX);
	return dew_point_MAX;
}

inline void AdjustFloorFlowTemperature_HEATING()
{
	// control the boiler-floor mix valve to keep the setpoint
	// error is used as a timer value for motorized valve
	float error = FloorFlow_HEATING_Setpoint() - (mOutputAsFloat(TEMP_FLOOR_FLOW)+mOutputAsFloat(TEMP_FLOOR_RETURN))/2;

	if( error < -1.0 ) // too hot
		HeatingMixValve_StepMove(HEATINGMIXVALVE_COLD_DIRECTION, abs(round(error)), 50); // cycle of 105 seconds

	else if( error > 1.0 ) // too cold
		HeatingMixValve_StepMove(HEATINGMIXVALVE_WARM_DIRECTION, abs(round(error)), 50); // cycle of 105 seconds
}

#define COLLECTOR_FLOOR_MIX_VALVE_MIN	0
#define COLLECTOR_FLOOR_MIX_VALVE_MAX	200
U8 gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

inline void AdjustFloorFlowTemperature_COOLING()
{
	// control the collector-floor mix valve to keep the setpoint
	// simple proportional control on return floor temperature
	float error = FloorFlow_COOLING_Setpoint() - (mOutputAsFloat(TEMP_FLOOR_FLOW)+mOutputAsFloat(TEMP_FLOOR_RETURN))/2;

	if(gCollectorToFloorMixValvePos - error > COLLECTOR_FLOOR_MIX_VALVE_MAX)
		gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MAX;

	else if(gCollectorToFloorMixValvePos - error < COLLECTOR_FLOOR_MIX_VALVE_MIN)
		gCollectorToFloorMixValvePos = COLLECTOR_FLOOR_MIX_VALVE_MIN;

	else
		gCollectorToFloorMixValvePos -= (U8) error;

	analogWrite(COLLECTOR_FLOOR_MIX_VALVE_PIN, gCollectorToFloorMixValvePos);
}
