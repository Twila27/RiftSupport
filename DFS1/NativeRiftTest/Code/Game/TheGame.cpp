#include "Game/TheGame.hpp"

#include "Engine/Math/MathUtils.hpp"
#include "Engine/Physics/PhysicsUtils.hpp"
#include "Engine/Input/TheInput.hpp"
#include "Engine/Audio/TheAudio.hpp"
#include "Engine/Core/Command.hpp"
#include "Engine/Core/TheConsole.hpp"
#include "Engine/Math/Camera2D.hpp"
#include "Engine/Math/Camera3D.hpp"

#include "Engine/Renderer/Sprite.hpp"
#include "Engine/Renderer/SpriteResource.hpp"
#include "Engine/Renderer/SpriteRenderer.hpp"
#include "Engine/Renderer/FramebufferEffect.hpp"

#include "Game/GameCommon.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC CameraMode TheGame::s_activeCameraMode = CAMERA_MODE_3D;
STATIC Camera3D* TheGame::s_playerCamera3D = new Camera3D( Vector3f( -5.f, 0, 0 ) );
STATIC Camera2D* TheGame::s_playerCamera2D = new Camera2D(CAMERA2D_DEFAULT_POSITION);
STATIC void TheGame::ToggleActiveCamType2D(Command&) { s_playerCamera2D->m_usesPolarTranslations = !s_playerCamera2D->m_usesPolarTranslations; }
STATIC void TheGame::ToggleActiveCamType3D(Command&) { s_playerCamera3D->m_usesPolarTranslations = !s_playerCamera3D->m_usesPolarTranslations; }
STATIC void TheGame::ToggleActiveCameraMode(Command&) { s_activeCameraMode = (s_activeCameraMode == CAMERA_MODE_2D) ? CAMERA_MODE_3D : CAMERA_MODE_2D; }

//--------------------------------------------------------------------------------------------------------------
static void RegisterConsoleCommands()
{
	//AES A1
	g_theConsole->RegisterCommand("ToggleActiveCameraMode", TheGame::ToggleActiveCameraMode);
	g_theConsole->RegisterCommand("TogglePolarTranslations2D", TheGame::ToggleActiveCamType2D);
	g_theConsole->RegisterCommand("TogglePolarTranslations3D", TheGame::ToggleActiveCamType3D);
}


//-----------------------------------------------------------------------------
TheGame* g_theGame = nullptr;


//-----------------------------------------------------------------------------
TheGame::TheGame()
{
}


//-----------------------------------------------------------------------------
TheGame::~TheGame()
{
	delete s_playerCamera3D;
}


//-----------------------------------------------------------------------------
const Camera3D* TheGame::GetActiveCamera3D() const
{
	return s_playerCamera3D;
}


//-----------------------------------------------------------------------------
const Camera2D* TheGame::GetActiveCamera2D() const
{
	return s_playerCamera2D;
}


//-----------------------------------------------------------------------------
void TheGame::UpdateFromKeyboard2D(float deltaSeconds)
{
	//Super speed handling.
	float speed = 0.f;
	if (g_theInput->IsKeyDown(KEY_TO_MOVE_FASTER))
		speed = FLYCAM_SPEED_SCALAR;
	else speed = 1.f;


	float deltaMove = (speed * deltaSeconds);

	Vector2f camForwardXY = s_playerCamera2D->GetForwardXY();
	Vector2f camLeftXY = s_playerCamera2D->GetLeftXY();
	Vector2f& camPos = s_playerCamera2D->m_worldPosition;

	if (s_playerCamera2D->m_usesPolarTranslations)
	{
		static float radius = camPos.CalcFloatLength();
		static float theta = atan2f(camPos.y, camPos.x);
		static float polarSpeed = 1.f;

		deltaMove *= polarSpeed;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_LEFT_2D))
			theta -= deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_RIGHT_2D))
			theta += deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_UP_2D))
			radius += deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_DOWN_2D))
			radius -= deltaMove;

		if (g_theInput->IsKeyDown('Q'))
			polarSpeed += deltaMove;

		if (g_theInput->IsKeyDown('E'))
			polarSpeed -= deltaMove;

		camPos = Vector2f(CosDegrees(theta), SinDegrees(theta)) * radius;

		return;
	}

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_UP_2D))
		camPos += camLeftXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_DOWN_2D))
		camPos -= camLeftXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_RIGHT_2D))
		camPos += camForwardXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_LEFT_2D))
		camPos -= camForwardXY * deltaMove;
}


//-----------------------------------------------------------------------------
void TheGame::UpdateFromKeyboard3D(float deltaSeconds)
{
	//Super speed handling.
	float speed = 0.f;
	if (g_theInput->IsKeyDown(KEY_TO_MOVE_FASTER))
		speed = FLYCAM_SPEED_SCALAR;
	else speed = 1.f;


	float deltaMove = (speed * deltaSeconds); //speed is then in blocks per second!

	Vector3f camForwardXY = s_playerCamera3D->GetForwardXY();
	Vector3f camLeftXY = s_playerCamera3D->GetLeftXY();
	Vector3f& camPos = s_playerCamera3D->m_worldPosition;

	if (g_theRenderer->IsPilotingLight())
	{
		if (g_theInput->IsKeyDown('I'))
			camPos += camForwardXY * deltaMove;

		if (g_theInput->IsKeyDown('K'))
			camPos -= camForwardXY * deltaMove;

		if (g_theInput->IsKeyDown('J'))
			camPos += camLeftXY * deltaMove;

		if (g_theInput->IsKeyDown('L'))
			camPos -= camLeftXY * deltaMove;

		if (g_theInput->IsKeyDown('U'))
			camPos.z += deltaMove; //Scaled by z-axis, so *1.0f.

		if (g_theInput->IsKeyDown('O'))
			camPos.z -= deltaMove; //Scaled by z-axis, so *1.0f.

		return;
	}

	if (s_playerCamera3D->m_usesPolarTranslations)
	{
		static float radius = camPos.CalcFloatLength();
		static float theta = atan2f(camPos.y, camPos.x);
		static float phi = acosf(camPos.z / radius);
		static float polarSpeed = 1.f;
		float radiusScaleAlongXY = CosDegrees(phi);

		deltaMove *= polarSpeed;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_FORWARD_3D))
			phi -= deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_BACKWARD_3D))
			phi += deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_LEFT_3D))
			theta -= deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_RIGHT_3D))
			theta += deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_UP_3D))
			radius += deltaMove;

		if (g_theInput->IsKeyDown(KEY_TO_MOVE_DOWN_3D))
			radius -= deltaMove;

		if (g_theInput->IsKeyDown('Q'))
			polarSpeed += deltaMove;

		if (g_theInput->IsKeyDown('E'))
			polarSpeed -= deltaMove;

		camPos = Vector3f(radiusScaleAlongXY * CosDegrees(theta), radiusScaleAlongXY * SinDegrees(theta), -SinDegrees(phi)) * radius;

		return;
	}

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_FORWARD_3D))
		camPos += camForwardXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_BACKWARD_3D))
		camPos -= camForwardXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_LEFT_3D))
		camPos += camLeftXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_RIGHT_3D))
		camPos -= camLeftXY * deltaMove;

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_UP_3D))
		camPos.z += deltaMove; //Scaled by z-axis, so *1.0f.

	if (g_theInput->IsKeyDown(KEY_TO_MOVE_DOWN_3D))
		camPos.z -= deltaMove; //Scaled by z-axis, so *1.0f.
}


//-----------------------------------------------------------------------------
void TheGame::UpdateFromMouse2D()
{
	const float mouseSensitivityYaw = 0.044f;
	s_playerCamera2D->m_orientationDegrees -= mouseSensitivityYaw * (float)g_theInput->GetCursorDeltaX();
}


//-----------------------------------------------------------------------------
void TheGame::UpdateFromMouse3D()
{
	//The following is basis-independent: mouse X controls yaw around camera/view up j-vector, mouse Y controls pitch around camera/view right i-vector.
	const float mouseSensitivityYaw = 0.044f;
	s_playerCamera3D->m_orientation.m_yawDegrees -= mouseSensitivityYaw * (float)g_theInput->GetCursorDeltaX();
	s_playerCamera3D->m_orientation.m_pitchDegrees += mouseSensitivityYaw * (float)g_theInput->GetCursorDeltaY();
	s_playerCamera3D->FixAndClampAngles(); //This function body, however, is is basis-dependent, as what i-j-k equals is defined by the basis.
}


//-----------------------------------------------------------------------------
void TheGame::UpdateCamera(float deltaSeconds)
{
	switch (s_activeCameraMode)
	{
	case CAMERA_MODE_2D:
		UpdateFromKeyboard2D(deltaSeconds);
		UpdateFromMouse2D();
		s_playerCamera2D->SatisfyConstraints();
		break;
	case CAMERA_MODE_3D:
		UpdateFromKeyboard3D(deltaSeconds);
		UpdateFromMouse3D();
		break;
	}
}


//-----------------------------------------------------------------------------
void TheGame::Update(float deltaSeconds)
{
	if (m_player.ship != nullptr)
	{
		m_player.time += deltaSeconds;
		m_player.ship->m_transform.m_position = Vector2f(sin(m_player.time), cos(m_player.time)) * 3.f;
		m_player.ship->m_transform.m_rotationAngleDegrees = m_player.time;
		m_player.ship->m_transform.m_rotationAngleDegrees = WrapNumberWithinCircularRange(m_player.ship->m_transform.m_rotationAngleDegrees, 0.f, 360.f);
	}
	UpdateCamera(deltaSeconds);

	const Vector3f& camPos = s_playerCamera3D->m_worldPosition;
	FMOD_3D_ATTRIBUTES attributes = { { 0 } };
	attributes.position.x = camPos.x;
	attributes.position.y = camPos.y;
	attributes.position.z = camPos.z;
	attributes.up.x = 0.f;
	attributes.up.y = 0.f;
	attributes.up.z = 1.f;
	attributes.forward.x = 1.f;
	attributes.forward.y = 0.f;
	attributes.forward.z = 0.f;
	g_theAudio->SetListenerAttributes( 0, &attributes );

	SpriteRenderer::Update(deltaSeconds);
}


//-----------------------------------------------------------------------------
void TheGame::Startup()
{
	RegisterConsoleCommands();

	s_playerCamera2D->m_usesScrollingLimits = true;
	s_playerCamera2D->m_worldScrollingLimits.mins = Vector2f(-15.f);
	s_playerCamera2D->m_worldScrollingLimits.maxs = Vector2f(15.f);

	InitSceneRendering();

	InitSpriteRenderer();

	InitSceneAudio();
}


//--------------------------------------------------------------------------------------------------------------
void TheGame::InitSceneAudio()
{
	g_theAudio->LoadBankFile( "Data/Audio/Master Bank.bank" );
	g_theAudio->LoadBankFile( "Data/Audio/Master Bank.strings.bank" );
	g_theAudio->LoadBankFile( "Data/Audio/AmbientLoopBank.bank" );
	g_theAudio->LoadBankFile( "Data/Audio/SoundEffectBank.bank" );

	const float SPACING_BETWEEN_SOUNDS = 7.5f;
	const float POINT_LIFETIME_SECONDS = 20000.f;

	Vector3f nightSoundPos = Vector3f::ZERO;
	FMOD::Studio::EventInstance* soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Night", nightSoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( nightSoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::MAGENTA, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	Vector3f restaurantSoundPos = Vector3f::UNIT_X * SPACING_BETWEEN_SOUNDS;
	soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Restaurant", restaurantSoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( restaurantSoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::RED, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	Vector3f factorySoundPos = restaurantSoundPos * -1.f;
	soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Factory", factorySoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( factorySoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::RED, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	Vector3f rainSoundPos = Vector3f::UNIT_Y * SPACING_BETWEEN_SOUNDS;
	soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Rain", rainSoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( rainSoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::GREEN, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	Vector3f oceanSoundPos = rainSoundPos * -1.f;
	soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Ocean", rainSoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( rainSoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::GREEN, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	Vector3f windSoundPos = Vector3f::UNIT_Z * SPACING_BETWEEN_SOUNDS;
	soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Wind", windSoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( windSoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::CYAN, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	Vector3f underwaterSoundPos = windSoundPos * -1.f;
	soundInst = g_theAudio->CreateEventInstanceAtPosition( "event:/Ambient Loops/Underwater", underwaterSoundPos );
	AddDebugRenderCommand( new DebugRenderCommandPoint( underwaterSoundPos, POINT_LIFETIME_SECONDS, DEPTH_TEST_DUAL, Rgba::CYAN, 5.f ) );
	g_theAudio->StartOrRestartEventInstance( soundInst );

	//	PLAYING THIS WAY BELOW WILL NOT SPATIALIZE ("3D-IFY") THE SOUND, MUST WRAP IT INSIDE AN FMOD PROJECT + A DISTANCE ATTEN PARAM!
	//	SoundID wavtest = g_theAudio->CreateOrGetSound( "Data/Audio/ambient_city_rain_lp.wav" );
	//	SoundID wavtest = g_theAudio->CreateOrGetSound( "Data/Audio/ambient_light_wind_through_trees_with_birds_01_lp.wav" );
	//	g_theAudio->PlaySound( wavtest );
}


//--------------------------------------------------------------------------------------------------------------
void TheGame::InitSpriteRenderer()
{
	SpriteRenderer::LoadAllSpriteResources();

	SpriteRenderer::Startup( s_playerCamera2D, Rgba::DARK_GRAY, true );

	SpriteRenderer::SetDefaultVirtualSize( 15 );
	/*
	Suggested to make 1 unit == size of the player's ship sprite.
	HOWEVER, do NOT base virtual or import size on an asset--these should both be decided prior to making assets/game.
	Nuclear Throne example:
	- 240p import size, implying a pixel is 1/240th of the screen (or there are 240 rows of pixels in the window).
	- 15 virtual size, implying a 15x15 grid of virtual units is what the screen can see in total.
	-> Therefore, a single virtual unit takes up 240/15 px == 16 (by 16) px on screen.
	-> Therefore, the average size of a sprite, e.g. the player sprite, should also be 16x16 px.
	*/
	SpriteRenderer::SetImportSize( 240 ); // # vertical lines of resolution.

	SpriteRenderer::UpdateAspectRatio( (float)g_theRenderer->GetScreenWidth(), (float)g_theRenderer->GetScreenHeight() ); //SET THIS at any moment the resolution is changed in the options.

	RegisterSprites();

}


//--------------------------------------------------------------------------------------------------------------
void TheGame::RegisterSprites()
{
	Sprite* redMage = Sprite::Create("Mage", Vector2f::ZERO, Rgba::MAGENTA);
	redMage->Enable();
	Sprite* blueMage = Sprite::Create("Mage", Vector2f(2.f, 0.f), Rgba::CYAN);
	blueMage->Enable();

	m_player.ship = Sprite::Create("Ship", Vector2f::ONE * 3.f);
	m_player.ship->Enable();
	blueMage->SetParent(m_player.ship);
	redMage->SetParent(blueMage);

	Sprite* title = Sprite::Create("Title_UI", Vector2f(0.f, 5.f));
	const RenderLayerID UI_ID = -1000;
	title->SetLayerID(UI_ID, "UI");
	SpriteRenderer::SetLayerVirtualSize(UI_ID, 23, 23);
	SpriteRenderer::SetLayerIsScrolling(UI_ID, false);
	SpriteRenderer::AddLayerEffect(UI_ID, FramebufferEffect::GetFboEffect("FboEffect_PostProcessObama"));
	title->Enable();
}


//-----------------------------------------------------------------------------
void TheGame::Shutdown()
{
	delete m_player.ship;
	SpriteRenderer::Shutdown();
}
