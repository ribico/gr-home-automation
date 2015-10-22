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
