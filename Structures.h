#ifndef STRUCTURES_H
#define STRUCTURES_H
#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "DDSTextureLoader.h"
#include "resource.h"

using namespace DirectX;

enum DIRECTION
{
	MOVE_NONE = 0,
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};

struct NDCVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 TexC;
};

struct PhongCB
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	XMFLOAT4 ambientMtrl;
	XMFLOAT4 ambientLight;

	XMFLOAT4 diffuseMtrl;
	XMFLOAT4 diffuseLight;

	XMFLOAT4 specularMtrl;
	XMFLOAT4 specularLight;
	float specularPower;
	XMFLOAT3 eyePosW;

	XMFLOAT3 lightVecW;
	float gTime;
};

struct BlurCB
{
	float texelOffset;
	XMFLOAT3 pad;
};

struct Mesh
{
	std::string meshName;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT strideVB;
	UINT offsetVB;

	UINT indexCount;

	Mesh()
	{
		vertexBuffer = nullptr;
		indexBuffer = nullptr;

		strideVB = 0;
		offsetVB = 0;

		indexCount = 0;
	}

	Mesh(std::string meshName, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, int indexAmount)
	{
		this->meshName = meshName;
		this->vertexBuffer = vertexBuffer;
		this->indexBuffer = indexBuffer;
		this->strideVB = sizeof(SimpleVertex);
		this->offsetVB = 0;
		this->indexCount = indexAmount;
	}
};

struct Material
{
	bool isTransparent;

	XMFLOAT4 blendFactor;

	XMFLOAT4 ambientMaterial;
	XMFLOAT4 diffuseMaterial;
	XMFLOAT4 specularMaterial;

	ID3D11BlendState* transparency;
	ID3D11RasterizerState* rasterizerState;

	ID3D11ShaderResourceView* shaderRV;
	ID3D11SamplerState* samplerLinear;

	Material()
	{
	}

	Material(XMFLOAT4 blendFactor, XMFLOAT4 ambientMtrl, XMFLOAT4 diffuseMtrl, XMFLOAT4 specularMtrl, 
		bool isTransparent, ID3D11BlendState* transparency, ID3D11RasterizerState* rState,
		ID3D11ShaderResourceView* textureRV, ID3D11SamplerState* samplerLinear)
	{
		this->isTransparent = isTransparent;
		this->blendFactor = blendFactor;

		this->ambientMaterial = ambientMtrl;
		this->diffuseMaterial = diffuseMtrl;
		this->specularMaterial = specularMtrl;

		this->transparency = transparency;
		this->rasterizerState = rState;

		this->shaderRV = textureRV;
		this->samplerLinear = samplerLinear;
	}
};

struct Convertion
{
	XMFLOAT4 VF4(XMVECTOR& vec)
	{
		XMFLOAT4 val;
		XMStoreFloat4(&val, vec);
		return val;
	}

	XMVECTOR F4V(XMFLOAT4& val)
	{
		return XMLoadFloat4(&val);
	}

	XMFLOAT3 VF3(XMVECTOR& vec)
	{
		XMFLOAT3 val;
		XMStoreFloat3(&val, vec);
		return val;
	}

	XMVECTOR F3V(XMFLOAT3& val)
	{
		return XMLoadFloat3(&val);
	}
};
#endif