#pragma once
class Session;

class DummyPlayer
{
	enum
	{
		MOVE_TICK = 1000
	};

private:
	int32		_sessionId;
	Vector3		_pos;
	Vector3		_prevPos;
	State		_state;
	Dir			_dir;
	Dir			_mouseDir;
	Quaternion  _cameraLocalRotation;
	int32		_lastTick = -1;
	float		_speed = 3.0f;
	bool		_resetPos = false;

public:
	DummyPlayer(int32 sessionId, const Vector3& pos, const Quaternion& cameraLocalLotation, State state, Dir dir, Dir mouseDir);
	~DummyPlayer();

	Vector3&	GetPos() { return _pos; }
	Vector3&	GetPrevPos() { return _prevPos; }
	State		GetState() { return _state; }
	Dir			GetDir() { return _dir; }
	Dir			GetMouseDir() { return _mouseDir; }
	Quaternion& GetCameraLocalRotation() { return _cameraLocalRotation; }
	void		SetPrevPos(const Vector3& prevPos) { _prevPos = prevPos; }

	void		PlayerSync(const Vector3& pos, State state, Dir dir, Dir mousedir, const Quaternion& cameraLocalRotation);
	void		PlayerSyncSend(Session* server);
	void		RandomBehavior(Session* server);
	void		CanMoveCalculate(int32 xMax, int32 xMin, int32 zMax, int32 zMin);
	void		PlayerSyncMap(Session* server);
};

