/*********************
TEST BOARD MEGA with Ethernet only acting as GATEWAY

***********************/


#define DEBUG



#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway.h"

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_Mega_2560.h"

#define ANALOGDAQ               0           // This is the memory slot used for the execution of the logic in network_address1
#define DEADBAND                0.05        // Deadband value 5%  


void setup()
{
	grhOpenSerialOnDebug();
	Initialize();
	grhSetIpAddress(IP_ADDRESS_TESTB1);
	SetAsGateway(IP_ADDRESS_TESTB1);
	SetAddressingServer();
	grhInitMEGA();

	// LIST OF NODES
//	SetAsPeerNode(IP_ADDRESS_TESTB2,1);
//	SetAsPeerNode(RS485_ADDRESS_TESTB2,2);

    Set_Thermostat(ANALOGDAQ);          // Set a logic for the thermostat
}

void loop()
{ 
	EXECUTEFAST()
	{						
		UPDATEFAST();
		
		FAST_30ms() 
		{


		} 

		FAST_50ms() 
		{
			float temp = 20.0;
    		Souliss_ImportAnalog(memory_map, ANALOGDAQ+1, &temp);

            Logic_Thermostat(ANALOGDAQ);
		}

		FAST_GatewayComms();	
	}
	
	EXECUTESLOW()
	{	
		UPDATESLOW();

	}
} 