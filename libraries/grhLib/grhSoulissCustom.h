#ifndef SOULISS_CUSTOM_H
#define SOULISS_CUSTOM_H

#define LIGHT_GROUPS_STEP 500 // duration of a step in milliseconds
#define LightsGroupIn(pin, firstSlot, lastSlot) Souliss_DigInHoldSteps(pin, memory_map, firstSlot, lastSlot, LIGHT_GROUPS_STEP)
#define LowLightsGroupIn(pin, firstSlot, lastSlot) Souliss_LowDigInHoldSteps(pin, memory_map, firstSlot, lastSlot, LIGHT_GROUPS_STEP)

#define WINDOW_DELAY 800
#define DigInWindowToggle(pin, slot) Souliss_DigInHold(pin, Souliss_T2n_StopCmd, Souliss_T2n_ToggleCmd, memory_map, slot, WINDOW_DELAY)

#endif