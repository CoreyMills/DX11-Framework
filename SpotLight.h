#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "DirectionalLight.h"

class SpotLight : DirectionalLight
{
public:
	SpotLight();
	SpotLight(float angle, float lightRange, XMFLOAT3 lightDirection, 
		XMFLOAT4 diffuseLight, XMFLOAT4 ambientLight, XMFLOAT4 specularLight, 
		float specularPower, XMFLOAT3 eyePosW);
	~SpotLight();

private:
	float _spotAngle;
	float _lightRange;
	float _attenuation;
};
#endif 