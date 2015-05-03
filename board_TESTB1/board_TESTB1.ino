
#define	IP_ADDRESS_4	IP_ADDRESS_TESTB1



//#define DEBUG



#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway.h"

#include "grhSoulissCommon.h"
#include "Souliss.h"
#include "SPI.h"

#include "grhLib.h"
//#include "HW_Setup_Mega_2560.h"

void setup()
{
	OPEN_SERIAL_ON_DEBUG

	//	InitMEGA();

	SET_IP_ADDRESS
	
	SetAsGateway(IP_ADDRESS_4);

	SetAsPeerNode(IP_ADDRESS_TESTB2,1);
	SetAsPeerNode(RS485_ADDRESS_TESTB2,2);

}

void loop()
{ 
	EXECUTEFAST() {						
		UPDATEFAST();
		FAST_GatewayComms();		
		
		FAST_30ms() 
		{


		} 

		FAST_50ms() 
		{

		}
	}
	
	EXECUTESLOW() {	
		UPDATESLOW();

		}
} 