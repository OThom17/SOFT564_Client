#include "clsController.h"

clsController::clsController()
{

}

clsController::~clsController()
{

}

void clsController::InitialiseQueues()
{
	pTcpIncomingPacketQueue = new clsQueue<String>;
	pTcpOutgoingPacketQueue = new clsQueue<String>;

	oTcpClient.SetIncomingPacketQueue(pTcpIncomingPacketQueue);
	oTcpClient.SetOutgoingPacketQueue(pTcpOutgoingPacketQueue);

	oDecoder.SetOutgoingQueue(pTcpOutgoingPacketQueue);
}

void clsController::InitialiseController()
{
	// IPAddress IP(192, 168, 1, 137);
	oTcpClient = clsTcpClient(IPAddress(192,168,43,196), 5000, "OnePlus 5", "20202020");
	oBLEServer = clsBLEServer();

	InitialiseQueues();
	oTcpClient.Initialise();
	oBLEServer.Initialise();
	szMACAddress = oTcpClient.GetMacAddress();
	oDecoder.SetMacAddress(szMACAddress);

	oSensors = clsSensors();
	oSensors.Initialise();
}

bool clsController::RunProgram()
{
	// oTcpClient.StartClient();
	for (;;)
	{
		// TCP Packet Collection
		oTcpClient.CollectPackets();
		std::vector<clsBasePacket> voPackets = oTcpClient.GetPacketVector();

		// Decode messages
		oDecoder.DecodePackets(voPackets);
#
		oTcpClient.SendPackets();
		delay(1000);
	}
}
