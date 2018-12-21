#include "SmokeParticle.h"

SmokeParticle::SmokeParticle(Camera* camObj, float lifeSpan)
{
	Init(camObj, lifeSpan);
}

SmokeParticle::~SmokeParticle()
{
	Cleanup();
}

void SmokeParticle::Init(Camera* camObj, float lifeSpan)
{
	_camObj = camObj;
	_lifeSpan = lifeSpan;
}

void SmokeParticle::Cleanup()
{
}

void SmokeParticle::Update(float deltaTime)
{
	_currentCount += deltaTime;
	if (_currentCount > _lifeSpan)
	{
		_currentCount = 0.0f;
		//delete here
	}

	XMFLOAT3 pPos = { _camObj->GetEye().x,  _camObj->GetEye().y, _camObj->GetEye().z };
	XMFLOAT3 pForward = { _camObj->GetAt().x - _camObj->GetEye().x,
							_camObj->GetAt().y - _camObj->GetEye().y,
							_camObj->GetAt().z - _camObj->GetEye().z };

	XMFLOAT3 particleToCam = { _camObj->GetEye().x - _pos.x,
								 _camObj->GetEye().y - _pos.y,
								 _camObj->GetEye().z - _pos.z };

	_forward = _convert->VF3(XMVector3Normalize(_convert->F3V(_forward)));
	particleToCam = _convert->VF3(XMVector3Normalize(_convert->F3V(particleToCam)));

	_forward = particleToCam;
	XMFLOAT3 right = _convert->VF3(XMVector3Normalize(XMVector3Cross(_convert->F3V(_forward), XMVECTOR{0, 1, 0})));
	XMFLOAT3 up = _convert->VF3(XMVector3Normalize(XMVector3Cross(_convert->F3V(_forward), _convert->F3V(right))));

	_world =
	{
		-right.x,	right.y,	-right.z,	_world._14,
		up.x,		up.y,		up.z,		_world._24,
		-_forward.x, _forward.y, -_forward.z, _world._34,
		_pos.x,		_pos.y,		_pos.z,		1.0f
	};

	/*_world =
	{
		0.991908252f,	0.0f,	-0.124034829f,	_world._14,
		-0.00307598151f,	-0.999692440f,	-0.0246078335f,	_world._24,
		-0.135565698,	0.0271131378,	-0.990397274,	_world._34,
		_pos.x,	_pos.y,	_pos.z,	1.0f
	};

	XMFLOAT3 temp = _convert->VF3(XMVector3Dot(_convert->F3V(_forward), _convert->F3V(particleToCam)));
	//XMFLOAT3 temp = _convert->VF3(XMVectorATan2(_convert->F3V(_forward), _convert->F3V(particleToCam)));

	temp = { temp.x * (180 / XM_PI), temp.y * (180 / XM_PI), temp.z * (180 / XM_PI) };

	temp = { (temp.x * 0.01745f) / XM_PI, (temp.y * 0.01745f) / XM_PI, (temp.z * 0.01745f) / XM_PI };
	float tempY = _rot.y;
	float diffY = (abs(_rot.y) - abs(temp.y));
	ApplyRotation({ 0.0f, diffY, 0.0f });
	float newY = _rot.y;
	
	//XMMATRIX tempM = XMMatrixRotationY(temp.y);
	//BaseObject::Update(deltaTime);
	//XMStoreFloat4x4(&_world, XMMatrixRotationY(temp.z));
	//XMStoreFloat4x4(&_world, XMMatrixMultiply(tempM, XMLoadFloat4x4(&_world)));

	//_rot = _convert->VF3(XMVectorATan2(_convert->F3V(_forward), _convert->F3V(particleToCam)));
	//use trig to find angle between particle forward and the direction from particlepos to camera pos
	//use applyrot with angle

	XMFLOAT3 tempUp = _convert->VF3(XMVector3Cross(_convert->F3V(pPos), _convert->F3V(_pos)));

	tempUp = { 0.0f, 1.0f, 0.0f };

	XMFLOAT4X4 tempM;
	XMStoreFloat4x4(&_world, XMMatrixLookAtLH(XMVectorSet(_pos.x, _pos.y, _pos.z, 0.0f),
		XMVectorSet(pPos.x, pPos.y, pPos.z, 0.0f), XMVectorSet(tempUp.x, tempUp.y, tempUp.z, 0.0f)));

	XMFLOAT3 zVector = _convert->VF3(XMVector3Normalize(_convert->F3V(pPos) - _convert->F3V(_pos)));
	XMFLOAT3 xVector = _convert->VF3(XMVector3Normalize(XMVector3Cross(_convert->F3V(tempUp), _convert->F3V(zVector))));
	XMFLOAT3 yVector = _convert->VF3(XMVector3Cross(_convert->F3V(zVector), _convert->F3V(xVector)));

	_world =
	{
		xVector.x, xVector.y, xVector.z, 0,
		yVector.x, yVector.y, yVector.z, 0,
		zVector.x, zVector.y, zVector.z, 0,
		0, 0, 0, 1
	};

	//BaseObject::Update(deltaTime);
	*/
}

void SmokeParticle::Move(float force, DIRECTION dir)
{

}