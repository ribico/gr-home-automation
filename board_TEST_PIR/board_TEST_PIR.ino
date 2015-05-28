//#define DEBUG



#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway.h"
#include "conf/Webhook.h" // needed to compile pushetta

#include "grhSoulissCommon.h"

#include <SPI.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "grhSoulissNetwork.h"
#include "grhSoulissCustom.h"
#include "HW_Setup_Mega_2560.h"

#include "grhPushetta.h"


#define ANTITHEFT               0           // This is the memory slot used for the execution of the anti-theft
    


void setup()
{
	Ethernet.begin();

	grhOpenSerialOnDebug();
	Initialize();
	grhSetIpAddress(IP_ADDRESS_TESTB1);
	SetAsGateway(IP_ADDRESS_TESTB1);
	SetAddressingServer();
	grhInitMEGA();

  	pinMode(8, INPUT);
    Set_T41(ANTITHEFT);	
}



void loop()
{ 

	EXECUTEFAST()
	{						
		UPDATEFAST();
		
		FAST_30ms() 
		{
			DigIn(8, Souliss_T4n_Alarm, ANTITHEFT);
//			if (digitalRead(8) == HIGH)
//				Serial.println("INPUT ALLARME");

		} 

		FAST_50ms() 
		{
			Logic_T41(ANTITHEFT);

		}
		FAST_91110ms()
		{
			Timer_T41(ANTITHEFT);

			if( mOutput(ANTITHEFT) == Souliss_T4n_InAlarm)
			{
//				Serial.println("ALLARME");
				sendToPushetta("grhSouliss", "ALLARME");
			}
		}

		FAST_GatewayComms();	
	}
	
	EXECUTESLOW()
	{	
		UPDATESLOW();

	}

}
