#include "pch.h"
#include "DummyPlayer.h"
#include "BufferWriter.h"
#include "Session.h"
Vector3 zero = { 0,0,0 };
Vector3 forward = { 0,0,1 };
Vector3 right = { 1,0,0 };
Vector3 back = { 0,0,-1 };
Vector3 left = { -1,0,0 };

int GetRandom0to8()
{
	thread_local std::mt19937 generator(std::random_device{}());

	std::uniform_int_distribution<int> distribution(0, 8);

	return distribution(generator);
}

void GetMoveDirVector3(int dir, Vector3& v3)
{
	switch ((int)dir)
	{
	case NONE:
		v3 = zero;
		break;
	case UP:
		v3 = forward;
		break;
	case UPRIGHT:
		v3 = forward + right;
		break;
	case RIGHT:
		v3 = right;
		break;
	case RIGHTDOWN:
		v3 = right + back;
		break;
	case DOWN:
		v3 = back;
		break;
	case LEFTDOWN:
		v3 = back + left;
		break;
	case LEFT:
		v3 = left;
		break;
	case LEFTUP:
		v3 = forward + left;
	}
}
void RandomCmd(int32 randomCmd, State& state, Dir& dir)
{
	switch (randomCmd)
	{
	case 0: // 멈춤
		state = State::IDLE;
		dir = Dir::NONE;
		break;

	case 1: // 위로 이동
		state = State::MOVE;
		dir = Dir::UP;
		break;

	case 2: // 오른쪽 위로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::RIGHT | Dir::UP);
		break;

	case 3: // 오른쪽으로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::RIGHT);
		break;

	case 4: // 오른쪽 아래로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::RIGHT | Dir::DOWN);
		break;

	case 5: // 아래로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::DOWN);
		break;

	case 6: // 왼쪽 아래로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::LEFT | Dir::DOWN);
		break;

	case 7: // 왼쪽으로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::LEFT);
		break;

	case 8: // 왼쪽 위로 이동
		state = State::MOVE;
		dir = (Dir)(Dir::LEFT | Dir::UP);
		break;
	}
}


DummyPlayer::DummyPlayer(int32 sessionId, const Vector3& pos, const Quaternion& cameraLocalLotation, State state, Dir dir, Dir mouseDir)
	: _sessionId(sessionId), _pos(pos), _state(state), _dir(dir), _mouseDir(mouseDir), _cameraLocalRotation(cameraLocalLotation)
{
}

DummyPlayer::~DummyPlayer()
{
}

void DummyPlayer::PlayerSync(const Vector3& pos, State state, Dir dir, Dir mousedir, const Quaternion& cameraLocalRotation)
{
	_pos = pos;
	_state = state;
	_dir = dir;
	_cameraLocalRotation = cameraLocalRotation;
	_mouseDir = mousedir;
}

void DummyPlayer::PlayerSyncSend(Session* server)
{
	BYTE sendBuffer[42];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();

	bw.Write((int32)_sessionId);
	bw.Write((uint16)_state);
	bw.Write((uint16)_dir);
	bw.Write((uint16)_mouseDir);
	bw.Write((Vector3)_pos);
	bw.Write((Quaternion)_cameraLocalRotation);

	pktHeader->_type = PacketProtocol::C2S_PLAYERSYNC;
	pktHeader->_pktSize = bw.GetWriterSize();
	server->Send(sendBuffer, pktHeader->_pktSize);
}

void DummyPlayer::RandomBehavior(Session* server)
{
	{
		int32 currentTick = GetTickCount64();

		if (_lastTick == -1)
			_lastTick = GetTickCount64();

		int32 deltaTick = currentTick - _lastTick;

		if (deltaTick < MOVE_TICK)
			return;
	}

	{
		Vector3 movedir = zero;
		GetMoveDirVector3(_dir, movedir);

		int32 currentTick = GetTickCount64();
		int32 deltaTick = currentTick - _lastTick;

		float moveTime = (float)deltaTick / 1000;
		Vector3 moveVector = movedir * moveTime * _speed;

		_pos = _pos + moveVector;

		if (_resetPos)
			_pos = { 70,0,70 };

	}

	CanMoveCalculate(252, 8, 252, 8);

	PlayerSyncSend(server);

	_lastTick = GetTickCount64();
}

void DummyPlayer::CanMoveCalculate(int32 xMax, int32 xMin, int32 zMax, int32 zMin)
{
	int32 spinCount = 0;

	while (true)
	{
		if (spinCount >= 150)
		{
			_resetPos = true;
			break;
		}

		int32 randomCode = GetRandom0to8();
		RandomCmd(randomCode, _state, _dir);

		Vector3 movedir = zero;
		GetMoveDirVector3(_dir, movedir);

		Vector3 moveVector = movedir * 1 * _speed;

		Vector3 nextPos = _pos + moveVector;

		if (nextPos.x >= xMax || nextPos.x <= xMin || nextPos.z >= zMax || nextPos.z <= zMin)
		{
			spinCount++;
		}
		else
			break;
	}
}

void DummyPlayer::PlayerSyncMap(Session* server)
{
	// 맵 좌표 동기화
	BYTE sendBuffer[42];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();

	bw.Write((int32)_sessionId);
	bw.Write((uint16)_state);
	bw.Write((uint16)_dir);
	bw.Write((uint16)_mouseDir);
	bw.Write((Vector3)_pos);
	bw.Write((Quaternion)_cameraLocalRotation);

	pktHeader->_type = PacketProtocol::C2S_MAPSYNC;
	pktHeader->_pktSize = bw.GetWriterSize();
	server->Send(sendBuffer, pktHeader->_pktSize);
}
