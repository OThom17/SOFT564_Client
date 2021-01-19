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
	oSensors = clsSensors();
	oBLEServer = clsBLEServer();

	InitialiseQueues();

	oTcpClient.Initialise();
	oBLEServer.Initialise();

	szMACAddress = oTcpClient.GetMacAddress();
	szLocalAddress = oTcpClient.GetLocalIPAddress();
	oDecoder.SetMacAddress(szMACAddress);
	oDecoder.SetLocalIPAddress(szLocalAddress);

	oSensors.SetProperties(&oClientProperties);
	oDecoder.SetProperties(&oClientProperties);

	oSensors.SetSensorPacket(&oSensorPacket);

	oSensors.Initialise();
}

void clsController::DispatchSensorData()
{
	if (oClientProperties.bAutoSensorSample)
	{
		// Build Sensor Packet Payload
		std::vector<String> vszSensorString = { String(oSensorPacket.fTemp), String(oSensorPacket.fHumidity), String(oSensorPacket.fHeatIndex), String(oSensorPacket.fUltraDistance)};

		clsBasePacket oBase = clsBasePacket("MYCSNS", "CN", szLocalAddress, vszSensorString);
		pTcpOutgoingPacketQueue->push(oBase.GetPacketSentence());
	}
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

		DispatchSensorData();

		oTcpClient.SendPackets();
		delay(1000);
	}
}
