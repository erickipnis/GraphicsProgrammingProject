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

	if (enemyBoat != nullptr)
	{
		delete enemyBoat;
		enemyBoat = nullptr;
	}

	if (battleship != nullptr)
	{
		delete battleship;
		battleship = nullptr;
	}

	if (assaultBoat != nullptr)
	{
		delete assaultBoat;
		assaultBoat = nullptr;
	}

	if (submarine != nullptr)
	{
		delete submarine;
		submarine = nullptr;
	}

	if (torpedo != nullptr)
	{
		delete torpedo;
		torpedo = nullptr;
	}

	if (bullet != nullptr)
	{
		delete bullet;
		bullet = nullptr;
	}

	if (tileMesh != nullptr)
	{
		delete tileMesh;
		tileMesh = nullptr;
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

	if (mine != nullptr)
	{
		delete mine;
		mine = nullptr;
	}

	if (base != nullptr)
	{
		delete base;
		base = nullptr;
	}

	if (fullScreenQuad != nullptr)
	{
		delete fullScreenQuad;
		fullScreenQuad = nullptr;
	}

	if (startScreen != nullptr)
	{
		delete startScreen;
		startScreen = nullptr;
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

	if (bulletMaterial != nullptr)
	{
		delete bulletMaterial;
		bulletMaterial = nullptr;
	}

	if (tileMaterial != nullptr)
	{
		delete tileMaterial;
		tileMaterial = nullptr;
	}

	if (torpedoMaterial != nullptr)
	{
		delete torpedoMaterial;
		torpedoMaterial = nullptr;
	}

	if (startDefaultMaterial != nullptr)
	{
		delete startDefaultMaterial;
		startDefaultMaterial = nullptr;
	}

	if (startStartMaterial != nullptr)
	{
		delete startStartMaterial;
		startStartMaterial = nullptr;
	}

	if (startInstructMaterial != nullptr)
	{
		delete startInstructMaterial;
		startInstructMaterial = nullptr;
	}

	if (startScoreMaterial != nullptr)
	{
		delete startScoreMaterial;
		startScoreMaterial = nullptr;
	}

	if (gameOverMaterial != nullptr)
	{
		delete gameOverMaterial;
		gameOverMaterial = nullptr;
	}

	if (battleMaterial != nullptr)
	{
		delete battleMaterial;
		battleMaterial = nullptr;
	}

	if (enemyMaterial != nullptr)
	{
		delete enemyMaterial;
		enemyMaterial = nullptr;
	}

	if (subMaterial != nullptr)
	{
		delete subMaterial;
		subMaterial = nullptr;
	}

	if (assaultMaterial != nullptr)
	{
		delete assaultMaterial;
		assaultMaterial = nullptr;
	}

	if (mineMaterial != nullptr)
	{
		delete mineMaterial;
		mineMaterial = nullptr;
	}

	if (baseMaterial != nullptr)
	{
		delete baseMaterial;
		baseMaterial = nullptr;
	}

	for (UINT i = 0; i < entities.size(); i++)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
	for (UINT i = 0; i < ships.size(); i++)
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

	if (shadowCamera != nullptr)
	{
		delete shadowCamera;
		shadowCamera = nullptr;
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

	if (shadowPixelShader != nullptr)
	{
		delete shadowPixelShader;
		shadowPixelShader = nullptr;
	}

	if (shadowVertexShader != nullptr)
	{
		delete shadowVertexShader;
		shadowVertexShader = nullptr;
	}

	if (simpleVertShader != nullptr)
	{
		delete simpleVertShader;
		simpleVertShader = nullptr;
	}

	if (grid != nullptr)
	{
		delete grid;
		grid = nullptr;
	}
	if (smokeParticleEmitter != nullptr)
	{
		delete smokeParticleEmitter;
		smokeParticleEmitter = nullptr;
	}
	if (quadVS != nullptr)
	{
		delete quadVS;
		quadVS = nullptr;
	}

	if (quadPS != nullptr)
	{
		delete quadPS;
		quadPS = nullptr;
	}

	samplerState->Release();
	shadowMap->Release();
	depthStencilView2->Release();
	shadowResourceView->Release();
	comparisonSampler->Release();
	drawingRenderState->Release();
	shadowRenderState->Release();

	//waterReflectionTexture->Release();
	//waterReflectionRTV->Release();
	//waterReflectionSRV->Release();
	

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
	directionalLight.Direction = XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f);
	directionalLight.Position = XMFLOAT4(-15.0f, 10.0f, 2.0f, 1.0f);

	/*directionalLight2.AmbientColor = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	directionalLight2.DiffuseColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directionalLight2.Direction = XMFLOAT3(-1.0f, 1.0f, -1.0f);*/

	// Set up camera-related matrices
	InitializeCameraMatrices();

	// Make sure DirectX initializes properly
	if( !DirectXGame::Init() )
		return false;

	DirectX::CreateWICTextureFromFile(device, deviceContext, L"water-normal-map.png", 0, &waterNormalMapSRV);

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	// Create the necessary DirectX buffers to draw something
	CreateGeometryBuffers();

	//Init Shadows
	InitializeShadows();
	// create sampler state and resource view for materials
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* tileSRV;

	// Initialize sampler description
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);

	// Initialize sampler description with no texture wrapping
	ZeroMemory(&noWrapSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	noWrapSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	noWrapSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	noWrapSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	noWrapSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	noWrapSamplerDesc.MaxLOD = 0;
	noWrapSamplerDesc.MinLOD = 0;

	device->CreateSamplerState(&noWrapSamplerDesc, &noWrapSamplerState);

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	device->CreateDepthStencilState(&dsDesc, &depthStencilState);
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	device->CreateDepthStencilState(&dsDesc, &depthStencilStateParticles);

	// Testing Blend States for transparency
	ID3D11BlendState* blendState;
	// Blend States for transparency
	ID3D11BlendState* blendStateAlphaAdd;
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&blendDesc, &blendStateAlphaAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	ID3D11BlendState* blendStateColorAdd;
	device->CreateBlendState(&blendDesc, &blendStateColorAdd);


	ID3D11ShaderResourceView* waterSRV;
	//ID3D11ShaderResourceView* waterSRV;
	ID3D11ShaderResourceView* defaultSRV;
	ID3D11ShaderResourceView* startSRV;
	ID3D11ShaderResourceView* overSRV;
	ID3D11ShaderResourceView* instructSRV;
	ID3D11ShaderResourceView* scoreSRV;

	ID3D11ShaderResourceView* assaultSRV;
	ID3D11ShaderResourceView* battleshipSRV;
	ID3D11ShaderResourceView* submarineSRV;
	ID3D11ShaderResourceView* enemySRV;
	// Particle SRVs
	ID3D11ShaderResourceView* pSmokeSRV;
	ID3D11ShaderResourceView* pFireSRV;

	ID3D11ShaderResourceView* bulletSRV;
	ID3D11ShaderResourceView* torpedoSRV;
	ID3D11ShaderResourceView* baseSRV;
	ID3D11ShaderResourceView* mineSRV;


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
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"water.png", 0, &waterSRV);
	
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"Particle_Smoke.png", 0, &pSmokeSRV);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"Particle_Fire.png", 0, &pFireSRV);
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"pebbleDiffuse.jpg", 0, &waterSRV);
	//DirectX::CreateWICTextureFromFile(device, deviceContext, L"pebbleNormal.jpg", 0, &waterNormalMapSRV);

	//skybox
	
	//DirectX::CreateWICTextureFromFileEx(device, deviceContext, L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	//DirectX::CreateWICTextureFromFile(device, deviceContextL"Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);

	//device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	//device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);

	InitializeScreenRenderToTexture();

	material = new Material(pixelShader, vertexShader, srv, samplerState);

	waterMaterial = new Material(normalMapPixelShader, normalMapVertexShader, screenShaderResourceView, noWrapSamplerState);
	startDefaultMaterial = new Material(pixelShader, vertexShader, defaultSRV, samplerState);
	startStartMaterial = new Material(pixelShader, vertexShader, startSRV, samplerState);
	startInstructMaterial = new Material(pixelShader, vertexShader, instructSRV, samplerState);
	startScoreMaterial = new Material(pixelShader, vertexShader, scoreSRV, samplerState);
	gameOverMaterial = new Material(pixelShader, vertexShader, overSRV, samplerState);

	// Particle Materials
	pSmokeMaterial = new Material(pSmokePixelShader, particleVertexShader, pSmokeSRV, samplerState);
	pFireMaterial = new Material(pSmokePixelShader, particleVertexShader, pFireSRV, samplerState);


	// Create the game entities
	startScreen = new GameEntity(waterMesh, startDefaultMaterial);
	startScreen->SetPosition(XMFLOAT3(0.0f, -5.0f, 2.0f));
	startScreen->SetRotation(XMFLOAT3(-0.2f, 0.0f, 0.0f));
	startScreen->SetScale(XMFLOAT3(1.8f, 1.0f, 1.18f));
	startScreen->Update();


	// Create the game entities
	//entities.push_back(new GameEntity(mesh2, material));
	//entities[0]->SetPosition(XMFLOAT3(-5.0f, -1.0f, 1.0f));
	//entities[0]->Update();

	// Create water entity

	// Create the static game entities

	//base
	entities.push_back(new GameEntity(base, baseMaterial));
	entities[0]->SetPosition(XMFLOAT3(-5.5f, 0.f, 2.5f));
	entities[0]->SetRotation(XMFLOAT3(0.0f, 1.57f, 0.0f));
	entities[0]->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
	entities[0]->Update();

	//water
	entities.push_back(new GameEntity(waterMesh, waterMaterial));
	entities[1]->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
	entities[1]->SetPosition(XMFLOAT3(5.0f, 0.01f, 2.0f));
	entities[1]->Update();

	//placement ship
	entities.push_back(new GameEntity(assaultBoat, assaultMaterial));
	entities[2]->SetRotation(XMFLOAT3(0.0f, 3.14f, 0.0f));
	entities[2]->SetPosition(XMFLOAT3(5.0f, 1.0f, 1.0f));
	entities[2]->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));		

	// Set up world matrix
	// In an actual game, each object will need one of these and they should
	//  update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	
	// Set up the grid
	grid = new Grid(6, 10, 1.0f, XMFLOAT3(-4.5f, 0.0f, -0.5f), tileMesh, tileMaterial, blendStateAlphaAdd);

	//Make the player
	player = Player();

	//Make the enemy class
	enemy = Enemy();

	m_font.reset(new SpriteFont(device, L"myfile.spritefont"));
	m_spriteBatch.reset(new SpriteBatch(deviceContext));
	
	// Create a particle emiiter description to pass into the particle emitters
	EMITTER_DESC emitDesc;
	emitDesc.blendState = blendStateAlphaAdd;
	emitDesc.device = device;
	emitDesc.emitRate = 0.01f;
	emitDesc.lifeSpan = 3.0f;
	emitDesc.fadeTime = 1.0f;
	emitDesc.loop = true;
	emitDesc.material = pSmokeMaterial;
	emitDesc.maxAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	emitDesc.minAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	emitDesc.maxDistance = 0.25f;
	emitDesc.maxParticles = 2000;
	emitDesc.maxSize = 1.0f;
	emitDesc.minSize = 0.5f;
	emitDesc.maxVelocity = XMFLOAT3(0.3f, 1.0f, 0.3f);
	emitDesc.minVelocity = XMFLOAT3(-0.3f, 2.0f, -0.3f);
	emitDesc.position = XMFLOAT3(0.0f, 1.0f, 3.0f);
	
	// initialize particle emitters
	smokeParticleEmitter = new ParticleEmitter(emitDesc);

	emitDesc.material = pFireMaterial;
	emitDesc.blendState = blendStateColorAdd;
	emitDesc.position = XMFLOAT3(0.0f, 0.0f, 3.0f);

	emitDesc.emitRate = 0.01f;
	emitDesc.maxParticles = 500;
	emitDesc.lifeSpan = 3.0f;
	emitDesc.loop = true;

	fireParticleEmitter2 = new ParticleEmitter(emitDesc);

	emitDesc.position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	fireParticleEmitter = new ParticleEmitter(emitDesc);

	//make sure we draw tris correctly
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//skybox stuff
	//CreateSphere(10, 10);

	// Successfully initialized
	return true;


}

void MyDemoGame::InitializeShadows()
{
	//Depth Buffer
	D3D11_TEXTURE2D_DESC shadowMapDesc;
	ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
	shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	//shadowMapDesc.Height = static_cast<UINT>(m_shadowMapDimension);
	//shadowMapDesc.Width = static_cast<UINT>(m_shadowMapDimension);
	shadowMapDesc.Height = 1024;
	shadowMapDesc.Width = 1024;
	HR(device->CreateTexture2D(
		&shadowMapDesc,
		nullptr,
		&shadowMap
		));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	device->CreateDepthStencilView(
		shadowMap,
		&depthStencilViewDesc,
		&depthStencilView2
		);

	device->CreateShaderResourceView(
		shadowMap,
		&shaderResourceViewDesc,
		&shadowResourceView
		);

	//Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Depth test parameters
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	//Comparison State
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	// Point filtered shadows can be faster, and may be a good choice when
	// rendering on hardware with lower feature levels. This sample has a
	// UI option to enable/disable filtering so you can see the difference
	// in quality and speed.

	HR(device->CreateSamplerState(
		&comparisonSamplerDesc,
		&comparisonSampler
		));

	//Render States
	D3D11_RASTERIZER_DESC drawingRenderStateDesc;
	ZeroMemory(&drawingRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
	drawingRenderStateDesc.CullMode = D3D11_CULL_BACK;
	drawingRenderStateDesc.FillMode = D3D11_FILL_SOLID;
	drawingRenderStateDesc.DepthClipEnable = true; // Feature level 9_1 requires DepthClipEnable == true
	HR(device->CreateRasterizerState(
		&drawingRenderStateDesc,
		&drawingRenderState
		));

	D3D11_RASTERIZER_DESC shadowRenderStateDesc;
	ZeroMemory(&shadowRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
	shadowRenderStateDesc.CullMode = D3D11_CULL_FRONT;
	shadowRenderStateDesc.FillMode = D3D11_FILL_SOLID;
	shadowRenderStateDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(
		&shadowRenderStateDesc,
		&shadowRenderState
		));


	//D3D11_BUFFER_DESC viewProjectionConstantBufferDesc;
	//viewProjectionConstantBufferDesc.

	//Constant Buffers
	//HR(device->CreateBuffer(
	//	&viewProjectionConstantBufferDesc,
	//	nullptr,
	//	&lightViewProjectionBuffer
	//	));


	//Viewport
	ZeroMemory(&shadowViewport, sizeof(D3D11_VIEWPORT));
	shadowViewport.Height = 1024;
	shadowViewport.Width = 1024;
	shadowViewport.MinDepth = 0.f;
	shadowViewport.MaxDepth = 1.f;

	//
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::CreateGeometryBuffers()
{
	Vertex quad[4];
	quad[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	quad[1].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	quad[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	quad[3].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);

	quad[0].UV = XMFLOAT2(0.0f, 0.0f);
	quad[1].UV = XMFLOAT2(0.0f, 1.0f);
	quad[2].UV = XMFLOAT2(1.0f, 1.0f);
	quad[3].UV = XMFLOAT2(1.0f, 0.0f);

	unsigned int indices[6];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	fullScreenQuad = new Mesh(quad, 4, indices, 6, device);

	Vertex vertices[4] = 
	{
		{ XMFLOAT3(0.5, 0.0f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5, 0.0f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-0.5, 0.0f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }
	};


	//UINT indices[] = { 0, 1, 2, 2, 3, 0 };
	//mesh1 = new Mesh(vertices, 4, indices, 6, device);

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

	// Initialize vertex shaders
	vertexShader = new SimpleVertexShader(device, deviceContext);
	normalMapVertexShader = new SimpleVertexShader(device, deviceContext);
	quadVS = new SimpleVertexShader(device, deviceContext);
	refractVS = new SimpleVertexShader(device, deviceContext);
	shadowVertexShader = new SimpleVertexShader(device, deviceContext);
	simpleVertShader = new SimpleVertexShader(device, deviceContext);

	particleVertexShader = new SimpleVertexShader(device, deviceContext);
	
	// Initialize pixel shaders
	pixelShader = new SimplePixelShader(device, deviceContext);
	normalMapPixelShader = new SimplePixelShader(device, deviceContext);
	quadPS = new SimplePixelShader(device, deviceContext);	
	refractPS = new SimplePixelShader(device, deviceContext);
	shadowPixelShader = new SimplePixelShader(device, deviceContext);
	pSmokePixelShader = new SimplePixelShader(device, deviceContext);

	// Load vertex shader files
	vertexShader->LoadShaderFile(L"VertexShader.cso");
	normalMapVertexShader->LoadShaderFile(L"NormalMapVertexShader.cso");
	quadVS->LoadShaderFile(L"QuadVertexShader.cso");
	refractVS->LoadShaderFile(L"RefractVertexShader.cso");
	shadowVertexShader->LoadShaderFile(L"ShadowVertexShader.cso");
	simpleVertShader->LoadShaderFile(L"SimpleVertexShader.cso");

	// Load pixel shader files
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	normalMapPixelShader->LoadShaderFile(L"NormalMapPixelShader.cso");
	quadPS->LoadShaderFile(L"QuadPixelShader.cso");
	refractPS->LoadShaderFile(L"RefractPixelShader.cso");
	shadowPixelShader->LoadShaderFile(L"ShadowPixelShader.cso");

	normalMapPixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	shadowPixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	pixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight));

	/*pixelShader->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight));*/

	/*normalMapPixelShader->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight));*/

	normalMapPixelShader->SetShaderResourceView("waterNormalMap", waterNormalMapSRV);
	normalMapPixelShader->SetShaderResourceView("diffuseTexture", screenShaderResourceView);

	normalMapPixelShader->SetFloat3("camPos", XMFLOAT3(0.0f, 10.0f, 0.0f));
	particleVertexShader->LoadShaderFile(L"ParticleVertexShader.cso");
	pSmokePixelShader->LoadShaderFile(L"ParticlePixelShader.cso");

}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	camera = new Camera();
	camera->SetDirection(XMFLOAT3(0.0f, -0.5f, 0.1f));
	//camera->Rotate(0, 310);
	camera->Update();

	shadowCamera = new Camera();
	shadowCamera->SetDirection(XMFLOAT3(directionalLight.Direction.x, directionalLight.Direction.y, directionalLight.Direction.z));
	shadowCamera->SetPosition(XMFLOAT3(directionalLight.Position.x, directionalLight.Position.y, directionalLight.Position.z));
	shadowCamera->Update();
}

void MyDemoGame::InitializeScreenRenderToTexture()
{
	// Set up the water reflection render texture
	D3D11_TEXTURE2D_DESC screenRenderTextureDesc;
	screenRenderTextureDesc.ArraySize = 1;
	screenRenderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	screenRenderTextureDesc.CPUAccessFlags = 0;
	screenRenderTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	screenRenderTextureDesc.Height = windowHeight;
	screenRenderTextureDesc.MipLevels = 1;
	screenRenderTextureDesc.MiscFlags = 0;
	screenRenderTextureDesc.SampleDesc.Count = 1;
	screenRenderTextureDesc.SampleDesc.Quality = 0;
	screenRenderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	screenRenderTextureDesc.Width = windowWidth;
	device->CreateTexture2D(&screenRenderTextureDesc, 0, &screenRenderTexture);

	// Set up the water reflection render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = screenRenderTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	device->CreateRenderTargetView(screenRenderTexture, &renderTargetViewDesc, &screenRenderTargetView);

	// Set up the water reflection shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC screenShaderResourceViewDesc;
	ZeroMemory(&screenShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	screenShaderResourceViewDesc.Format = screenRenderTextureDesc.Format;
	screenShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	screenShaderResourceViewDesc.Texture2D.MipLevels = 1;
	screenShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(screenRenderTexture, &screenShaderResourceViewDesc, &screenShaderResourceView);
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
			else
			{
				pauseKeyDown = false;
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
				float r = rand() % 6 + 1.0f;
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
			for (UINT i = 0; i < player.ships.size(); i++)
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
				for (UINT j = 0; j < player.ships[i]->projectiles.size(); j++)
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
					for (UINT k = 0; k < enemy.ships.size(); k++)
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
			for (UINT i = 0; i < enemy.ships.size(); i++)
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
				for (UINT k = 0; k < player.ships.size(); k++)
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

			smokeParticleEmitter->Update(dt);
			fireParticleEmitter->Update(dt);
			fireParticleEmitter2->Update(dt);
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
	const float waterColor[4] = { 0.0f, 0.46667f, 0.74510f, 1.0f };

	// Set up input primitive topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	UINT stride;
	UINT offset;
	ID3D11Buffer* vb;
	ID3D11ShaderResourceView* unset[1] = { 0 };

	switch (state)
	{
	case Start:

		startScreen->Draw(*deviceContext, *camera);

		break;

	case Game:
		RenderShadowMap();

		//deviceContext->OMSetDepthStencilState(depthStencilState, 1);
		deviceContext->ClearRenderTargetView(renderTargetView, gameColor);
		//entities[1]->setMaterial(waterMaterial);

		// Set render target view to render to texture in memory
		deviceContext->OMSetRenderTargets(1, &screenRenderTargetView, depthStencilView);

		deviceContext->RSSetState(drawingRenderState);
		deviceContext->OMSetDepthStencilState(depthStencilState, 1);
		deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
		deviceContext->RSSetViewports(1, &viewport);

		// Clear the buffer (erases what's on the screen)
		deviceContext->ClearRenderTargetView(screenRenderTargetView, waterColor);

		//draw base
		entities[0]->Draw(*deviceContext, *camera);
		//draw all of the player ships
		for (UINT i = 0; i < player.ships.size(); i++)
		{
			player.ships[i]->shipEntity->Draw(*deviceContext, *camera, *shadowCamera, shadowPixelShader, shadowVertexShader, shadowResourceView, comparisonSampler);
		}
		//draw all of the projectiles
		for (UINT i = 0; i < player.ships.size(); i++)
		{
			for (UINT j = 0; j < player.ships[i]->projectiles.size(); j++)
			{
				player.ships[i]->projectiles[j]->projectileEntity->Draw(*deviceContext, *camera, *shadowCamera, shadowPixelShader, shadowVertexShader, shadowResourceView, comparisonSampler);
			}
		}
		//draw all of the enemy ships
		for (UINT i = 0; i < enemy.ships.size(); i++)
		{
			enemy.ships[i]->shipEntity->Draw(*deviceContext, *camera, *shadowCamera, shadowPixelShader, shadowVertexShader, shadowResourceView, comparisonSampler);
		}

		// Draw the grid
		grid->Draw(*deviceContext, *camera);

		// Go back to the regular "back buffer"
		deviceContext->OMSetRenderTargets(1, &renderTargetView, 0);
		deviceContext->ClearRenderTargetView(renderTargetView, gameColor);



		quadVS->SetShader();
		quadPS->SetSamplerState("basicSampler", samplerState);
		quadPS->SetShaderResourceView("diffuseTexture", screenShaderResourceView);
		quadPS->SetShader();

		// Set quad data
		stride = sizeof(Vertex);
		offset = 0;
		vb = fullScreenQuad->GetVertexBuffer();
		deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		deviceContext->IASetIndexBuffer(fullScreenQuad->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// Draw the quad
		deviceContext->DrawIndexed(fullScreenQuad->GetIndexCount(), 0, 0);

		// Re-enable the back buffer's depth buffer
		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		// Send data to normal vertex for water refraction calculations
		//refractVS->SetMatrix4x4("view", viewMatrix);
		//refractVS->SetMatrix4x4("projection", projectionMatrix);
		////refractVS->SetFloat3("camPos", XMFLOAT3(0, 0, -5));

		//normalMapVertexShader->SetMatrix4x4("view", viewMatrix);
		//normalMapVertexShader->SetMatrix4x4("projection", projectionMatrix);

		//// Send data to pixel shader for water refraction calculations
		//refractPS->SetFloat4("ambientColor", XMFLOAT4(0.5f, 0.5f, 0.5f, 1));
		//refractPS->SetFloat4("lightColor", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		//refractPS->SetFloat3("lightDir", XMFLOAT3(-1.0f, -1.0f, 0.0f));
		//refractPS->SetFloat3("lightPos", XMFLOAT3(0.0f, 3.0f, 0.0f));
		//refractPS->SetFloat3("camPos", XMFLOAT3(0.0f, 0.0f, -5.0f));
		//refractPS->SetShaderResourceView("normalTexture", waterNormalMapSRV);

		// Draw the water with refraction
		normalMapPixelShader->SetShaderResourceView("waterNormalMap", waterNormalMapSRV);
		normalMapPixelShader->SetShaderResourceView("diffuseTexture", screenShaderResourceView);

		normalMapPixelShader->SetFloat3("camPos", XMFLOAT3(0.0f, 10.0f, 0.0f));
		entities[1]->Draw(*deviceContext, *camera);

		//draw the placement ship
		entities[2]->Draw(*deviceContext, *camera, *shadowCamera, shadowPixelShader, shadowVertexShader, shadowResourceView, comparisonSampler);
		// Draw the grid
		grid->Draw(*deviceContext, *camera, *shadowCamera, shadowPixelShader, shadowVertexShader, shadowResourceView, comparisonSampler);
		deviceContext->OMSetDepthStencilState(depthStencilStateParticles, 0);
		// particles
		smokeParticleEmitter->Draw(*deviceContext, *camera);
		fireParticleEmitter->Draw(*deviceContext, *camera);
		fireParticleEmitter2->Draw(*deviceContext, *camera);
		//deviceContext->OMSetDepthStencilState(depthStencilState, 1);
		//text
		m_spriteBatch->Begin();
		m_font->DrawString(m_spriteBatch.get(), healthText, XMFLOAT2(0, 0));
		m_font->DrawString(m_spriteBatch.get(), text, XMFLOAT2(350, 0));
		m_font->DrawString(m_spriteBatch.get(), roundText, XMFLOAT2(800, 0));
		m_spriteBatch->End();

		deviceContext->OMSetDepthStencilState(depthStencilState, 1);
		
		// Unset the shader resource
		deviceContext->PSSetShaderResources(0, 1, unset);


		


		break;

	case Paused:
		deviceContext->ClearRenderTargetView(renderTargetView, pauseColor);
		for (UINT i = 0; i < entities.size(); i++)
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

/*
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
*/

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

void MyDemoGame::RenderShadowMap()
{
	shadowCamera->SetPosition(XMFLOAT3(directionalLight.Position.x, directionalLight.Position.y, directionalLight.Position.z));
	shadowCamera->SetDirection(XMFLOAT3(directionalLight.Direction.x, directionalLight.Direction.y, directionalLight.Direction.z));
	//shadowCamera->SetDirection(XMFLOAT3(0, -1, 0));
	shadowCamera->Update();
	//shadowCamera->UpdateProjection(AspectRatio());
	shadowCamera->UpdateProjection(1.0f);

	deviceContext->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue);
	deviceContext->ClearDepthStencilView(depthStencilView2, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	ID3D11RenderTargetView* rt[1] = { 0 };
	deviceContext->OMSetRenderTargets(
		1,
		rt,
		depthStencilView2
		);

	deviceContext->RSSetState(shadowRenderState);
	deviceContext->RSSetViewports(1, &shadowViewport);

	//DRAW
	grid->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);

	//draw water and base
	entities[0]->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);
	entities[1]->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);
	entities[2]->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);
	//draw all of the player ships
	for (UINT i = 0; i < player.ships.size(); i++)
	{
		player.ships[i]->shipEntity->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);
	}
	//draw all of the projectiles
	for (UINT i = 0; i < player.ships.size(); i++)
	{
		for (UINT j = 0; j < player.ships[i]->projectiles.size(); j++)
		{
			player.ships[i]->projectiles[j]->projectileEntity->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);
		}
	}
	//draw all of the enemy ships
	for (UINT i = 0; i < enemy.ships.size(); i++)
	{
		enemy.ships[i]->shipEntity->ShadowMapDraw(*deviceContext, *shadowCamera, simpleVertShader);
	}
}