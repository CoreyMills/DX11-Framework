#ifndef BASEPARMANAGER_H
#define BASEPARMANAGER_H

#include "Structures.h"
#include "BaseParticle.h"
#include "Camera.h"

class BaseParManager
{
protected:
	std::string _name;
	std::vector<BaseParticle*> _particleArray;

	Camera* _childCam;
	float _childLifeSpan;

public:
	BaseParManager();
	~BaseParManager();

	void Init();
	void Cleanup();

	virtual void UpdateParticles(float deltaTime);
	void DrawParticles(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb);

	virtual void AddParticles(Mesh* mesh, Material* mtrl, int numOfPars, XMFLOAT3 startingPos) = 0;
	virtual void RemoveParticles(int numOfPars);

	std::string GetName() { return _name; }
};
#endif