#include "Application.h"

//global vars
float mouseX, mouseY;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
	
    switch (message)
    {
    case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
		break;
	/*case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		//OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
		//OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;*/
    default:
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
	return 0;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_phongShader = nullptr;
	_blurShader = nullptr;
	_pVertexBufferQuad = nullptr;
	_pIndexBufferQuad = nullptr;
	_pVertexBufferSkybox = nullptr;
	_pIndexBufferSkybox = nullptr;
	_pVertexBufferCube = nullptr;
	_pIndexBufferCube = nullptr;
	_pVertexBufferPyramid = nullptr;
	_pIndexBufferPyramid = nullptr;
	_pVertexBufferPlaneS4 = nullptr;
	_pIndexBufferPlaneS4 = nullptr;
	_pVertexBufferPlaneS50 = nullptr;
	_pIndexBufferPlaneS50 = nullptr;

	_pPhongCB = nullptr;
	_pBlurCB = nullptr;
	_pSamplerLinear = nullptr;

	_pTransparency = nullptr;

	_depthStencilView = nullptr;
	_depthStencilBuffer = nullptr;

	_pWireFrame = nullptr;
	_pSolid = nullptr;

	_rManager = nullptr;
	_parSystem = nullptr;

	_screenObject = nullptr;

	_currentCamera = nullptr;
	_cameraFront = nullptr;
	_cameraBack = nullptr;
	_cameraSide = nullptr;
	_cameraTop = nullptr;

	_dLight = nullptr;
	_sLight = nullptr;

	_WindowHeight = 0;
	_WindowWidth = 0;

	_totalTime = 0.0f;
	_deltaTime = 0.0f;
}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initialise window
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

	_rManager = new ResourceManager();

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }

	_dLight = new DirectionalLight(XMFLOAT3(0.0f, 0.5f, -1.0f),
		XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 10.0f, XMFLOAT3(0.0f, 0.0f, -3.0f));

	/*_sLight = new SpotLight(45, 10, XMFLOAT3(0.25f, 0.5f, -1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.8f, 0.2f, 0.2f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, XMFLOAT3(0.0f, 0.0f, -3.0f));
	*/

	InitGameObjects();
	InitCameras();
	InitParticles();

	return S_OK;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;

	// Create vertex buffer
	SimpleVertex skyboxVertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.25f, 0.335f) },	 //0 //0 // 
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.5f, 0.335f) },   //1 //1
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.25f, 0.655f) }, //2 //2
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.5f, 0.655f) },	 //3 //3

		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.75f, 0.335f) },   //4 //4 //
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.335f) },	 //5 //5
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.75f, 0.655f) },	 //6 //6
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.655f) }, //7 //7

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.335f) },	 //5 //8
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.25f, 0.335f) },	 //0 //9
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.655f) },	 //7 //10
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.25f, 0.655f) }, //2 //11

		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.335f) },  //1 //12
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.75f, 0.335f) },   //4 //13
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.655f) }, //3 //14
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.75f, 0.655f) },	 //6 //15

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.25f, 0.0f) },  //5 //16
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) },   //4 //17
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.25f, 0.335f) }, //0 //18
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.5f, 0.335f) },  //1 //19

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.25f, 0.655f) }, //2 //20
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.655f) },	 //3 //21
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.25f, 1.0f) },	 //7 //22
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 1.0f) },	 //6 //23
	};

	D3D11_BUFFER_DESC bd0;
	ZeroMemory(&bd0, sizeof(bd0));
	bd0.Usage = D3D11_USAGE_DEFAULT;
	bd0.ByteWidth = sizeof(SimpleVertex) * 24;
	bd0.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd0.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData0;
	ZeroMemory(&InitData0, sizeof(InitData0));
	InitData0.pSysMem = skyboxVertices;

	hr = _pd3dDevice->CreateBuffer(&bd0, &InitData0, &_pVertexBufferSkybox);

	if (FAILED(hr))
		return hr;

    // Create vertex buffer
    SimpleVertex cubeVertices[] =
    {
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },	//0 //0
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },     //1 //1
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },	//2 //2
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },	//3 //3

		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },       //4 //4
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },		//5 //5
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },		//6 //6
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },		//7 //7

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },		//5 //8
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },	//0 //9
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },	//7 //10
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },	//2 //11

		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },      //1 //12
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },       //4 //13
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },		//3 //14
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },		//6 //15

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },		//5 //16
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },       //4 //17
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },		//0 //18
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },      //1 //19

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },	//2 //20
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },	//3 //21
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },	//7 //22
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },		//6 //23
    };

	D3D11_BUFFER_DESC bd1;
	ZeroMemory(&bd1, sizeof(bd1));
	bd1.Usage = D3D11_USAGE_DEFAULT;
	bd1.ByteWidth = sizeof(SimpleVertex) * 24;
	bd1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd1.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData1;
	ZeroMemory(&InitData1, sizeof(InitData1));
	InitData1.pSysMem = cubeVertices;

	hr = _pd3dDevice->CreateBuffer(&bd1, &InitData1, &_pVertexBufferCube);

	if (FAILED(hr))
		return hr;

	/////////////////////////////////Finish the Normals///////////////////////////
	// Create vertex buffer
	SimpleVertex pyramidVertices[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },		//0 //0
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },     //1 //1
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },    //2 //2

		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },		//0 //3
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },    //2 //4
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, //3 //5

		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },       //0 //6
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }, //3 //7
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },   //4 //8

		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },       //0 //9
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },   //4 //10
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },     //1 //11

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },     //1 //12
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },    //2 //13
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },   //3 //14
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },    //4 //15
	};

    D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
    bd2.Usage = D3D11_USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(SimpleVertex) * 16;
    bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd2.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData2;
	ZeroMemory(&InitData2, sizeof(InitData2));
    InitData2.pSysMem = pyramidVertices;

    hr = _pd3dDevice->CreateBuffer(&bd2, &InitData2, &_pVertexBufferPyramid);

    if (FAILED(hr))
        return hr;

	// Create vertex buffer
	SimpleVertex planeVerticesS4[] =
	{
		{ XMFLOAT3(-2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },    //0
		{ XMFLOAT3(-1.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //1
		{ XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(2.0f, 0.0f) },     //2
		{ XMFLOAT3(1.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },    //3
		{ XMFLOAT3(2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },     //4

		{ XMFLOAT3(-2.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },   //5
		{ XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },  //6
		{ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(2.0f, 1.0f) },    //7
		{ XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },   //8
		{ XMFLOAT3(2.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },    //9

		{ XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },    //10
		{ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 2.0f) },   //11
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(2.0f, 2.0f) },     //12
		{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 2.0f) },    //13
		{ XMFLOAT3(2.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 2.0f) },     //14

		{ XMFLOAT3(-2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },  //15
		{ XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) }, //16
		{ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(2.0f, 3.0f) },   //17
		{ XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 3.0f) },  //18
		{ XMFLOAT3(2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 3.0f) },   //19

		{ XMFLOAT3(-2.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 4.0f) },   //20
		{ XMFLOAT3(-1.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 4.0f) },  //21
		{ XMFLOAT3(0.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(2.0f, 4.0f) },    //22
		{ XMFLOAT3(1.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 4.0f) },   //23
		{ XMFLOAT3(2.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 4.0f) },    //24
	};

	D3D11_BUFFER_DESC bd3;
	ZeroMemory(&bd3, sizeof(bd3));
	bd3.Usage = D3D11_USAGE_DEFAULT;
	bd3.ByteWidth = sizeof(SimpleVertex) * 25;
	bd3.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd3.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData3;
	ZeroMemory(&InitData3, sizeof(InitData3));
	InitData3.pSysMem = planeVerticesS4;

	hr = _pd3dDevice->CreateBuffer(&bd3, &InitData3, &_pVertexBufferPlaneS4);

	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	SimpleVertex planeVerticesS50[] =
	{
		{ XMFLOAT3(-2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },    //0
		{ XMFLOAT3(-1.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(12.5f, 0.0f) },   //1
		{ XMFLOAT3(0.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(25.0f, 0.0f) },     //2
		{ XMFLOAT3(1.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(37.5f, 0.0f) },    //3
		{ XMFLOAT3(2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(50.0f, 0.0f) },     //4

		{ XMFLOAT3(-2.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 12.5f) },   //5
		{ XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(12.5f, 12.5f) },  //6
		{ XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(25.0f, 12.5f) },    //7
		{ XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(37.5f, 12.5f) },   //8
		{ XMFLOAT3(2.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(50.0f, 12.5f) },    //9

		{ XMFLOAT3(-2.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 25.0f) },    //10
		{ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(12.5f, 25.0f) },   //11
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(25.0f, 25.0f) },     //12
		{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(37.5f, 25.0f) },    //13
		{ XMFLOAT3(2.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(50.0f, 25.0f) },     //14

		{ XMFLOAT3(-2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 37.5f) },  //15
		{ XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(12.5f, 37.5f) }, //16
		{ XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(25.0f, 37.5f) },   //17
		{ XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(37.5f, 37.5f) },  //18
		{ XMFLOAT3(2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(50.0f, 37.5f) },   //19

		{ XMFLOAT3(-2.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 50.0f) },   //20
		{ XMFLOAT3(-1.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(12.5f, 50.0f) },  //21
		{ XMFLOAT3(0.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(25.0f, 50.0f) },    //22
		{ XMFLOAT3(1.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(37.5f, 50.0f) },   //23
		{ XMFLOAT3(2.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(50.0f, 50.0f) },    //24
	};

	D3D11_BUFFER_DESC bd4;
	ZeroMemory(&bd4, sizeof(bd4));
	bd4.Usage = D3D11_USAGE_DEFAULT;
	bd4.ByteWidth = sizeof(SimpleVertex) * 25;
	bd4.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd4.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData4;
	ZeroMemory(&InitData4, sizeof(InitData4));
	InitData4.pSysMem = planeVerticesS50;

	hr = _pd3dDevice->CreateBuffer(&bd4, &InitData4, &_pVertexBufferPlaneS50);

	if (FAILED(hr))
		return hr;

	SimpleVertex QuadVertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },  //0
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //1
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },   //2
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },    //3
	};

	D3D11_BUFFER_DESC bd5;
	ZeroMemory(&bd5, sizeof(bd5));
	bd5.Usage = D3D11_USAGE_DEFAULT;
	bd5.ByteWidth = sizeof(SimpleVertex) * 4;
	bd5.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd5.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData5;
	ZeroMemory(&InitData5, sizeof(InitData5));
	InitData5.pSysMem = QuadVertices;

	hr = _pd3dDevice->CreateBuffer(&bd5, &InitData5, &_pVertexBufferQuad);

	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	NDCVertex NDCQuadVertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },  //0
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //1
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },   //2
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },    //3
	};

	D3D11_BUFFER_DESC bd6;
	ZeroMemory(&bd6, sizeof(bd6));
	bd6.Usage = D3D11_USAGE_DEFAULT;
	bd6.ByteWidth = sizeof(NDCVertex) * 4;
	bd6.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd6.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData6;
	ZeroMemory(&InitData6, sizeof(InitData6));
	InitData6.pSysMem = NDCQuadVertices;

	hr = _pd3dDevice->CreateBuffer(&bd6, &InitData6, &_pVertexBufferNDCQuad);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitIndexBuffer()
{
	HRESULT hr;

	//Trippy Cube Time
	/*0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23,*/

	// Create index buffer
	WORD skyboxIndices[] =
	{
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23,
	};

	D3D11_BUFFER_DESC bd0;
	ZeroMemory(&bd0, sizeof(bd0));
	bd0.Usage = D3D11_USAGE_DEFAULT;
	bd0.ByteWidth = sizeof(WORD) * 36;
	bd0.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd0.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData0;
	ZeroMemory(&InitData0, sizeof(InitData0));
	InitData0.pSysMem = skyboxIndices;
	hr = _pd3dDevice->CreateBuffer(&bd0, &InitData0, &_pIndexBufferSkybox);

	if (FAILED(hr))
		return hr;

    // Create index buffer
    WORD cubeIndices[] =
    {
        0,2,1,
        2,3,1,

		4,6,5,
		6,7,5,

		8,10,9,
		10,11,9,

		12,14,13,
		14,15,13,

		16,18,17,
		18,19,17,

		20,22,21,
		22,23,21,
    };

	D3D11_BUFFER_DESC bd1;
	ZeroMemory(&bd1, sizeof(bd1));
	bd1.Usage = D3D11_USAGE_DEFAULT;
	bd1.ByteWidth = sizeof(WORD) * 36;
	bd1.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd1.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData1;
	ZeroMemory(&InitData1, sizeof(InitData1));
	InitData1.pSysMem = cubeIndices;
	hr = _pd3dDevice->CreateBuffer(&bd1, &InitData1, &_pIndexBufferCube);

	if (FAILED(hr))
		return hr;

	// Create index buffer
	WORD pyramidIndices[] =
	{
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,

		14,13,15,
		13,12,15,
	};

	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
    bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD) * 18;
    bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData2;
	ZeroMemory(&InitData2, sizeof(InitData2));
    InitData2.pSysMem = pyramidIndices;
    hr = _pd3dDevice->CreateBuffer(&bd2, &InitData2, &_pIndexBufferPyramid);

    if (FAILED(hr))
        return hr;

	// Create index buffer
	WORD planeIndicesS4[] =
	{
		/*0,1,5,
		1,6,5,
		1,2,6,
		2,7,6,
		2,3,7,
		3,8,7,
		3,4,8,
		4,9,8,

		5,6,10,
		6,11,10,
		6,7,11,
		7,12,11,
		7,8,12,
		8,13,12,
		8,9,13,
		9,14,13,

		10,11,15,
		11,16,15,
		11,12,16,
		12,17,16,
		12,13,17,
		13,18,17,
		13,14,18,
		14,19,18,

		15,16,20,
		16,21,20,
		16,17,21,
		17,22,21,
		17,18,22,
		18,23,22,
		18,19,23,
		19,24,23,*/
		0,5,1,
		1,5,6,
		1,6,2,
		2,6,7,
		2,7,3,
		3,7,8,
		3,8,4,
		4,8,9,

		5,10,6,
		6,10,11,
		6,11,7,
		7,11,12,
		7,12,8,
		8,12,13,
		8,13,9,
		9,13,14,

		10,15,11,
		11,15,16,
		11,16,12,
		12,16,17,
		12,17,13,
		13,17,18,
		13,18,14,
		14,18,19,

		15,20,16,
		16,20,21,
		16,21,17,
		17,21,22,
		17,22,18,
		18,22,23,
		18,23,19,
		19,23,24,
	};

	D3D11_BUFFER_DESC bd3;
	ZeroMemory(&bd3, sizeof(bd3));
	bd3.Usage = D3D11_USAGE_DEFAULT;
	bd3.ByteWidth = sizeof(WORD) * 96;
	bd3.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd3.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData3;
	ZeroMemory(&InitData3, sizeof(InitData3));
	InitData3.pSysMem = planeIndicesS4;
	hr = _pd3dDevice->CreateBuffer(&bd3, &InitData3, &_pIndexBufferPlaneS4);

	if (FAILED(hr))
		return hr;

	// Create index buffer
	WORD planeIndicesS100[] =
	{
		/*0,1,5,
		1,6,5,
		1,2,6,
		2,7,6,
		2,3,7,
		3,8,7,
		3,4,8,
		4,9,8,

		5,6,10,
		6,11,10,
		6,7,11,
		7,12,11,
		7,8,12,
		8,13,12,
		8,9,13,
		9,14,13,

		10,11,15,
		11,16,15,
		11,12,16,
		12,17,16,
		12,13,17,
		13,18,17,
		13,14,18,
		14,19,18,

		15,16,20,
		16,21,20,
		16,17,21,
		17,22,21,
		17,18,22,
		18,23,22,
		18,19,23,
		19,24,23,*/
		0,5,1,
		1,5,6,
		1,6,2,
		2,6,7,
		2,7,3,
		3,7,8,
		3,8,4,
		4,8,9,

		5,10,6,
		6,10,11,
		6,11,7,
		7,11,12,
		7,12,8,
		8,12,13,
		8,13,9,
		9,13,14,

		10,15,11,
		11,15,16,
		11,16,12,
		12,16,17,
		12,17,13,
		13,17,18,
		13,18,14,
		14,18,19,

		15,20,16,
		16,20,21,
		16,21,17,
		17,21,22,
		17,22,18,
		18,22,23,
		18,23,19,
		19,23,24,
	};

	D3D11_BUFFER_DESC bd4;
	ZeroMemory(&bd4, sizeof(bd4));
	bd4.Usage = D3D11_USAGE_DEFAULT;
	bd4.ByteWidth = sizeof(WORD) * 96;
	bd4.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd4.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData4;
	ZeroMemory(&InitData4, sizeof(InitData4));
	InitData4.pSysMem = planeIndicesS100;
	hr = _pd3dDevice->CreateBuffer(&bd4, &InitData4, &_pIndexBufferPlaneS50);

	if (FAILED(hr))
		return hr;

	// Create index buffer
	WORD QuadIndices[] =
	{
		0,1,2,
		2,1,3,
	};

	D3D11_BUFFER_DESC bd5;
	ZeroMemory(&bd5, sizeof(bd5));
	bd5.Usage = D3D11_USAGE_DEFAULT;
	bd5.ByteWidth = sizeof(WORD) * 6;
	bd5.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd5.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData5;
	ZeroMemory(&InitData5, sizeof(InitData5));
	InitData5.pSysMem = QuadIndices;
	hr = _pd3dDevice->CreateBuffer(&bd5, &InitData5, &_pIndexBufferQuad);

	// Create index buffer
	WORD NDCQuadIndices[] =
	{
		0,1,2,
		2,1,3,
	};

	D3D11_BUFFER_DESC bd6;
	ZeroMemory(&bd6, sizeof(bd6));
	bd6.Usage = D3D11_USAGE_DEFAULT;
	bd6.ByteWidth = sizeof(WORD) * 6;
	bd6.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd6.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData6;
	ZeroMemory(&InitData6, sizeof(InitData6));
	InitData6.pSysMem = NDCQuadIndices;
	hr = _pd3dDevice->CreateBuffer(&bd6, &InitData6, &_pIndexBufferNDCQuad);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 1080, 760};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
							nullptr, nullptr, hInstance, nullptr);

    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);

    return S_OK;
}

HRESULT Application::InitGameObjects()
{
	_rManager->LoadTexture(*_pd3dDevice, L"Textures/Default.dds");
	_rManager->LoadTexture(*_pd3dDevice, L"Textures/Crate_COLOR.dds");
	_rManager->LoadTexture(*_pd3dDevice, L"Textures/Skybox.dds");
	_rManager->LoadTexture(*_pd3dDevice, L"Textures/asphalt/asphalt.dds");
	_rManager->LoadTexture(*_pd3dDevice, L"Textures/ChainLink.dds");
	_rManager->LoadTexture(*_pd3dDevice, L"Textures/ball.dds");

	_rManager->LoadOBJ("car", "Mesh/car.obj", _pd3dDevice);
	_rManager->LoadOBJ("ball", "Mesh/ball.obj", _pd3dDevice);
	_rManager->LoadOBJ("ground", "Mesh/ground.obj", _pd3dDevice);
	_rManager->LoadOBJ("goal", "Mesh/goal.obj", _pd3dDevice);
	_rManager->LoadOBJ("cage", "Mesh/cage.obj", _pd3dDevice);
	_rManager->AddOBJ(new Mesh("skybox", _pVertexBufferSkybox, _pIndexBufferSkybox, 36));
	_rManager->AddOBJ(new Mesh("cube", _pVertexBufferCube, _pIndexBufferCube, 36));
	_rManager->AddOBJ(new Mesh("pyramid", _pVertexBufferPyramid, _pIndexBufferPyramid, 18));
	_rManager->AddOBJ(new Mesh("quad", _pVertexBufferQuad, _pIndexBufferQuad, 6));
	_rManager->AddOBJ(new Mesh("quadNDC", _pVertexBufferNDCQuad, _pIndexBufferNDCQuad, 6));
	_rManager->AddOBJ(new Mesh("planeS4", _pVertexBufferPlaneS4, _pIndexBufferPlaneS4, 96));
	_rManager->AddOBJ(new Mesh("planeS50", _pVertexBufferPlaneS50, _pIndexBufferPlaneS50, 96));

	Material* skyboxMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.6, 0.4f, 0.6f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(2), _pSamplerLinear);

	Material* chainLinkMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(4), _pSamplerLinear);

	Material* defaultMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.6f, 0.2f, 0.6f, 1.0f),
		XMFLOAT4(0.8f, 0.2f, 0.2f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(0), _pSamplerLinear);

	Material* goalPostsMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(0), _pSamplerLinear);

	Material* asphaltMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(3), _pSamplerLinear);

	Material* ballMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(5), _pSamplerLinear);
	
	Material* crateTransparentMtrl = new Material(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), 
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), 
		true, _pTransparency, _pSolid, _rManager->GetTextureAt(1), _pSamplerLinear);

	Material* tvQuadMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(0), _pSamplerLinear);
	
	_screenObject = new ScreenObject();
	_screenObject->SetName("screenQuad");
	_screenObject->SetMesh(_rManager->GetMeshByName("quadNDC"));

	GameObject* gameObject;

	gameObject = new GameObject();
	gameObject->SetName("skybox");
	gameObject->SetMesh(_rManager->GetMeshByName("skybox"));
	gameObject->SetMtrl(skyboxMtrl);
	gameObject->LoadStartingValues("StartingValues/Skybox.txt");
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("cage");
	gameObject->SetMesh(_rManager->GetMeshByName("cage"));
	gameObject->SetMtrl(chainLinkMtrl);
	gameObject->SetScale({ 2.0f, 2.0f, 2.0f });
	gameObject->ApplyTransformation({ 0.0f, 0.0f, 0.0f });
	_rManager->Add3DObject(gameObject);
	
	gameObject = new GameObject();
	gameObject->SetName("ground");
	gameObject->SetMesh(_rManager->GetMeshByName("planeS4"));
	gameObject->SetMtrl(asphaltMtrl);
	gameObject->LoadStartingValues("StartingValues/Ground.txt");
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("car");
	gameObject->SetMesh(_rManager->GetMeshByName("car"));
	gameObject->SetMtrl(defaultMtrl);
	gameObject->EnableMovement();
	gameObject->LoadStartingValues("StartingValues/Car.txt");
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("ball");
	gameObject->SetMesh(_rManager->GetMeshByName("ball"));
	gameObject->SetMtrl(ballMtrl);
	gameObject->LoadStartingValues("StartingValues/Ball.txt");
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("goal1");
	gameObject->SetMesh(_rManager->GetMeshByName("goal"));
	gameObject->SetMtrl(goalPostsMtrl);
	gameObject->SetScale({ -1.0f, -1.0f, -1.0f });
	gameObject->ApplyTransformation({ 0.0f, 6.0f, -30.0f });
	gameObject->ApplyRotation({ XM_PI, (XM_PI / 2.0f), 0.0f });
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("goal2");
	gameObject->SetMesh(_rManager->GetMeshByName("goal"));
	gameObject->SetMtrl(goalPostsMtrl);
	gameObject->SetScale({ -1.0f, -1.0f, -1.0f });
	gameObject->ApplyTransformation({ 0.0f, 6.0f, 30.0f });
	gameObject->ApplyRotation({ XM_PI, (XM_PI / 2.0f), 0.0f });
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("cube");
	gameObject->SetMesh(_rManager->GetMeshByName("cube"));
	gameObject->SetMtrl(crateTransparentMtrl);
	gameObject->LoadStartingValues("StartingValues/Cube.txt");
	_rManager->Add3DObject(gameObject);

	gameObject = new GameObject();
	gameObject->SetName("tvQuad");
	gameObject->SetMesh(_rManager->GetMeshByName("quad"));
	gameObject->SetMtrl(tvQuadMtrl);
	gameObject->LoadStartingValues("StartingValues/tvQuad.txt");
	_rManager->Add3DObject(gameObject);
	_inSceneTV = false;
	_switch = false;

	gameObject = nullptr;
	delete gameObject;

	skyboxMtrl = nullptr;
	delete skyboxMtrl;
	chainLinkMtrl = nullptr;
	delete chainLinkMtrl;
	defaultMtrl = nullptr;
	delete defaultMtrl;
	asphaltMtrl = nullptr;
	delete asphaltMtrl;
	ballMtrl = nullptr;
	delete ballMtrl;
	crateTransparentMtrl = nullptr;
	delete crateTransparentMtrl;

	return S_OK;
}

HRESULT Application::InitCameras()
{
	_cameraFront = new Camera(_rManager->GetObjectByName("car"), _WindowWidth, _WindowHeight, LOOK_AT);
	_cameraFront->SetEye(XMFLOAT4(0.0f, 6.0f, -10.0f, 0.0f));
	_cameraFront->SetAt(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	_cameraFront->SetUp(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
	_cameraFront->SetViewM();
	_cameraFront->SetProjectionM();

	_cameraBack = new Camera(_rManager->GetObjectByName("car"), _WindowWidth, _WindowHeight, LOOK_AT);
	_cameraBack->SetEye(XMFLOAT4(0.0f, 4.0f, 6.0f, 0.0f));
	_cameraBack->SetAt(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	_cameraBack->SetUp(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
	_cameraBack->SetViewM();
	_cameraBack->SetProjectionM();

	_cameraSide = new Camera(nullptr, _WindowWidth, _WindowHeight, LOOK_TO);
	_cameraSide->SetEye(XMFLOAT4(60.0f, 30.0f, 0.0f, 0.0f));
	_cameraSide->SetAt(XMFLOAT4(-1.9f, -0.8f, 0.0f, 0.0f));
	_cameraSide->SetUp(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
	_cameraSide->SetViewM();
	_cameraSide->SetProjectionM();

	_cameraTop = new Camera(nullptr, _WindowWidth, _WindowHeight, LOOK_TO);
	_cameraTop->SetEye(XMFLOAT4(0.0f, 70.0f, 0.0f, 0.0f));
	_cameraTop->SetAt(XMFLOAT4(0.0f, -1.8f, 0.01f, 0.0f));
	_cameraTop->SetUp(XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
	_cameraTop->SetViewM();
	_cameraTop->SetProjectionM();

	_currentCamera = _cameraFront;
	_tvCamera = _cameraSide;

	return S_OK;
}

HRESULT Application::InitParticles()
{
	_parSystem = new ParticleSystem();

	_rManager->LoadTexture(*_pd3dDevice, L"Textures/SmokeParticle.dds");

	Material* smokeParticleMtrl = new Material(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f),
		XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		false, _pTransparency, _pSolid, _rManager->GetTextureAt(6), _pSamplerLinear);

	SmokeManager* parManager = new SmokeManager("smokeManager", _currentCamera, 5.0f);
	parManager->AddParticles(_rManager->GetMeshByName("quad"), smokeParticleMtrl, 1, { 5.0f, 5.0f, 0.0f });

	_parSystem->AddParticleManager(parManager);

	smokeParticleMtrl = nullptr;
	delete smokeParticleMtrl;

	return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _WindowWidth;
    sd.BufferDesc.Height = _WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _WindowWidth;
	depthStencilDesc.Height = _WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr =_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	hr = _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr =_pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);
	if (FAILED(hr))
		return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_WindowWidth;
    vp.Height = (FLOAT)_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 0.1f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	InitVertexBuffer();
	InitIndexBuffer();

	//Init PhongShader
	_phongShader = new PhongShader(L"PhongShader.fx");
	_phongShader->screenWidth = _WindowWidth;
	_phongShader->screenHeight = _WindowHeight;

	//max is 3. not sure why
	_phongShader->_numOfTextures = 2;
	_phongShader->InitShadersAndInputLayout(_pd3dDevice);
	_phongShader->InitRenderToTexture(_pd3dDevice);

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the phong constant buffer
	D3D11_BUFFER_DESC bd1;
	ZeroMemory(&bd1, sizeof(bd1));
	bd1.Usage = D3D11_USAGE_DEFAULT;
	bd1.ByteWidth = sizeof(PhongCB);
	bd1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd1.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd1, nullptr, &_pPhongCB);
	if (FAILED(hr))
		return hr;

	_blurShader = new BlurShader(L"BlurShader.fx");
	_blurShader->screenWidth = _WindowWidth;
	_blurShader->screenHeight = _WindowHeight;

	//max is 3. not sure why
	_blurShader->_numOfTextures = 1;
	_blurShader->InitShadersAndInputLayout(_pd3dDevice);
	_blurShader->InitRenderToTexture(_pd3dDevice);

	// Create the blur constant buffer
	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(BlurCB);
	bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd2.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd2, nullptr, &_pBlurCB);
	if (FAILED(hr))
		return hr;

	//WireFrame RState
	D3D11_RASTERIZER_DESC wfdesc;

	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	wfdesc.FrontCounterClockwise = true;

	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_pWireFrame);
	if (FAILED(hr))
		return hr;

	//Solid RState
	D3D11_RASTERIZER_DESC sdesc;

	ZeroMemory(&sdesc, sizeof(D3D11_RASTERIZER_DESC));
	sdesc.FillMode = D3D11_FILL_SOLID;
	sdesc.CullMode = D3D11_CULL_BACK;
	sdesc.FrontCounterClockwise = true;

	hr = _pd3dDevice->CreateRasterizerState(&sdesc, &_pSolid);
    if (FAILED(hr))
        return hr;

	//pixel blending
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	hr = _pd3dDevice->CreateBlendState(&blendDesc, &_pTransparency);
	if (FAILED(hr))
		return hr;

    return S_OK;
}

void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();

    if (_pPhongCB) _pPhongCB->Release();
	if (_pBlurCB) _pBlurCB->Release();

	if (_pIndexBufferNDCQuad) _pIndexBufferNDCQuad->Release();
	if (_pVertexBufferQuad) _pVertexBufferQuad->Release();
	if (_pVertexBufferSkybox) _pVertexBufferSkybox->Release();
	if (_pIndexBufferSkybox) _pIndexBufferSkybox->Release();
    if (_pVertexBufferCube) _pVertexBufferCube->Release();
    if (_pIndexBufferCube) _pIndexBufferCube->Release();
	if (_pVertexBufferPyramid) _pVertexBufferPyramid->Release();
	if (_pIndexBufferPyramid) _pIndexBufferPyramid->Release();
	if (_pVertexBufferPlaneS4) _pVertexBufferPlaneS4->Release();
	if (_pIndexBufferPlaneS4) _pIndexBufferPlaneS4->Release();
	if (_pVertexBufferPlaneS50) _pVertexBufferPlaneS50->Release();
	if (_pIndexBufferPlaneS50) _pIndexBufferPlaneS50->Release();

	if (_phongShader) _phongShader->~PhongShader();
	if (_blurShader) _blurShader->~BlurShader();
    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();
    if (_pImmediateContext) _pImmediateContext->Release();
    if (_pd3dDevice) _pd3dDevice->Release();

	if (_pSamplerLinear) _pSamplerLinear->Release();

	if (_pTransparency) _pTransparency->Release();

	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();

	if (_pWireFrame) _pWireFrame->Release();
	if (_pSolid) _pSolid->Release();

	if (_rManager) _rManager->Cleanup();;
	if (_parSystem) _parSystem->Cleanup();

	if(_currentCamera) _currentCamera->~Camera();
	if (_cameraFront) _cameraFront->~Camera();
	if (_cameraBack) _cameraBack->~Camera();
	if (_cameraSide) _cameraSide->~Camera();
	if (_cameraTop) _cameraTop->~Camera();

	if (_dLight) _dLight->~DirectionalLight();
	if (_sLight) _sLight->~SpotLight();
}

void Application::Update()
{
	float previousTime = _totalTime;

    // Update our time
    if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        _totalTime = (float) XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();

        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;

        _totalTime = (dwTimeCur - dwTimeStart) / 1000.0f;
    }
	_phongShader->_cb.gTime = _totalTime;

	_deltaTime = _totalTime - previousTime;

	//update game objects
	for (int i = 0; i < _rManager->GetObjectArraySize(); i++)
	{
		GameObject* object = _rManager->GetObjectByIndex(i);
		if (object->GetName() == "ball")
		{
			//object->SetTransform(XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixTranslation(0, 1, 0));
		}
		object->Update(_deltaTime);
	}

	//update screen quad
	_screenObject->Update(_deltaTime);

	//update particles
	_parSystem->UpdateParticles(_deltaTime);

	if (!_blurShader->IsShaderEnabled())
	{
		//get keyboard inputs
		if (GetAsyncKeyState('1'))
		{
			_currentCamera = _cameraFront;
		}
		else if (GetAsyncKeyState('2'))
		{
			_currentCamera = _cameraBack;
		}
		else if (GetAsyncKeyState('3'))
		{
			_tvCamera = _cameraSide;
		}
		else if (GetAsyncKeyState('4'))
		{
			_tvCamera = _cameraTop;
		}
	}

	if (_currentCamera->GetLookState() == LOOK_STATE::LOOK_AT)
	{
		//turn blur on and off
		if (GetAsyncKeyState('5'))
		{
			_blurShader->EnableShader();
		}
		else if (GetAsyncKeyState('6'))
		{
			_blurShader->DisableShader();
		}
	}

	//turn blur on and off
	if (GetAsyncKeyState('7'))
	{
		_inSceneTV = true;
	}
	else if (GetAsyncKeyState('8'))
	{
		_inSceneTV = false;
	}

	//update camera
	_currentCamera->SetMousePos(mouseX, mouseY);
	_currentCamera->Update(_deltaTime);
	_tvCamera->Update(_deltaTime);

	XMFLOAT4 tempEye = _currentCamera->GetEye();
	XMFLOAT4 tempAt = _currentCamera->GetAt();
	_dLight->_eyePosW = {tempEye.x, tempEye.y, tempEye.z};
	_dLight->_lightDirection = { tempAt.x - tempEye.x, tempAt.y - tempEye.y, tempAt.z - tempEye.z };
}

void Application::Draw()
{
	//currently working on
    float ClearColor[4] = {0.15f, 0.5f, 0.7f, 1.0f};

	if (_phongShader->IsShaderEnabled())
	{
		//Set the input layout
		_pImmediateContext->IASetInputLayout(_phongShader->GetVLayout());
		_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* nullRTV = nullptr;
		_pImmediateContext->OMSetRenderTargets(1, &nullRTV, nullptr);
		ID3D11ShaderResourceView *const pSRV[1] = { NULL };
		_pImmediateContext->PSSetShaderResources(0, 1, pSRV);

		if (_inSceneTV)
		{
			DrawSecondCamera();

			_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}

		if (_blurShader->IsShaderEnabled())
		{
			_pImmediateContext->OMSetRenderTargets(1, &_blurShader->_renderTVArray[0], _depthStencilView);
			_pImmediateContext->ClearRenderTargetView(_blurShader->_renderTVArray[0], ClearColor);
		}
		else
		{
			_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);
			_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
		}

		if (!_inSceneTV)
		{
			_pImmediateContext->PSSetShader(_phongShader->GetPixelShader(), nullptr, 0);
			_pImmediateContext->VSSetShader(_phongShader->GetVertexShader(), nullptr, 0);
		}

		XMMATRIX view = XMLoadFloat4x4(&_currentCamera->GetViewM());
		XMMATRIX projection = XMLoadFloat4x4(&_currentCamera->GetProjectionM());

		_phongShader->_cb.mView = XMMatrixTranspose(view);
		_phongShader->_cb.mProjection = XMMatrixTranspose(projection);

		_phongShader->_cb.lightVecW = _dLight->_lightDirection;
		_phongShader->_cb.ambientLight = _dLight->_ambientLight;
		_phongShader->_cb.diffuseLight = _dLight->_diffuseLight;
		_phongShader->_cb.specularLight = _dLight->_specularLight;
		_phongShader->_cb.specularPower = _dLight->_specularPower;
		_phongShader->_cb.eyePosW = _dLight->_eyePosW;

		//switch shaderRV's to avoid error.
		if (_inSceneTV)
		{
			Material* temp = _rManager->GetObjectByName("tvQuad")->GetMtrl();
			if (_switch)
			{
				temp->textureShaderRV = _phongShader->_shaderRVArray.at(0);
			}
			else
			{
				temp->textureShaderRV = _phongShader->_shaderRVArray.at(1);
			}
			_rManager->GetObjectByName("tvQuad")->SetMtrl(temp);
			_switch = !_switch;
		}

		//rendering particles
		_parSystem->DrawParticles(_pImmediateContext, _pPhongCB, _phongShader->_cb);

		//rendering opaque objects
		for (int i = 0; i < _rManager->GetObjectArraySize(); i++)
		{
			if (!_rManager->GetObjectByIndex(i)->GetMtrl()->isTransparent)
			{
				_rManager->GetObjectByIndex(i)->Draw(_pImmediateContext, _pPhongCB, _phongShader->_cb);
			}
		}

		//rendering transparent objects
		for (int i = 0; i < _rManager->GetObjectArraySize(); i++)
		{
			if (_rManager->GetObjectByIndex(i)->GetMtrl()->isTransparent)
			{
				_rManager->GetObjectByIndex(i)->Draw(_pImmediateContext, _pPhongCB, _phongShader->_cb);
			}
		}

		if (_blurShader->IsShaderEnabled())
		{
			int currentLoop = 1;
			DrawBlur(currentLoop, _blurShader->_shaderRVArray[0]);
		}

		// Present our back buffer to our front buffer
		_pSwapChain->Present(0, 0);
	}
}

void Application::DrawSecondCamera()
{
	//currently working on
	float ClearColor[4] = { 0.15f, 0.5f, 0.7f, 1.0f };

	//switch shaderRV's to avoid error.
	if (_switch)
	{
		_pImmediateContext->OMSetRenderTargets(1, &_phongShader->_renderTVArray[0], _depthStencilView);
		_pImmediateContext->ClearRenderTargetView(_phongShader->_renderTVArray[0], ClearColor);
	}
	else
	{
		_pImmediateContext->OMSetRenderTargets(1, &_phongShader->_renderTVArray[1], _depthStencilView);
		_pImmediateContext->ClearRenderTargetView(_phongShader->_renderTVArray[1], ClearColor);
	}

	_pImmediateContext->PSSetShader(_phongShader->GetPixelShader(), nullptr, 0);
	_pImmediateContext->VSSetShader(_phongShader->GetVertexShader(), nullptr, 0);

	XMMATRIX view = XMLoadFloat4x4(&_tvCamera->GetViewM());
	XMMATRIX projection = XMLoadFloat4x4(&_tvCamera->GetProjectionM());

	_phongShader->_cb.mView = XMMatrixTranspose(view);
	_phongShader->_cb.mProjection = XMMatrixTranspose(projection);

	_phongShader->_cb.lightVecW = _dLight->_lightDirection;
	_phongShader->_cb.ambientLight = _dLight->_ambientLight;
	_phongShader->_cb.diffuseLight = _dLight->_diffuseLight;
	_phongShader->_cb.specularLight = _dLight->_specularLight;
	_phongShader->_cb.specularPower = _dLight->_specularPower;
	_phongShader->_cb.eyePosW = _dLight->_eyePosW;

	//rendering particles
	_parSystem->DrawParticles(_pImmediateContext, _pPhongCB, _phongShader->_cb);

	//rendering opaque objects
	for (int i = 0; i < _rManager->GetObjectArraySize(); i++)
	{
		if (!_rManager->GetObjectByIndex(i)->GetMtrl()->isTransparent)
		{
			_rManager->GetObjectByIndex(i)->Draw(_pImmediateContext, _pPhongCB, _phongShader->_cb);
		}
	}

	//rendering transparent objects
	for (int i = 0; i < _rManager->GetObjectArraySize(); i++)
	{
		if (_rManager->GetObjectByIndex(i)->GetMtrl()->isTransparent)
		{
			_rManager->GetObjectByIndex(i)->Draw(_pImmediateContext, _pPhongCB, _phongShader->_cb);
		}
	}
}

void Application::DrawBlur(int currentLoop, ID3D11ShaderResourceView* shaderRV)
{
	float ClearColor[4] = { 0.15f, 0.5f, 0.7f, 1.0f };

	if (currentLoop >= _blurShader->_numOfTextures - 1)
	{
		_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, nullptr);
		_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);

		// Set the input layout
		_pImmediateContext->IASetInputLayout(_blurShader->GetVLayout());

		_pImmediateContext->PSSetShader(_blurShader->GetPixelShader(), nullptr, 0);
		_pImmediateContext->VSSetShader(_blurShader->GetVertexShader(), nullptr, 0);

		_screenObject->SetTextureRV(shaderRV);
		_screenObject->SetRState(_pSolid);
		_screenObject->SetSamLinear(_pSamplerLinear);
		_screenObject->Draw(_pImmediateContext, _pBlurCB, _blurShader->_cb);
	}
	else
	{
		_pImmediateContext->OMSetRenderTargets(1, &_blurShader->_renderTVArray[currentLoop], _depthStencilView);
		_pImmediateContext->ClearRenderTargetView(_blurShader->_renderTVArray[currentLoop], ClearColor);

		// Set the input layout
		_pImmediateContext->IASetInputLayout(_blurShader->GetVLayout());

		_pImmediateContext->PSSetShader(_blurShader->GetPixelShader(), nullptr, 0);
		_pImmediateContext->VSSetShader(_blurShader->GetVertexShader(), nullptr, 0);

		_screenObject->SetTextureRV(shaderRV);
		_screenObject->SetRState(_pSolid);
		_screenObject->SetSamLinear(_pSamplerLinear);
		_screenObject->Draw(_pImmediateContext, _pBlurCB, _blurShader->_cb);

		shaderRV = _blurShader->_shaderRVArray[currentLoop];
		currentLoop++;

		DrawBlur(currentLoop, shaderRV);
	}
}