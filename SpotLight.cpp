#include "SpotLight.h"

SpotLight::SpotLight()
{
	_spotAngle = 0;
}

SpotLight::SpotLight(float angle, float lightRange, XMFLOAT3 lightDirection, XMFLOAT4 diffuseLight, XMFLOAT4 ambientLight, XMFLOAT4 specularLight, float specularPower, XMFLOAT3 eyePosW)
{
	_spotAngle = angle;
	_lightRange = lightRange;
}

SpotLight::~SpotLight()
{


}