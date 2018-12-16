#include "SmokeManager.h"

SmokeManager::SmokeManager(std::string name, Camera* camObj, float parLifeSpan)
{
	Init(name, camObj, parLifeSpan);
}

SmokeManager::~SmokeManager()
{
	Cleanup();
}

void SmokeManager::Init(std::string name, Camera* camObj, float parLifeSpan)
{
	_name = name;
	_childCam = camObj;
	_childLifeSpan = parLifeSpan;
}

void SmokeManager::Cleanup()
{
}

void SmokeManager::AddParticles(Mesh* mesh, Material* mtrl, int numOfPars, XMFLOAT3 startingPos)
{
	for (int i = 0; i < numOfPars; i++)
	{
		SmokeParticle* par = new SmokeParticle(_childCam, _childLifeSpan);

		par->SetName("sParticle");
		par->SetMesh(mesh);
		par->SetMtrl(mtrl);
		par->ApplyTransformation(startingPos);

		_particleArray.push_back(par);
	}
}