#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	_eyePosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_lightDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	_diffuseLight = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	_ambientLight = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	_specularLight = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	_specularPower = 0.0f;
}

DirectionalLight::DirectionalLight(XMFLOAT3 lightDirection, XMFLOAT4 ambientLight, XMFLOAT4 diffuseLight, XMFLOAT4 specularLight, float specularPower, XMFLOAT3 eyePosW)
{
	this->_eyePosW = eyePosW;
	this->_lightDirection = lightDirection;

	this->_diffuseLight = diffuseLight;
	this->_ambientLight = ambientLight;

	this->_specularLight = specularLight;
	this->_specularPower = specularPower;
}

DirectionalLight::~DirectionalLight()
{
	_convert = nullptr;
	delete _convert;
}