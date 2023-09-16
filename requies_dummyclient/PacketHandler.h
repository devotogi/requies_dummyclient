#pragma once
class DummySession;

class PacketHandler
{
public:
	static void HandlePacket(DummySession* session, BYTE* packet, int32 packetSize);

private:
	static void HandlePacket_S2C_PLAYERINIT(DummySession* session, BYTE* packet, int32 packetSize);
};

