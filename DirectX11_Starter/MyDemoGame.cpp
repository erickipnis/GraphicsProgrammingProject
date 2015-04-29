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

	

	//delete skybox stuff
	/*
	sphereIndexBuffer->Release();
	sphereVertBuffer->Release();

	SKYMAP_VS->Release();
	SKYMAP_PS->Release();
	SKYMAP_VS_Buffer->Release();
	SKYMAP_PS_Buffer->Release();

	smrv->Release();

	DSLessEqual->Release();
	RSCullNone->Release();
	*/
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
	ID3D11ShaderResourceView* overSRV;
	ID3D11ShaderResourceView* instructSRV;
	ID3D11ShaderResourceView* scoreSRV;
	ID3D11ShaderResourceView* creditSRV;

	ID3D11ShaderResourceView* assaultSRV;
	ID3D11ShaderResourceView* battleshipSRV;
	ID3D11ShaderResourceView* submarineSRV;
	ID3D11ShaderResourceView* enemySRV;

	ID3D11ShaderResourceView* bulletSRV;
	ID3D11ShaderResourceView* torpedoSRV;
	ID3D11ShaderResourceView* baseSRV;
	ID3D11ShaderResourceView* mineSRV;



	device->CreateSamplerState(&samplerDesc, &samplerState);
	//device->CreateBlendState(&blendDesc, &blendState);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BoatUV.png", 0, &srv);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"AssualtBoat.png", 0, &assaultSRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"SubUV.png", 0, &submarineSRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BattleshipUV.png", 0, &battleshipSRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BoatEnemyUV.png", 0, &enemySRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BulletUV.png", 0, &bulletSRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"TorpedoUV.png", 0, &torpedoSRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"BaseUV.png", 0, &baseSRV);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"MineUV.png", 0, &mineSRV);




	DirectX::CreateWICTextureFromFile(device, deviceContext, L"tile2.png", 0, &tileSRV);

	material = new Material(pixelShader, vertexShader, srv, samplerState);
	tileMaterial = new Material(pixelShader, vertexShader, tileSRV, samplerState);
	assaultMaterial = new Material(pixelShader, vertexShader, assaultSRV, samplerState);
	subMaterial = new Material(pixelShader, vertexShader,submarineSRV, samplerState);
	battleMaterial = new Material(pixelShader, vertexShader, battleshipSRV, samplerState);
	enemyMaterial = new Material(pixelShader, vertexShader, enemySRV, samplerState);

	bulletMaterial = new Material(pixelShader, vertexShader, bulletSRV, samplerState);
	torpedoMaterial = new Material(pixelShader, vertexShader, torpedoSRV, samplerState);
	baseMaterial = new Material(pixelShader, vertexShader, baseSRV, samplerState);
	mineMaterial = new Material(pixelShader, vertexShader, mineSRV, samplerState);

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"MainMenu.png", 0, &defaultSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"MainMenu_Start.png", 0, &startSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"MainMenu_Instructions.png", 0, &instructSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"MainMenu_Scores.png", 0, &scoreSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"GameOverScreen.png", 0, &overSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"water.png", 0, &waterSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"water-normal-map.png", 0, &waterNormalMapSRV);
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"pebbleDiffuse.jpg", 0, &waterSRV);
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"pebbleNormal.jpg", 0, &waterNormalMapSRV);

	//skybox
	
	//DirectX::CreateWICTextureFromFileEx(device, deviceContext, L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	//DirectX::CreateWICTextureFromFile(device, deviceContextL"Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);

	//device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	//device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);

	material = new Material(pixelShader, vertexShader, srv, samplerState);
	waterMaterial = new Material(normalMapPixelShader, normalMapVertexShader, waterSRV, samplerState);
	startDefaultMaterial = new Material(pixelShader, vertexShader, defaultSRV, samplerState);
	startStartMaterial = new Material(pixelShader, vertexShader, startSRV, samplerState);
	startInstructMaterial = new Material(pixelShader, vertexShader, instructSRV, samplerState);
	startScoreMaterial = new Material(pixelShader, vertexShader, scoreSRV, samplerState);
	gameOverMaterial = new Material(pixelShader, vertexShader, overSRV, samplerState);


	// Create the game entities
	startScreen = new GameEntity(waterMesh, startDefaultMaterial);
	startScreen->SetPosition(XMFLOAT3(0.0f, -5.0f, 2.0f));
	startScreen->SetRotation(XMFLOAT3(-0.2f, 0.0f, 0.0f));
	startScreen->SetScale(XMFLOAT3(1.8f, 1.0f, 1.18f));
	startScreen->Update();


	// Create the static game entities

	//base
	entities.push_back(new GameEntity(base, baseMaterial));
	entities[0]->SetPosition(XMFLOAT3(-6.0f, 0.f, 2.5f));
	entities[0]->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
	entities[0]->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
	entities[0]->Update();

	//water
	entities.push_back(new GameEntity(waterMesh, waterMaterial));
	entities[1]->SetPosition(XMFLOAT3(5.0f, 1.0f, 1.0f));
	entities[1]->SetScale(XMFLOAT3(2, 2, 2));
	entities[1]->Update();

	//placement ship
	entities.push_back(new GameEntity(assaultBoat, assaultMaterial));
	entities[2]->SetRotation(XMFLOAT3(0.0f, 3.14f, 0.0f));
	entities[2]->SetPosition(XMFLOAT3(5.0f, 1.0f, 1.0f));
	entities[2]->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));

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


	//skybox stuff
	CreateSphere(10, 10);

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

	//Make all the meshes
	mesh1 = new Mesh("sphere.obj", device);

	mesh2 = new Mesh("cube.obj", device);

	mesh3 = new Mesh("Boat.obj", device);

	enemyBoat = new Mesh("BoatNew.obj", device);

	battleship = new Mesh("Battleship.obj", device);

	submarine = new Mesh("Sub.obj", device);

	assaultBoat = new Mesh("Assault.obj", device);

	torpedo = new Mesh("Torpedo.obj", device);

	bullet = new Mesh("Bullet.obj", device);

	tileMesh = new Mesh("tile.obj", device);
	//startMenu = new Mesh(vertices, 4, indices, 6, device);

	waterMesh = new Mesh("plain.obj", device);

	base = new Mesh("Base.obj", device);

	mine = new Mesh("Mine.obj", device);
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
			if (prevMousePos.x > 0 && prevMousePos.x < 640)
			{
				if (prevMousePos.y > 170 && prevMousePos.y < 245)
				{
					startScreen = new GameEntity(waterMesh, startStartMaterial);
					if (mouseDown)
					{
						state = Game;
					}
				}
				else if (prevMousePos.y > 325 && prevMousePos.y < 400)
				{
					startScreen = new GameEntity(waterMesh, startInstructMaterial);
				}
				else if (prevMousePos.y > 480 && prevMousePos.y < 555)
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
			if (GetAsyncKeyState('1') && 0x8000)
			{
				player.currentShip = 'a';
				player.nextShipCost = 5;
				entities[2]->setMesh(assaultBoat);
				entities[2]->setMaterial(assaultMaterial);
				entities[2]->SetRotation(XMFLOAT3(0.0f, 3.14f, 0.0f));
			}
			if (GetAsyncKeyState('2') && 0x8000)
			{
				player.currentShip = 's';
				player.nextShipCost = 10;
				entities[2]->setMesh(submarine);
				entities[2]->setMaterial(subMaterial);
				entities[2]->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
			}
			if (GetAsyncKeyState('3') && 0x8000)
			{
				player.currentShip = 'b';
				player.nextShipCost = 15;
				entities[2]->setMesh(battleship);
				entities[2]->setMaterial(battleMaterial);
				entities[2]->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
			}if (GetAsyncKeyState('4') && 0x8000)
			{
				player.currentShip = 'm';
				player.nextShipCost = 20;
				entities[2]->setMesh(mine);
				entities[2]->setMaterial(mineMaterial);
				entities[2]->SetRotation(XMFLOAT3(0.0f, -1.57f, 0.0f));
			}
			else
			{
				pauseKeyDown = false;
			}
			// Take input, update game logic, etc.
			enemy.spawnTimer++;
			if (enemy.kills >= enemy.killsForNextRound)
			{
				enemy.round++;
				enemy.kills = 0;
				enemy.killsForNextRound += 1;
			}

			//update static entities
			entities[2]->Update();

			if (enemy.spawnTimer * enemy.round > 3 * (1 / dt))
			{
				float r = rand() % 6 + 1;
				GridTile closest = grid->getTile(r * 10 - 1);
				XMFLOAT3 shipPos = closest.GetPosition();

				entities.push_back(new GameEntity(enemyBoat, enemyMaterial));
				enemy.ships.push_back(new Ship(entities[entities.size() - 1], 'e'));

				enemy.ships[enemy.ships.size() - 1]->shipEntity->SetPosition(shipPos);
				enemy.ships[enemy.ships.size() - 1]->shipEntity->Translate(XMFLOAT3(2, 0, 0));
				enemy.ships[enemy.ships.size() - 1]->speed = -1;
				enemy.ships[enemy.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
				enemy.ships[enemy.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, -1.57f, 0.0f));
				enemy.ships[enemy.ships.size() - 1]->shipEntity->Update();

				enemy.spawnTimer = 0;
			}
			//update player ships
			for (int i = 0; i < player.ships.size(); i++)
			{
					//shooting
					player.ships[i]->projectileTimer += 1;
					if (player.ships[i]->projectileTimer > (1 / dt) * player.ships[i]->shootRate)
					{
						if (player.ships[i]->shipType != 'm')
						{
							if (player.ships[i]->shipType == 'a' || player.ships[i]->shipType == 'b')
							{
								entities.push_back(new GameEntity(bullet, bulletMaterial));
								player.ships[i]->projectiles.push_back(new Projectile(entities[entities.size() - 1], 20));
								player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->speed = 10;
							}
							else if (player.ships[i]->shipType == 's')
							{
								entities.push_back(new GameEntity(torpedo, torpedoMaterial));
								player.ships[i]->projectiles.push_back(new Projectile(entities[entities.size() - 1], 35));
								player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->speed = 4;
							}
							player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->SetPosition(player.ships[i]->shipEntity->GetPosition());
							player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
							player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->SetRotation(XMFLOAT3(0.0f, 0.0f, -1.57f));
							player.ships[i]->projectiles[player.ships[i]->projectiles.size() - 1]->projectileEntity->Update();

							player.ships[i]->projectileTimer = 0;
						}
						else if (player.ships[i]->shipType == 'm')
						{
							player.resources++;
							player.ships[i]->projectileTimer = 0;
						}
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
						break;
					}

					//check collisions with enemies
					for (int k = 0; k < enemy.ships.size(); k++)
					{
						BoundingBox tempBB = enemy.ships[k]->shipEntity->GetBoundingBox();
						BoundingBox tempBBP = player.ships[i]->projectiles[j]->projectileEntity->GetBoundingBox();
						if (tempBB.IsColliding(tempBBP))
						{
							enemy.ships[k]->health -= player.ships[i]->projectiles[j]->damage;
							player.ships[i]->projectiles.erase(player.ships[i]->projectiles.begin() + j);
							break;
						}
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
					player.health -= 10;
				}
				if (enemy.ships[i]->health <= 0)
				{
					enemy.kills++;
					enemy.ships.erase(enemy.ships.begin() + i);
				}
				//see if the enemy has collided with a player ship
				for (int k = 0; k < player.ships.size(); k++)
				{
					BoundingBox tempBB = enemy.ships[i]->shipEntity->GetBoundingBox();
					BoundingBox tempBBP = player.ships[k]->shipEntity->GetBoundingBox();
					if (tempBB.IsColliding(tempBBP))
					{
						enemy.ships.erase(enemy.ships.begin() + i);
						player.ships.erase(player.ships.begin() + k);
						break;
					}
				}
			}
			camera->Update();

			//lose game condition
			if (player.health <= 0)
			{
				startScreen->setMaterial(gameOverMaterial);
				state = Over;
			}
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
			/*
			startScreen = new GameEntity(waterMesh, gameOverMaterial);
			startScreen->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
			startScreen->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
			startScreen->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
			startScreen->Update();
			//camera->SetDirection(XMFLOAT3(0.0f, 0.0f, 0.0f));
			camera->SetRotation(180, 0);
			//camera->Rotate(0, 310);
			camera->Update();
			*/
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

	//set up strings
	//resources
	std::wstring s = std::to_wstring(player.resources);
	std::wstring s2 = L"Resources: ";
	s = s2 + s;
	const wchar_t* text = s.c_str();

	//round
	std::wstring round = std::to_wstring(enemy.round);
	std::wstring round2 = L"Round: ";
	round = round2 + round;
	const wchar_t* roundText = round.c_str();

	//health
	std::wstring health = std::to_wstring(player.health);
	std::wstring health2 = L"Health: ";
	health = health2 + health;
	const wchar_t* healthText = health.c_str();


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

		//draw the placement ship
		entities[2]->Draw(*deviceContext, *camera);

		//text
		m_spriteBatch->Begin();
		m_font->DrawString(m_spriteBatch.get(), healthText, XMFLOAT2(0, 0));
		m_font->DrawString(m_spriteBatch.get(), text, XMFLOAT2(350, 0));
		m_font->DrawString(m_spriteBatch.get(), roundText, XMFLOAT2(800, 0));
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
		/*
		XMFLOAT2 m_fontPos;
		m_fontPos.x = windowWidth / 2.f;
		m_fontPos.y = windowHeight / 2.f;

		m_spriteBatch->Begin();
		const wchar_t* output = L"Game Over";

		m_font->DrawString(m_spriteBatch.get(), output,
			m_fontPos, Colors::White, 0.f, XMFLOAT2(150, 0));

		m_spriteBatch->End();
		*/
		startScreen->Draw(*deviceContext, *camera);
		break;
	}

	
	// Present the buffer
	//  - Puts the stuff on the screen
	//  - Do this EXACTLY once per frame
	//  - Always at the end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

void MyDemoGame::CreateSphere(int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3)*(LongLines)* 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].Position.x = 0.0f;
	vertices[0].Position.y = 0.0f;
	vertices[0].Position.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		XMMATRIX Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			XMMATRIX Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].Position.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].Position.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].Position.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].Position.x = 0.0f;
	vertices[NumSphereVertices - 1].Position.y = 0.0f;
	vertices[NumSphereVertices - 1].Position.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];

	//???
	//hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);
	device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i*LongLines + j + 1;
			indices[k + 1] = i*LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i*LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}

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

	if (player.resources >= player.nextShipCost)
	{
		GridTile* closest = grid->GetNearestTile(prevMousePos.x, prevMousePos.y, 1280, 720, camera);
		XMFLOAT3 shipPos = closest->GetPosition();

		//make a new ship
		//determine which ship type to make
		if (player.currentShip == 'm' && (grid->lastIndexUsed % 10 == 0))
		{
			entities.push_back(new GameEntity(mine, mineMaterial));
			player.ships.push_back(new Ship(entities[entities.size() - 1], 'm'));

			player.ships[player.ships.size() - 1]->shipEntity->SetPosition(shipPos);
			player.ships[player.ships.size() - 1]->speed = 0;
			player.ships[player.ships.size() - 1]->shootRate = 2;
			player.ships[player.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
			player.ships[player.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, -1.57f, 0.0f));
			player.numShips--;
			player.ships[player.ships.size() - 1]->shipEntity->Update();

			//use up resources to buy ship
			player.resources -= player.ships[player.ships.size() - 1]->cost;
		}
		if (player.currentShip == 'a' && (grid->lastIndexUsed % 10 != 0))
		{
			entities.push_back(new GameEntity(assaultBoat, assaultMaterial ));
			player.ships.push_back(new Ship(entities[entities.size() - 1], 'a'));

			player.ships[player.ships.size() - 1]->shipEntity->SetPosition(shipPos);
			player.ships[player.ships.size() - 1]->speed = 0;
			player.ships[player.ships.size() - 1]->shootRate = 1;
			player.ships[player.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
			player.ships[player.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, 3.14f, 0.0f));
			player.numShips--;
			player.ships[player.ships.size() - 1]->shipEntity->Update();

			//use up resources to buy ship
			player.resources -= player.ships[player.ships.size() - 1]->cost;
		}
		else if (player.currentShip == 's' && (grid->lastIndexUsed % 10 != 0))
		{
			entities.push_back(new GameEntity(submarine, subMaterial));
			player.ships.push_back(new Ship(entities[entities.size() - 1], 's'));

			player.ships[player.ships.size() - 1]->shipEntity->SetPosition(shipPos);
			player.ships[player.ships.size() - 1]->speed = 0;
			player.ships[player.ships.size() - 1]->shootRate = 1.5;
			player.ships[player.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
			player.ships[player.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
			player.numShips--;
			player.ships[player.ships.size() - 1]->shipEntity->Update();

			//use up resources to buy ship
			player.resources -= player.ships[player.ships.size() - 1]->cost;
		}
		else if (player.currentShip == 'b' && (grid->lastIndexUsed % 10 != 0))
		{
			entities.push_back(new GameEntity(battleship, battleMaterial));
			player.ships.push_back(new Ship(entities[entities.size() - 1], 'b'));

			player.ships[player.ships.size() - 1]->shipEntity->SetPosition(shipPos);
			player.ships[player.ships.size() - 1]->speed = 0;
			player.ships[player.ships.size() - 1]->shootRate = 0.5f;
			player.ships[player.ships.size() - 1]->shipEntity->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
			player.ships[player.ships.size() - 1]->shipEntity->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
			player.numShips--;
			player.ships[player.ships.size() - 1]->shipEntity->Update();

			//use up resources to buy ship
			player.resources -= player.ships[player.ships.size() - 1]->cost;
		}
	}
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

	GridTile* closest = grid->GetNearestTile(prevMousePos.x, prevMousePos.y, 1280, 720, camera);
	XMFLOAT3 shipPos = closest->GetPosition();
	
	entities[2]->SetPosition(shipPos);
}
#pragma endregion