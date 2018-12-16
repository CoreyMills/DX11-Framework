#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "BaseObject.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

class GameObject : public BaseObject
{
private:
	Mesh * _mesh;
	Material* _mtrl;

	DIRECTION _direction;

	bool _movementEnabled;

	float _acceleration;
	float _maxSpeed;

public:
	GameObject();
	~GameObject();

	void Init();
	void Cleanup();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb);

	void Move(float force, DIRECTION dir);

	void SetMesh(Mesh* mesh) { _mesh = mesh; }
	void SetMtrl(Material* material) { _mtrl = material; }
	void EnableMovement() { _movementEnabled = true; }
	void DisableMovement() { _movementEnabled = false; }

	const bool IsMovementEnabled() { return _movementEnabled; }
	Mesh* GetMesh() { return _mesh; }
	Material* GetMtrl() { return _mtrl; }
};
#endif