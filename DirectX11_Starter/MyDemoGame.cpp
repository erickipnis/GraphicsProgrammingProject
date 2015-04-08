// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include <Windows.h>
#include <d3dcompiler.h>
#include "MyDemoGame.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	MyDemoGame game(hInstance);
	
	// If we can't initialize, we can't run
	if( !game.Init() )
		return 0;
	
	// All set to run the game
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

MyDemoGame::MyDemoGame(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Demo DX11 Game";
	windowWidth = 1280;
	windowHeight = 720;
	state = Start;
}

MyDemoGame::~MyDemoGame()
{
	// delete the pointers
	if (mesh1 != nullptr)
	{
		delete mesh1;
		mesh1 = nullptr;
	}
	if (mesh2 != nullptr)
	{
		delete mesh2;
		mesh2 = nullptr;
	}
	if (mesh3 != nullptr)
	{
		delete mesh3;
		mesh3 = nullptr;
	}
	if (waterMesh != nullptr)
	{
		delete waterMesh;
		waterMesh = nullptr;
	}

	if (material != nullptr)
	{
		delete material;
		material = nullptr;
	}
	if (waterMaterial != nullptr)
	{
		delete waterMaterial;
		waterMaterial = nullptr;
	}

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
	for (int i = 0; i < ships.size(); i++)
	{
		if (ships[i] != nullptr)
		{
			delete ships[i];
			ships[i] = nullptr;
		}
	}


	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}

	if (pixelShader != nullptr)
	{
		delete pixelShader;
		pixelShader = nullptr;
	}

	if (vertexShader != nullptr)
	{
		delete vertexShader;
		vertexShader = nullptr;
	}

	if (grid != nullptr)
	{
		delete grid;
		grid = nullptr;
	}
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	// Initialize Lights
	directionalLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.DiffuseColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);

	directionalLight2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight2.DiffuseColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight2.Direction = XMFLOAT3(-1.0f, 1.0f, -1.0f);

	// Set up camera-related matrices
	InitializeCameraMatrices();

	// Make sure DirectX initializes properly
	if( !DirectXGame::Init() )
		return false;

	// Create the necessary DirectX buffers to draw something
	CreateGeometryBuffers();

	// initialize shaders
	pixelShader = new SimplePixelShader(device, deviceContext);
	vertexShader = new SimpleVertexShader(device, deviceContext);

	// create sampler state and resource view for materials
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* samplerState;
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11ShaderResourceView* waterSRV;

	device->CreateSamplerState(&samplerDesc, &samplerState);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BoatUV.png", 0, &srv);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"water.jpg", 0, &waterSRV);

	material = new Material(pixelShader, vertexShader, srv, samplerState);
	waterMaterial = new Material(pixelShader, vertexShader, waterSRV, samplerState);

	// Create the game entities
	//entities.push_back(new GameEntity(mesh1, material));
	entities.push_back(new GameEntity(mesh2, material));
	//entities.push_back(new GameEntity(mesh2, material));
	//entities.push_back(new GameEntity(mesh3, material));
	entities[0]->SetPosition(XMFLOAT3(-5.0f, -1.0f, 1.0f));
	//entities[1]->SetPosition(XMFLOAT3(0.0f, -1.0f, 1.0f));
	//entities[2]->SetPosition(XMFLOAT3(-1.0f, -2.0f, 0.0f));
	//entities[3]->SetPosition(XMFLOAT3(-3.0f, -1.0f, 5.0f));

	entities.push_back(new GameEntity(waterMesh, waterMaterial));
	entities[1]->SetPosition(XMFLOAT3(5.0f, 1.0f, 1.0f));

	//create the ships
	//ships.push_back(new Ship(entities[0]));
	//ships.push_back(new Ship(entities[1]));

	//ships[0]->shipEntity->SetPosition(XMFLOAT3(-5.0f, -1.0f, 1.0f));
	//ships[1]->shipEntity->SetPosition(XMFLOAT3(-5.0f, -2.0f, 0.0f));

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	pixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	pixelShader->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight));

	// Set up world matrix
	// In an actual game, each object will need one of these and they should
	//  update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	
	// Set up the grid
	grid = new Grid(2, 2, 2.0f, XMFLOAT3(0.0f, 0.0f, 0.0f));

	//make sure we draw tris correctly
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Successfully initialized
	return true;
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::CreateGeometryBuffers()
{

	Vertex vertices[4] = 
	{
		{ XMFLOAT3(0.5, 0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5, -0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5, -0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-0.5, 0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }
	};

	UINT indices[] = { 0, 1, 2, 2, 3, 0 };
	//mesh1 = new Mesh(vertices, 4, indices, 6, device);
	mesh1 = new Mesh("sphere.obj", device);

	mesh2 = new Mesh("cube.obj", device);

	mesh3 = new Mesh("Boat.obj", device);

	waterMesh = new Mesh("plain.obj", device);
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void MyDemoGame::LoadShadersAndInputLayout()
{
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	vertexShader->LoadShaderFile(L"VertexShader.cso");
	pixelShader->LoadShaderFile(L"PixelShader.cso");

}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	camera = new Camera();
	camera->SetDirection(XMFLOAT3(0.0f, -0.99f, 0.01f));
	//camera->Rotate(0, 310);
	camera->Update();
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	camera->UpdateProjection(AspectRatio());
}
#pragma endregion

#pragma region Game Loop

// Update your game state
void MyDemoGame::UpdateScene(float dt)
{
	switch (state)
	{
		case Start:

			break;

		case Game:
			if (GetAsyncKeyState('P') && 0x8000)
			{
				if (!pauseKeyDown)
				{
					state = Paused;
				}
				pauseKeyDown = true;
			}
			else
			{
				pauseKeyDown = false;
			}
			// Take input, update game logic, etc.
			if (GetAsyncKeyState('M'))
			{
				ships[0]->shipEntity->Translate(XMFLOAT3(1.0f * dt, 0.0f, 0.0f));
				ships[0]->shipEntity->Update();
			}
			for (int i = 0; i < ships.size(); i++)
			{
				ships[i]->shipEntity->Translate(XMFLOAT3(ships[i]->speed * dt, 0.0f, 0.0f));
				ships[i]->shipEntity->Update();
			}

			camera->Update();
			break;

		case Paused:
			if (GetAsyncKeyState('P') && 0x8000)
			{
				if (!pauseKeyDown)
				{
					state = Game;
				}
				pauseKeyDown = true;
			}
			else
			{
				pauseKeyDown = false;
			}
			break;

		case Over:

			break;
	}
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float startColor[4] = {0.2f, 0.2f, 0.2f, 0.0f};
	const float gameColor[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	const float pauseColor[4] = { 0.15f, 0.35f, 0.5f, 0.0f };


	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearRenderTargetView(renderTargetView, startColor);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	switch (state)
	{
	case Start:

		break;

	case Game:
		deviceContext->ClearRenderTargetView(renderTargetView, gameColor);
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->Draw(*deviceContext, *camera);
		}
		break;

	case Paused:
		deviceContext->ClearRenderTargetView(renderTargetView, pauseColor);
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->Draw(*deviceContext, *camera);
		}
		break;

	case Over:

		break;
	}

	
	// Present the buffer
	//  - Puts the stuff on the screen
	//  - Do this EXACTLY once per frame
	//  - Always at the end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	prevMousePos.x = x;
	prevMousePos.y = y;

	float pointX = (2.0f *  (float)x / (float)1280) - 1.0f;
	float pointY = (2.0f *  (float)y / (float)720) - 1.0f;


	SetCapture(hMainWnd);

	//make a new ship
	entities.push_back(new GameEntity(mesh3, material));
	ships.push_back(new Ship(entities[entities.size() - 1]));

	ships[ships.size() - 1]->shipEntity->SetPosition(XMFLOAT3(pointX * 7, 0.0f, -pointY * 5));
	ships[ships.size() - 1]->speed = 1;
	ships[ships.size() - 1]->shipEntity->Update();
	ships[ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	ships[ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
	if (state == Start)
	{
		state = Game;
	}
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	int xDif = x - prevMousePos.x;
	int yDif = y - prevMousePos.y;

	if (prevMousePos.x != 0)
		//camera->Rotate(xDif, yDif);
	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion