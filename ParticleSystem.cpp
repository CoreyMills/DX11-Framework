#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
}

void ParticleSystem::Cleanup()
{
	if (!_parManagerArray.empty()) _parManagerArray.erase(_parManagerArray.begin(), _parManagerArray.end());
}

void ParticleSystem::UpdateParticles(float deltaTime)
{
	for (unsigned int i = 0; i < _parManagerArray.size(); i++)
	{
		_parManagerArray.at(i)->UpdateParticles(deltaTime);

		//testing out casting with objects
		//prob use with switch statement of the managers types if needed
		//((SmokeManager*)_parManagerArray.at(i))->test();
	}
}

void ParticleSystem::DrawParticles(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pConstantBuffer, PhongCB &cb)
{
	for (unsigned int i = 0; i < _parManagerArray.size(); i++)
	{
		_parManagerArray.at(i)->DrawParticles(deviceContext, pConstantBuffer, cb);
	}
}

void ParticleSystem::AddParticleManager(BaseParManager* parManager)
{
	_parManagerArray.push_back(parManager);
}

void ParticleSystem::RemoveParticleManager(std::string name)
{
	for (unsigned int i = 0; i < _parManagerArray.size(); i++)
	{
		if (_parManagerArray.at(i)->GetName() == name)
		{
			_parManagerArray.erase(_parManagerArray.begin() + i);
			_parManagerArray.shrink_to_fit();
		}
	}
}