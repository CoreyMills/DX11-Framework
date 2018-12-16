#include "GameObject.h"

GameObject::GameObject()
{
	Init();
}

GameObject::~GameObject()
{
	Cleanup();
}

void GameObject::Init()
{
	_mesh = new Mesh();
	_mtrl = new Material();

	_direction = MOVE_NONE;

	_movementEnabled = false;

	_acceleration = 0.0f;
	_maxSpeed = 10.0f;
}

void GameObject::Cleanup()
{
	delete _mesh;
	_mesh = nullptr;

	_mtrl = nullptr;
	delete _mtrl;
}

void GameObject::Update(float deltaTime)
{
	if (_movementEnabled)
	{
		bool isDown = false;

		//once rotation working add transform to forward vector
		if (GetAsyncKeyState('W'))
		{
			_direction = MOVE_FORWARD;
			isDown = true;
		}
		if (GetAsyncKeyState('S'))
		{
			_direction = MOVE_BACKWARD;
			isDown = true;
		}

		if (isDown)
		{
			if (_direction == MOVE_FORWARD && _acceleration < _maxSpeed)
			{
				_acceleration += 0.01f;
			}
			else if (_direction == MOVE_BACKWARD && _acceleration > -_maxSpeed)
			{
				_acceleration -= 0.01f;
			}
		}

		if (!isDown)
		{
			if (_direction == MOVE_FORWARD && _acceleration > 0.0f)
			{
				_acceleration -= 0.005f;
			}
			else if(_direction == MOVE_BACKWARD && _acceleration < 0.0f)
			{
				_acceleration += 0.005f;
			}

			if (_acceleration == 0.0f)
			{
				_direction = MOVE_NONE;
			}
		}

		Move((_movementSpeed * _acceleration) * deltaTime, _direction);
		
		if (GetAsyncKeyState('D'))
		{
			if (_direction == DIRECTION::MOVE_FORWARD)
			{
				ApplyRotation({ 0.0f, _rotationSpeed * deltaTime, 0.0f });
			}
			if (_direction == DIRECTION::MOVE_BACKWARD)
			{
				ApplyRotation({ 0.0f, -_rotationSpeed * deltaTime, 0.0f });
			}
		}
		else if (GetAsyncKeyState('A'))
		{
			if (_direction == DIRECTION::MOVE_FORWARD)
			{
				ApplyRotation({ 0.0f, -_rotationSpeed * deltaTime, 0.0f });
			}
			if (_direction == DIRECTION::MOVE_BACKWARD)
			{
				ApplyRotation({ 0.0f, _rotationSpeed * deltaTime, 0.0f });
			}
		}
	}

	BaseObject::Update(deltaTime);
}

void GameObject::Move(float force, DIRECTION dir)
{
	XMFLOAT3 oldPos = _pos;
	XMVECTOR nForward = XMVector3Normalize({ _forward.x, _forward.y, _forward.z });

	XMFLOAT3 offset = { _convert->VF4(nForward).x * -force, oldPos.y, _convert->VF4(nForward).z * force };

	switch (dir)
	{
	case MOVE_FORWARD:
	case MOVE_BACKWARD:
		_pos = { _pos.x + offset.x, _pos.y + offset.y, _pos.z + offset.z };
		break;
	}
}

void GameObject::Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb)
{
	//take values from 'XMFLOAT4' and put them into 'const float*'
	float tempBF[] = { _mtrl->blendFactor.x, _mtrl->blendFactor.y,
		_mtrl->blendFactor.z, _mtrl->blendFactor.w };

	if (_mtrl->isTransparent)
	{
		// Set the blend state for transparent objects
		deviceContext->OMSetBlendState(_mtrl->transparency, tempBF, 0xffffffff);
	}
	else
	{
		// Set the default blend state (no blending) for opaque objects
		deviceContext->OMSetBlendState(0, 0, 0xffffffff);
	}

	cb.ambientMaterial = _mtrl->ambientMaterial;
	cb.diffuseMtrl = _mtrl->diffuseMaterial;
	cb.specularMtrl = _mtrl->specularMaterial;

	deviceContext->IASetVertexBuffers(0, 1, &_mesh->vertexBuffer, &_mesh->strideVB, &_mesh->offsetVB);
	deviceContext->IASetIndexBuffer(_mesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//update world coords to next set
	XMMATRIX world = XMLoadFloat4x4(&_world);
	cb.mWorld = XMMatrixTranspose(world);

	//set render state
	deviceContext->RSSetState(_mtrl->rasterizerState);

	// Renders a cube
	deviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	deviceContext->PSSetShaderResources(0, 1, &_mtrl->textureShaderRV);
	deviceContext->PSSetSamplers(0, 1, &_mtrl->samplerLinear);
	deviceContext->DrawIndexed(_mesh->indexCount, 0, 0);
}