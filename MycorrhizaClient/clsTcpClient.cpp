#include "clsTcpClient.h"


clsTcpClient::clsTcpClient(IPAddress oIP, int iPrt, String Ssid, String Psswd)
{
    oHost = oIP;
    iPort = iPrt;
    szSsid =  Ssid.c_str();
    szPsswd = Psswd.c_str();
}

clsTcpClient::~clsTcpClient()
{
    client.stop();
}

# pragma region GetSets


std::vector<clsBasePacket> clsTcpClient::GetPacketVector()
{
    std::vector<clsBasePacket> voPacketVector;
    if (pTcpPacketQueue != NULL)
    {
        //While queue still has data pop and send message using UDP.
        while (pTcpPacketQueue->size() > 0)
        {
            String szMsg = pTcpPacketQueue->pop();
            clsBasePacket oPacket = clsBasePacket(szMsg);
            voPacketVector.push_back(oPacket);
        }
    }
    else
    {
        Serial.println("Error in tcp client queue, null pointer");
    }
    return voPacketVector;
}


void clsTcpClient::SetPacketQueue(clsQueue<String> *pQueue)
{
    pTcpPacketQueue = pQueue;
}

String clsTcpClient::GetMacAddress()
{
    return szMACAddr;
}

#pragma endregion



bool clsTcpClient::Initialise()
{
    do
    {
        const char* SSID = "OnePlus 5"; // "BTHub6-MF87";
        const char* PASSWORD = "20202020"; //"bwDUJThHJ6mv";
        WiFi.begin(SSID, PASSWORD);
        delay(5000);
        Serial.println("Connecting to WiFi..");
    } while (WiFi.status() != WL_CONNECTED);

    szMACAddr = WiFi.macAddress();

    Serial.println("Connected to the WiFi network");
    Serial.print("Device Assigned IP: ");
    Serial.println(WiFi.localIP());

    int iConnectionAttempts = 0;
    do 
    {
        Serial.print("Connection Attempt: ");
        Serial.println(iConnectionAttempts);
        iConnectionAttempts++;

        client.connect(oHost, iPort);
        // TODO Include Retry Timeout
    } while (!client.connected());

    if (client.connected())
    {
        Serial.println("Confirm Connected ");
    }
    Serial.println("Connection successful");
    return true;
}


bool clsTcpClient::StartClient()
{
    client.connect(oHost, iPort);
    if (client.connected())
    {
        Serial.println("Client Connected");
        return true;
    }
    Serial.println("Client Not Connected");
    return false;
}

void clsTcpClient::CollectPackets()
{
    if (client.connected()) 
    {
        // Receive the TCP response
        if (client.available()) 
        {
            Serial.println("TCP Data Available");
            String szInputMsg = "";
            while (client.available() > 0)
            {
                char c = client.read();
                szInputMsg += c;
            }
            // Output and Place onto Queue Vector
            pTcpPacketQueue->push(szInputMsg);
        }
        else
        {
            Serial.println("TCP Data Unavailable");
        }
    }
    else
    {
        Serial.println("TCP Client Not Connected, attempting reconnect");
        StartClient();
    }
}

void clsTcpClient::SendString(String szPacket)
{
    try
    {
        client.print(szPacket);
        if (bVerbose)
            Serial.println("INFO: Sent TCP message");
        
    }
    catch (std::exception &e)
    {
        Serial.print("ERROR: Sending TCP Packet");
        Serial.println(e.what());
    }
}

void clsTcpClient::SendPacket(clsBasePacket oPacket)
{
    try
    {
        String szPacket = oPacket.szPacket;
        client.print(szPacket);
        if (bVerbose)
            Serial.println("INFO: Sent TCP message");
        
    }
    catch (std::exception &e)
    {
        Serial.print("ERROR: Sending TCP Packet");
        Serial.println(e.what());
    }
}
