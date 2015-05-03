
#define	IP_ADDRESS_4	IP_ADDRESS_TESTB1



//#define DEBUG



#include "bconf/StandardArduino.h"
//#include "bconf/DINo_v2.h"
#include "conf/ethW5100.h"
#include "conf/Gateway.h"

#include "Souliss.h"
#include "SPI.h"

#include "grhLib.h"
//#include "HW_Setup_Mega_2560.h"
//#include "HW_Setup_DINo_v2.h"

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

	//	InitMEGA();
	//DINo_v2_HW_Setup();

	uint8_t ip_address[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4};
  	uint8_t subnet_mask[4] = {255, 255, 255, 0};
  	uint8_t ip_gateway[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4_NETGW};

	Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
	
	SetAsGateway(IP_ADDRESS_4);

	SetAsPeerNode(IP_ADDRESS_TESTB2,1);
	SetAsPeerNode(RS485_ADDRESS_TESTB2,2);

}

void loop()
{ 
	EXECUTEFAST() {						
		UPDATEFAST();	
		
		FAST_30ms() 
		{


		} 

		FAST_50ms() 
		{

		}
		
		// Here we process all communication with other nodes
		FAST_GatewayComms();		
				
	}
	
	EXECUTESLOW() {	
		UPDATESLOW();

		}
} 