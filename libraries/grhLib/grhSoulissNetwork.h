/**************************************************************************
	GRHOME - Common Souliss Network Definitions
	Architecture reference:
		|->WiFi-> SoulissApp
		|->eth-> OpenHab on RaspberryPI (192.168.1.2)
		|->eth-> OpenHab on RaspberryPI2 (192.168.1.3)
		|-----------------------------------------------------------------
	 	|->eth-> LOFTB1 (gw)
		|->eth-> ROW1B1 ->485-> ROW1B2 ->485-> ROW1B3 ->485-> ROW1B4
		|->eth-> ROW2B1 ->485-> ROW2B2 ->485-> ROW2B3 ->485-> ROW2B4 ->485-> GTW1B1 ->485-> GTW2B1
		|->eth-> BED1B1
	 	|->eth-> BED2B1 ->485-> BED2B2 ->485-> BED2B3
		|->eth-> GARDB1
		|->WiFi-> LYTB1
***************************************************************************/

#ifndef GR_SOULISS_NETWORK_H
#define GR_SOULISS_NETWORK_H

#define IP_ADDRESS_1				192
#define IP_ADDRESS_2				168
#define IP_ADDRESS_3				1
#define IP_ADDRESS_4_NETGW	1

#define IP_ADDRESS_LOFTB1			100
#define IP_ADDRESS_ROW1B1			101
#define IP_ADDRESS_ROW2B1			102
#define IP_ADDRESS_BED1B1			103
#define IP_ADDRESS_BED2B1			104
#define IP_ADDRESS_GARDB1			105
#define IP_ADDRESS_LYTB1			106
#define IP_ADDRESS_LOFTB2			107

#define IP_ADDRESS_TESTB1			110
#define IP_ADDRESS_TESTB2			111

#define SOULISS_SUBNET_MASK			0xFF00
//#define SOULISS_SUPERNODE			0x0000

#define RS485_ADDRESS_ROW1SN		0xD100
#define RS485_ADDRESS_ROW1B1		0xD101
#define RS485_ADDRESS_ROW1B2		0xD102
#define RS485_ADDRESS_ROW1B3		0xD103
#define RS485_ADDRESS_ROW1B4		0xD104

#define RS485_ADDRESS_ROW2SN		0xD200
#define RS485_ADDRESS_ROW2B1		0xD201
#define RS485_ADDRESS_ROW2B2		0xD202
#define RS485_ADDRESS_ROW2B3		0xD203
#define RS485_ADDRESS_ROW2B4		0xD204
#define RS485_ADDRESS_GTW1B1		0xD205
#define RS485_ADDRESS_GTW2B1		0xD206

#define RS485_ADDRESS_BED1SN		0xD300
#define RS485_ADDRESS_BED1B1		0xD301

#define RS485_ADDRESS_BED2SN		0xD400
#define RS485_ADDRESS_BED2B1		0xD401
#define RS485_ADDRESS_BED2B2		0xD402
#define RS485_ADDRESS_BED2B3		0xD403

#define RS485_ADDRESS_TESTSN		0xD500
#define RS485_ADDRESS_TESTB2		0xD501
#define RS485_ADDRESS_TESTB3		0xD502

#endif
