#ifndef SMOKEPARTICLE_H
#define SMOKEPARTICLE_H

#include "BaseParticle.h"

class SmokeParticle : public BaseParticle
{
private:
public:
	SmokeParticle(Camera* parentObj, float lifeSpan);
	~SmokeParticle();

	void Init(Camera* camObj, float lifeSpan);
	void Cleanup();

	void Update(float deltaTime) override;
	void Move(float force, DIRECTION dir) override;
};
#endif