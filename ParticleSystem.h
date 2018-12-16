#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Structures.h"
#include "BaseParManager.h"
#include "SmokeManager.h"

class ParticleSystem
{
private:
	std::vector<BaseParManager*> _parManagerArray;

public:
	ParticleSystem();
	~ParticleSystem();

	void Init();
	void Cleanup();

	void UpdateParticles(float deltaTime);
	void DrawParticles(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb);

	void AddParticleManager(BaseParManager* parManager);
	void RemoveParticleManager(std::string name);
};
#endif