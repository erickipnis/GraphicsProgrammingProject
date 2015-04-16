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
	if (startMenu != nullptr)
	{
		delete startMenu;
		startMenu = nullptr;
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


	if (normalMapPixelShader != nullptr)
	{
		delete normalMapPixelShader;
		normalMapPixelShader = nullptr;
	}

	if (normalMapVertexShader != nullptr)
	{
		delete normalMapVertexShader;
		normalMapVertexShader = nullptr;
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
	/* initialize random seed: */
	srand(time(NULL));

	// Initialize Lights
	directionalLight.AmbientColor = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	directionalLight.DiffuseColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight.Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);

	directionalLight2.AmbientColor = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
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
	normalMapPixelShader = new SimplePixelShader(device, deviceContext);
	vertexShader = new SimpleVertexShader(device, deviceContext);
	normalMapVertexShader = new SimpleVertexShader(device, deviceContext);

	// create sampler state and resource view for materials
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* tileSRV;

	ID3D11SamplerState* samplerState;
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	// Testing Blend States for transparency
	/*ID3D11BlendState* blendState;
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;*/

	ID3D11ShaderResourceView* waterSRV;
	ID3D11ShaderResourceView* waterNormalMapSRV;
	ID3D11ShaderResourceView* defaultSRV;
	ID3D11ShaderResourceView* startSRV;
	ID3D11ShaderResourceView* instructSRV;
	ID3D11ShaderResourceView* scoreSRV;
	ID3D11ShaderResourceView* creditSRV;


	device->CreateSamplerState(&samplerDesc, &samplerState);
	//device->CreateBlendState(&blendDesc, &blendState);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BoatUV.png", 0, &srv);


	DirectX::CreateWICTextureFromFile(device, deviceContext, L"tile2.png", 0, &tileSRV);

	material = new Material(pixelShader, vertexShader, srv, samplerState);
	tileMaterial = new Material(pixelShader, vertexShader, tileSRV, samplerState);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"StartScreenTextureDefault.png", 0, &defaultSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"StartScreenTextureStart.png", 0, &startSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"StartScreenTextureInstructions.png", 0, &instructSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"StartScreenTextureScores.png", 0, &scoreSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"water.png", 0, &waterSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"water-normal-map.png", 0, &waterNormalMapSRV);
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"pebbleDiffuse.jpg", 0, &waterSRV);
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"pebbleNormal.jpg", 0, &waterNormalMapSRV);

	material = new Material(pixelShader, vertexShader, srv, samplerState);
	waterMaterial = new Material(normalMapPixelShader, normalMapVertexShader, waterSRV, samplerState);
	startDefaultMaterial = new Material(pixelShader, vertexShader, defaultSRV, samplerState);
	startStartMaterial = new Material(pixelShader, vertexShader, startSRV, samplerState);
	startInstructMaterial = new Material(pixelShader, vertexShader, instructSRV, samplerState);
	startScoreMaterial = new Material(pixelShader, vertexShader, scoreSRV, samplerState);


	// Create the game entities
	startScreen = new GameEntity(waterMesh, startDefaultMaterial);
	startScreen->SetPosition(XMFLOAT3(0.0f, -5.0f, 2.0f));
	startScreen->SetRotation(XMFLOAT3(-0.2f, 0.0f, 0.0f));
	startScreen->SetScale(XMFLOAT3(1.8f, 1.0f, 1.18f));
	startScreen->Update();
		// Create the game entities
	entities.push_back(new GameEntity(mesh2, material));
	entities[0]->SetPosition(XMFLOAT3(-5.0f, -1.0f, 1.0f));
	entities[0]->Update();

	entities.push_back(new GameEntity(waterMesh, waterMaterial));
	entities[1]->SetPosition(XMFLOAT3(5.0f, 1.0f, 1.0f));

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

	normalMapPixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	normalMapPixelShader->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight));

	normalMapPixelShader->SetShaderResourceView("waterNormalMap", waterNormalMapSRV);	

	// Set up world matrix
	// In an actual game, each object will need one of these and they should
	//  update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	
	// Set up the grid
	grid = new Grid(6, 10, 1.0f, XMFLOAT3(-4.5f, 0.0f, -0.5f), tileMesh, tileMaterial);

	//Make the player
	player = Player();

	//Make the enemy class
	enemy = Enemy();

	m_font.reset(new SpriteFont(device, L"myfile.spritefont"));
	m_spriteBatch.reset(new SpriteBatch(deviceContext));

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
		{ XMFLOAT3(0.5, 0.0f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-0.5, 0.0f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }
	};

	UINT indices[] = { 0, 1, 2, 2, 3, 0 };
	//mesh1 = new Mesh(vertices, 4, indices, 6, device);
	mesh1 = new Mesh("sphere.obj", device);

	mesh2 = new Mesh("cube.obj", device);

	mesh3 = new Mesh("Boat.obj", device);

	torpedo = new Mesh("Torpedo.obj", device);

	tileMesh = new Mesh("tile.obj", device);
	//startMenu = new Mesh(vertices, 4, indices, 6, device);

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
	normalMapPixelShader->LoadShaderFile(L"NormalMapPixelShader.cso");
	normalMapVertexShader->LoadShaderFile(L"NormalMapVertexShader.cso");

}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	camera = new Camera();
	camera->SetDirection(XMFLOAT3(0.0f, -0.5f, 0.1f));
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
			if (prevMousePos.x > 404 && prevMousePos.x < 872)
			{
				if (prevMousePos.y > 215 && prevMousePos.y < 318)
				{
					startScreen = new GameEntity(waterMesh, startStartMaterial);
					if (mouseDown)
					{
						state = Game;
					}
				}
				else if (prevMousePos.y > 355 && prevMousePos.y < 458)
				{
					startScreen = new GameEntity(waterMesh, startInstructMaterial);
				}
				else if (prevMousePos.y > 495 && prevMousePos.y < 598)
				{
					startScreen = new GameEntity(waterMesh, startScoreMaterial);
				}
				else
				{
					startScreen = new GameEntity(waterMesh, startDefaultMaterial);
				}
				startScreen->SetPosition(XMFLOAT3(0.0f, 0.0f, 2.0f));
				startScreen->SetRotation(XMFLOAT3(-0.2f, 0.0f, 0.0f));
				startScreen->SetScale(XMFLOAT3(1.8f, 1.0f, 1.18f));
				startScreen->Update();
			}
			else
			{
				startScreen = new GameEntity(waterMesh, startDefaultMaterial);
				startScreen->SetPosition(XMFLOAT3(0.0f, 0.0f, 2.0f));
				startScreen->SetRotation(XMFLOAT3(-0.2f, 0.0f, 0.0f));
				startScreen->SetScale(XMFLOAT3(1.8f, 1.0f, 1.18f));
				startScreen->Update();
			}
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
			enemy.spawnTimer++;
			if (enemy.spawnTimer > 1 / dt)
			{
				entities.push_back(new GameEntity(mesh3, material));
				enemy.ships.push_back(new Ship(entities[entities.size() - 1]));

				enemy.ships[enemy.ships.size() - 1]->shipEntity->SetPosition(XMFLOAT3(4, 0.0f, rand() % 6 - 2));
				enemy.ships[enemy.ships.size() - 1]->speed = -1;
				enemy.ships[enemy.ships.size() - 1]->shipEntity->Update();
				enemy.ships[enemy.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
				enemy.ships[enemy.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, -1.57f, 0.0f));

				enemy.spawnTimer = 0;
			}
			//update player ships
			for (int i = 0; i < player.ships.size(); i++)
			{
				//shooting
				player.ships[i]->projectileTimer += 1;
				if (player.ships[i]->projectileTimer > 1 / dt)
				{
					entities.push_back(new GameEntity(torpedo, material));
					player.ships[i]->projectiles.push_back(new Projectile(entities[entities.size() - 1]));
					player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->SetPosition(player.ships[i]->shipEntity->GetPosition());
					player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->speed = 4;
					player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
					player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->SetRotation(XMFLOAT3(0.0f, 0.0f, -1.57f));
					player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->Update();

					player.ships[i]->projectileTimer = 0;
				}

				//update mesh
				player.ships[i]->shipEntity->Update();
				//update bullets
				for (int j = 0; j < player.ships[i]->projectiles.size(); j++)
				{
					player.ships[i]->projectiles[j]->projectileEntity->Translate(XMFLOAT3(player.ships[i]->projectiles[j]->speed * dt, 0.0f, 0.0f));
					player.ships[i]->projectiles[j]->projectileEntity->Update();

					//delete if off screen
					if (player.ships[i]->projectiles[j]->projectileEntity->GetPosition().x > 8)
					{
						//delete player.ships[i]->projectiles[j];
						player.ships[i]->projectiles.erase(player.ships[i]->projectiles.begin() + j);
					}
				}
			}
			//update enemy ships
			for (int i = 0; i < enemy.ships.size(); i++)
			{
				enemy.ships[i]->shipEntity->Translate(XMFLOAT3(enemy.ships[i]->speed * dt, 0.0f, 0.0f));
				enemy.ships[i]->shipEntity->Update();
				if (enemy.ships[i]->shipEntity->GetPosition().x < -6)
				{
					enemy.ships.erase(enemy.ships.begin() + i);
				}
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

	normalMapPixelShader->SetFloat("time", this->timer.TotalTime());
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
		startScreen->Draw(*deviceContext, *camera);
		break;

	case Game:
		deviceContext->ClearRenderTargetView(renderTargetView, gameColor);

		grid->Draw(*deviceContext, *camera);

		//draw water and base
		entities[0]->Draw(*deviceContext, *camera);
		entities[1]->Draw(*deviceContext, *camera);
		//draw all of the player ships
		for (int i = 0; i < player.ships.size(); i++)
		{
			player.ships[i]->shipEntity->Draw(*deviceContext, *camera);
		}
		//draw all of the projectiles
		for (int i = 0; i < player.ships.size(); i++)
		{
			for (int j = 0; j < player.ships[i]->projectiles.size(); j++)
			{
				player.ships[i]->projectiles[j]->projectileEntity->Draw(*deviceContext, *camera);
			}
		}
		//draw all of the enemy ships
		for (int i = 0; i < enemy.ships.size(); i++)
		{
			enemy.ships[i]->shipEntity->Draw(*deviceContext, *camera);
		}
		//text
		m_spriteBatch->Begin();
		m_font->DrawString(m_spriteBatch.get(), L"Ships Left:", XMFLOAT2(0, 0));
		m_spriteBatch->End();
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
	mouseDown = true;
	prevMousePos.x = x;
	prevMousePos.y = y;

	float pointX = (2.0f *  (float)x / (float)1280) - 1.0f;
	float pointY = (2.0f *  (float)y / (float)720) - 1.0f;

	SetCapture(hMainWnd);

	GridTile* closest = grid->GetNearestTile(prevMousePos.x, prevMousePos.y, 1280, 720, camera);
	XMFLOAT3 shipPos = closest->GetPosition();

	//make a new ship
	entities.push_back(new GameEntity(mesh3, material));
	player.ships.push_back(new Ship(entities[entities.size() - 1]));

	player.ships[player.ships.size() - 1]->shipEntity->SetPosition(shipPos);
	player.ships[player.ships.size() - 1]->speed = 0;
	player.ships[player.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	player.ships[player.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
	player.numShips--;
	player.ships[player.ships.size() - 1]->shipEntity->Update();

}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	mouseDown = false;
	ReleaseCapture();
	//if (state == Start)
	//{
	//	state = Game;
	//}
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	prevMousePos.x = x;
	prevMousePos.y = y;

	int xDif = x - prevMousePos.x;
	int yDif = y - prevMousePos.y;

	if (prevMousePos.x != 0)
		//camera->Rotate(xDif, yDif);
	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion