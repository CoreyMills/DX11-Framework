#include "Camera.h"

Camera::Camera(BaseObject* parentObj, UINT windowWidth, UINT windowHeight, LOOK_STATE lookState)
{
	_convert = new Convertion();

	_parentObj = parentObj;

	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_lookState = lookState;
}

Camera::~Camera()
{
	_parentObj = nullptr;

	_convert = nullptr;
	delete _convert;
}

void Camera::Update(float deltaTime)
{
	if (_parentObj != nullptr && _lookState == LOOK_STATE::LOOK_AT)
	{
		XMFLOAT3 pPos = _parentObj->GetPos();
		XMFLOAT3 pForward = _convert->VF3(XMVector3Normalize(_convert->F3V(_parentObj->GetForwardV())));

		XMFLOAT3 offset = { pForward.x * -_originalEye.z, _originalEye.y, pForward.z * _originalEye.z };

		XMFLOAT4 oldEye = _eye;
		XMFLOAT4 newEye = { pPos.x + offset.x,  pPos.y + offset.y, pPos.z + offset.z, 1.0f };
		
		_at = { pPos.x,  pPos.y, pPos.z, 1.0f };
		_eye = _convert->VF4(XMVectorLerp(_convert->F4V(newEye), _convert->F4V(oldEye), 0.01f));
	}
	SetViewM();
}

void Camera::SetViewM()
{ 
	if (_lookState == LOOK_AT)
	{
		//Initialize the view matrix to look at
		XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMVectorSet(_eye.x, _eye.y, _eye.z, _eye.w), 
			XMVectorSet(_at.x, _at.y, _at.z, _at.w), XMVectorSet(_up.x, _up.y, _up.z, _up.w)));
	}
	else if(_lookState == LOOK_TO)
	{
		//Initialize the view matrix to look towards
		XMStoreFloat4x4(&_view, XMMatrixLookToLH(XMVectorSet(_eye.x, _eye.y, _eye.z, _eye.w),
			XMVectorSet(_at.x, _at.y, _at.z, _at.w), XMVectorSet(_up.x, _up.y, _up.z, _up.w)));
	}
}

void Camera::SetProjectionM() 
{ 
	// Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / (FLOAT)_windowHeight, 0.01f, 100.0f));
}