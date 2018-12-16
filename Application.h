#ifndef APPLICATION_H
#define APPLICATION_H

#include <WindowsX.h>
#include <vector>
#include "Structures.h"
#include "GameObject.h"
#include "ScreenObject.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "PhongShader.h"
#include "BlurShader.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

class Application
{
private:
	HINSTANCE _hInst;
	HWND _hWnd;
	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;
	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext* _pImmediateContext;
	IDXGISwapChain* _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	PhongShader* _phongShader;
	BlurShader* _blurShader;

	ID3D11Buffer* _pVertexBufferNDCQuad;
	ID3D11Buffer* _pIndexBufferNDCQuad;

	ID3D11Buffer* _pVertexBufferQuad;
	ID3D11Buffer* _pIndexBufferQuad;

	ID3D11Buffer* _pVertexBufferSkybox;
	ID3D11Buffer* _pIndexBufferSkybox;

	ID3D11Buffer* _pVertexBufferCube;
	ID3D11Buffer* _pIndexBufferCube;

	ID3D11Buffer* _pVertexBufferPyramid;
	ID3D11Buffer* _pIndexBufferPyramid;

	ID3D11Buffer* _pVertexBufferPlaneS4;
	ID3D11Buffer* _pIndexBufferPlaneS4;

	ID3D11Buffer* _pVertexBufferPlaneS50;
	ID3D11Buffer* _pIndexBufferPlaneS50;
	
	ID3D11SamplerState* _pSamplerLinear;

	ID3D11BlendState* _pTransparency;

	ID3D11RasterizerState* _pWireFrame;
	ID3D11RasterizerState* _pSolid;

	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;

	ID3D11Buffer* _pPhongCB;
	ID3D11Buffer* _pBlurCB;

	ResourceManager* _rManager;
	ParticleSystem* _parSystem;

	ScreenObject* _screenObject;

	Camera* _currentCamera;
	Camera* _tvCamera;
	Camera* _cameraFront;
	Camera* _cameraBack;
	Camera* _cameraSide;
	Camera* _cameraTop;

	DirectionalLight* _dLight;
	SpotLight* _sLight;

	UINT _WindowHeight;
	UINT _WindowWidth;

	float _totalTime;
	float _deltaTime;

	bool _inSceneTV;
	bool _switch;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitGameObjects();
	HRESULT InitParticles();
	HRESULT InitCameras();
	HRESULT InitDevice();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	void Cleanup();

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
	void DrawSecondCamera();
	void DrawBlur(int currentLoop, ID3D11ShaderResourceView* shaderRV);
};
#endif