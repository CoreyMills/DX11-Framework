#include "ScreenObject.h"

ScreenObject::ScreenObject()
{
	Init();
}

ScreenObject::~ScreenObject()
{
	Cleanup();
}

void ScreenObject::Init()
{
	_mesh = new Mesh();

	_textureShaderRV = nullptr;
	_rasterizerState = nullptr;
	_samplerLinear = nullptr;

	_texelOffset = 0.0f;
}

void ScreenObject::Cleanup()
{
	delete _mesh;
	_mesh = nullptr;

	delete _textureShaderRV;
	_textureShaderRV = nullptr;

	delete _rasterizerState;
	_rasterizerState = nullptr;

	delete _samplerLinear;
	_samplerLinear = nullptr;

	_texelOffset = 0.0f;
}

void ScreenObject::Update(float deltaTime)
{
	BaseObject::Update(deltaTime);

	if (GetAsyncKeyState(VK_UP))
	{
		_texelOffset += 0.0001f;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		_texelOffset -= 0.0001f;
	}
}

void ScreenObject::Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, BlurCB &cb)
{
	// Set the default blend state (no blending) for opaque objects
	deviceContext->OMSetBlendState(0, 0, 0xffffffff);

	_mesh->strideVB = sizeof(NDCVertex);

	cb.texelOffset = _texelOffset;

	deviceContext->IASetVertexBuffers(0, 1, &_mesh->vertexBuffer, &_mesh->strideVB, &_mesh->offsetVB);
	deviceContext->IASetIndexBuffer(_mesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//set render state to solid
	deviceContext->RSSetState(_rasterizerState);

	// Renders a cube
	deviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	deviceContext->PSSetShaderResources(0, 1, &_textureShaderRV);
	deviceContext->PSSetSamplers(0, 1, &_samplerLinear);
	deviceContext->DrawIndexed(_mesh->indexCount, 0, 0);
}