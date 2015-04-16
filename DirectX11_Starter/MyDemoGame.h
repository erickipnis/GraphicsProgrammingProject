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
	XMFLOAT3 Direction;
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

	Mesh* torpedo;

	Mesh* tileMesh;
	Mesh* startMenu;

	Mesh* waterMesh;

	GameEntity* startScreen;

	// vector of entities
	std::vector<GameEntity*> entities;

	//vector of ships
	std::vector<Ship*> ships;

	// SimpleShaders
	SimplePixelShader* pixelShader;
	SimplePixelShader* normalMapPixelShader;
	SimpleVertexShader* vertexShader;
	SimpleVertexShader* normalMapVertexShader;

	// The camera
	Camera* camera;

	// Materials
	Material* material;
	Material* tileMaterial;



	Material* startDefaultMaterial;
	Material* startStartMaterial;
	Material* startInstructMaterial;
	Material* startScoreMaterial;
	Material* waterMaterial;



	// Lighting
	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;

	//GameState enum
	enum GameState { Start, Game, Paused, Over };
	GameState state;
	bool pauseKeyDown;

	// Grid
	Grid* grid;

	//Player - will have ships, health, resources, etc. 
	Player player;

	//Enemy - will have ships, spawn rate, etc. 
	Enemy enemy;

	std::unique_ptr<DirectX::SpriteFont> m_font;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
};