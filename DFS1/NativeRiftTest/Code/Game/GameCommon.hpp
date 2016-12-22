#pragma once


//-----------------------------------------------------------------------------
#include "Engine/EngineCommon.hpp"
#include "Engine/Renderer/DebugRenderCommand.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Matrix4x4.hpp"


//-----------------------------------------------------------------------------
#define STATIC
#define VK_SHIFT 0x10
#define VK_SPACE 0x20
#define VK_F5 0x74


//-----------------------------------------------------------------------------
//Operating System
static const char* g_appName = "Unnamed Project by Benjamin Gibson";


//-----------------------------------------------------------------------------
//Naming Keys
static const char KEY_TO_MOVE_FASTER		= VK_SHIFT;

static const char KEY_TO_MOVE_FORWARD_3D	= 'W';
static const char KEY_TO_MOVE_BACKWARD_3D	= 'S';
static const char KEY_TO_MOVE_LEFT_3D		= 'A';
static const char KEY_TO_MOVE_RIGHT_3D		= 'D';
static const char KEY_TO_MOVE_UP_3D			= VK_SPACE;
static const char KEY_TO_MOVE_DOWN_3D		= 'X';

static const char KEY_TO_MOVE_LEFT_2D		= 'A';
static const char KEY_TO_MOVE_RIGHT_2D		= 'D';
static const char KEY_TO_MOVE_UP_2D			= 'W';
static const char KEY_TO_MOVE_DOWN_2D		= 'S';


//-----------------------------------------------------------------------------
//Camera
static const float FLYCAM_SPEED_SCALAR = 8.f;
static const Vector3f CAMERA3D_DEFAULT_POSITION = Vector3f( -5.f, 0.f, 0.f );
static const Vector2f CAMERA2D_DEFAULT_POSITION = Vector2f::ZERO;


//-----------------------------------------------------------------------------
//HUD

static const Vector2f HUD_BOTTOM_LEFT_POSITION = Vector2f( 100.f, 27.f ); //In from left, up from bottom of screen.
static const float HUD_CROSSHAIR_RADIUS = 20.f;
static const float HUD_CROSSHAIR_THICKNESS = 4.f;

//-----------------------------------------------------------------------------
//Lighting

static const unsigned int LIGHTS_IN_SCENE_MAX = 2;