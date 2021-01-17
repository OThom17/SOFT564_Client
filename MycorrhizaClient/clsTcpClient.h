// clsTcpClient.h

#ifndef _CLSTCPCLIENT_h
#define _CLSTCPCLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"

#else
	#include "WProgram.h"
#endif

#include "WiFi.h"
#include "clsDecoder.h"
#include "clsQueue.h"
#include "clsPacket.h"
#include <thread>

class clsTcpClient
{
public:
	clsTcpClient(IPAddress oIP, int iPort, String szSsid, String szPsswd);
	clsTcpClient() {}
	~clsTcpClient();

	WiFiClient client;

	bool Initialise();
	bool StartClient();
	void CollectPackets();

	void SendPackets();
	void SendString(String szMsg);
	std::vector<clsBasePacket> GetPacketVector();
	void SetOutgoingPacketQueue(clsQueue<String> *pQueue);
	void SetIncomingPacketQueue(clsQueue<String> *pQueue);
	String GetMacAddress();
	String GetLocalIPAddress();

	
private:
	
	bool bVerbose = true;
	IPAddress oHost;
	int iPort = 5000;
	const char * szSsid = "";
	const char * szPsswd = "";

	String szMACAddr = "";
	String szLocalIP = "";

	clsQueue<String> *pTcpOutgoingPacketQueue = NULL;
	clsQueue<String> *pTcpIncomingPacketQueue = NULL;
};


#endif

