#include "BaseObject.h"
#include <string>

BaseObject::BaseObject()
{
	Init();
}

BaseObject::~BaseObject()
{
	Cleanup();
}

void BaseObject::Init()
{
	_convert = new Convertion();

	_pos = { 0.0f, 0.0f, 0.0f };
	_rot = { 0.0f, 0.0f, 0.0f };
	_scale = { 1.0f, 1.0f, 1.0f };

	_forward = { 0.0f, 0.0f, 1.0f };
	/*value setup in objects world matrix
	{
		_right.x, _up.x, _forward.x, _pos.x,
		_right.y, _up.y, _forward.y, _pos.y,
		_right.z, _up.z, _forward.z, _pos.z,
		0.0f, 0.0f, 0.0f, 1.0f,
	};*/
	
	XMStoreFloat4x4(&_world, XMMatrixIdentity());

	_movementSpeed = 5.0f;
	_rotationSpeed = 3.0f;
}

void BaseObject::Cleanup()
{
	_convert = nullptr;
	delete _convert;
}

void BaseObject::LoadStartingValues(std::string fileName)
{
	std::ifstream inFile;
	inFile.open(fileName);

	if (!inFile.good())
	{
		OutputDebugStringA("loading values failed");
	}
	else
	{
		while (!inFile.eof())
		{
			std::string input;

			inFile >> input;

			if (input.compare("scale") == 0)
			{
				inFile >> _scale.x;
				inFile >> _scale.y;
				inFile >> _scale.z;
			}

			if (input.compare("pos") == 0)
			{
				inFile >> _pos.x;
				inFile >> _pos.y;
				inFile >> _pos.z;
			}

			if (input.compare("rot") == 0)
			{
				inFile >> _rot.x;
				inFile >> _rot.y;
				inFile >> _rot.z;

				_rot.x = _rot.x * 0.01745f;
				_rot.y = _rot.y * 0.01745f;
				_rot.z = _rot.z * 0.01745f;
			}
		}
	}
}

void BaseObject::Update(float deltaTime)
{
	XMStoreFloat4x4(&_world, XMMatrixScaling(_scale.x, _scale.y, _scale.z));
	
	//XMStoreFloat4x4(&_world, XMMatrixMultiply(XMLoadFloat4x4(&_world),
											//XMMatrixRotationX(_rot.x) * XMMatrixRotationY(_rot.y) * XMMatrixRotationZ(_rot.z) *
											//XMMatrixTranslation(_pos.x, _pos.y, _pos.z)));
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, XMMatrixRotationX(_rot.x) * XMMatrixRotationY(_rot.y) * 
							XMMatrixRotationZ(_rot.z) * XMMatrixTranslation(_pos.x, _pos.y, _pos.z));

	_forward = { temp._13, temp._23, temp._33 };

	XMStoreFloat4x4(&_world, XMMatrixMultiply(XMLoadFloat4x4(&_world), XMLoadFloat4x4(&temp)));
}

void BaseObject::SetScale(XMFLOAT3 scaleFactor)
{
	_scale = scaleFactor;
}

void BaseObject::ApplyTransformation(XMFLOAT3 newPos)
{
	_pos = { _pos.x + newPos.x, _pos.y + newPos.y, _pos.z + newPos.z }; 
}

void BaseObject::ApplyRotation(XMFLOAT3 newRot)
{
	_rot = { _rot.x + newRot.x, _rot.y + newRot.y, _rot.z + newRot.z };
}