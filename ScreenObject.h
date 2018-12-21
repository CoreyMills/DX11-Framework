#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H

#include "BaseObject.h"

class ScreenObject : public BaseObject
{
public:
	ScreenObject();
	~ScreenObject();

	void Init();
	void Cleanup();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, BlurCB &cb);

	void SetMesh(Mesh* mesh) { _mesh = mesh; }
	void SetShaderRV(ID3D11ShaderResourceView* textureRV) { _shaderRV = textureRV; }
	void SetSamLinear(ID3D11SamplerState* samplerLinear) { _samplerLinear = samplerLinear; }
	void SetRState(ID3D11RasterizerState* rasterizerState) { _rasterizerState = rasterizerState; }

	const Mesh* GetMesh() { return _mesh; }
	const ID3D11ShaderResourceView* GetShaderRV() { return _shaderRV; }
	const ID3D11SamplerState* GetSamLinear() { return _samplerLinear; }
	const ID3D11RasterizerState* GetRState() { return _rasterizerState; }

private:
	Mesh* _mesh;

	ID3D11ShaderResourceView* _shaderRV;
	ID3D11RasterizerState* _rasterizerState;
	ID3D11SamplerState* _samplerLinear;

	float _texelOffset;
};
#endif