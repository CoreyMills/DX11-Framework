#ifndef SMOKEMANAGER_H
#define SMOKEMANAGER_H

#include "Structures.h"
#include "BaseParManager.h"
#include "SmokeParticle.h"

class SmokeManager : public BaseParManager
{
private:

public:
	SmokeManager(std::string name, Camera* camObj, float parLifeSpan);
	~SmokeManager();

	void Init(std::string name, Camera* camObj, float parLifeSpan);
	void Cleanup();

	void AddParticles(Mesh* mesh, Material* mtrl, int numOfPars, XMFLOAT3 startingPos) override;
};
#endif