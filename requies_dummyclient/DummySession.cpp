#include "pch.h"
#include "DummySession.h"
#include "PacketHandler.h"
#include "DummyPlayer.h"
#include "DummySessionContext.h"
#include "BufferWriter.h"

DummySession::DummySession(const SOCKET& socket, const SOCKADDR_IN& sock) : Session(socket, sock)
{
}

DummySession::~DummySession()
{
	if (_player)
		delete _player;
}

void DummySession::OnRecv(Session* session, BYTE* dataPtr, int32 dataLen)
{
	PacketHandler::HandlePacket(static_cast<DummySession*>(session), dataPtr, dataLen);
}

void DummySession::OnDisconnect()
{
	DummySessionContext::GetInstance()->RemoveSession(this);
}

void DummySession::OnConnect()
{

}

void DummySession::DummyBehavior()
{
	Vector3 prevPos = _player->GetPos();

	int prevPosX = (int)prevPos.x;
	int prevPosZ = (int)prevPos.z;

	_player->RandomBehavior(this);
	Vector3 nowPos = _player->GetPos();

	int nowPosX = (int)nowPos.x;
	int nowPosZ = (int)nowPos.z;

	if (prevPosX != nowPosX || prevPosZ != nowPosZ)
		_player->PlayerSyncMap(this);
}
