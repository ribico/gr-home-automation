#ifndef SOULISS_CUSTOM_H
#define SOULISS_CUSTOM_H

/**************************************************************************
/*!
	Link an hardware pin to the shared memory map, active on rising edge
	Identify multiple states, press and hold (with increasing value).
	The memory_map associated value is equal to count of STEP_DURATION elapsed during hold.

	Used to detect how long the input has been held.
*/	
/**************************************************************************/
#define STEP_DURATION 500 // duration of a step in milliseconds
/*
U8 Souliss_DigInHoldDuration(U8 pin, U8 *memory_map, U8 slot)
{
	// If pin is on, set the "value"
	if(digitalRead(pin) && !InPin[pin]) // first press: current input=1, previous input=0
	{
		if(memory_map[MaCaco_AUXIN_s + slot] != 0)
			memory_map[MaCaco_AUXIN_s + slot] = 0; // pressed to turn off
		else
			memory_map[MaCaco_AUXIN_s + slot] = 1; // first press set to 1

		time = millis();								// Record time
		
		InPin[pin] = true;
		memory_map[MaCaco_OUT_s + slot] = MaCaco_DATACHANGED; // data has changed due to pin press
		return MaCaco_DATACHANGED;
	}
	else if(digitalRead(pin) && (abs(millis()-time) > STEP_DURATION))
	{
		// Write elapsed STEP_DURATION count in memory map
		if ( memory_map[MaCaco_AUXIN_s + slot] == (U8) ( abs(millis()-time) / STEP_DURATION + 1 ) )
		{
			return MaCaco_NODATACHANGED;	
		}

		memory_map[MaCaco_AUXIN_s + slot] = (U8) ( abs(millis()-time) / STEP_DURATION + 1 );		
		memory_map[MaCaco_OUT_s + slot] = MaCaco_DATACHANGED;  // data has changed due to pin press
		return MaCaco_DATACHANGED;
	}
	else if(!digitalRead(pin))
		InPin[pin] = false;

	memory_map[MaCaco_OUT_s + slot] = MaCaco_NODATACHANGED;		
	return MaCaco_NODATACHANGED;
}
*/

U8 Souliss_DigInHoldDuration_Helper(U8 pin, U8 pin_value, U8 *memory_map, U8 firstSlot, U8 lastSlot)
{
	if (pin_value == PINRESET) // unpressed button
	{
		InPin[pin] = PINRESET;
		return MaCaco_NODATACHANGED;
	}

	// if here the button is pressed
	if(InPin[pin] == PINRESET) // it was unpressed before
	{
		InPin[pin] = PINSET;
		time = millis();								// Record time
		// this is the first cycle detecting the button press: current input=1, previous input=0

		// verify if some of the lights in the group are ON
		for(U8 i=firstSlot; i<=lastSlot; i++)
		{
			if(memory_map[MaCaco_OUT_s + i] == Souliss_T1n_OnCoil)
			{
				// there's at least one light ON
				// the user must have been pressing to turn everything OFF
				// then cycle on all the remaining slots to put set all of them to OFF
				for (U8 j = i; j <= lastSlot; j++)
					memory_map[MaCaco_IN_s + j] = Souliss_T1n_OffCmd;

				return MaCaco_DATACHANGED; 
			}
		}

		// do nothing to filter false activations for spikes
		// the first slot will be set to on on the next cicle

		// if here all lights were OFF (remember it with InPin[pin]=2)
		InPin[pin] = PINACTIVE;
		return MaCaco_NODATACHANGED;
	}
	else if(InPin[pin]==PINACTIVE && (abs(millis()-time) > 0) && (abs(millis()-time) < STEP_DURATION))
	{
		if(memory_map[MaCaco_OUT_s + firstSlot] != Souliss_T1n_OnCoil)
		{
			// the user must have been pressing to turn some lights ON
			// let's start to turn ON the first light in the group
			memory_map[MaCaco_IN_s + firstSlot] = Souliss_T1n_OnCmd;
			return MaCaco_DATACHANGED;	
		}

	}	
	else if(InPin[pin]==PINACTIVE && (abs(millis()-time) > STEP_DURATION))
	{
		// this cycle is executed while the button is kept pressed
		// the current input is 1, the previous input was 1 and some time passed from the first press

		U8 powered_lights_count = (U8) ( abs(millis()-time) / STEP_DURATION + 1 );
		if (powered_lights_count > lastSlot - firstSlot + 1)
			powered_lights_count = lastSlot - firstSlot + 1;
	
		U8 MaCaco_STATUS = MaCaco_NODATACHANGED;

		for(U8 i=0; i<powered_lights_count; i++)
		{
			if(memory_map[MaCaco_OUT_s + firstSlot + i] == Souliss_T1n_OffCoil)
			{
				// only if the light is not powered, turn it on
				memory_map[MaCaco_IN_s + firstSlot + i] = Souliss_T1n_OnCmd;
				MaCaco_STATUS = MaCaco_DATACHANGED;
			}
		}
		return MaCaco_STATUS;
	}
	
	return MaCaco_NODATACHANGED;
}

U8 Souliss_DigInHoldDuration(U8 pin, U8 *memory_map, U8 firstSlot, U8 lastSlot)
{
	Souliss_DigInHoldDuration_Helper(pin, digitalRead(pin), memory_map, firstSlot, lastSlot);
}

U8 Souliss_LowDigInHoldDuration(U8 pin, U8 *memory_map, U8 firstSlot, U8 lastSlot)
{
	Souliss_DigInHoldDuration_Helper(pin, !digitalRead(pin), memory_map, firstSlot, lastSlot);
}


void Souliss_Logic_T11Group(U8 *memory_map, U8 firstSlot, U8 lastSlot, U8 *trigger)
{
	// cycle upon all OUT slots of the groups
	for(U8 i=firstSlot; i<=lastSlot; i++)
		Souliss_Logic_T11(memory_map, i, trigger);	
}



void Souliss_T11Group_Timer(U8 *memory_map, U8 firstSlot, U8 lastSlot)
{
	for(U8 slot=firstSlot; slot<=lastSlot; slot++)
	{
		Souliss_T11_Timer(memory_map, slot);
	}
}

void Souliss_SetT11Group(U8 *memory_map, U8 firstSlot, U8 lastSlot)
{
	for(U8 slot=firstSlot; slot<=lastSlot; slot++)
	{
		memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_RstCmd;
		Souliss_SetT11(memory_map, slot);
	}
}

#define Set_LightsGroup(firstSlot, lastSlot) Souliss_SetT11Group(memory_map, firstSlot, lastSlot)
#define Timer_LightsGroup(firstSlot, lastSlot) Souliss_T11Group_Timer(memory_map, firstSlot, lastSlot)
//#define Logic_LightsGroup(group, offset, firstSlot, lastSlot) Souliss_Logic_T11Group(memory_map, group, offset, firstSlot, lastSlot, &data_changed)
#define Logic_LightsGroup(firstSlot, lastSlot) Souliss_Logic_T11Group(memory_map, firstSlot, lastSlot, &data_changed)
//#define ssLightsGroupIN(pin, slot) Souliss_DigInHoldDuration(pin, memory_map, slot)
#define LightsGroupIn(pin, firstSlot, lastSlot) Souliss_DigInHoldDuration(pin, memory_map, firstSlot, lastSlot)
#define LowLightsGroupIn(pin, firstSlot, lastSlot) Souliss_LowDigInHoldDuration(pin, memory_map, firstSlot, lastSlot)

#define WINDOW_DELAY 800
#define DigInWindowToggle(pin, slot) Souliss_DigInHold(pin, Souliss_T2n_StopCmd, Souliss_T2n_ToggleCmd, memory_map, slot, WINDOW_DELAY)

#endif