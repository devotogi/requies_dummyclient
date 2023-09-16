#pragma once
#include "Session.h"
class DummyPlayer;

class DummySession : public Session
{
private:
	DummyPlayer* _player;

public:
	static Session* MakeGameSession(const SOCKET& socket, const SOCKADDR_IN& sockAddr) { return new DummySession(socket, sockAddr); }

public:
	DummyPlayer* GetPlayer() { return _player; }
	void SetSessionID(int32 playerId) { _sessionId = playerId; }
	void SetPlayer(DummyPlayer* player) { _player = player; }
public:
	DummySession(const SOCKET& socket, const SOCKADDR_IN& sock);
	virtual ~DummySession();

	virtual void OnRecv(Session* session, BYTE* dataPtr, int32 dataLen);
	virtual void OnDisconnect();
	virtual void OnConnect();

	void DummyBehavior();
};

