#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "Structures.h"
#include "BaseShader.h"

class PhongShader : public BaseShader
{
public:
	PhongShader(WCHAR* fxFilePath);
	~PhongShader();

	HRESULT InitShadersAndInputLayout(ID3D11Device* device);

	PhongCB _cb;

private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

};
#endif