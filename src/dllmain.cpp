#define _USE_MATH_DEFINES
#define MINI_CASE_SENSITIVE
#define NOMINMAX

#include <Windows.h>
#include <ShellScalingAPI.h>
#include <VersionHelpers.h>
#include <shlobj.h>

#include <string>
#include <filesystem>

#include <GL/gl.h>

#include "MinHook.hpp"
#include "ini.hpp"
#include "dllmain.hpp"
#include "helper.hpp"

#pragma comment(lib, "libMinHook.x86.lib")

// =============================
// Memory Addresses 
// =============================
const int MAIN_ENTRY_POINT = 0x4DF0A3;
const int CODE_CAVE_SOUND = 0x513490;
const int CODE_CAVE_INTRO = 0x513B90;
const int CODE_CAVE_BLINK = 0x513E08;
const int CODE_CAVE_WIDTH = 0x513E40;
const int STARTUP_STATE_ADDR = 0x7CCA20;
const int CONSOLE_THREAD_PTR_ADDR = 0x7CCA54;
const int CURRENT_LANG = 0x7CF868;
const int DISPLAY_MODE_IDX = 0x7D40C4;
const int DISPLAY_MODE_NUM = 0x7D40C8;
const int IS_MENU_LOCKED = 0x11C2770;
const int BLINK_TIMER = 0x11C35EC;
const int UI_WAIT_TIMER = 0x12EF948;
const int IS_CINEMATIC = 0x12F3CE8;
const int CURRENT_WEAPON_ID = 0x12F3D60;
const int IS_IN_MENU = 0x14EB498;
const int SHADERS_CACHE_ADDR = 0x1BFCEF4;
const int DISPLAY_MODE_ARRAY_WIDTH_ADDR = 0x1C1D2E0;
const int DISPLAY_MODE_ARRAY_HEIGHT_ADDR = 0x1C1D2E4;
const int CURRENT_WIDTH_ADDR = 0x1C4798C;
const int CURRENT_HEIGHT_ADDR = 0x1C47990;

// =============================
// VorpalFix Menu
// =============================
int VF_LANGUAGE_PTR;
int VF_REMASTERED_MODELS_PTR;
int VF_UI_CONSOLE_HUD_PTR;
int VF_UI_PS3_PTR;
int VF_UI_LETTERBOX_PTR;
int VF_R_EXT_MAX_ANISOTROPY_PTR;
int VF_COM_MAXFPS_PTR;

bool isScreenRateFps = false;
bool isVFMenuUsed = false;

// =============================
// Variables 
// =============================

// Misc
bool isCursorResized = false;
bool isDefaultFullscreenSettingSkipped = false;
bool isUsingControllerMenu = false;
bool isMainMenuShown = false;
bool isTitleBgSet = false;
bool isUsingCustomSaveDir = false;
bool skipAutoResolution = false;
bool isAnisotropyRetrieved = false;
bool isInSettingMenu = false;
bool isTakingSaveScreenshot = false;
bool isRenderingConsole = false;
bool isLoadingSaveFromMenuButton = false;

// Misc (read-only)
constexpr float ASPECT_RATIO_4_3 = 4.0f / 3.0f;
constexpr int LEFT_BORDER_X_ID = 0x1000000;
constexpr int RIGHT_BORDER_X_ID = 0x2000000;
const char* ALICE2_DEFAULT_PATH = "..\\..\\Alice2\\Binaries\\Win32\\AliceMadnessReturns.exe";

// Scaling
int currentWidth = 0;
int currentHeight = 0;
float currentAspectRatio = 0;
bool isWiderThan4By3 = false;
float scaleFactor = 0;
float scaledWidth = 0;
float widthDifference = 0;
float consoleHudAdjustmentDivisor = 0;

// Localization pk3
bool hasLookedForLocalizationFiles = false;
size_t localizationFilesToLoad = 0;
std::vector<std::string> pk3LocFiles;

// Additional commands
bool isHoldingLeftStick = false;
int lastQuickSaveFrame = 0;
char* lastWeaponIdUp = nullptr;
char* lastWeaponIdDown = nullptr;
char* lastWeaponIdLeft = nullptr;
char* lastWeaponIdRight = nullptr;
const char* weaponCommands[] =
{
	"use watch",
	"use knife",
	"use cards",
	"use mallet",
	"use jackbomb",
	"use icewand",
	"use jacks",
	"use demondice",
	"use eyestaff",
	"use blunderbuss"
};

// pak5 fix
bool isUsingBrokenPak5 = false;
const char* correctPaths[] =
{
	"models/characters/cheshire/skin01.ftx",
	"models/characters/cheshire/skin02.ftx",
	"models/characters/cheshire/skin02g.ftx",
	"models/characters/cardguard_club/skin01.ftx",
	"models/characters/cardguard_diamond/skin02.ftx",
	"models/characters/cardguard_heart/skin04.ftx",
	"models/characters/cardguard_spade/skin03.ftx",
	"models/characters/mock_turtle/mshell.ftx",
	"models/characters/mock_turtle/mturt.ftx"
};

const char* pak5BrokenPaths[] =
{
	"models/characters/cheshire_cat/skin01.ftx",
	"models/characters/cheshire_cat/skin02.ftx",
	"models/characters/cheshire_cat/skin02g.ftx",
	"models/characters/cardguard/cardguard_club/skin01.ftx",
	"models/characters/cardguard/cardguard_diamond/skin02.ftx",
	"models/characters/cardguard/cardguard_heart/skin04.ftx",
	"models/characters/cardguard/cardguard_spade/skin03.ftx",
	"models/characters/morckturtle/mshell.ftx",
	"models/characters/morckturtle/mturt.ftx"
};

// =============================
// Original Function Pointers
// =============================
int(__cdecl* JumpCommand)() = nullptr; // 0x4061D0
int(__cdecl* Bind)(int, char*) = nullptr; // 0x407870
int(__cdecl* HandleKeyboardInput)(int, int, int) = nullptr; // 0x4081B0
void(__cdecl* CL_InitRef)() = nullptr; // 0x409FD0
void(__cdecl* CL_ParsePacketEntities)(int, int, int) = nullptr; // 0x40C1B0
int(__cdecl* CallCmd)(const char*, char) = nullptr; // 0x4158F0
char* (__cdecl* GetSavePath)() = nullptr; // 0x417400
void(__cdecl* CVAR_Init)() = nullptr; // 0x417530
int(__cdecl* Cvar_Set)(const char*, const char*, int) = nullptr; // 0x419910
int(__cdecl* FS_FOpenFileRead)(char*, int*, int, int) = nullptr; // 0x41A590
int(__cdecl* CheckDiskFreeSpace)() = nullptr; // 0x41D1E0
float(__cdecl* UpdateHeadOrientation)(DWORD*, float*) = nullptr; // 0x423740
BYTE(__cdecl* Str_To_Lower)(char*) = nullptr; // 0x4256E0
FILE(__cdecl* FS_LoadZipFile)(const char*) = nullptr; // 0x43E030
int(__cdecl* PrepareHUDRendering)(float, float, float, float, int, float*, float*, float*, int, const char*, __int16, float*, float*, float, float, int) = nullptr; // 0x446050
int(__cdecl* IsGameStarted)() = nullptr; // 0x449DF0
void(__cdecl* SetUIBorder)() = nullptr; // 0x44B100
int(__cdecl* PushMenu)(const char*) = nullptr; // 0x44C1B0
int(__cdecl* ForceMenu)(const char*) = nullptr; // 0x44C280
void(__cdecl* TriggerMainMenu)(int) = nullptr; // 0x44C4F0
void(__thiscall* ShowDialogBoxText)(int) = nullptr; // 0x452CF0
void(__thiscall* LoadSaveFromUI)(DWORD*, int) = nullptr; // 0x456380
const char* (__cdecl* LoadLocalizationFile)() = nullptr; // 0x4615F0
MMRESULT(__cdecl* UpdateControllerState)() = nullptr; // 0x4635A0
int(__stdcall* lpfnWndProc_MSG)(HWND, UINT, int, LPARAM) = nullptr; // 0x46C600
int(__cdecl* TakeSaveScreenshot)(int, int, int) = nullptr; // 0x46D280
int(__cdecl* SetupOpenGLParameters)() = nullptr; // 0x46E0D0
void(__cdecl* FetchDisplayResolutions)() = nullptr; // 0x46F850
int(__cdecl* GLW_CreatePFD)(void*, unsigned __int8, char, unsigned __int8, int) = nullptr; // 0x46FC70
int(__cdecl* QGL_Init)(LPCSTR) = nullptr; // 0x47ABE0
void(__cdecl* RE_StretchFont)(int, BYTE, float, float, float, float, float, float, float, int) = nullptr; // 0x48F1E0
int(__cdecl* RE_StretchPic)(float, float, float, float, float, float, float, float, int) = nullptr; // 0x48FC00
int(__cdecl* RE_StretchRaw)(int, int, int, int, int, int, int) = nullptr; // 0x490130
void(__cdecl* UpdateRenderContext)(int, int, int, int, float, float, float, float, float, float) = nullptr; // 0x4907F0
void(__cdecl* ConfigureScissor)(int, int, int, int) = nullptr; // 0x4908D0
int(__thiscall* UpdateAndConfigureRenderContext)(int) = nullptr; // 0x4B1130
DWORD(__thiscall* UISetCvars)(DWORD*, char*) = nullptr; // 0x4B9FD0
BYTE(__thiscall* LoadUI)(DWORD*, char*) = nullptr; // 0x4C1AC0
DWORD* (__cdecl* SetAliceMirrorViewportParams)(DWORD*, float, float, float, float, float) = nullptr; // 0x4C5D30
int(__thiscall* UpdateHeadOrientationFromMouse)(int, float*, int, float*, float*) = nullptr; // 0x4C6050
HWND(WINAPI* ori_CreateWindowExA)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
void(WINAPI* ori_glTexParameterf)(GLenum, GLenum, GLfloat);
void(WINAPI* ori_glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void*);

// =============================
// Ini Variables
// =============================

// Fixes
bool FixSoundRandomization = false;
bool FixHardDiskFull = false;
bool FixSaveScreenshotBufferOverflow = false;
bool FixDisplayModeBufferOverflow = false;
bool FixPak5 = false;
bool FixBlinkingAnimationSpeed = false;
bool FixStretchedHUD = false;
bool FixStretchedFMV = false;
bool FixStretchedGUI = false;
bool FixDPIScaling = false;
bool FixFullscreenSetting = false;
bool FixParticleDistanceRatio = false;
bool FixMenuAnimationSpeed = false;
bool FixMenuTransitionTiming = false;
bool FixCutsceneJumpSound = false;
bool FixResolutionModeOOB = false;
bool FixLocalizationFiles = false;
bool FixProton = false;

// General
bool CustomControllerBindings = false;
float CameraSmoothingFactor = 0;
bool LaunchWithoutAlice2 = false;
bool PreventAlice2OnExit = false;
bool DisableWinsockInitialization = false;
char* Alice2Path = nullptr;
int LanguageId = 0;
bool UseConsoleTitleScreen = false;
bool UseOriginalIntroVideos = false;
bool Disable8BitAudioAsDefault = false;
bool DisableRemasteredModels = false;
char* CustomSavePath = nullptr;

// Display
bool ConsolePortHUD = false;
bool EnableControllerIcons = false;
bool UsePS3ControllerIcons = false;
bool HideConsoleAtLaunch = false;
bool DisableLetterbox = false;
bool ForceBorderlessFullscreen = false;
bool FirstAutoResolution = false;
bool AutoResolution = false;
bool CustomResolution = false;
int CustomResolutionWidth = 0;
int CustomResolutionHeight = 0;
bool EnableAltF4Close = 0;

// Graphics
float MaxAnisotropy = 0;
bool TrilinearTextureFiltering = false;
bool EnhancedLOD = false;
int CustomFPSLimit = 0;
bool EnableVsyncAsDefault = false;
bool AutoFOV = false;

static void ReadConfig()
{
	// Configuration reader initialization
	IniHelper::Init();

	// Fixes
	FixSoundRandomization = IniHelper::ReadInteger("Fixes", "FixSoundRandomization", 1) == 1;
	FixHardDiskFull = IniHelper::ReadInteger("Fixes", "FixHardDiskFull", 1) == 1;
	FixSaveScreenshotBufferOverflow = IniHelper::ReadInteger("Fixes", "FixSaveScreenshotBufferOverflow", 1) == 1;
	FixDisplayModeBufferOverflow = IniHelper::ReadInteger("Fixes", "FixDisplayModeBufferOverflow", 1) == 1;
	FixPak5 = IniHelper::ReadInteger("Fixes", "FixPak5", 1) == 1;
	FixBlinkingAnimationSpeed = IniHelper::ReadInteger("Fixes", "FixBlinkingAnimationSpeed", 1) == 1;
	FixStretchedHUD = IniHelper::ReadInteger("Fixes", "FixStretchedHUD", 1) == 1;
	FixStretchedFMV = IniHelper::ReadInteger("Fixes", "FixStretchedFMV", 1) == 1;
	FixStretchedGUI = IniHelper::ReadInteger("Fixes", "FixStretchedGUI", 1) == 1;
	FixDPIScaling = IniHelper::ReadInteger("Fixes", "FixDPIScaling", 1) == 1;
	FixFullscreenSetting = IniHelper::ReadInteger("Fixes", "FixFullscreenSetting", 1) == 1;
	FixParticleDistanceRatio = IniHelper::ReadInteger("Fixes", "FixParticleDistanceRatio", 1) == 1;
	FixMenuAnimationSpeed = IniHelper::ReadInteger("Fixes", "FixMenuAnimationSpeed", 1) == 1;
	FixMenuTransitionTiming = IniHelper::ReadInteger("Fixes", "FixMenuTransitionTiming", 1) == 1;
	FixResolutionModeOOB = IniHelper::ReadInteger("Fixes", "FixResolutionModeOOB", 1) == 1;
	FixCutsceneJumpSound = IniHelper::ReadInteger("Fixes", "FixCutsceneJumpSound", 1) == 1;
	FixLocalizationFiles = IniHelper::ReadInteger("Fixes", "FixLocalizationFiles", 1) == 1;
	FixProton = IniHelper::ReadInteger("Fixes", "FixProton", 0) == 1;

	// General
	LaunchWithoutAlice2 = IniHelper::ReadInteger("General", "LaunchWithoutAlice2", 1) == 1;
	CameraSmoothingFactor = IniHelper::ReadFloat("General", "CameraSmoothingFactor", 0.8);
	CustomControllerBindings = IniHelper::ReadInteger("General", "CustomControllerBindings", 1) == 1;
	PreventAlice2OnExit = IniHelper::ReadInteger("General", "PreventAlice2OnExit", 0) == 1;
	DisableWinsockInitialization = IniHelper::ReadInteger("General", "DisableWinsockInitialization", 1) == 1;
	Alice2Path = IniHelper::ReadString("General", "Alice2Path", ALICE2_DEFAULT_PATH);
	LanguageId = IniHelper::ReadInteger("General", "LanguageId", 0);
	UseOriginalIntroVideos = IniHelper::ReadInteger("General", "UseOriginalIntroVideos", 0) == 1;
	Disable8BitAudioAsDefault = IniHelper::ReadInteger("General", "Disable8BitAudioAsDefault", 1) == 1;
	UseConsoleTitleScreen = IniHelper::ReadInteger("General", "UseConsoleTitleScreen", 1) == 1;
	DisableRemasteredModels = IniHelper::ReadInteger("General", "DisableRemasteredModels", 0) == 1;
	CustomSavePath = IniHelper::ReadString("General", "CustomSavePath", "");

	// Display
	ConsolePortHUD = IniHelper::ReadInteger("Display", "ConsolePortHUD", 0) == 1;
	EnableControllerIcons = IniHelper::ReadInteger("Display", "EnableControllerIcons", 1) == 1;
	UsePS3ControllerIcons = IniHelper::ReadInteger("Display", "UsePS3ControllerIcons", 0) == 1;
	HideConsoleAtLaunch = IniHelper::ReadInteger("Display", "HideConsoleAtLaunch", 1) == 1;
	DisableLetterbox = IniHelper::ReadInteger("Display", "DisableLetterbox", 0) == 1;
	GameHelper::DisableCursorScaling = IniHelper::ReadInteger("Display", "DisableCursorScaling", 0) == 1;
	ForceBorderlessFullscreen = IniHelper::ReadInteger("Display", "ForceBorderlessFullscreen", 0) == 1;
	FirstAutoResolution = IniHelper::ReadInteger("Display", "FirstAutoResolution", 1) == 1;
	AutoResolution = IniHelper::ReadInteger("Display", "AutoResolution", 0) == 1;
	CustomResolution = IniHelper::ReadInteger("Display", "CustomResolution", 0) == 1;
	CustomResolutionWidth = IniHelper::ReadInteger("Display", "CustomResolutionWidth", 640);
	CustomResolutionHeight = IniHelper::ReadInteger("Display", "CustomResolutionHeight", 480);
	EnableAltF4Close = IniHelper::ReadInteger("Display", "EnableAltF4Close", 1) == 1;

	// Graphics
	MaxAnisotropy = IniHelper::ReadFloat("Graphics", "MaxAnisotropy", 16);
	TrilinearTextureFiltering = IniHelper::ReadInteger("Graphics", "TrilinearTextureFiltering", 1) == 1;
	EnhancedLOD = IniHelper::ReadInteger("Graphics", "EnhancedLOD", 1) == 1;
	CustomFPSLimit = IniHelper::ReadInteger("Graphics", "CustomFPSLimit", 85);
	EnableVsyncAsDefault = IniHelper::ReadInteger("Graphics", "EnableVsyncAsDefault", 1) == 1;
	AutoFOV = IniHelper::ReadInteger("Graphics", "AutoFOV", 1) == 1;

	// Set to monitor's refresh rate
	if (CustomFPSLimit == -1)
	{
		CustomFPSLimit = SystemHelper::GetCurrentDisplayFrequency();
		isScreenRateFps = true;

		// Unstable at > 500fps
		if (CustomFPSLimit > 500)
		{
			CustomFPSLimit = 500;
		}
	}

	// UseConsoleTitleScreen rely on FixStretchedGUI
	if (!FixStretchedGUI && UseConsoleTitleScreen)
	{
		UseConsoleTitleScreen = false;
	}

	// Clamp CameraSmoothingFactor
	CameraSmoothingFactor = std::clamp(CameraSmoothingFactor, 0.0f, 1.0f);

	// Check if the user has specified a custom save directory
	isUsingCustomSaveDir = (CustomSavePath != NULL) && (CustomSavePath[0] != '\0');

	// If specified, format the custom save path
	if (isUsingCustomSaveDir)
	{
		static char formattedSavePath[MAX_PATH];

		strncpy(formattedSavePath, CustomSavePath, MAX_PATH - 1);
		formattedSavePath[MAX_PATH - 1] = '\0';

		// Add trailing backslash if not already present
		size_t len = strlen(formattedSavePath);
		if (len > 0 && formattedSavePath[len - 1] != '\\' && len < MAX_PATH - 1)
		{
			formattedSavePath[len] = '\\';
			formattedSavePath[len + 1] = '\0';
		}

		CustomSavePath = formattedSavePath;
	}
}

#pragma region

// Used to force borderless fullscreen when the main window is initialized
static HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	// Style of the main window
	if (dwStyle == 0x10C80000)
	{
		auto [screenWidth, screenHeight] = SystemHelper::GetScreenResolution();

		dwStyle = WS_VISIBLE + WS_POPUP;

		nWidth = screenWidth;
		nHeight = screenHeight;

		X = 0;
		Y = 0;
	}

	return ori_CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

// Hook glTexParameterf to perform anisotropy filtering
static void WINAPI glTexParameterf_Hook(GLenum target, GLenum pname, GLfloat param)
{
	// Check if the param is a valid mipmap filter mode
	if (param >= 0x2700 && param <= 0x2703)
	{
		if (!isAnisotropyRetrieved)
		{
			// Retrieve the maximum anisotropy level supported by the hardware with GL_MAX_TEXTURE_MAX_ANISOTROPY
			float maxAnisotropy;
			glGetFloatv(0x84FF, &maxAnisotropy);

			// Check if the user's MaxAnisotropy exceeds the hardware's capability
			if (MaxAnisotropy > maxAnisotropy)
			{
				// Adjust to hardware-supported level
				MaxAnisotropy = maxAnisotropy;
			}

			isAnisotropyRetrieved = true;
		}

		ori_glTexParameterf(target, pname, param);
		ori_glTexParameterf(target, 0x84FE, (float)MaxAnisotropy); // GL_TEXTURE_MAX_ANISOTROPY
	}
	else
	{
		ori_glTexParameterf(target, pname, param);
	}
}

// Hook glReadPixels to adjust the x position of the screeshot used for the save snapshot
static void WINAPI glReadPixels_Hook(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data)
{
	if (isTakingSaveScreenshot)
	{
		// 4:3 x position
		x = static_cast<int>(widthDifference);
		isTakingSaveScreenshot = false;
	}

	ori_glReadPixels(x, y, width, height, format, type, data);
}

// Hook of the function used by the "+moveup" command
static int __cdecl JumpCommand_Hook()
{
	// If jumping during a cutscene
	if (MemoryHelper::ReadMemory<int>(IS_CINEMATIC))
	{
		// Don't play 'jump.wav' (from a player's action) during a cutscene
		return 0;
	}

	return JumpCommand();
}

// Hook of the function used by the "bind" command
static int __cdecl Bind_Hook(int keyId, char* cmd_name)
{
	if (strcmp(cmd_name, "toggleconsole") == 0)
	{
		// Handle default keys that are not recognized by the game, use "F2" as default
		if (keyId == 96 || keyId == 126)
		{
			keyId = GameHelper::GetKeyId("F2");
		}

		uint8_t cmpInstruction[] = { 0x81, 0xFF };

		// cmp toggleConsoleKeyId instead of hardcoded cmp 96 and cmp 126 (` and ~)
		MemoryHelper::WriteMemoryRaw(0x40823A, cmpInstruction, sizeof(cmpInstruction));
		MemoryHelper::WriteMemory<int>(0x40823C, keyId);
		MemoryHelper::MakeNOP(0x408240, 6);
	}

	return Bind(keyId, cmd_name);
}

// Handle specific keys to get past the title screen
static int __cdecl HandleKeyboardInput_Hook(int keyId, int a2, int a3)
{
	// Got past the title screen, no longer need to use this hook
	if (!UseConsoleTitleScreen || isMainMenuShown)
	{
		MH_DisableHook((void*)0x4081B0);
		return HandleKeyboardInput(keyId, a2, a3);
	}

	// Enter, Space, Left Mouse Click, Controller 'A'
	if (isTitleBgSet && (keyId == 13 || keyId == 32 || keyId == 178 || keyId == 199))
	{
		// Set keyId to 'Escape' (27) to skip the title screen
		keyId = 27;
	}

	return HandleKeyboardInput(keyId, a2, a3);
}

// Restart the engine to show the menu correctly on Linux
static void __cdecl CL_InitRef_Hook()
{
	// Only needed once, make sure that we are not stuck in a loop
	MH_DisableHook((void*)0x409FD0);

	CL_InitRef();

	// Workaround for Linux, must be called after CL_InitRef()
	GameHelper::VidRestart();
}

// Process the snapshot returned from the server (fgamex86.dll) and modify specific properties
static void __cdecl CL_ParsePacketEntities_Hook(int msg, int oldframe, int newframe)
{
	float* newFovPtr = (float*)(newframe + 0x418);
	float originalFov = *newFovPtr;

	if (AutoFOV && isWiderThan4By3)
	{
		bool shouldScale = !oldframe; // Scale first frame

		if (oldframe)
		{
			float* oldFovPtr = (float*)(oldframe + 0x418);
			shouldScale = (*newFovPtr != *oldFovPtr);
		}

		if (shouldScale)
		{
			float verticalFovRad = 2.0f * atan(tan(originalFov * (M_PI / 180.0f) * 0.5f) / ASPECT_RATIO_4_3);
			*newFovPtr = 2.0f * atan(tan(verticalFovRad * 0.5f) * currentAspectRatio) * (180.0f / M_PI);
		}
	}

	if (DisableLetterbox)
	{
		*(int*)(newframe + 0x10C) = 0; // No letterbox

		bool isCinematic = *(int*)(newframe + 0x104);
		bool wasCinematic = oldframe ? *(int*)(oldframe + 0x104) : false;

		if (isCinematic != wasCinematic) // Cutscene state changed
		{
			if (isCinematic) // Entering cutscene
			{
				*newFovPtr = *newFovPtr / 1.11f;
			}
			else // Exiting cutscene, default FOV is 90 during gameplay
			{
				float verticalFovRad = 2.0f * atan(tan(90.0f * (M_PI / 180.0f) * 0.5f) / ASPECT_RATIO_4_3);
				*newFovPtr = 2.0f * atan(tan(verticalFovRad * 0.5f) * currentAspectRatio) * (180.0f / M_PI);
			}
		}
		else if (isCinematic && oldframe) // During cutscene, check for FOV changes
		{
			float* oldFovPtr = (float*)(oldframe + 0x418);
			if (*newFovPtr != *oldFovPtr) // FOV changed during cutscene
			{
				*newFovPtr = *newFovPtr / 1.11f;
			}
		}
	}

	CL_ParsePacketEntities(msg, oldframe, newframe);
}

// Hook of the command function
static int __cdecl CallCmd_Hook(char* cmd, char a2)
{
    if (!CustomControllerBindings || !isHoldingLeftStick || !cmd)
        return CallCmd(cmd, a2);

    std::string inputCmd(cmd);
    std::transform(inputCmd.begin(), inputCmd.end(), inputCmd.begin(), ::tolower);

	for (const auto& command : weaponCommands)
	{
		if (inputCmd == command) return 0; // Return 0 if the command matches, skip the weapon switch command that has been replaced
	}
    
    return CallCmd(cmd, a2);
}

// Return the current save directory
static char* __cdecl GetSavePath_Hook()
{
	if (isUsingCustomSaveDir)
	{
		return CustomSavePath;
	}

	return GetSavePath();
}

// Hook of the function initializing some of the game's CVAR
static void __cdecl CVAR_Init_Hook()
{
	// Make sure that the controller keys are correctly binded
	if (CustomControllerBindings)
	{
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY1"), "cheshire");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY4"), "togglemenu");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY9"), "+attackleft");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY10"), "+attackright");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY11"), "prevweapon");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY12"), "nextweapon");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY13"), "+attackleft");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY14"), "+UseAndDown");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY15"), "+moveup");
		GameHelper::AssignCmdKeyId(GameHelper::GetKeyId("JOY16"), "+attackright");
	}

	CVAR_Init();

	// Nothing else to do here
	MH_DisableHook((void*)0x417530);
}

// Caches display resolutions; bug: DISPLAY_MODE_IDX is never reset to 0
static void __cdecl FetchDisplayResolutions_Hook()
{
	if (!FixDisplayModeBufferOverflow)
	{
		FetchDisplayResolutions();
		return;
	}

	// If already executed, skip it
	if (MemoryHelper::ReadMemory<int>(DISPLAY_MODE_IDX) == 0)
	{
		FetchDisplayResolutions();
	}
}

// Intercepts and updates cvars
static int __cdecl Cvar_Set_Hook(const char* var_name, const char* value, int flag)
{
	// If the game has fully started (all relevant cvars have been initialized), disable this hook
	if (MemoryHelper::ReadMemory<int>(STARTUP_STATE_ADDR) == -1)
	{
		MH_DisableHook((void*)0x419910);
	}

	if (EnhancedLOD)
	{
		if (StringHelper::stricmp(var_name, "r_lodbias"))
		{
			value = "-2";
		}
		else if (StringHelper::stricmp(var_name, "r_lodCurveError"))
		{
			value = "10000";
		}
	}

	if (ForceBorderlessFullscreen && StringHelper::stricmp(var_name, "r_fullscreen"))
	{
		value = "0";
		flag = 0x10; // read-only
	}

	if (StringHelper::stricmp(var_name, "s_Alice2URL"))
	{
		value = Alice2Path;

		// File doesn't exist, don't try to launch it
		if (!std::filesystem::exists(value))
		{
			MemoryHelper::WriteMemory<int>(0x559A08, 0, false);
		}
	}

	if (CustomFPSLimit != 60 && StringHelper::stricmp(var_name, "com_maxfps"))
	{
		value = StringHelper::IntegerToCString(CustomFPSLimit);
	}

	if (CameraSmoothingFactor != 0.8f && StringHelper::stricmp(var_name, "cg_camerascale"))
	{
		value = StringHelper::FloatToCString(1.0f - CameraSmoothingFactor);
	}

	// Read settings from "base" folder, skip it
	if (FixFullscreenSetting && !isDefaultFullscreenSettingSkipped && StringHelper::stricmp(var_name, "r_fullscreen"))
	{
		isDefaultFullscreenSettingSkipped = true;
		return 0;
	}

	if (!skipAutoResolution && (FirstAutoResolution || AutoResolution || FixResolutionModeOOB || ForceBorderlessFullscreen) && StringHelper::stricmp(var_name, "r_mode"))
	{
		// Prevent re-entering this condition
		skipAutoResolution = true;

		// Get config file path
		const char* configFile = (MemoryHelper::ReadMemory<int>(CURRENT_LANG) == 2) ? "config_pc_fra.cfg" : "config.cfg";

		std::string directoryPath = GetSavePath_Hook();
		std::filesystem::path configFilePath = std::filesystem::path(directoryPath) / configFile;

		// Execute this earlier to cache the resolution list
		FetchDisplayResolutions_Hook();

		auto [screenWidth, screenHeight] = SystemHelper::GetScreenResolution();
		int resolutionNum = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_NUM);

		// Check if 'r_mode' exceeds the total count of available resolution modes
		if (FixResolutionModeOOB)
		{
			int index;
			std::istringstream(value) >> index;

			// Check if "index" is within bounds and adjust if necessary
			if (index >= resolutionNum)
			{
				// Set to the current resolution instead of crashing
				AutoResolution = true;
			}
		}

		// If we want 'r_mode' to match the screen resolution
		if ((FirstAutoResolution && !std::filesystem::exists(configFilePath)) || (AutoResolution || ForceBorderlessFullscreen))
		{
			// Find the 'r_mode' value matching the screen resolution
			for (int i = 0; i < resolutionNum; i++)
			{
				int screenWidthMode = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_ARRAY_WIDTH_ADDR + (i * 8));
				int screenHeightMode = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_ARRAY_HEIGHT_ADDR + (i * 8));

				if (screenWidth == screenWidthMode && screenHeight == screenHeightMode)
				{
					Cvar_Set(var_name, value, flag); // Set the default value
					GameHelper::UpdateCvar(var_name, StringHelper::IntegerToCString(i), flag); // Update 'r_mode' to the index of the matching screen resolution
					break;
				}
			}
		}
	}

	return Cvar_Set(var_name, value, flag);
}

// Hook to redirect texture file reads to correct paths in 'pak5_mod.pk3'
static int __cdecl FS_FOpenFileRead_Hook(char* Source, int* a2, int a3, int a4)
{
	// Fast check for "s/ch" substring in the file path and if the character name start with either 'c' or 'm'
	if (*reinterpret_cast<uint32_t*>(Source + 5) == 0x68632F73 && (Source[18] == 'c' || Source[18] == 'm'))
	{
		for (int i = 0; i < 9; i++)
		{
			// Check if the provided file path matches a known incorrect path
			if (strcmp(Source, correctPaths[i]) == 0)
			{
				// If a match is found, redirect to the corresponding correct texture path
				Source = (char*)pak5BrokenPaths[i];
				break;
			}
		}
	}

	return FS_FOpenFileRead(Source, a2, a3, a4);
}

// Reimplementation of 'sub_41D1E0' to fix free space reporting and ensure accurate target disk
static int __cdecl CheckDiskFreeSpace_Hook()
{
	unsigned __int64 freeBytesAvailable = 0;
	unsigned __int64 totalNumberOfBytes = 0;
	unsigned __int64 totalNumberOfFreeBytes = 0;

	if (GetDiskFreeSpaceExA(GetSavePath_Hook(), (PULARGE_INTEGER)&freeBytesAvailable, (PULARGE_INTEGER)&totalNumberOfBytes, (PULARGE_INTEGER)&totalNumberOfFreeBytes))
	{
		// Convert the available free bytes to KB
		unsigned __int64 freeSpaceInKB = freeBytesAvailable >> 10;

		// If the free space is >= 0x80000000, cap it at 0x7FFFFFFF instead of 0x80000000
		if (freeSpaceInKB >= 0x80000000)
		{
			freeSpaceInKB = 0x7FFFFFFF;
		}

		return (int)freeSpaceInKB;
	}

	// Return a default value if something goes wrong
	return 2048;
}

// Make sure Alice is not looking at the top left of the screen when using a controller
static float __cdecl UpdateHeadOrientation_Hook(DWORD* a1, float* a2)
{
	if (isInSettingMenu && *a1 == 0xC1B40000 && *(a1 + 1) == 0xC2100000 && *(a1 + 2) == 0)
	{
		*a1 = 0xC119EB80;
		isInSettingMenu = false;
	}

	return UpdateHeadOrientation(a1, a2);
}

// Hook to check if we are in the setting menu and not in-game 
static int __fastcall UpdateHeadOrientationFromMouse_Hook(int thisPtr, int*, float* a2, int a3, float* a4, float* a5)
{
	isInSettingMenu = true;
	return UpdateHeadOrientationFromMouse(thisPtr, a2, a3, a4, a5);
}

// Initialize the cvars used for the VorpalFix menu
static int __cdecl SetupOpenGLParameters_Hook()
{
	// Current game language
	int lang = MemoryHelper::ReadMemory<int>(CURRENT_LANG);

	VF_LANGUAGE_PTR = Cvar_Set("vf_language", StringHelper::IntegerToCString(lang), 0);
	VF_REMASTERED_MODELS_PTR = Cvar_Set("vf_remastered_models", StringHelper::BoolToCString(!DisableRemasteredModels), 0);
	VF_UI_CONSOLE_HUD_PTR = Cvar_Set("vf_ui_console_hud", StringHelper::BoolToCString(ConsolePortHUD), 0);
	VF_UI_PS3_PTR = Cvar_Set("vf_ui_ps3", StringHelper::BoolToCString(UsePS3ControllerIcons), 0);
	VF_UI_LETTERBOX_PTR = Cvar_Set("vf_ui_letterbox", StringHelper::BoolToCString(!DisableLetterbox), 0);
	VF_R_EXT_MAX_ANISOTROPY_PTR = Cvar_Set("vf_r_ext_max_anisotropy", StringHelper::FloatToCString(MaxAnisotropy), 0);
	VF_COM_MAXFPS_PTR = Cvar_Set("vf_com_maxfps", isScreenRateFps ? "-1" : StringHelper::IntegerToCString(CustomFPSLimit), 0);

	// We only need to do that once
	MH_DisableHook((void*)0x46E0D0);
	return SetupOpenGLParameters();
}

// Hook of the function used by the "ui_setcvars" command, used to save the VorpalFix menu settings to the INI
static DWORD __fastcall UISetCvars_Hook(DWORD* thisPtr, int*, char* group_name)
{
	// Apply the changes
	if (strcmp(group_name, "group_vf") == 0)
	{
		// LanguageId - Need a restart
		int languageId = MemoryHelper::ReadMemory<int>(VF_LANGUAGE_PTR + 0x20);
		IniHelper::iniReader["General"]["LanguageId"] = StringHelper::IntegerToCString(languageId);

		// DisableRemasteredModels - Need a restart
		bool disableRemasteredModels = MemoryHelper::ReadMemory<int>(VF_REMASTERED_MODELS_PTR + 0x20);
		IniHelper::iniReader["General"]["DisableRemasteredModels"] = StringHelper::IntegerToCString(!disableRemasteredModels);

		// ConsolePortHUD
		ConsolePortHUD = MemoryHelper::ReadMemory<int>(VF_UI_CONSOLE_HUD_PTR + 0x20);
		IniHelper::iniReader["Display"]["ConsolePortHUD"] = StringHelper::IntegerToCString(ConsolePortHUD);

		// UsePS3ControllerIcons - Need a restart
		int usePS3ControllerIcons = MemoryHelper::ReadMemory<int>(VF_UI_PS3_PTR + 0x20);
		IniHelper::iniReader["Display"]["UsePS3ControllerIcons"] = StringHelper::IntegerToCString(usePS3ControllerIcons);

		// DisableLetterbox
		bool isLetterboxEnabled = MemoryHelper::ReadMemory<int>(VF_UI_LETTERBOX_PTR + 0x20);
		IniHelper::iniReader["Display"]["DisableLetterbox"] = StringHelper::IntegerToCString(!isLetterboxEnabled);
		DisableLetterbox = !isLetterboxEnabled;

		// MaxAnisotropy
		float maxAnisotropy = MemoryHelper::ReadMemory<float>(VF_R_EXT_MAX_ANISOTROPY_PTR + 0x1C);
		IniHelper::iniReader["Graphics"]["MaxAnisotropy"] = StringHelper::FloatToCString(maxAnisotropy);

		// Execute vid_restart to refresh the textures
		if (maxAnisotropy != MaxAnisotropy)
		{
			MaxAnisotropy = maxAnisotropy;
			isAnisotropyRetrieved = false;
			GameHelper::VidRestart();
		}

		// CustomFPSLimit
		CustomFPSLimit = MemoryHelper::ReadMemory<int>(VF_COM_MAXFPS_PTR + 0x20);
		IniHelper::iniReader["Graphics"]["CustomFPSLimit"] = StringHelper::IntegerToCString(CustomFPSLimit);

		// Set to monitor's refresh rate
		if (CustomFPSLimit == -1)
		{
			CustomFPSLimit = SystemHelper::GetCurrentDisplayFrequency();
			if (CustomFPSLimit > 500)
			{
				CustomFPSLimit = 500;
			}
		}

		// Write the new value
		GameHelper::UpdateCvar("com_maxfps", StringHelper::IntegerToCString(CustomFPSLimit), 1);

		// Update the INI
		IniHelper::Save();
	}

	return UISetCvars(thisPtr, group_name);
}

// Function executed during the unzipping phase to check for specific file presence
static BYTE __cdecl Str_To_Lower_Hook(char* Buffer)
{
	// Check if using the original 'pak5_mod.pk3' with broken paths
	if (!isUsingBrokenPak5 && FixPak5 && StringHelper::stricmp(Buffer, pak5BrokenPaths[0]))
	{
		HookHelper::ApplyHook((void*)0x41A590, &FS_FOpenFileRead_Hook, (LPVOID*)&FS_FOpenFileRead);
		isUsingBrokenPak5 = true;
	}

	// Check if 'pak7_VorpalFix_menu.pk3' is used
	if (!isVFMenuUsed && StringHelper::stricmp(Buffer, "ui/control/vf_options.tga"))
	{
		HookHelper::ApplyHook((void*)0x46E0D0, &SetupOpenGLParameters_Hook, (LPVOID*)&SetupOpenGLParameters);
		HookHelper::ApplyHook((void*)0x4B9FD0, &UISetCvars_Hook, (LPVOID*)&UISetCvars);
		isVFMenuUsed = true;
	}

	// Disable this hook, nothing else to check
	if (StringHelper::stricmp(Buffer, "opengl32"))
	{
		MH_DisableHook((void*)0x4256E0);
	}

	return Str_To_Lower(Buffer);
}

// Function used to open and load the pk3 files
static FILE __cdecl FS_LoadZipFile_Hook(const char* FileName)
{
	// Skip pak5_mod.pk3 and disable the hook
	if (strstr(FileName, "\pak5_mod.pk3\x00"))
	{
		FileName = "\x00";
		MH_DisableHook((void*)0x43E030);
	}

	return FS_LoadZipFile(FileName);
}

// Adjust the HUD position and scaling for non-4:3 aspect ratios
static int __cdecl PrepareHUDRendering_Hook(float x_position, float y_position, float resolution_width, float resolution_height, int a5, float* a6, float* a7, float* a8, int a9, const char* a10, __int16 a11, float* a12, float* a13, float a14, float a15, int a16)
{
	if (isWiderThan4By3)
	{
		if (FixStretchedHUD)
		{
			// Step 1: Unstretch HUD
			float hud_object_x_position = (x_position * 640.0f) / resolution_width;
			resolution_width *= scaleFactor; // Adjust width to maintain aspect ratio

			// Step 2: 4:3 Position
			x_position = currentWidth / 640.0f * hud_object_x_position;
			x_position = (x_position - currentWidth / 2.0f) * scaleFactor + currentWidth / 2.0f;

			// Step 3: Center the HUD horizontally by adjusting for the width difference between 4:3 and the current aspect ratio
			if (hud_object_x_position > 0) // Elements on the right side
			{
				x_position += widthDifference;
			}
			else if (hud_object_x_position < 0) // Elements on the left side
			{
				x_position -= widthDifference;
			}
		}

		if (ConsolePortHUD)
		{
			if (x_position < 0)
			{
				x_position += currentWidth / consoleHudAdjustmentDivisor;
			}

			if (x_position > 0)
			{
				x_position -= currentWidth / consoleHudAdjustmentDivisor;
			}

			y_position -= currentHeight / 10.0f;
		}
	}

	return PrepareHUDRendering(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
}

// Hook of the function used by the "loadgame" and "savegame" commands, to determine if those can be used
static int __cdecl IsGameStarted_Hook()
{
	// Don't add a delay when loading a save from the UI
	if (isLoadingSaveFromMenuButton)
	{
		isLoadingSaveFromMenuButton = false;
		return IsGameStarted();
	}

	// Loading a quicksave while the menu is transitioning can cause issues
	if (FixMenuTransitionTiming && MemoryHelper::ReadMemory<uint8_t>(IS_MENU_LOCKED))
	{
		return 0;
	}

	if (!UseConsoleTitleScreen || isMainMenuShown)
	{
		return IsGameStarted();
	}
	else
	{
		// Block loadgame/savegame commands on the title screen
		return 0;
	}
}

// Hook of the "pushmenu" command
static int __cdecl PushMenu_Hook(const char* menu_name)
{
	if (FixMenuTransitionTiming && strcmp(menu_name, "newgame") == 0)
	{
		// Clicking too fast and one time on any difficulty won't remove the menu from the FMV, add a delay
		MemoryHelper::WriteMemory<int>(UI_WAIT_TIMER, 1200, false);
	}

	// Override the main menu with the title screen
	if (!isTitleBgSet && UseConsoleTitleScreen && strcmp(menu_name, "main") == 0)
	{
		// Don't show the title screen if the game has already started
		int start_state = MemoryHelper::ReadMemory<int>(STARTUP_STATE_ADDR);
		if (start_state == 4 || start_state == 6 || start_state == 100)
		{
			// Hide the cursor on the title screen
			GameHelper::ResizeCursor(true, currentWidth, currentHeight);
			isTitleBgSet = true;
			return PushMenu("title");
		}
		else
		{
			// Skipped
			isTitleBgSet = true;
			return PushMenu(menu_name);
		}
	}
	else
	{
		return PushMenu(menu_name);
	}
}

// Hook of the "forcemenu" command
static int __cdecl ForceMenu_Hook(const char* menu_name)
{
	// Check if the start screen is skipped
	if (MemoryHelper::ReadMemory<int>(STARTUP_STATE_ADDR) == 1)
	{
		UseConsoleTitleScreen = false;
	}

	return ForceMenu(menu_name);
}

// Hook of the function used to trigger the main menu when 'Escape' is pressed
static void __cdecl TriggerMainMenu_Hook(int a1)
{
	if (isMainMenuShown)
	{
		// We no longer need this hook
		MH_DisableHook((void*)0x44C4F0);
		TriggerMainMenu(a1);
	}
	else
	{
		// Hack to force the main menu when on the title screen, otherwise black screen
		PushMenu_Hook("main");
	}
}

// Adjust the vertical alignment of the text in the dialog box for two-line messages
static void __fastcall ShowDialogBoxText_Hook(int thisPtr, int*)
{
	// Hide dialog box when still in menu
	if (!MemoryHelper::ReadMemory<uint8_t>(IS_MENU_LOCKED))
	{
		// Adjust vertical position for 2-line messages (4 -> 3)
		if (*(DWORD*)(thisPtr + 0x1D8) == 4)
		{
			*(DWORD*)(thisPtr + 0x1D8) = 3;
		}

		ShowDialogBoxText(thisPtr);
	}
}

// Hook function to set 'isLoadingSaveFromMenuButton' to true
static void __fastcall LoadSaveFromUI_Hook(DWORD* thisPtr, int*, int a2)
{
	isLoadingSaveFromMenuButton = true;
	LoadSaveFromUI(thisPtr, a2);
}

// Hook function used to load the localization pk3 file
static const char* __cdecl LoadLocalizationFile_Hook()
{
	if (!hasLookedForLocalizationFiles)
	{
		int lang = MemoryHelper::ReadMemory<int>(CURRENT_LANG);

		std::string searchPath = "";
		switch (lang)
		{
			case 1: searchPath = "base/loc/DEU/"; break;
			case 2: searchPath = "base/loc/FRA/"; break;
			case 3: searchPath = "base/loc/ESN/"; break;
			default: searchPath = "base/loc/INT/"; break;
		}

		// Get *.pk3 inside "base/loc/<lang>/"
		pk3LocFiles = SystemHelper::GetLocPk3Files(searchPath);

		// Get the number of files to load
		localizationFilesToLoad = pk3LocFiles.size();

		// Loop back to this function if we got more than one file to load
		if (localizationFilesToLoad > 0)
		{
			// Fix stack pointer after the CALL
			uint8_t opCodeArray[] = { 0x83, 0xEC, 0x0C };
			MemoryHelper::WriteMemoryRaw(0x41CB4B, opCodeArray, sizeof(opCodeArray));

			// Loop back to 4615F0 for every files
			MemoryHelper::MakeCALL(0x41CB4E, 0x41CB32);
		}

		// Load the original localization file
		hasLookedForLocalizationFiles = true;
		return LoadLocalizationFile();
	}

	// If we still have files to load
	if (localizationFilesToLoad > 0)
	{
		int currentIndex = pk3LocFiles.size() - localizationFilesToLoad;
		localizationFilesToLoad--;

		if (localizationFilesToLoad == 0)
		{
			// No more files, restore original instructions
			uint8_t opCodeArray[] = { 0x33, 0xFF, 0x39, 0x7C, 0x24, 0x0C, 0x7E, 0x3F };
			MemoryHelper::WriteMemoryRaw(0x41CB4B, opCodeArray, sizeof(opCodeArray));
		}

		// Return the file path
		return pk3LocFiles[currentIndex].c_str();
	}

	return LoadLocalizationFile();
}

// Hook of the function used to parse the controller state, add some more features
static MMRESULT __cdecl UpdateControllerState_Hook()
{
	int xinput_state = MemoryHelper::ReadMemory<int>(0x7CF880);

	// Save current weapon to d-pad
	if (xinput_state & 0x40) // If left stick is pressed
	{
		isHoldingLeftStick = true;

		int dpadId = -1;
		switch (xinput_state & 0x0F)
		{
			case 0x01: // D-Pad Up
				dpadId = 1;
				break;
			case 0x02: // D-Pad Down
				dpadId = 2;
				break;
			case 0x04: // D-Pad Left
				dpadId = 3;
				break;
			case 0x08: // D-Pad Right
				dpadId = 4;
				break;
			default:
				dpadId = -1; // No matching D-Pad button pressed
				break;
		}

		int currentWeaponId = MemoryHelper::ReadMemory<int>(CURRENT_WEAPON_ID);
		if (dpadId != -1 && currentWeaponId != -1) // If dpad is pressed and if holding a weapon
		{
			char** lastWeaponSet = nullptr;

			switch (dpadId)
			{
				case 1:
					lastWeaponSet = &lastWeaponIdUp;
					break;
				case 2:
					lastWeaponSet = &lastWeaponIdDown;
					break;
				case 3:
					lastWeaponSet = &lastWeaponIdLeft;
					break;
				case 4:
					lastWeaponSet = &lastWeaponIdRight;
					break;
			}

			char* currentWeaponName = GameHelper::GetWeaponName(currentWeaponId);

			// If holding a different weapon
			if (*lastWeaponSet == nullptr || strcmp(currentWeaponName, *lastWeaponSet) != 0)
			{
				// Build the command
				char* weaponCommand = (char*)malloc(strlen("use ") + strlen(currentWeaponName) + 1);
				if (weaponCommand != NULL)
				{
					sprintf(weaponCommand, "use %s", currentWeaponName);

					// Free the old weapon name
					if (*lastWeaponSet != nullptr)
					{
						free(*lastWeaponSet);
					}

					// Remember the last weapon name
					*lastWeaponSet = strdup(currentWeaponName);

					// Bind the new command
					switch (dpadId)
					{
						case 1:
							Bind(GameHelper::GetKeyId("JOY5"), weaponCommand);
							break;
						case 2:
							Bind(GameHelper::GetKeyId("JOY7"), weaponCommand);
							break;
						case 3:
							Bind(GameHelper::GetKeyId("JOY8"), weaponCommand);
							break;
						case 4:
							Bind(GameHelper::GetKeyId("JOY6"), weaponCommand);
							break;
					}
				}
			}
		}
	}
	else
	{
		isHoldingLeftStick = false;
	}

	// Right Stick Pressed + A
	if ((xinput_state & 0x1080) == 0x1080)
	{
		static auto lastQuickActionTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(1000);
		auto now = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastQuickActionTime).count() >= 1000)
		{
			CallCmd("loadgame quicksave\n", 0);
			lastQuickActionTime = now;
		}
	}

	// Right Stick Pressed + B
	if ((xinput_state & 0x2080) == 0x2080)
	{
		static auto lastQuickActionTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(1000);
		auto now = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastQuickActionTime).count() >= 1000)
		{
			CallCmd("savegame quicksave\n", 0);
			lastQuickActionTime = now;
		}
	}

	return UpdateControllerState();
}

// Force the game to exit if Alt+F4 is pressed
static int __stdcall lpfnWndProc_MSG_Hook(HWND hWnd, UINT Msg, int wParam, LPARAM lParam)
{
	// Alt+F4
	if (Msg == WM_SYSKEYDOWN && wParam == VK_F4)
	{
		Msg = WM_CLOSE;
	}

	return lpfnWndProc_MSG(hWnd, Msg, wParam, lParam);
}

// Hook for the function that takes a screenshot for the saved snapshot, ensuring it is captured as if the game is running in a 4:3 aspect ratio
static int __cdecl TakeSaveScreenshot_Hook(int a1, int a2, int a3)
{
	if (isWiderThan4By3)
	{
		// Tell glReadPixels_Hook to update the x position to simulate a 4:3 screenshot
		isTakingSaveScreenshot = true;
	}

	return TakeSaveScreenshot(a1, a2, a3);
}

// Used during OpenGL refresh
static int __cdecl GLW_CreatePFD_Hook(void* pPFD, unsigned __int8 colorbits, char depthbits, unsigned __int8 stencilbits, int stereo)
{
	// Resolution updated, cache the new variables
	currentWidth = MemoryHelper::ReadMemory<int>(CURRENT_WIDTH_ADDR);
	currentHeight = MemoryHelper::ReadMemory<int>(CURRENT_HEIGHT_ADDR);
	currentAspectRatio = static_cast<float>(currentWidth) / static_cast<float>(currentHeight);
	isWiderThan4By3 = currentAspectRatio > ASPECT_RATIO_4_3;

	if (isWiderThan4By3)
	{
		scaledWidth = currentHeight * ASPECT_RATIO_4_3;
		scaleFactor = scaledWidth / currentWidth;
		widthDifference = ((currentWidth - scaledWidth) / 2.0f);
		consoleHudAdjustmentDivisor = 17.0f * (16.0f / 9.0f) / currentAspectRatio;
	}

	// If we are going to update how the width is read during a save screenshot
	if (FixStretchedGUI || FixSaveScreenshotBufferOverflow)
	{
		int screenshotWidth = (FixStretchedGUI && isWiderThan4By3) ? static_cast<int>(scaledWidth) : currentWidth;

		// Check if screenshotWidth is not a multiple of 4
		if (FixSaveScreenshotBufferOverflow && screenshotWidth % 4 != 0)
		{
			// Adjust the width to be the largest multiple of 4 less than currentWidth
			screenshotWidth = screenshotWidth - (screenshotWidth % 4);
		}

		// Write the new width to a safe location
		MemoryHelper::WriteMemory<int>(CODE_CAVE_WIDTH, screenshotWidth);

		// Redirect the width read by sub_46D280 to the safe width
		MemoryHelper::WriteMemory<int>(0x46D28B, CODE_CAVE_WIDTH);
		MemoryHelper::WriteMemory<int>(0x46D307, CODE_CAVE_WIDTH);
		MemoryHelper::WriteMemory<int>(0x46D321, CODE_CAVE_WIDTH);
		MemoryHelper::WriteMemory<int>(0x46D3B7, CODE_CAVE_WIDTH);
	}

	// Update the cursor size
	if (isMainMenuShown)
	{
		isCursorResized = false;
	}

	// Skip console title screen for aspect ratios greater than 21:9 
	if (UseConsoleTitleScreen && currentAspectRatio > 2.0f)
	{
		UseConsoleTitleScreen = false;
	}

	return GLW_CreatePFD(pPFD, colorbits, depthbits, stencilbits, stereo);
}

// Function used to adjust the resolution settings before the initialization of OpenGL
static int __cdecl QGL_Init_Hook(LPCSTR lpLibFileName)
{
	if (!ForceBorderlessFullscreen)
	{
		// Set current resolution mode to 0
		GameHelper::UpdateCvar("r_mode", "0", 33);

		// Replace resolution 0 with the custom one
		MemoryHelper::WriteMemory<int>(DISPLAY_MODE_ARRAY_WIDTH_ADDR, CustomResolutionWidth, false);
		MemoryHelper::WriteMemory<int>(DISPLAY_MODE_ARRAY_HEIGHT_ADDR, CustomResolutionHeight, false);
	}

	// We only have to do this once
	MH_DisableHook((void*)0x47ABE0);

	return QGL_Init(lpLibFileName);
}

// Scale the font
static void __cdecl RE_StretchFont_Hook(int a1, BYTE a2, float font_x_position, float font_y_position, float a5, float a6, float font_spacing, float font_scale_width, float font_scale_height, int a10)
{
	// Don't mess with the console or with the credits
	if (font_x_position <= 3.0 || font_spacing == -8.0)
	{
		RE_StretchFont(a1, a2, font_x_position, font_y_position, a5, a6, font_spacing, font_scale_width, font_scale_height, a10);
		return;
	}

	if (isWiderThan4By3)
	{
		font_x_position = font_x_position * scaleFactor + widthDifference;
		font_scale_width *= scaleFactor;
	}

	RE_StretchFont(a1, a2, font_x_position, font_y_position, a5, a6, font_spacing, font_scale_width, font_scale_height, a10);
}

// Adjust the menu position and scaling for non-4:3 aspect ratios
static int __cdecl RE_StretchPic_Hook(float x_position, float y_position, float resolution_width, float resolution_height, float a5, float a6, float a7, float a8, int ShaderHandle)
{
	// Pillarbox borders
	if (x_position == LEFT_BORDER_X_ID || x_position == RIGHT_BORDER_X_ID)
	{
		float scale_factor = currentHeight / 720.0f;
		resolution_width = 320.0f * scale_factor;
		resolution_height = currentHeight;

		x_position = (x_position == LEFT_BORDER_X_ID) ? widthDifference - resolution_width : currentWidth - widthDifference;

		return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
	}
	else
	{
		char* ShaderName = *(char**)(SHADERS_CACHE_ADDR + 0x4 * ShaderHandle);
		BYTE isConsoleOpen = (*(BYTE**)CONSOLE_THREAD_PTR_ADDR && *(*(BYTE**)CONSOLE_THREAD_PTR_ADDR + 0xCC)) ? *(*(BYTE**)CONSOLE_THREAD_PTR_ADDR + 0xCC) : 0;

		// Resize the mouse when needed
		if (!isCursorResized && strcmp(ShaderName, "gfx/2d/mouse_arrow") == 0)
		{
			GameHelper::ResizeCursor(isUsingControllerMenu, currentWidth, currentHeight);
			GameHelper::ResizePopupMessage(currentWidth, currentHeight); // Does not scale well with this function
			isCursorResized = true;
		}

		// Workaround to be able to use the console
		if (isConsoleOpen == 1 && strcmp(ShaderName, "gfx/2d/mouse_arrow") == 0)
		{
			return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Once we get to the main menu
		if (!isMainMenuShown && strstr(ShaderName, "main"))
		{
			// Resize the cursor if hidden for the title screen
			GameHelper::ResizeCursor(isUsingControllerMenu, currentWidth, currentHeight);
			isMainMenuShown = true;
		}

		// Handle pre-main menu elements that need scaling and centering
		if (!isMainMenuShown)
		{
			// Scale and center legalplate
			if (strstr(ShaderName, "legalplate"))
			{
				if (currentWidth > 1280)
				{
					float scale_factor = (float)currentHeight / 720.0f;
					resolution_width *= scale_factor;
					resolution_height *= scale_factor;
					x_position = (currentWidth - resolution_width) / 2.0f;
					y_position = (currentHeight - resolution_height) / 2.0f;
				}

				return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
			}

			// Scale and center title screen background
			if (strcmp(ShaderName, "title_bg") == 0)
			{
				float titlebg_aspect_ratio = 1280.0f / 720.0f;
				float scale_factor = (currentAspectRatio > titlebg_aspect_ratio) ? (float)currentWidth / 1280.0f : (float)currentHeight / 720.0f;

				resolution_width = 1280 * scale_factor;
				resolution_height = 720 * scale_factor;
				x_position = (currentWidth - resolution_width) / 2.0f;
				y_position = (currentHeight - resolution_height) / 2.0f;

				return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
			}
		}

		// Full screen effects
		if (strcmp(ShaderName, "textures/special/drugfade") == 0)
		{
			return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Pop-up message that does not scale well with this function
		if (strcmp(ShaderName, "ui/control/press_any_key") == 0)
		{
			return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Console Elements
		if (isConsoleOpen && x_position == 0 && (strcmp(ShaderName, "ui/control/slider2_bar") == 0 || strcmp(ShaderName, "ui/control/slider2_top") == 0 || strcmp(ShaderName, "ui/control/slider2_indicator") == 0))
		{
			return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Credits
		if (strcmp(ShaderName, "ui/bar") == 0 || strcmp(ShaderName, "ui/credits/alice") == 0 || strcmp(ShaderName, "ui/credits/bill") == 0)
		{
			return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Scale the UI
		if (isWiderThan4By3)
		{
			resolution_width *= scaleFactor;

			// Save camera position similar to the console version
			if (strcmp(ShaderName, "ui/quicksavecam/quicksavecam") == 0)
			{
				if (ConsolePortHUD)
				{
					x_position = currentWidth / 6.0f;
					y_position = currentHeight / 14.25f;
				}
			}
			else
			{
				// Don't change the x position of the save camera
				x_position = (x_position * scaleFactor) + widthDifference;
			}
		}
	}

	return RE_StretchPic(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
}

// Adjust the FMV position and scaling for non-4:3 aspect ratios
static int __cdecl RE_StretchRaw_Hook(int x_position, int y_position, int resolution_width, int resolution_height, int a5, int a6, int a7)
{
	if (isWiderThan4By3)
	{
		x_position = static_cast<int>(widthDifference);
		resolution_width = static_cast<int>(scaledWidth);
	}

	return RE_StretchRaw(x_position, y_position, resolution_width, resolution_height, a5, a6, a7);
}

// Adjust the scaling of the 'AutoScroll' control in credits
static void __cdecl UpdateRenderContext_Hook(int x, int y, int width, int height, float a5, float a6, float a7, float a8, float a9, float a10)
{
	if (isWiderThan4By3 && x != 0 && !isRenderingConsole)
	{
		if (MemoryHelper::ReadMemory<int>(IS_IN_MENU)) // Autoscroll
		{
			int adjusted_x = (x * scaleFactor) + widthDifference;
			int adjusted_width = width * scaleFactor;

			UpdateRenderContext(adjusted_x, y, adjusted_width, height, a5, a6, a7, a8, a9, a10);
		}
		else // dialog
		{
			// Make sure to display the borders while transitioning from menu to in-game
			if (MemoryHelper::ReadMemory<uint8_t>(IS_MENU_LOCKED))
			{
				return;
			}

			int adjusted_x = x * scaleFactor;
			UpdateRenderContext(adjusted_x, y, width, height, a5, a6, a7, a8, a9, a10);
		}
	}
	else
	{
		UpdateRenderContext(x, y, width, height, a5, a6, a7, a8, a9, a10);
	}
}

// Used right after 'UpdateRenderContext', do the same adjustements
static void __cdecl ConfigureScissor_Hook(int x, int y, int width, int height)
{
	if (isWiderThan4By3 && x != 0 && !isRenderingConsole) // Autoscroll
	{
		if (MemoryHelper::ReadMemory<int>(IS_IN_MENU))
		{
			int adjusted_x = (x * scaleFactor) + widthDifference;
			int adjusted_width = width * scaleFactor;

			ConfigureScissor(adjusted_x, y, adjusted_width, height);
		}
		else // dialog
		{
			// Make sure to display the borders while transitioning from menu to in-game
			if (MemoryHelper::ReadMemory<uint8_t>(IS_MENU_LOCKED))
			{
				return;
			}

			int adjusted_x = x * scaleFactor;
			ConfigureScissor(adjusted_x, y, width, height);
		}
	}
	else
	{
		ConfigureScissor(x, y, width, height);
	}
}

// Detect if the console is going to be used in 'UpdateRenderContext', we don't want that to happen while in the credits or during a dialog
static DWORD __fastcall UpdateAndConfigureRenderContext_Hook(int thisPtr, int*)
{
	int x = *(int*)(thisPtr + 0x38);
	BYTE isEnabled = *(BYTE*)(thisPtr + 0x1C1);

	if (x != 0 && isEnabled)
	{
		isRenderingConsole = true;
	}
	else
	{
		isRenderingConsole = false;
	}
	
	return UpdateAndConfigureRenderContext(thisPtr);
}

// Load the menu files from 'pak6_VorpalFix.pk3' when required
static DWORD __fastcall LoadUI_Hook(DWORD* thisPtr, int*, char* ui_path)
{
	int lang = MemoryHelper::ReadMemory<int>(CURRENT_LANG);

	const char* langPrefix = "INT";
	switch (lang)
	{
		case 1: langPrefix = "DEU"; break;
		case 2: langPrefix = "FRA"; break;
		case 3: langPrefix = "ESN"; break;
	}

	// Check if a controller is connected
	if (EnableControllerIcons && !isUsingControllerMenu)
	{
		if (GameHelper::IsControllerConnected() == 1)
		{
			isUsingControllerMenu = true;

			// Disable mouse navigation
			MemoryHelper::MakeNOP(0x40675E, 2);
			MemoryHelper::MakeNOP(0x40676E, 2);

			// For Alice's 3d model in the settings
			HookHelper::ApplyHook((void*)0x423740, &UpdateHeadOrientation_Hook, (LPVOID*)&UpdateHeadOrientation);
			HookHelper::ApplyHook((void*)0x4C6050, &UpdateHeadOrientationFromMouse_Hook, (LPVOID*)&UpdateHeadOrientationFromMouse);
		}
	}

	// Use the proper title screen file
	if (UseConsoleTitleScreen && ui_path != NULL && strcmp(ui_path, "ui/title.urc") == 0)
	{
		ui_path = StringHelper::ConstructPath(langPrefix, isUsingControllerMenu ? "/title_console.urc" : "/title.urc");
	}

	// Override using the menus located inside 'pak6_VorpalFix.pk3'
	if (isUsingControllerMenu) 
	{
		if (UsePS3ControllerIcons) 
		{
			langPrefix = StringHelper::ConstructPath(langPrefix, "_ps3");
		}

		if (ui_path && strcmp(ui_path, "ui/controls.urc") == 0) 
		{
			ui_path = StringHelper::ConstructPath(langPrefix, "/controls2.urc");
		}
		else if (ui_path && strcmp(ui_path, "ui/credits.urc") == 0) 
		{
			ui_path = StringHelper::ConstructPath(langPrefix, "/credits2.urc");
		}
		else if (ui_path && strcmp(ui_path, "ui/loadsave.urc") == 0) 
		{
			ui_path = StringHelper::ConstructPath(langPrefix, "/loadsave2.urc");
		}
		else if (ui_path && strcmp(ui_path, "ui/main.urc") == 0) 
		{
			ui_path = StringHelper::ConstructPath(langPrefix, "/main2.urc");
		}
		else if (ui_path && strcmp(ui_path, "ui/newgame.urc") == 0) 
		{
			ui_path = StringHelper::ConstructPath(langPrefix, "/newgame2.urc");
		}
		else if (ui_path && strcmp(ui_path, "ui/quit.urc") == 0) 
		{
			ui_path = StringHelper::ConstructPath(langPrefix, "/quit2.urc");
		}
	}

	return LoadUI(thisPtr, ui_path);
}

// Adjust the scaling and position of Alice's 3D model in the settings menu
static DWORD* __cdecl SetAliceMirrorViewportParams_Hook(DWORD* a1, float param_x, float param_y, float param_width, float param_height, float param_fov)
{
	DWORD* renderEntity = SetAliceMirrorViewportParams(a1, param_x, param_y, param_width, param_height, param_fov);

	if (isWiderThan4By3)
	{
		// X position
		renderEntity[0] = static_cast<int>(widthDifference);
		// Width
		renderEntity[2] = static_cast<int>(scaledWidth);
	}

	return renderEntity;
}

// Adds borders to hide the pillarbox when scaling the menu
static void __cdecl SetUIBorder_Hook()
{
	if (!isWiderThan4By3) return;

	int border = GameHelper::UI_GetStaticMap(18, "border1_left.tga");
	RE_StretchPic_Hook(LEFT_BORDER_X_ID, 0, 0, 0, 0.0, 0.0, 1.0, 1.0, *(DWORD*)(border + 4));
	RE_StretchPic_Hook(RIGHT_BORDER_X_ID, 0, 0, 0, 1.0, 0.0, 0.0, 1.0, *(DWORD*)(border + 4));
}

// Update 'BLINK_TIMER' at a consistent rate, independent of frame rate
static void UpdateBlinkTimer()
{
	using namespace std::chrono;
	static auto lastUpdate = steady_clock::now();

	const auto now = steady_clock::now();
	const auto elapsed = duration_cast<milliseconds>(now - lastUpdate);

	if (elapsed >= 50ms) 
	{
		MemoryHelper::WriteMemory<int>(BLINK_TIMER, MemoryHelper::ReadMemory<int>(BLINK_TIMER) + 1, false);
		lastUpdate = now;
	}
}

// Hook to update 'BLINK_TIMER' and jump to the original code
__declspec(naked) static void BlinkAnimationHookStub()
{
	__asm 
	{
		pushad
		pushfd
		call UpdateBlinkTimer
		popfd
		popad
		jmp[CODE_CAVE_BLINK]
	}
}

#pragma endregion Hooks with MinHook

#pragma region

static void ApplyFixSoundRandomization()
{
	if (!FixSoundRandomization) return;

	// Instructions from the 2000 version, ported to the remaster
	uint8_t portedInstructions[0x3C0] =
	{
		// sub_423770
		0x8B, 0x44, 0x24, 0x0C, 0x8B, 0x4C, 0x24, 0x10, 0x83, 0xEC, 0x28, 0xC7, 0x00, 0xFF, 0xFF, 0xFF,
		0xFF, 0x8B, 0x44, 0x24, 0x2C, 0x53, 0x55, 0x56, 0x57, 0x85, 0xC0, 0xC7, 0x01, 0xFF, 0xFF, 0xFF,
		0xFF, 0x0F, 0x84, 0xE6, 0x01, 0x00, 0x00, 0x8B, 0x5C, 0x24, 0x40, 0x85, 0xDB, 0x0F, 0x84, 0xDA,
		0x01, 0x00, 0x00, 0x8B, 0x48, 0x20, 0x85, 0xC9, 0x74, 0x09, 0x50, 0xE8, 0x30, 0xDF, 0xEE, 0xFF,
		0x83, 0xC4, 0x04, 0x8B, 0xFB, 0x83, 0xC9, 0xFF, 0x33, 0xC0, 0xF2, 0xAE, 0xF7, 0xD1, 0x49, 0x8B,
		0xE9, 0x89, 0x6C, 0x24, 0x14, 0x8D, 0x55, 0x01, 0x83, 0xFA, 0x20, 0x0F, 0x8F, 0xAC, 0x01, 0x00,
		0x00, 0x33, 0xF6, 0x85, 0xED, 0x7E, 0x22, 0x8B, 0xFB, 0x8D, 0x44, 0x24, 0x18, 0x2B, 0xF8, 0x8D,
		0x0C, 0x37, 0x0F, 0xBE, 0x54, 0x0C, 0x18, 0x52, 0xE8, 0x0E, 0x1A, 0xFC, 0xFF, 0x83, 0xC4, 0x04,
		0x88, 0x44, 0x34, 0x18, 0x46, 0x3B, 0xF5, 0x7C, 0xE6, 0x8B, 0x44, 0x24, 0x3C, 0x33, 0xDB, 0xC6,
		0x44, 0x34, 0x18, 0x00, 0x8B, 0x78, 0x24, 0x4F, 0x0F, 0x88, 0x6F, 0x01, 0x00, 0x00, 0x8B, 0x4C,
		0x24, 0x3C, 0x8D, 0x34, 0x1F, 0xD1, 0xFE, 0x8B, 0x51, 0x28, 0x55, 0x8D, 0x4C, 0x24, 0x1C, 0x8B,
		0x04, 0xB2, 0x50, 0x51, 0xE8, 0xF0, 0x17, 0xFC, 0xFF, 0x83, 0xC4, 0x0C, 0x85, 0xC0, 0x7D, 0x05,
		0x8D, 0x7E, 0xFF, 0xEB, 0x05, 0x7E, 0x0F, 0x8D, 0x5E, 0x01, 0x3B, 0xFB, 0x7D, 0xD0, 0x5F, 0x5E,
		0x5D, 0x5B, 0x83, 0xC4, 0x28, 0xC3, 0x3B, 0xFB, 0x0F, 0x8C, 0x2F, 0x01, 0x00, 0x00, 0x8B, 0x54,
		0x24, 0x44, 0x8B, 0x44, 0x24, 0x48, 0x8B, 0x4C, 0x24, 0x3C, 0xC7, 0x44, 0x24, 0x40, 0x00, 0x00,
		0x00, 0x00, 0x89, 0x32, 0x89, 0x30, 0x8B, 0x51, 0x28, 0xC7, 0x44, 0x24, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x85, 0xF6, 0x8B, 0xEE, 0x8D, 0x3C, 0xB2, 0x7C, 0x5C, 0x90, 0x90, 0x90, 0x90, 0x90, 0x8B,
		0x44, 0x24, 0x14, 0x8B, 0x0F, 0x50, 0x8D, 0x54, 0x24, 0x1C, 0x51, 0x52, 0xE8, 0x88, 0x17, 0xFC,
		0xFF, 0x83, 0xC4, 0x0C, 0x85, 0xC0, 0x75, 0x3E, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x8B, 0x07, 0x8B, 0x4C, 0x24, 0x14, 0x80, 0x3C, 0x08, 0x5F, 0x74, 0x24, 0xD9, 0x44, 0x24, 0x10,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x8B, 0x17, 0x8B, 0x44, 0x24, 0x44, 0x41, 0x83, 0xC3, 0x04,
		0xD8, 0x82, 0xA0, 0x00, 0x00, 0x00, 0x89, 0x4C, 0x24, 0x40, 0x89, 0x28, 0xD9, 0x5C, 0x24, 0x10,
		0x83, 0xEF, 0x04, 0x4D, 0x79, 0xA9, 0x8B, 0x44, 0x24, 0x3C, 0x8D, 0x7E, 0x01, 0x8B, 0x48, 0x28,
		0x8D, 0x34, 0xB9, 0x8B, 0x48, 0x24, 0x3B, 0xF9, 0x7D, 0x65, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x8B, 0x6C, 0x24, 0x14, 0x8B, 0x06, 0x55, 0x8D, 0x4C, 0x24, 0x1C,
		0x50, 0x51, 0xE8, 0x12, 0x17, 0xFC, 0xFF, 0x83, 0xC4, 0x0C, 0x85, 0xC0, 0x75, 0x41, 0x90, 0x90,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x8B, 0x06, 0x80, 0x3C, 0x28, 0x5F, 0x74, 0x24, 0xD9, 0x44,
		0x24, 0x10, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x8B, 0x16, 0x8B, 0x44,	0x24, 0x48, 0x41, 0x83,
		0xC3, 0x04, 0xD8, 0x82, 0xA0, 0x00, 0x00, 0x00, 0x89, 0x4C, 0x24, 0x40, 0x89, 0x38, 0xD9, 0x5C,
		0x24, 0x10, 0x8B, 0x4C, 0x24, 0x3C, 0x83, 0xC6, 0x04, 0x47, 0x3B, 0x79, 0x24, 0x7C, 0xA6, 0x8B,
		0x44, 0x24, 0x40, 0x85, 0xC0, 0x74, 0x12, 0xD9, 0x44, 0x24, 0x10, 0x8B, 0x54, 0x24, 0x4C, 0x5F,
		0x5E, 0x5D, 0xD9, 0x1A, 0x5B, 0x83, 0xC4, 0x28, 0xC3, 0x8B, 0x44, 0x24, 0x44, 0x8B, 0x4C, 0x24,
		0x48, 0xC7, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x5F, 0x5E, 0x5D,
		0x5B, 0x83, 0xC4, 0x28, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,

		// sub_423200
		0x51, 0x8D, 0x44, 0x24, 0x00, 0x56, 0x8B, 0x74, 0x24, 0x0C, 0x8D, 0x4C, 0x24, 0x0C, 0x50, 0x8B,
		0x44, 0x24, 0x14, 0x8D, 0x54, 0x24, 0x14, 0x51, 0x52, 0x50, 0x56, 0xE8, 0xC0, 0xFD, 0xFF, 0xFF,
		0x8B, 0x44, 0x24, 0x24, 0x83, 0xC4, 0x14, 0x83, 0xF8, 0xFF, 0x74, 0x3E, 0x3B, 0x44, 0x24, 0x0C,
		0x75, 0x38, 0x8B, 0x4E, 0x28, 0x8B, 0x14, 0x81, 0x8A, 0x44, 0x24, 0x18, 0x88, 0x82, 0xB4, 0x00,
		0x00, 0x00, 0x8B, 0x4E, 0x28, 0x8B, 0x54, 0x24, 0x10, 0x8B, 0x04, 0x91, 0x8B, 0x4C, 0x24, 0x1C,
		0x89, 0x88, 0xB8, 0x00, 0x00, 0x00, 0x8B, 0x56, 0x28, 0x8B, 0x44, 0x24, 0x10, 0x8B, 0x0C, 0x82,
		0x8B, 0x54, 0x24, 0x14, 0x89, 0x91, 0xB0, 0x00, 0x00, 0x00, 0x5E, 0x59, 0xC3, 0x90, 0x90, 0x90,

		// sub_423270
		0x51, 0x8D, 0x44, 0x24, 0x00, 0x57, 0x8B, 0x7C, 0x24, 0x0C, 0x8D, 0x4C, 0x24, 0x0C, 0x50, 0x8B,
		0x44, 0x24, 0x14, 0x8D, 0x54, 0x24, 0x14, 0x51, 0x52, 0x50, 0x57, 0xE8, 0x50, 0xFD, 0xFF, 0xFF,
		0x8B, 0x44, 0x24, 0x24, 0x83, 0xC4, 0x14, 0x83, 0xF8, 0xFF, 0x74, 0x4B, 0x3B, 0x44, 0x24, 0x0C,
		0x75, 0x45, 0x8B, 0x4F, 0x28, 0x56, 0x8B, 0x74, 0x24, 0x18, 0x8B, 0x14, 0x81, 0x56, 0x52, 0xE8,
		0x0C, 0xDC, 0xEE, 0xFF, 0x83, 0xC4, 0x08, 0x85, 0xC0, 0x75, 0x14, 0x8B, 0x47, 0x28, 0x8B, 0x4C,
		0x24, 0x14, 0x56, 0x8B, 0x14, 0x88, 0x52, 0xE8, 0x24, 0xDC, 0xEE, 0xFF, 0x83, 0xC4, 0x08, 0x8A,
		0x4C, 0x24, 0x20, 0x8B, 0x54, 0x24, 0x24, 0x88, 0x48, 0x08, 0x8B, 0x4C, 0x24, 0x1C, 0x89, 0x30,
		0x89, 0x50, 0x0C, 0x89, 0x48, 0x04, 0x5E, 0x5F, 0x59, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,

		// sub_4236C0
		0x83, 0xEC, 0x0C, 0x8D, 0x44, 0x24, 0x00, 0x8D, 0x4C, 0x24, 0x10, 0x8D, 0x54, 0x24, 0x14, 0x53,
		0x56, 0x57, 0x8B, 0x7C, 0x24, 0x1C, 0x50, 0x8B, 0x44, 0x24, 0x24, 0x51, 0x52, 0x50, 0x57, 0xE8,
		0xCC, 0xFC, 0xFF, 0xFF, 0x8B, 0x4C, 0x24, 0x34, 0x83, 0xC4, 0x14, 0x83, 0xF9, 0xFF, 0x74, 0x10,
		0x8B, 0x44, 0x24, 0x1C, 0x83, 0xF8, 0xFF, 0x74, 0x07, 0x2B, 0xC1, 0x40, 0x8B, 0xF0, 0xEB, 0x02,
		0x33, 0xF6, 0xC7, 0x44, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x21, 0xDF, 0xEE, 0xFF, 0xD8,
		0x4C, 0x24, 0x0C, 0x33, 0xC9, 0x85, 0xF6, 0xD9, 0x5C, 0x24, 0x14, 0x7E, 0x2D, 0x8B, 0x7F, 0x28,
		0x8B, 0x5C, 0x24, 0x20, 0xD9, 0x44, 0x24, 0x10, 0x8D, 0x14, 0x9F, 0x8B, 0x02, 0xD8, 0x80, 0xA0,
		0x00, 0x00, 0x00, 0xD9, 0x44, 0x24, 0x14, 0xD8, 0xD9, 0xDF, 0xE0, 0xF6, 0xC4, 0x01, 0x75, 0x13,
		0x41, 0x83, 0xC2, 0x04, 0x3B, 0xCE, 0x7C, 0xE3, 0xDD, 0xD8, 0x5F, 0x5E, 0x33, 0xC0, 0x5B, 0x83,
		0xC4, 0x0C, 0xC3, 0x03, 0xCB, 0xDD, 0xD8, 0x8B, 0x04, 0x8F, 0x5F, 0x5E, 0x83, 0xC0, 0x20, 0x5B,
		0x83, 0xC4, 0x0C, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};

	MemoryHelper::WriteMemoryRaw(CODE_CAVE_SOUND, portedInstructions, sizeof(portedInstructions));

	MemoryHelper::MakeCALL(0x402131, CODE_CAVE_SOUND + 0x310);
	MemoryHelper::MakeJMP(0x4348BF, CODE_CAVE_SOUND + 0x310);
	MemoryHelper::MakeJMP(0x43491F, CODE_CAVE_SOUND + 0x220);
	MemoryHelper::MakeJMP(0x43494F, CODE_CAVE_SOUND + 0x290);
}

static void ApplyFixHardDiskFull()
{
	if (!FixHardDiskFull) return;

	HookHelper::ApplyHook((void*)0x41D1E0, &CheckDiskFreeSpace_Hook, (LPVOID*)&CheckDiskFreeSpace);
}

static void ApplyFixDisplayModeBufferOverflow()
{
	// Used for resolution mode checking
	HookHelper::ApplyHook((void*)0x46F850, &FetchDisplayResolutions_Hook, (LPVOID*)&FetchDisplayResolutions);
}

static void ApplyFixBlinkingAnimationSpeed()
{
	if (!FixBlinkingAnimationSpeed) return;

	// mov	ecx, dword_11C35EC
	uint8_t originalCode[] = { 0x8B, 0x0D, 0xEC, 0x35, 0x1C, 0x01 };

	// Step 1: Copy original code to trampoline
	MemoryHelper::WriteMemoryRaw(CODE_CAVE_BLINK, originalCode, sizeof(originalCode));

	// Step 2: Add a jump back to the original code + 8 (0x4C6364), skipping 'add ecx, ebx' as we will handle that ourselves
	MemoryHelper::MakeJMP(CODE_CAVE_BLINK + sizeof(originalCode), 0x4C6364);

	// Step 3: Overwrite original code with JMP to our stub
	MemoryHelper::MakeNOP(0x4C635C, 8);
	MemoryHelper::MakeJMP(0x4C635C, reinterpret_cast<uintptr_t>(BlinkAnimationHookStub));

	// Make sure Alice's eyes aren't closed for too long
	MemoryHelper::WriteMemory<uint8_t>(0x4C6394, 0x01);
}

static void ApplyFixStretchedHUD()
{
	// Hook regardless of 'FixStretchedHUD' since 'ConsolePortHUD' can be toggled from the menu
	HookHelper::ApplyHook((void*)0x446050, &PrepareHUDRendering_Hook, (LPVOID*)&PrepareHUDRendering);
	
	// Improve HUD positioning
	if (FixStretchedHUD)
	{
		// Show more of the switching animation
		MemoryHelper::WriteMemory<float>(0x5218A8, 258.6f); // hud_item_foldout
		MemoryHelper::WriteMemory<float>(0x5218F8, -258.6f); // hud_weapon_foldout
		// Show more of both bars
		MemoryHelper::WriteMemory<float>(0x521878, 311.85f); // mana
		MemoryHelper::WriteMemory<float>(0x521964, -311.85f); // health
	}
}

static void ApplyFixStretchedFMV()
{
	if (!FixStretchedFMV) return;

	HookHelper::ApplyHook((void*)0x490130, &RE_StretchRaw_Hook, (LPVOID*)&RE_StretchRaw);
}

static void ApplyFixStretchedGUI()
{
	if (!FixStretchedGUI) return;

	HookHelper::ApplyHook((void*)0x48FC00, &RE_StretchPic_Hook, (LPVOID*)&RE_StretchPic); // UI Scaling
	HookHelper::ApplyHook((void*)0x44B100, &SetUIBorder_Hook, (LPVOID*)&SetUIBorder); // Add the borders
	HookHelper::ApplyHook((void*)0x48F1E0, &RE_StretchFont_Hook, (LPVOID*)&RE_StretchFont); // Font Scaling
	HookHelper::ApplyHook((void*)0x4C5D30, &SetAliceMirrorViewportParams_Hook, (LPVOID*)&SetAliceMirrorViewportParams); // Scale Alice's 3D model in the settings menu
	HookHelper::ApplyHook((void*)0x452CF0, &ShowDialogBoxText_Hook, (LPVOID*)&ShowDialogBoxText);
	MemoryHelper::MakeNOP(0x4D2AB1, 7); // Dark rectangle when reassigning a control

	// Save screenshot
	HookHelper::ApplyHook((void*)0x46D280, &TakeSaveScreenshot_Hook, (LPVOID*)&TakeSaveScreenshot);
	HookHelper::ApplyHookAPI(L"opengl32", "glReadPixels", &glReadPixels_Hook, (LPVOID*)&ori_glReadPixels);

	// AutoScroll credits scaling
	HookHelper::ApplyHook((void*)0x4907F0, &UpdateRenderContext_Hook, (LPVOID*)&UpdateRenderContext);
	HookHelper::ApplyHook((void*)0x4908D0, &ConfigureScissor_Hook, (LPVOID*)&ConfigureScissor);
	HookHelper::ApplyHook((void*)0x4B1130, &UpdateAndConfigureRenderContext_Hook, (LPVOID*)&UpdateAndConfigureRenderContext);
}

static void ApplyFixDPIScaling()
{
	if (!FixDPIScaling) return;

	// Try Windows 10 1607+ (DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)
	HMODULE user32 = GetModuleHandleW(L"user32.dll");
	if (user32)
	{
		auto setDpiAwarenessContext = reinterpret_cast<decltype(&SetProcessDpiAwarenessContext)>(GetProcAddress(user32, "SetProcessDpiAwarenessContext"));
		if (setDpiAwarenessContext && setDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
		{
			return;
		}
	}

	// Try Windows 8.1+ (PROCESS_PER_MONITOR_DPI_AWARE)
	HMODULE shcore = LoadLibraryW(L"shcore.dll");
	if (shcore)
	{
		auto setDpiAwareness = reinterpret_cast<decltype(&SetProcessDpiAwareness)>(GetProcAddress(shcore, "SetProcessDpiAwareness"));
		if (setDpiAwareness && SUCCEEDED(setDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE)))
		{
			FreeLibrary(shcore);
			return;
		}
		FreeLibrary(shcore);
	}

	// Fallback to Windows Vista+ (System DPI aware)
	if (user32)
	{
		auto setDPIAware = reinterpret_cast<decltype(&SetProcessDPIAware)>(GetProcAddress(user32, "SetProcessDPIAware"));
		if (setDPIAware)
		{
			setDPIAware();
		}
	}
}

static void ApplyFixParticleDistanceRatio()
{
	if (!FixParticleDistanceRatio) return;

	// cg_particledistanceratio "150" -> "0"
	MemoryHelper::WriteMemory<int>(0x417A1F, 0x5151BC);
}

static void ApplyFixMenuAnimationSpeed()
{
	if (!FixMenuAnimationSpeed) return;

	// Read 640x480 instead of the current resolution
	MemoryHelper::WriteMemory<int>(0x4B5B56, 0x561358);
	MemoryHelper::WriteMemory<int>(0x4B5BC4, 0x561354);
	MemoryHelper::WriteMemory<int>(0x4B5D20, 0x561358);
	MemoryHelper::WriteMemory<int>(0x4B5E41, 0x561354);
}

static void ApplyFixMenuTransitionTiming()
{
	// Used for more than just this fix
	HookHelper::ApplyHook((void*)0x44C1B0, &PushMenu_Hook, (LPVOID*)&PushMenu);
	HookHelper::ApplyHook((void*)0x449DF0, &IsGameStarted_Hook, (LPVOID*)&IsGameStarted);
	HookHelper::ApplyHook((void*)0x456380, &LoadSaveFromUI_Hook, (LPVOID*)&LoadSaveFromUI);

	if (!FixMenuTransitionTiming) return;

	// Increase the time allowed to skip the intro so that the game can load properly
	MemoryHelper::WriteMemory<int>(0x4082FC, 0x3200);
}

static void ApplyFixCutsceneJumpSound()
{
	if (!FixCutsceneJumpSound) return;

	HookHelper::ApplyHook((void*)0x4061D0, &JumpCommand_Hook, (LPVOID*)&JumpCommand);
}

static void ApplyFixLocalizationFiles()
{
	if (!FixLocalizationFiles) return;

	HookHelper::ApplyHook((void*)0x4615F0, &LoadLocalizationFile_Hook, (LPVOID*)&LoadLocalizationFile);
}

static void ApplyFixProton()
{
	if (!FixProton) return;

	HookHelper::ApplyHook((void*)0x409FD0, &CL_InitRef_Hook, (LPVOID*)&CL_InitRef);
}

static void ApplyLaunchWithoutAlice2()
{
	if (!LaunchWithoutAlice2) return;

	// Disable the '-RunningFromAlice2' launch argument check
	MemoryHelper::MakeNOP(0x4655F4, 30);
}

static void ApplyCustomControllerBindings()
{
	if (!CustomControllerBindings) return;

	HookHelper::ApplyHook((void*)0x4635A0, &UpdateControllerState_Hook, (LPVOID*)&UpdateControllerState);
	HookHelper::ApplyHook((void*)0x417530, &CVAR_Init_Hook, (LPVOID*)&CVAR_Init);
}

static void ApplyPreventAlice2OnExit()
{
	if (!PreventAlice2OnExit) return;

	// Launch Alice2 Off
	MemoryHelper::WriteMemory<int>(0x559A08, 0, false);
}

static void ApplyDisableWinsockInitialization()
{
	if (!DisableWinsockInitialization) return;

	// Disable call to NET_Init
	MemoryHelper::MakeNOP(0x46562E, 5);
}

static void ApplyLanguageId()
{
	MemoryHelper::WriteMemory<int>(0x461A90, LanguageId);
}

static void ApplyUseConsoleTitleScreen()
{
	if (!UseConsoleTitleScreen) return;

	HookHelper::ApplyHook((void*)0x4081B0, &HandleKeyboardInput_Hook, (LPVOID*)&HandleKeyboardInput);
	HookHelper::ApplyHook((void*)0x4C1AC0, &LoadUI_Hook, (LPVOID*)&LoadUI);
	HookHelper::ApplyHook((void*)0x44C4F0, &TriggerMainMenu_Hook, (LPVOID*)&TriggerMainMenu);
}

static void ApplyUseOriginalIntroVideos()
{
	if (!UseOriginalIntroVideos) return;

	// Instructions from the 2000 version, ported to the remaster
	uint8_t portedIntroData[0x270] =
	{
		0x8B, 0x0D, 0x20, 0xCA, 0x7C, 0x00, 0x83, 0xEC, 0x40, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x56, 0x33,
		0xF6, 0x41, 0x83, 0xF9, 0x65, 0x0F, 0x87, 0xE8, 0x00, 0x00, 0x00, 0x33, 0xD2, 0x8A, 0x91, 0x64,
		0x3D, 0x51, 0x00, 0xFF, 0x24, 0x95, 0x40, 0x3D, 0x51, 0x00, 0xBE, 0x88, 0x13, 0x00, 0x00, 0xE9,
		0xCF, 0x00, 0x00, 0x00, 0xC7, 0x05, 0x24, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x80, 0x3F, 0xC7, 0x05,
		0x34, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x80, 0x3F, 0xC7, 0x05, 0x30, 0xCA, 0x7C, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xEB, 0x63, 0x8B, 0x15, 0x0C, 0xCA, 0x7C, 0x00, 0xC7, 0x05, 0x24, 0xCA, 0x7C, 0x00,
		0x00, 0x00, 0x80, 0x3F, 0xC7, 0x05, 0x34, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x80, 0x3F, 0xC7, 0x05,
		0x30, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x80, 0x3F, 0xBE, 0x88, 0x13, 0x00, 0x00, 0xC7, 0x05, 0x28,
		0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x05, 0x2C, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x89, 0x15, 0x38, 0xCA, 0x7C, 0x00, 0xEB, 0x6A, 0xC7, 0x05, 0x24, 0xCA, 0x7C, 0x00, 0x00,
		0x00, 0x80, 0x3F, 0xC7, 0x05, 0x34, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x05, 0x30,
		0xCA, 0x7C, 0x00, 0x00, 0x00, 0x80, 0x3F, 0xDB, 0x05, 0x8C, 0xBD, 0x48, 0x01, 0x8B, 0x0D, 0x0C,
		0xCA, 0x7C, 0x00, 0xBE, 0xE8, 0x03, 0x00, 0x00, 0xC7, 0x05, 0x28, 0xCA, 0x7C, 0x00, 0x00, 0x00,
		0x7A, 0x44, 0x89, 0x0D, 0x38, 0xCA, 0x7C, 0x00, 0xD9, 0x1D, 0x2C, 0xCA, 0x7C, 0x00, 0xEB, 0x23,
		0x68, 0xED, 0x4D, 0x51, 0x00, 0x68, 0x9C, 0x4E, 0x51, 0x00, 0xE8, 0x41, 0x53, 0xF0, 0xFF, 0x68,
		0xD2, 0x3D, 0x51, 0x00, 0x68, 0xDA, 0x3D, 0x51, 0x00, 0xE8, 0x72, 0x1A, 0xEF, 0xFF, 0x83, 0xC4,
		0x10, 0x33, 0xC0, 0x8B, 0x15, 0x20, 0xCA, 0x7C, 0x00, 0x42, 0x85, 0xC0, 0x89, 0x15, 0x20, 0xCA,
		0x7C, 0x00, 0x74, 0x1F, 0x56, 0x68, 0xB0, 0x0F, 0x52, 0x00, 0x8D, 0x44, 0x24, 0x0C, 0x6A, 0x40,
		0x50, 0xE8, 0x7A, 0x1B, 0xF1, 0xFF, 0x8D, 0x4C, 0x24, 0x14, 0x51, 0xE8, 0x30, 0x1C, 0xF0, 0xFF,
		0x83, 0xC4, 0x14, 0x5E, 0x83, 0xC4, 0x40, 0xC3, 0x68, 0xED, 0x4D, 0x51, 0x00, 0x68, 0x9C, 0x4E,
		0x51, 0x00, 0xE8, 0xE9, 0x52, 0xF0, 0xFF, 0x68, 0xD6, 0x3D, 0x51, 0x00, 0x68, 0xE6, 0x3D, 0x51,
		0x00, 0xEB, 0xA6, 0x8B, 0x15, 0x10, 0x04, 0x56, 0x00, 0x52, 0xE8, 0xC1, 0x84, 0xF3, 0xFF, 0x83,
		0xC4, 0x04, 0x68, 0xE8, 0x64, 0x51, 0x00, 0xE8, 0x04, 0x59, 0xF1, 0xFF, 0x83, 0xC4, 0x04, 0xC7,
		0x05, 0x34, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x05, 0x30, 0xCA, 0x7C, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xC7, 0x05, 0x28, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x05, 0x2C,
		0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x05, 0x38, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xC7, 0x05, 0x20, 0xCA, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5E, 0x83, 0xC4, 0x40, 0xC3,
		0xF2, 0x3C, 0x51, 0x00, 0xBA, 0x3B, 0x51, 0x00, 0xC4, 0x3B, 0x51, 0x00, 0xE4, 0x3B, 0x51, 0x00,
		0x29, 0x3C, 0x51, 0x00, 0x70, 0x3C, 0x51, 0x00, 0xC8, 0x3C, 0x51, 0x00, 0xE3, 0x3C, 0x51, 0x00,
		0x93, 0x3C, 0x51, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x07, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x00, 0x00, 0x32, 0x35, 0x00, 0x00, 0x32, 0x36, 0x00, 0x00, 0x65, 0x61, 0x6C, 0x6F, 0x67, 0x6F,
		0x2E, 0x72, 0x6F, 0x71, 0x00, 0x00, 0x72, 0x6F, 0x67, 0x75, 0x65, 0x6C, 0x6F, 0x67, 0x6F, 0x2E,
		0x72, 0x6F, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	MemoryHelper::WriteMemoryRaw(CODE_CAVE_INTRO, portedIntroData, sizeof(portedIntroData), true);

	MemoryHelper::MakeJMP(0x44D91F, CODE_CAVE_INTRO);
	MemoryHelper::WriteMemory(0x44E409, CODE_CAVE_INTRO);
}

static void ApplyDisable8BitAudioAsDefault()
{
	if (!Disable8BitAudioAsDefault) return;

	// s_loadas8bit "1" -> "0"
	MemoryHelper::WriteMemory<int>(0x427607, 0x5151BC);
}

static void ApplyDisableRemasteredModels()
{
	if (!DisableRemasteredModels) return;

	HookHelper::ApplyHook((void*)0x43E030, &FS_LoadZipFile_Hook, (LPVOID*)&FS_LoadZipFile);
}

static void ApplyEnableDevConsole()
{
	HookHelper::ApplyHook((void*)0x407870, &Bind_Hook, (LPVOID*)&Bind);
}

static void ApplyCustomSavePath()
{
	HookHelper::ApplyHook((void*)0x417400, &GetSavePath_Hook, (LPVOID*)&GetSavePath);
}

static void ApplyEnableControllerIcons()
{
	if (!EnableControllerIcons) return;

	// Already hooked if 'UseConsoleTitleScreen' is used
	if (!UseConsoleTitleScreen)
	{
		HookHelper::ApplyHook((void*)0x4C1AC0, &LoadUI_Hook, (LPVOID*)&LoadUI);
	}
}

static void ApplyHideConsoleAtLaunch()
{
	if (!HideConsoleAtLaunch) return;

	// ShowWindow(hWndParent, 10) -> ShowWindow(hWndParent, 0)
	MemoryHelper::WriteMemory<uint8_t>(0x46C28F, 0x00);
}

static void ApplyForceBorderlessFullscreen()
{
	if (!ForceBorderlessFullscreen) return;

	HookHelper::ApplyHookAPI(L"user32.dll", "CreateWindowExA", &CreateWindowExA_Hook, (LPVOID*)&ori_CreateWindowExA);
}

static void ApplyCustomResolution()
{
	if (!CustomResolution) return;

	HookHelper::ApplyHook((void*)0x47ABE0, &QGL_Init_Hook, (LPVOID*)&QGL_Init);
}

static void ApplyEnableAltF4Close()
{
	if (!EnableAltF4Close) return;

	HookHelper::ApplyHook((void*)0x46C600, &lpfnWndProc_MSG_Hook, (LPVOID*)&lpfnWndProc_MSG);
}

static void ApplyTrilinearTextureFiltering()
{
	if (!TrilinearTextureFiltering) return;

	// r_textureMode "GL_LINEAR_MIPMAP_NEAREST" -> "GL_LINEAR_MIPMAP_LINEAR"
	MemoryHelper::WriteMemory<int>(0x46E5A7, 0x52A32C);
}

static void ApplyAnisotropicTextureFiltering()
{
	HookHelper::ApplyHookAPI(L"opengl32", "glTexParameterf", &glTexParameterf_Hook, (LPVOID*)&ori_glTexParameterf);
}

static void ApplyProcessAPIPacket()
{
	HookHelper::ApplyHook((void*)0x40C1B0, &CL_ParsePacketEntities_Hook, (LPVOID*)&CL_ParsePacketEntities);
}

static void ApplyEnableVsyncAsDefault()
{
	if (!EnableVsyncAsDefault) return;

	// r_swapInterval "0" -> "1"
	MemoryHelper::WriteMemory<int>(0x46E5BE, 0x515C1C);
}

static void ApplyCvarTweaks()
{
	HookHelper::ApplyHook((void*)0x419910, &Cvar_Set_Hook, (LPVOID*)&Cvar_Set);
}

static void ApplyResolutionChangeHook()
{
	HookHelper::ApplyHook((void*)0x46FC70, &GLW_CreatePFD_Hook, (LPVOID*)&GLW_CreatePFD);
}

static void InitializeGameLoadingChecks()
{
	HookHelper::ApplyHook((void*)0x4256E0, &Str_To_Lower_Hook, (LPVOID*)&Str_To_Lower);
	HookHelper::ApplyHook((void*)0x44C280, &ForceMenu_Hook, (LPVOID*)&ForceMenu);
	HookHelper::ApplyHook((void*)0x4158F0, &CallCmd_Hook, (LPVOID*)&CallCmd);
}

#pragma endregion Apply Patches

#pragma region

static void Init()
{
	ReadConfig();

	// Fixes
	ApplyFixSoundRandomization();
	ApplyFixHardDiskFull();
	ApplyFixDisplayModeBufferOverflow();
	ApplyFixBlinkingAnimationSpeed();
	ApplyFixStretchedHUD();
	ApplyFixStretchedFMV();
	ApplyFixStretchedGUI();
	ApplyFixDPIScaling();
	ApplyFixParticleDistanceRatio();
	ApplyFixMenuAnimationSpeed();
	ApplyFixMenuTransitionTiming();
	ApplyFixCutsceneJumpSound();
	ApplyFixLocalizationFiles();
	ApplyFixProton();
	// General
	ApplyLaunchWithoutAlice2();
	ApplyCustomControllerBindings();
	ApplyPreventAlice2OnExit();
	ApplyDisableWinsockInitialization();
	ApplyLanguageId();
	ApplyUseConsoleTitleScreen();
	ApplyUseOriginalIntroVideos();
	ApplyDisable8BitAudioAsDefault();
	ApplyDisableRemasteredModels();
	ApplyEnableDevConsole();
	ApplyCustomSavePath();
	// Display
	ApplyEnableControllerIcons();
	ApplyHideConsoleAtLaunch();
	ApplyForceBorderlessFullscreen();
	ApplyCustomResolution();
	ApplyEnableAltF4Close();
	// Graphics
	ApplyTrilinearTextureFiltering();
	ApplyAnisotropicTextureFiltering();
	ApplyProcessAPIPacket();
	ApplyEnableVsyncAsDefault();
	// Misc
	ApplyCvarTweaks();
	ApplyResolutionChangeHook();
	InitializeGameLoadingChecks();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			// Prevents DLL from receiving thread notifications
			DisableThreadLibraryCalls(hModule);

			uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
			IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
			IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

			// Check if the entry point matches our expected MAIN_ENTRY_POINT
			if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == MAIN_ENTRY_POINT)
			{
				SystemHelper::LoadProxyLibrary();
				Init();
			}
			else
			{
				MessageBoxA(NULL, "This .exe is not supported.", "VorpalFix", MB_ICONERROR);
				return FALSE;
			}
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			MH_Uninitialize();
			break;
		}
	}
	return TRUE;
}

#pragma endregion Initialization