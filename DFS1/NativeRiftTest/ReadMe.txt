Assignment Name: Oculus Rift CV1 Engine Support (Test Program)
Assignment Number: N/A
Name: Benjamin Gibson
Class: Directed Focus Study #1
Instructor: Chris Forseth


//-----------------------------------------------------------------------------
Recommended
	-> First change the BuildConfig.hpp settings to use VR (uncomment it) as well as the 2D Render Quad for the UI (bugs out).
	-> Load meshes from the executable directory and then change the render mode to accept vertex colors for viewing things in VR.
		- Console Command: MeshLoadFromFile <name.zmesh>, suggested files
			- AxisCube.zmesh (loads a single cube for basis testing)
			- DfsScene.zmesh (text positioned roughly where the 3D sounds are at)
			- DfsTakeaways.zmesh (presentation slides in VR)
		- Console Command: SetRenderMode 1 (should show "Set to PassthroughColor")
		

//-----------------------------------------------------------------------------		
Known Issues / Roadmap
	1. 3D sounds appear slightly offset from where you'd expect them in space.
	2. The VR build option needs to be run-time, not once-for-all-projects like it is now, being engine-side.
	3. The 2D render quad and the 3D FBOs both conflict with the Oculus FBO setup.
	4. Add controller support, Oculus Remote support!
	5. IPD seems messed up when close to 3D render objects?
	

Build Notes
	.exe will crash if it cannot find these directories from its working directory $(SolutionDir)Run_$(PlatformName)/ under Project Properties:
	Data/Fonts
	Data/Models
	Data/Images
	Data/Shaders
/!\	If skeleton not appearing upon import, be sure debug info is on (F1) in order to draw the debug render commands.

	
//-----------------------------------------------------------------------------
Flycam Mode Controls
	1. Keyboard
	!	R: Recenter HMD (in VR)
		O: Show/Hide Origin Axes
		W: Flycam Forward
		S: Flycam Backward
		A/D: Flycam Strafe
		Spacebar: Flycam Up
		X: Flycam Down
		Hold Shift: Flycam Speed-up (x8, editable as FLYCAM_SPEED_SCALAR in GameCommon.hpp)
		ESC: Exit
		F1: Toggle Debug Info (includes skeleton visualization!)
	2. Mouse: Look
	
Console Mode Controls
	Tilde: Show/Hide Dev Console
	Tab: Auto-complete (use with blank input to cycle through all commands)
	Enter: Execute/Close
	Escape: Clear/Close
	PageUp/PageDown/Scroll: Scroll Output (necessary for 'help' command, some are pushed off-screen now)
	Left/Right: Move Caret
	Home/End: Move Caret
	Delete: Delete
	Up/Down: Command History
	
Light Piloting Controls
	WASD/Space/X: Translate Selected Light
	IJKL/U/O: Translate Flycam
	Begin Mode: Enter "PilotLight <0-15>" command in console mode.
	Leave Mode: Re-enter "PilotLight" command in console mode (no arguments).
	
	
//-----------------------------------------------------------------------------
Resource Attributions
	Fonts		BMFont Software
	Input Font	http://input.fontbureau.com/
	Images		Chris Forseth
	Models		Chris Forseth, Unity Technologies Japan, Komal K.