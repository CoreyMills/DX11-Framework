#ifndef BASEPARTICLE_H
#define BASEPARTICLE_H

#include "Structures.h"
#include "BaseObject.h"
#include "Camera.h"
#include "DirectionalLight.h"

class BaseParticle : public BaseObject
{
protected:
	Camera* _camObj;

	Mesh * _mesh;
	Material* _mtrl;

	DIRECTION _direction;

	float _acceleration;
	float _maxSpeed;

	float _lifeSpan;
	float _currentCount;

public:
	BaseParticle();
	~BaseParticle();

	void Init();
	void Cleanup();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb);

	virtual void Move(float force, DIRECTION dir);

	void SetMesh(Mesh* mesh) { _mesh = mesh; }
	void SetMtrl(Material* material) { _mtrl = material; }

	const Mesh* GetMesh() { return _mesh; }
	const Material* GetMtrl() { return _mtrl; }

	const float GetLifeSpan() { return _lifeSpan; }
};
#endif