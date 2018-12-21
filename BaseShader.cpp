#include "BaseShader.h"

BaseShader::BaseShader()
{
	_numOfTextures = 0;
}

BaseShader::~BaseShader()
{
	_fxFilePath = nullptr;
	delete _fxFilePath;

	_vertexShader = nullptr;
	delete _vertexShader;

	_pixelShader = nullptr;
	delete _pixelShader;

	_inputLayout = nullptr;
	delete _inputLayout;
}

HRESULT BaseShader::InitRenderToTexture(ID3D11Device* device)
{
	ID3D11Texture2D* renderTargetTexture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;

	for (int i = 0; i < _numOfTextures; i++)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		//Initialize the render target texture description
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = _screenWidth;
		textureDesc.Height = _screenHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Create the render target texture
		result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
		if (FAILED(result))
			return result;

		//Setup the description of the render target view
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		//Create the render target view
		result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);
		if (FAILED(result))
			return result;

		//Setup the description of the shader resource view
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view
		result = device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
		if (FAILED(result))
			return result;

		_renderTTArray.push_back(renderTargetTexture);
		_renderTVArray.push_back(renderTargetView);
		_shaderRVArray.push_back(shaderResourceView);

		renderTargetTexture = nullptr;
		renderTargetView = nullptr;
		shaderResourceView = nullptr;
	}
	return S_OK;
}