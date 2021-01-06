// clsDecoder.h

#ifndef _CLSDECODER_h
#define _CLSDECODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "clsPacket.h"
#include "GlobalDefines.h"
#include "clsQueue.h"

class clsDecoder
{
public :
	clsDecoder();
	~clsDecoder();

	void DecodeSentence(String szMsg);
	void DecodePackets(std::vector<clsBasePacket> voPackets);
	void SetOutgoingQueue(clsQueue<String>* pQueue);
	void SetMacAddress(String szMac) { szMacAddress = szMac; }

private:
	void ActOnSentence();

	clsQueue<String> *pOutgoingQueue = NULL;

	String szMacAddress = "";

};


#endif
