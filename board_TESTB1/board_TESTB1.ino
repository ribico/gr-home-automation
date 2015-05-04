/*********************
TEST BOARD MEGA with Ethernet only acting as GATEWAY

***********************/


//#define DEBUG



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

#define LIGHT1                  0

void setup()
{
	grhOpenSerialOnDebug();
	Initialize();
	grhSetIpAddress(IP_ADDRESS_TESTB1);
	SetAsGateway(IP_ADDRESS_TESTB1);
	SetAddressingServer();
	grhInitMEGA();

	// LIST OF NODES
	SetAsPeerNode(IP_ADDRESS_TESTB2,1);
	SetAsPeerNode(RS485_ADDRESS_TESTB2,2);

	
	Set_SimpleLight(LIGHT1); 
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
			Logic_SimpleLight(LIGHT1);
		}

		FAST_GatewayComms();	
	}
	
	EXECUTESLOW()
	{	
		UPDATESLOW();

	}
} 