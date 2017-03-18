/**************************************************************************
	GRHOME -  Souliss Common Definitions
	Versione 0
***************************************************************************/

// Set static addressing mode
#define DYNAMICADDRESSING_INSKETCH
#define DYNAMICADDRESSING  		0x00

// Define the souliss network dimensions
#define MaCaco_NODESIZE_INSKETCH
#define MaCaco_NODES			30		// MaCaco_NODES < 255
#define MaCaco_SLOT				40 		// MaCaco_SLOT < VNET_MAX_PAYLOAD (def 46) - MaCaco_HEADER (def 5) = 41
																// and MaCaco_SLOT  < 255 / 3 = 85

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))
// USAGE: TODO(Remember to fix this)

#ifdef DEBUG
	#define grhOpenSerialOnDebug()	Serial.begin(9600);\
									while (!Serial) { ; }\
									Serial.println("Serial started to DEBUG");
#else
	#define grhOpenSerialOnDebug()
#endif

#ifdef DEBUG_SOULISS
	#define MaCaco_DEBUG_INSKETCH
	#define MaCaco_DEBUG 	1
	#define VNET_DEBUG_INSKETCH
	#define VNET_DEBUG 		1
	#define grhOpenSerialOnDebug()	Serial.begin(9600);\
									while (!Serial) { ; }\
									Serial.println("Serial started to DEBUG");
#endif

// use on linux with netcat -ul 1111
// use on windows with nc -u -l -p 1111
#ifdef WIFI_UDP_DEBUG
	WiFiUDP Udp;
	IPAddress remote_ip(192, 168, 1, 204);
	#define UDP_PORT 1111

	#define UDP_DEBUG_BEGIN Udp.begin(UDP_PORT)

	#define grhSendUDPMessage(msg) Udp.beginPacket(remote_ip, UDP_PORT);\
	Udp.write(msg);\
	Udp.write("\r\n");\
	Udp.endPacket();
#else
	#define grhSendUDPMessage(msg)
#endif


#define grhSetIpAddress(ip4)	uint8_t ip_address[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, ip4};\
							uint8_t subnet_mask[4] = {255, 255, 255, 0};\
							uint8_t ip_gateway[4]  = {IP_ADDRESS_1, IP_ADDRESS_2, IP_ADDRESS_3, IP_ADDRESS_4_NETGW};\
							Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);

#define grhSetUSARTAddress(RS485_ADDRESS, RS485_SUPERNODE)	Souliss_SetAddress(RS485_ADDRESS, SOULISS_SUBNET_MASK, RS485_SUPERNODE);


#define grhFastPeerComms()		FAST_PeerComms();\
								START_PeerJoin();\
								FAST_1110ms()\
								{\
									if(JoinInProgress())\
										ToggleLED();\
									else\
										TurnOnLED();\
								}

#define SHUTTER_SHORT_TIMEOUT		(Souliss_T2n_Timer_Off + 11)
#define SHUTTER_LONG_TIMEOUT		(Souliss_T2n_Timer_Off + 17)

#define	grh_Logic_Temperature(slot)			Souliss_Logic_T52(memory_map, slot, 0.1, &data_changed)
#define	grh_Logic_HVAC_Temperature(slot)	Souliss_Logic_T52(memory_map, slot, 0.5, &data_changed)
#define	grh_Logic_EXT_Temperature(slot)		Souliss_Logic_T52(memory_map, slot, 0.5, &data_changed)
#define	grh_Logic_Humidity(slot)			Souliss_Logic_T53(memory_map, slot, 1.0, &data_changed)

#define grh_Weighted_Average(value1, value2, weight1, weight2)		(!isnan(value1) && !isnan(value2))?((value1*weight1 + value2*weight2) / (weight1+weight2)):(isnan(value1))?value2:value1
#define grh_W_Average(value1, value2) grh_Weighted_Average(value1, value2, 5.0, 1.0)

#define grh_SLOW_PeerJoin() SLOW_710s() { JoinNetwork(); }
