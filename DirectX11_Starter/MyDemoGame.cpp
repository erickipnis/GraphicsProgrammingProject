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
}

MyDemoGame::~MyDemoGame()
{
	// delete the pointers
	delete mesh1;
	delete mesh2;
	delete mesh3;

	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}

	delete camera;

	delete pixelShader;
	delete vertexShader;

	delete material;
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	// Initialize Lights
	directionalLight.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.DiffuseColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	directionalLight.Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);

	directionalLight2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight2.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
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


	material = new Material(pixelShader, vertexShader);

	// Create the game entities
	entities.push_back(new GameEntity(mesh1, material));
	entities.push_back(new GameEntity(mesh2, material));
	entities.push_back(new GameEntity(mesh2, material));
	entities.push_back(new GameEntity(mesh3, material));
	entities[0]->SetPosition(XMFLOAT3(0.0f, 0.0f, 10.0f));
	entities[1]->SetPosition(XMFLOAT3(0.0f, -1.0f, 1.0f));
	entities[2]->SetPosition(XMFLOAT3(-1.0f, -2.0f, 0.0f));
	entities[3]->SetPosition(XMFLOAT3(-3.0f, -1.0f, 5.0f));


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

	mesh3 = new Mesh("torus.obj", device);
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
	// Take input, update game logic, etc.
	entities[0]->Translate(XMFLOAT3(1.0f * dt, 0.0f, 0.0f));
	entities[0]->Rotate(XMFLOAT3(0.0f, cos(timer.TotalTime()) * dt, 0.0f));
	entities[0]->Update();

	entities[1]->Translate(XMFLOAT3(0.0f, cos(timer.TotalTime()) * dt, 0.0f));
	entities[1]->Rotate(XMFLOAT3(0.0f, 0.0f, 0.0f));
	entities[1]->Update();

	entities[2]->Translate(XMFLOAT3(0.0f, 0.0f, 1.0f * dt));
	entities[2]->Rotate(XMFLOAT3(0.0f, 0.0f, 1.0f * dt));
	entities[2]->Update();

	entities[3]->Translate(XMFLOAT3(0.0f, 0.0f, 0.0f));
	entities[3]->Rotate(XMFLOAT3(0.0f, 0.0f, 2.0f * dt));
	entities[3]->Update();

	camera->Update();
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Draw(*deviceContext, *camera);
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

	SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	int xDif = x - prevMousePos.x;
	int yDif = y - prevMousePos.y;

	if (prevMousePos.x != 0)
		camera->Rotate(xDif, yDif);
	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion