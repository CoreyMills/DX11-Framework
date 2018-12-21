#ifndef BASEPARTICLE_H
#define BASEPARTICLE_H

#include "Structures.h"
#include "GameObject.h"
#include "Camera.h"
#include "DirectionalLight.h"

class BaseParticle : public GameObject
{
protected:
	Camera* _camObj;

	float _lifeSpan;
	float _currentCount;

public:
	BaseParticle();
	~BaseParticle();

	void Init();
	void Cleanup();

	virtual void Update(float deltaTime) override;
	virtual void Move(float force, DIRECTION dir) override;

	const float GetLifeSpan() { return _lifeSpan; }
};
#endif