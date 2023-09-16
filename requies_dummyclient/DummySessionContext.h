#pragma once
class DummySession;
class DummySessionContext
{
private:

	static DummySessionContext* _instnace;
	std::set<DummySession*>		_sessions;
	CRITICAL_SECTION			_cs;

public:
	static DummySessionContext* GetInstance()
	{
		if (_instnace == nullptr)
		{
			_instnace = new DummySessionContext();
		}

		return _instnace;
	}

	DummySessionContext()
	{
		InitializeCriticalSection(&_cs);
	}

	~DummySessionContext()
	{
		DeleteCriticalSection(&_cs);
	}

	void AddSession(DummySession* session);
	void RemoveSession(DummySession* session);
	void Command();
};

