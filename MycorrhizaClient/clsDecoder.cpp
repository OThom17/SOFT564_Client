// 
// 
// 

#include "clsDecoder.h"

clsDecoder::clsDecoder()
{
	
}

clsDecoder::~clsDecoder()
{

}

void clsDecoder::SetOutgoingQueue(clsQueue<String>* pQueue)
{
	pOutgoingQueue = pQueue;
}

void clsDecoder::DecodePackets(std::vector<clsBasePacket> voPackets)
{
	for (auto& oElement : voPackets) 
	{
		DecodeSentence(oElement.szPacket);
	}
}

void clsDecoder::DecodeSentence(String szMsg)
{
	clsBasePacket oPacket = clsBasePacket(szMsg);
	Serial.print("Decoding: ");
	Serial.println(szMsg);
	Serial.print("Identifier");
	Serial.println(oPacket.szIdentifier);
	if(oPacket.szIdentifier.indexOf("REG") > 0)
	{
		Serial.println("Request Message Identified");
		// Respond with acknowledgment message
		clsBasePacket oPacket = clsBasePacket("MYCREG", "CN", "SRC", std::vector<String>{szLocalIP});
		String szMsg = oPacket.GetPacketSentence(false);
		Serial.print("Producing String: ");
		Serial.println(szMsg);
		// Add to outgoing tcp buffer
		pOutgoingQueue->push(szMsg);
	}
	else if (oPacket.szIdentifier.indexOf("MD") > 0)
	{

	}
}
