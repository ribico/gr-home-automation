#ifndef SOULISS_CUSTOM_H
#define SOULISS_CUSTOM_H

#define LIGHT_GROUPS_STEP 500 // duration of a step in milliseconds
#define LightsGroupIn(pin, firstSlot, lastSlot) Souliss_DigInHoldSteps(pin, memory_map, firstSlot, lastSlot, LIGHT_GROUPS_STEP)
#define LowLightsGroupIn(pin, firstSlot, lastSlot) Souliss_LowDigInHoldSteps(pin, memory_map, firstSlot, lastSlot, LIGHT_GROUPS_STEP)

#define WINDOW_DELAY 800
#define DigInWindowToggle(pin, slot) Souliss_DigInHold(pin, Souliss_T2n_StopCmd, Souliss_T2n_ToggleCmd, memory_map, slot, WINDOW_DELAY)


/**************************************************************************/
/*!
	Link the persistence shared memory map to an hardware pin

	It write a digital output pin based on the value of the output into
	persistence memory_map, let a logic act on external devices. Match criteria is based
	on bit-wise AND operation.
*/
/**************************************************************************/
void Souliss_pnLowDigOut(U8 pin, U8 value, U8 *memory_map, U8 node, U8 slot)
{
	// If output is active switch on the pin, else off
	if(pOutput(node,slot) & value)
		dWrite(pin, LOW);
	else
		dWrite(pin, HIGH);
}

/**************************************************************************/
/*!
	Link the shared memory map to an hardware pin

	It write a digital output pin based on the value of the output into
	memory_map, let a logic act on external devices.
*/
/**************************************************************************/
void Souliss_pLowDigOut(U8 pin, U8 value, U8 *memory_map, U8 node, U8 slot)
{
	// If output is active switch on the pin, else off
	if(pOutput(node,slot) == value)
		dWrite(pin, LOW);
	else
		dWrite(pin, HIGH);
}

#define pLowDigOut(pin,value,node,slot)								Souliss_pLowDigOut(pin, value, memory_map, node, slot)
#define pnLowDigOut(pin,value,node,slot)								Souliss_pnLowDigOut(pin, value, memory_map, node, slot)



#endif
