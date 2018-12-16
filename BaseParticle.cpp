#include "BaseParticle.h"

BaseParticle::BaseParticle()
{
	Init();
}

BaseParticle::~BaseParticle()
{
	Cleanup();
}

void BaseParticle::Init()
{
	_camObj = nullptr;

	_mesh = new Mesh();
	_mtrl = new Material();

	_direction = MOVE_NONE;

	_acceleration = 0.0f;
	_maxSpeed = 2.0f;

	_lifeSpan = 0.0f;
	_currentCount = 0.0f;
}

void BaseParticle::Cleanup()
{
	delete _mesh;
	_mesh = nullptr;

	_mtrl = nullptr;
	delete _mtrl;
}

void BaseParticle::Update(float deltaTime)
{
	BaseObject::Update(deltaTime);
}

void BaseParticle::Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb)
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

void BaseParticle::Move(float force, DIRECTION dir)
{

}