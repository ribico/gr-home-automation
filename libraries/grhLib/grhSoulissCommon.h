/**************************************************************************
	GRHOME -  Souliss Common Definitions
	Versione 0
***************************************************************************/

// Set static addressing mode
#define DYNAMICADDRESSING_INSKETCH
#define DYNAMICADDRESSING  		0x00

// Define the souliss network dimensions
#define NODESIZE_INSKETCH
#define MaCaco_NODES			30		// MaCaco_NODES < 255
#define MaCaco_SLOT				35 		// MaCaco_SLOT < VNET_MAX_PAYLOAD - MaCaco_HEADER and MaCaco_SLOT  < 255 / 3


#if defined(DEBUG)
	#define MaCaco_DEBUG_INSKETCH
	#define MaCaco_DEBUG 	1
	#define VNET_DEBUG_INSKETCH
	#define VNET_DEBUG 		1
#endif

inline void OpenSerialOnDebug()
{
	#if defined(DEBUG)
		Serial.begin(9600);
		while (!Serial) { ; }
		Serial.println("Serial started to DEBUG");
	#endif
}

#define SET_IP_ADDRESS		uint8_t ip_address[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4};\
							uint8_t subnet_mask[4] = {255, 255, 255, 0};\
							uint8_t ip_gateway[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4_NETGW};\
							Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);

#define SET_USART_ADDRESS	Souliss_SetAddress(RS485_ADDRESS, SOULISS_SUBNET_MASK, RS485_SUPERNODE);

#define SOULSS_FAST_PEER_COMMS	FAST_PeerComms();\
								START_PeerJoin();\
								FAST_1110ms()\ 
								{\
									if(JoinInProgress())\
										ToggleLED();\
									else\
										TurnOnLED();\				
								}

#define SOULSS_SLOW_PEER_COMMS 	SLOW_PeerJoin();