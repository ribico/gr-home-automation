#ifndef GRH_PUSHETTA_H
#define GRH_PUSHETTA_H

// Be sure this address is unique in your network
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };        
char APIKEY[] = "449cc02191bf06bed2e8c6d432a874e20944d817"; // Put here your API key
char CHANNEL[] = "grhSouliss";                              // and here your channel name
 
char serverName[] = "api.pushetta.com";
boolean lastConnected = false; 
EthernetClient client;
                
                
                
                
//Function for sending the request to Pushetta
inline void sendToPushetta(char channel[], String text)
{
	client.stop();

	if (client.connect(serverName, 80)) 
	{                   
		client.print("POST /api/pushes/");
		client.print(channel);
		client.println("/ HTTP/1.1");
		client.print("Host: ");
		client.println(serverName);
		client.print("Authorization: Token ");
		client.println(APIKEY);
		client.println("Content-Type: application/json");
		client.print("Content-Length: ");
		client.println(text.length()+46);
		client.println();
		client.print("{ \"body\" : \"");
		client.print(text);
		client.println("\", \"message_type\" : \"text/plain\" }");
		client.println(); 
	} 
}

inline void sendToPushetta(String text)
{
	sendToPushetta(CHANNEL, text);
}


#endif