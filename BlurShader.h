#ifndef BLURSHADER_H
#define BLURSHADER_H

#include "Structures.h"
#include "BaseShader.h"
class BlurShader : public BaseShader
{
public:
	BlurShader(WCHAR* fxFilePath);
	~BlurShader();

	HRESULT InitShaderAndInputLayout(ID3D11Device* device);

	BlurCB _cb;

private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};
#endif