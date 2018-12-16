#ifndef BASESHADER_H
#define BASESHADER_H

#include "Structures.h"

class BaseShader
{
public:
	BaseShader();
	~BaseShader();

	HRESULT InitRenderToTexture(ID3D11Device* device);

	ID3D11VertexShader* GetVertexShader() { return _vertexShader; }
	ID3D11PixelShader* GetPixelShader() { return _pixelShader; }
	ID3D11InputLayout* GetVLayout() { return _vertexLayout; }

	void EnableShader() { _shaderEnabled = true; }
	void DisableShader() { _shaderEnabled = false; }

	//starts as false
	bool IsShaderEnabled() { return _shaderEnabled; }

	std::vector<ID3D11Texture2D*> _renderTTArray;
	std::vector<ID3D11RenderTargetView*> _renderTVArray;
	std::vector<ID3D11ShaderResourceView*> _shaderRVArray;

	UINT screenWidth, screenHeight;
	int _numOfTextures;

protected:
	WCHAR* _fxFilePath;

	ID3D11Texture2D* _renderTargetTexture;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _shaderResourceView;

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _vertexLayout;

	bool _shaderEnabled;
};
#endif