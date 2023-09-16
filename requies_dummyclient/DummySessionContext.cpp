#include "pch.h"
#include "DummySessionContext.h"
#include "DummySession.h"

DummySessionContext* DummySessionContext::_instnace = nullptr;


void DummySessionContext::AddSession(DummySession* session)
{
	_sessions.insert(session);
}

void DummySessionContext::RemoveSession(DummySession* session)
{
	_sessions.erase(session);
}

void DummySessionContext::Command()
{
	EnterCriticalSection(&_cs);
	for (auto& session : _sessions)
	{
		if (session)
			session->DummyBehavior();
	}
	LeaveCriticalSection(&_cs);
}
