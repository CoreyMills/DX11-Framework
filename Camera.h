#ifndef CAMERA_H
#define CAMERA_H

#include "BaseObject.h"
#include "GameObject.h"

enum LOOK_STATE
{
	LOOK_AT = 0,
	LOOK_TO,
};

class Camera
{
public:
	//For LookAt cameras with parents.
	//The eye is based of the parents position and the at is set automatically.
	Camera(BaseObject* parentObj, UINT windowWidth, UINT windowHeight, LOOK_STATE lookState);
	~Camera();

	void Update(float deltaTime);

	void SetLookState(LOOK_STATE lookState) { _lookState = lookState; }

	void SetEye(XMFLOAT4 Eye) { _eye = Eye, _originalEye = Eye; }
	void SetAt(XMFLOAT4 At) { _at = At; }
	void SetUp(XMFLOAT4 Up) { _up = Up; }

	//true = LookAt, false = LookTo
	void SetViewM();
	void SetProjectionM();

	void SetMousePos(float x, float y) { _mouseX = x, _mouseY = y; }
	void SetParentObj(BaseObject* parent) { _parentObj = parent; }

	LOOK_STATE GetLookState() { return _lookState; }

	XMFLOAT4 GetEye() { return _eye; }
	XMFLOAT4 GetAt() { return _at; }
	XMFLOAT4 GetUp() { return _up; }

	XMFLOAT4X4 GetViewM() { return _view; }
	XMFLOAT4X4 GetProjectionM() { return _projection; }
	BaseObject* GetParentObj() { return _parentObj; }

private:
	Convertion* _convert;

	BaseObject* _parentObj;

	float _mouseX, _mouseY;

	LOOK_STATE _lookState;

	XMFLOAT4 _originalEye, _eye, _at, _up;
	XMFLOAT4X4 _view, _projection;

	UINT _windowWidth, _windowHeight;
};
#endif