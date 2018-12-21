#include "BaseParticle.h"

BaseParticle::BaseParticle()
{
	Init();
}

BaseParticle::~BaseParticle()
{
	Cleanup();
}

void BaseParticle::Init()
{
	_camObj = nullptr;

	_mesh = new Mesh();
	_mtrl = new Material();

	_direction = MOVE_NONE;

	_acceleration = 0.0f;
	_maxSpeed = 2.0f;

	_lifeSpan = 0.0f;
	_currentCount = 0.0f;
}

void BaseParticle::Cleanup()
{
	_camObj = nullptr;

	delete _mesh;
	_mesh = nullptr;

	_mtrl = nullptr;
	delete _mtrl;
}

void BaseParticle::Update(float deltaTime)
{
	BaseObject::Update(deltaTime);
}

void BaseParticle::Move(float force, DIRECTION dir)
{

}