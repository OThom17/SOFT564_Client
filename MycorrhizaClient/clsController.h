// clsController.h

#ifndef _CLSCONTROLLER_h
#define _CLSCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GlobalDefines.h"
#include "clsQueue.h"
#include "clsDecoder.h"
#include "clsTcpClient.h"
#include "clsBLEServer.h"
#include "clsSensors.h"

class clsController
{
public:
	clsController();
	~clsController();
	bool RunProgram();
	void InitialiseQueues();
	void InitialiseController();

private:
	clsDecoder oDecoder;
	clsTcpClient oTcpClient;
	clsBLEServer oBLEServer;
	clsSensors oSensors;

	IPAddress oIP = IPAddress(192, 168, 1, 137);
	int iPORT = 5000;

	clsQueue<String> *pTcpIncomingPacketQueue = NULL;
	clsQueue<String> *pTcpOutgoingTcpPacketQueue = NULL;

	String szMACAddress = "";
};


#endif

