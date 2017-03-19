

#ifdef USE_SSL
#   include <WiFiClientSecure.h>
    WiFiClientSecure client;
#   define PORT 443
#else
    WiFiClient client;
#   define PORT 80
#endif


const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

// Open connection to the HTTP server
bool connect(const char* hostName) {
  grhSendUDPMessage("Connect to ");
  grhSendUDPMessage(hostName);

  bool ok = client.connect(hostName, PORT);

  grhSendUDPMessage(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  grhSendUDPMessage("GET ");
  grhSendUDPMessage(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  return true;
}

// Close the connection with the HTTP server
void disconnect() {
  grhSendUDPMessage("Disconnect");
  client.stop();
}


// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    grhSendUDPMessage("No response or invalid response!");
  }

  return ok;
}

// Skip HTTP until a string is found
bool skipResponseSection(const char* endOfSkip) {

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfSkip);

  if (!ok) {
    grhSendUDPMessage("No response or invalid response!");
  }

  return ok;
}


// Read the body of the response from the HTTP server
void readReponseContent(char* content, size_t maxSize) {
  size_t length = client.readBytes(content, maxSize);
  content[length] = 0;
}
