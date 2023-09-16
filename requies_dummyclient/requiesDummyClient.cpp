#include "pch.h"
#include "DummySession.h"
#include "DummyService.h"
#include "ThreadManager.h"
#include "IOCPCore.h"

unsigned int _stdcall Dispatch(void* Args)
{
	DummyService* service = reinterpret_cast<DummyService*>(Args);
	while (true)
		service->GetIOCPCore()->Dispatch();
}

int main()
{
	const char* ip = "58.236.86.23";
	DummyService service(100, ip, 30002, DummySession::MakeGameSession);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	int32 threadCount = sysInfo.dwNumberOfProcessors * 2;

	for (int i = 0; i < threadCount; i++)
	{
		ThreadManager::GetInstance()->Launch(Dispatch, &service);
	}

	service.Start();
	return 0;
}