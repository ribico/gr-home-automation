
#define IP_ADDRESS_1				192
#define IP_ADDRESS_2				168
#define IP_ADDRESS_3				1
#define	IP_ADDRESS_4				110
#define IP_ADDRESS_4_NETGW			1


#define DEBUG

#if defined(DEBUG)
	#define MaCaco_DEBUG_INSKETCH
	#define MaCaco_DEBUG 	1
	#define VNET_DEBUG_INSKETCH
	#define VNET_DEBUG 		1
#endif


#include "bconf/StandardArduino.h"
#include "conf/ethW5100.h"
#include "conf/Gateway.h"

#include "Souliss.h"
#include "SPI.h"

#include "grhSoulissNetwork.h"
#include "HW_Setup_Mega_2560.h"

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

	InitMEGA();

	uint8_t ip_address[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4};
  	uint8_t subnet_mask[4] = {255, 255, 255, 0};
  	uint8_t ip_gateway[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4_NETGW};

	Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
	
	SetAsGateway(IP_ADDRESS_4);
	SetAsPeerNode(IP_ADDRESS_ROW1B1,1);
	SetAsPeerNode(RS485_ADDRESS_ROW1B2,2);
//	SetAsPeerNode(RS485_ADDRESS_ROW1B3,3);
//	SetAsPeerNode(RS485_ADDRESS_ROW1B4,4);	

//	SetAsPeerNode(IP_ADDRESS_ROW2B1,5);
//	SetAsPeerNode(RS485_ADDRESS_ROW2B2,6);
//	SetAsPeerNode(RS485_ADDRESS_ROW2B3,7);
//	SetAsPeerNode(RS485_ADDRESS_ROW2B4,8);

//	SetAsPeerNode(IP_ADDRESS_BED1B1,9);

//	SetAsPeerNode(IP_ADDRESS_BED2B1,10);
//	SetAsPeerNode(RS485_ADDRESS_BED2B2,11);
//	SetAsPeerNode(RS485_ADDRESS_BED2B3,12);
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