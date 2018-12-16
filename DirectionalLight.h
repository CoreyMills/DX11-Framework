#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Structures.h"

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(XMFLOAT3 lightDirection, XMFLOAT4 ambientLight,
		XMFLOAT4 diffuseLight, XMFLOAT4 specularLight, float specularPower, XMFLOAT3 eyePosW);
	~DirectionalLight();

	XMFLOAT3 _eyePosW;

	XMFLOAT3 _lightDirection;

	XMFLOAT4 _diffuseLight;
	XMFLOAT4 _ambientLight;

	XMFLOAT4 _specularLight;
	float _specularPower;

private:
	Convertion* _convert;
	XMFLOAT3 _originalEye;
};
#endif 