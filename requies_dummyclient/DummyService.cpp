#include "pch.h"
#include "DummyService.h"
#include "TCPConnector.h"
#include "IOCPCore.h"
#include "Session.h"
#include "DummySessionContext.h"

DummyService::DummyService(int32 maxSession, const char* connetIp, uint16 connetPort, Session* (*clientSessionFactory)(const SOCKET&, const SOCKADDR_IN&)) : ClientService(maxSession,connetIp,connetPort, clientSessionFactory)
{

}

DummyService::~DummyService()
{
}

void DummyService::Start()
{
    wprintf(L"ClientServerStart totalSession : %d\n", _maxSession);
    try
    {
        for (int i = 0; i < _maxSession; i++)
        {
            Session* newSession = _tcpConnector->Connect();
            if (newSession != nullptr)
            {
                _iocpCore->RegisterIOCP(newSession);
                newSession->OnConnect();
            }
        }

        while (true)
        {
            DummySessionContext::GetInstance()->Command();
        }
    }
    catch (std::wstring error)
    {
        wprintf(L"ClientServerStart Error\n");
    }
}
