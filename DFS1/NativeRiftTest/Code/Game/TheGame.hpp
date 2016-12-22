#pragma once


//-----------------------------------------------------------------------------
class TheGame;
class Camera3D;
class Camera2D;
class Command;
class Sprite;


//-----------------------------------------------------------------------------
extern TheGame* g_theGame;


//-----------------------------------------------------------------------------
struct Player
{
	Player() : ship(nullptr), time(0.f) {}
	Sprite* ship;
	float time;
};


//-----------------------------------------------------------------------------
enum CameraMode
{
	CAMERA_MODE_2D,
	CAMERA_MODE_3D
};


//-----------------------------------------------------------------------------
class TheGame
{
public:

	TheGame();
	~TheGame();

	void Startup();
	void Shutdown();

	bool IsQuitting() const { return m_isQuitting; }
	void Update(float deltaSeconds);

	void Render3D();
	void Render2D();
	void RenderDebug3D();
	void RenderDebug2D();

	const Camera3D* GetActiveCamera3D() const;
	const Camera2D* GetActiveCamera2D() const;
	static void ToggleActiveCamType3D(Command&);
	static void ToggleActiveCamType2D(Command&);
	static void ToggleActiveCameraMode(Command&);


private:

	static CameraMode s_activeCameraMode;
	bool m_isQuitting;

	void InitSceneAudio();

	void InitSpriteRenderer();
	void RegisterSprites();

	void InitSceneRendering();
	void CreateShaderPrograms();
	void CreateUniformValues();
	void CreateMeshRenderers();
	void CreateFBOs();
	void CreateSceneLights();

	void RenderReticle();
	void RenderLeftDebugText2D();
	void RenderRightDebugText2D();

	void UpdateCamera(float deltaSeconds);
	void UpdateFromKeyboard2D(float deltaSeconds);
	void UpdateFromMouse2D();
	void UpdateFromKeyboard3D(float deltaSeconds);
	void UpdateFromMouse3D();

	static Camera3D* s_playerCamera3D;
	static Camera2D* s_playerCamera2D;
	Player m_player;
};
