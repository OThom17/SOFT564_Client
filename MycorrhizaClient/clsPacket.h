#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <vector>

class clsBasePacket
{
public:
	String szDestination = "";
	String szSource = "";
	String szIdentifier = "";
	std::vector<String> vszPayload = {};
	String szPacket = "";

	clsBasePacket(String szMsg)
	{
		szPacket = szMsg;
		// Tokenise String
		std::vector<String> vszTokenisedMsg = TokeniseOnDelimiter(szMsg);
		if (vszTokenisedMsg.size() >= 3)
		{
			szIdentifier = vszTokenisedMsg.at(0);
			szDestination = vszTokenisedMsg.at(1);
			szSource = vszTokenisedMsg.at(2);
			if (vszTokenisedMsg.size() > 3)
			{
				vszPayload = std::vector<String>(vszTokenisedMsg.begin() + 3, vszTokenisedMsg.end());
			}
		}
		else
		{
			Serial.println("ERROR - Decoder: Insufficient Message Field Count");
		}
	}

	clsBasePacket(String szId, String szDest, String szSrc, std::vector<String> vszPayld)
	{
		szIdentifier = szId;
		szDestination = szDest;
		szSource = szSrc;
		vszPayload = vszPayld;
	}

	String GetPacketSentence(bool bChksum = false)
	{
		String szSentence = "$";
		szSentence = szSentence + szIdentifier + "," + szDestination + "," + szSource;

		if (vszPayload.size() > 0)
		{
			for (auto v : vszPayload)
			{
				szSentence += ',';
				szSentence += v;
			}
		}

		if (bChksum == true)
			szSentence = AppendChecksum(szSentence);
		return szSentence;
	}

private:
	// Delimiter list set for comma-delimited sentences
	char acDelimiterList[1] = { ',' };

	String AppendChecksum(String szMsg)
	{
		const char* cChecksum = szMsg.c_str();

		// Calculate checksum ignoring any $'s in the string
		unsigned int nLength = szMsg.length();

		unsigned int nStart = szMsg.indexOf('$');
		if (nStart > 0)
			nStart += 1;

		// Calculate checksum until either '*' character or the end
		char cChksum = 0;
		for (unsigned int i = nStart; i < nLength; i++)
		{
			char ch = szMsg.charAt(i);
			if (ch == '*')
				break;
			cChksum ^= ch;
		}

		String szMsgChk = szMsg + '*' + int(cChecksum);

		return szMsgChk;
	}

	std::vector<String> TokeniseOnDelimiter(String szMsg)
	{
		char* pMsg = const_cast<char*>(szMsg.c_str());
		std::vector<String> lszTokenised;
		char* pch = strtok(pMsg, acDelimiterList);
		while (pch != NULL)
		{
			lszTokenised.push_back(pch);
			pch = strtok(NULL, acDelimiterList);
		}
		// Serial.print("Tokenised List ->");
		//for (auto v : lszTokenised)
		//{
		//	Serial.print(v);
		//	Serial.print("  ");
		//}
		return lszTokenised;
	}
};

#endif // !BASEPACKET_H
