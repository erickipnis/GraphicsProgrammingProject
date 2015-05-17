#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "Vertex.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Grid.h"
#include "WICTextureLoader.h"
#include <vector>
#include "Ship.h"
#include "Player.h"
#include "Enemy.h"
#include <time.h>       /* time */
#include <SpriteFont.h>
#include <SpriteBatch.h>

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;


// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VertexShaderConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct DirectionalLight
{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT4 Direction;
	XMFLOAT4 Position;
};

// Demo class which extends the base DirectXGame class
class MyDemoGame : public DirectXGame
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void InitializeCameraMatrices();
	void InitializeScreenRenderToTexture();
	void InitializeShadows();
	void RenderShadowMap();

private:
	// The matrices to go from model space
	// to screen space
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Mesh object pointers
	Mesh* mesh1;
	Mesh* mesh2;
	Mesh* mesh3;

	Mesh* enemyBoat;

	Mesh* battleship;
	Mesh* assaultBoat;
	Mesh* submarine;

	Mesh* torpedo;
	Mesh* bullet;

	Mesh* tileMesh;
	Mesh* startMenu;

	Mesh* waterMesh;

	// Full screen quad
	Mesh* fullScreenQuad;
	SimpleVertexShader* quadVS;
	SimplePixelShader* quadPS;

	Mesh* base;
	Mesh* mine;

	GameEntity* startScreen;

	// vector of entities
	std::vector<GameEntity*> entities;

	//vector of ships
	std::vector<Ship*> ships;

	// SimpleShaders
	SimplePixelShader* pixelShader;
	SimplePixelShader* normalMapPixelShader;
	SimplePixelShader* shadowPixelShader;
	SimpleVertexShader* simpleVertShader;
	SimpleVertexShader* vertexShader;
	SimpleVertexShader* normalMapVertexShader;
	SimpleVertexShader* shadowVertexShader;

	// The camera
	Camera* camera;
	Camera* shadowCamera; //Used for rendering from the light view

	// Materials
	Material* material;
	Material* tileMaterial;

	Material* bulletMaterial;
	Material* torpedoMaterial;

	Material* startDefaultMaterial;
	Material* startStartMaterial;
	Material* startInstructMaterial;
	Material* startScoreMaterial;
	Material* gameOverMaterial;

	// Water materials
	Material* waterMaterial;

	//ship materials
	Material* assaultMaterial;
	Material* subMaterial;
	Material* battleMaterial;
	Material* enemyMaterial;

	Material* baseMaterial;
	Material* mineMaterial;

	// Lighting
	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;

	//GameState enum
	enum GameState { Start, Game, Paused, Over };
	GameState state;
	bool pauseKeyDown;
	bool mouseDown;

	// Grid
	Grid* grid;

	//Player - will have ships, health, resources, etc. 
	Player player;

	//Enemy - will have ships, spawn rate, etc. 
	Enemy enemy;

	//text stuff
	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// Water Reflection 
	ID3D11Texture2D* screenRenderTexture;
	ID3D11RenderTargetView* screenRenderTargetView;
	ID3D11ShaderResourceView* screenShaderResourceView;

	// Water Refraction
	ID3D11ShaderResourceView* waterNormalMapSRV;
	SimpleVertexShader* refractVS;
	SimplePixelShader* refractPS;

	// Texture-related variables
	ID3D11SamplerState* samplerState;
	ID3D11SamplerState* noWrapSamplerState;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_SAMPLER_DESC noWrapSamplerDesc;

	// Depth Stencil State
	ID3D11DepthStencilState* depthStencilState;

	//Shadows
	//ID3D11Buffer* shadowResource;
	ID3D11Texture2D* shadowMap;
	ID3D11DepthStencilView* depthStencilView2;
	ID3D11ShaderResourceView* shadowResourceView;
	ID3D11SamplerState* comparisonSampler;
	ID3D11RasterizerState* drawingRenderState;
	ID3D11RasterizerState* shadowRenderState;
	//ID3D11Buffer* viewProjectionConstantBufferDesc;
	D3D11_VIEWPORT shadowViewport;
	float   shadowMapDimension;

	//ID3D11SamplerState* m_comparisonSampler_point;
	//ID3D11SamplerState* m_comparisonSampler_linear;
	//ID3D11SamplerState* m_linearSampler;

	//skybox stuff
	/*
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;

	void CreateSphere(int LatLines, int LongLines);
	*/
};