
#define	IP_ADDRESS_4			IP_ADDRESS_TESTB2

#define	RS485_ADDRESS			RS485_ADDRESS_TESTB1
#define RS485_SUPERNODE			0x0000


#define LIGHT_LIVING_1			1
#define LIGHT_LIVING_2			2
#define LIGHT_LIVING_3			3		
#define LIGHT_LIVING_4			4


#define DEBUG

#if defined(DEBUG)
	#define MaCaco_DEBUG_INSKETCH
	#define MaCaco_DEBUG 	1
	#define VNET_DEBUG_INSKETCH
	#define VNET_DEBUG 		1
#endif


#include "bconf/DINo_v2.h"

#include "Souliss.h"
#include "SPI.h"

#include "grhSoulissNetwork.h"
#include "HW_Setup_DINo_v2.h"

#include "grhSoulissCustom.h"

void setup()
{
	#if defined(DEBUG)
		// Open serial communications and wait for port to open:
		Serial.begin(9600);
		while (!Serial) {
		  ; // wait for serial port to connect. Needed for Leonardo only. If need debug setup() void.
		}
		Serial.println("Serial started to DEBUG");
	#endif

	DINo_v2_HW_Setup();

	uint8_t ip_address[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4};
  	uint8_t subnet_mask[4] = {255, 255, 255, 0};
  	uint8_t ip_gateway[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4_NETGW};

	Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
	Souliss_SetAddress(RS485_ADDRESS, SOULISS_SUBNET_MASK, RS485_SUPERNODE);


	// DEFINE TYPICALS
	Set_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);
}

void loop()
{ 
	EXECUTEFAST() {						
		UPDATEFAST();	
		
		FAST_30ms() 
		{
			// SOULISS_READ_INPUTS
			LightsGroupIn(IN1, LIGHT_LIVING_1, LIGHT_LIVING_4);
		} 

		FAST_50ms() 
		{
			// SOULISS_PROCESS_LOGICS
			Logic_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);

			// SOULISS_SET_OUTPUT
			DigOut(RELAY1, Souliss_T1n_Coil, LIGHT_LIVING_1);
			DigOut(RELAY2, Souliss_T1n_Coil, LIGHT_LIVING_2);
			DigOut(RELAY3, Souliss_T1n_Coil, LIGHT_LIVING_3);
			DigOut(RELAY4, Souliss_T1n_Coil, LIGHT_LIVING_4); 
		}
		
		// Here we process all communication with other nodes
		FAST_PeerComms();

		// At first runs, we look for a gateway to join
		// (not needed if the board is listed in the gateway with SetAsPearNode) 
		START_PeerJoin();
		
		// Periodically check if the peer node has joined the gateway
		FAST_1110ms() 
		{
			if(JoinInProgress())	// If join is in progress, toggle the LED at every turn
				ToggleLED();
			else
				TurnOnLED();		// Once completed, turn ON				
		}

		FAST_2110ms()
		{
			// SOULISS_PROCESS_TIMERS;
			Timer_LightsGroup(LIGHT_LIVING_1, LIGHT_LIVING_4);
				
		}			
				
	}
	
	EXECUTESLOW() 
	{	
		UPDATESLOW();
		SLOW_PeerJoin();
	}
} 