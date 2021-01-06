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
	pTcpOutgoingTcpPacketQueue = new clsQueue<String>;
	oTcpClient.SetPacketQueue(pTcpIncomingPacketQueue);
	oDecoder.SetOutgoingQueue(pTcpOutgoingTcpPacketQueue);
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
}

bool clsController::RunProgram()
{
	// oTcpClient.StartClient();
	while (true)
	{
		// TCP Packet Collection
		oTcpClient.CollectPackets();
		std::vector<clsBasePacket> voPackets = oTcpClient.GetPacketVector();

		// Decode messages
		oDecoder.DecodePackets(voPackets);
		delay(1000);
	}
}
