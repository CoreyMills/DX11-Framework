#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "Structures.h"
#include <fstream>

class BaseObject
{
protected:
	Convertion* _convert;
	std::string _name;

	XMFLOAT4X4 _world;

	XMFLOAT3 _pos;
	XMFLOAT3 _rot;
	XMFLOAT3 _scale;

	XMFLOAT3 _forward;

	float _movementSpeed;
	float _rotationSpeed;

public:
	BaseObject();
	~BaseObject();

	void Init();
	void Cleanup();

	void LoadStartingValues(std::string fileName);

	virtual void Update(float deltaTime);

	void SetScale(XMFLOAT3 scaleFactor);
	void ApplyTransformation(XMFLOAT3 newPos);
	void ApplyRotation(XMFLOAT3 newRot);

	void SetName(std::string name) { _name = name; }

	std::string GetName() { return _name; }
	XMFLOAT4X4 GetWorldMatrix(){ return _world; }

	XMFLOAT3 GetPos() { return _pos; }
	XMFLOAT3 GetRot() { return _rot; }
	XMFLOAT3 GetScale() { return _scale; }
	XMFLOAT3 GetForwardV() { return _forward; }
};
#endif