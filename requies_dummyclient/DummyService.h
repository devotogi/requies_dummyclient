#pragma once
#include "ClientService.h"
class DummyService : public ClientService
{
public:
	DummyService(int32 maxSession, const char* connetIp, uint16 connetPort, Session* (*clientSessionFactory)(const SOCKET&, const SOCKADDR_IN&));
	virtual ~DummyService();

	virtual void Start() override;
};

