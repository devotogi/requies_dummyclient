#include "pch.h"
#include "PacketHandler.h"
#include "BufferWriter.h"
#include "BufferReader.h"
#include "DummySession.h"
#include "DummySessionContext.h"
#include "DummyPlayer.h"

void PacketHandler::HandlePacket(DummySession* session, BYTE* packet, int32 packetSize)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(packet);
	BYTE* dataPtr = packet + sizeof(PacketHeader);
	int32 dataSize = packetSize - sizeof(PacketHeader);

	switch (header->_type)
	{
	case PacketProtocol::S2C_PLAYERINIT:
		HandlePacket_S2C_PLAYERINIT(session, dataPtr, dataSize);
		break;
	}
}


void PacketHandler::HandlePacket_S2C_PLAYERINIT(DummySession* session, BYTE* packet, int32 packetSize)
{
	int32 sessionId;
	State state;
	Dir dir;
	Dir mouseDir;
	Vector3 pos;
	Quaternion cameraLocalRotation;

	BufferReader br(packet);
	br.Read(sessionId);
	br.Read(state);
	br.Read(dir);
	br.Read(mouseDir);
	br.Read(pos);
	br.Read(cameraLocalRotation);

	DummyPlayer* dp = new DummyPlayer(sessionId, pos, cameraLocalRotation, state, dir, mouseDir);
	session->SetPlayer(dp);
	DummySessionContext::GetInstance()->AddSession(reinterpret_cast<DummySession*>(session));
}
