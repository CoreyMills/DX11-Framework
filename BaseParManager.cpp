#include "BaseParManager.h"

BaseParManager::BaseParManager()
{
	Init();
}

BaseParManager::~BaseParManager()
{
	Cleanup();
}

void BaseParManager::Init()
{
	_name = "";
	_childCam = nullptr;
	_childLifeSpan = 0.0f;
}

void BaseParManager::Cleanup()
{
	if (!_particleArray.empty()) _particleArray.erase(_particleArray.begin(), _particleArray.end());

	_childCam = nullptr;
	delete _childCam;
}

void BaseParManager::UpdateParticles(float deltaTime)
{
	for (unsigned int i = 0; i < _particleArray.size(); i++)
	{
		_particleArray.at(i)->Update(deltaTime);
	}
}

void BaseParManager::DrawParticles(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb)
{
	for (unsigned int i = 0; i < _particleArray.size(); i++)
	{
		_particleArray.at(i)->Draw(deviceContext, pConstantBuffer, cb);
	}
}

void BaseParManager::RemoveParticles(int numOfPars)
{
	if (!_particleArray.empty()) _particleArray.erase(_particleArray.end() - numOfPars, _particleArray.end());
	_particleArray.shrink_to_fit();
}