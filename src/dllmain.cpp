#define _USE_MATH_DEFINES
#define NOMINMAX
#include <IniReader.h>
#include <shlobj.h>
#include "MinHook.h"
#include "dllmain.h"
#include "helper.hpp"
#include <Windows.h>
#include <VersionHelpers.h>
#include <ShellScalingAPI.h>
#include <GL/gl.h>

// =============================
// Memory Addresses 
// =============================
const int CURRENT_WIDTH_ADDR = 0x1C4798C;
const int CURRENT_HEIGHT_ADDR = 0x1C47990;
const int CURRENT_LANG = 0x7CF868;
const int IS_CURSOR_VISIBLE = 0x11C02E0;
const int STARTUP_STATE_ADDR = 0x7CCA20;
const int SHADERS_CACHE_ADDR = 0x1BFCEF4;
const int CONSOLE_THREAD_PTR_ADDR = 0x7CCA54;
const int DISPLAY_MODE_NUM = 0x7D40C8;
const int DISPLAY_MODE_PTR_ADDR = 0x1C463E8;
const int DISPLAY_MODE_ARRAY_WIDTH_ADDR = 0x1C1D2E0;
const int DISPLAY_MODE_ARRAY_HEIGHT_ADDR = 0x1C1D2E4;

// =============================
// Variables 
// =============================
bool isMHInitialized = false;
bool isDialog = false;
bool isResolutionApplied = false;
bool CvarHooking = false;
int currentWidth = 0;
int currentHeight = 0;
bool isCursorResized = false;
bool isDefaultFullscreenSettingSkipped = false;
bool isUsingControllerMenu = false;
bool isMainMenuShown = false;
bool isTitleBgSet = false;
bool isRestartedForLinux = false;
bool isUsingCustomSaveDir = false;
bool skipAutoResolution = false;
bool setAlice2Path = false;
bool isTexParameterfHooked = false;
bool isAnisotropyRetrieved = false;
const float ASPECT_RATIO_4_3 = 4.0f / 3.0f;
const int LEFT_BORDER_X_ID = 0x1000000;
const int RIGHT_BORDER_X_ID = 0x2000000;
const char* ALICE2_DEFAULT_PATH = "..\\..\\Alice2\\Binaries\\Win32\\AliceMadnessReturns.exe";

// =============================
// Ini Variables
// =============================

// Fixes
bool FixSoundRandomization = false;
bool FixHardDiskFull = false;
bool FixSaveScreenshotBufferOverflow = false;
bool FixBlinkingAnimationSpeed = false;
bool FixStretchedHUD = false;
bool FixStretchedFMV = false;
bool FixStretchedGUI = false;
bool FixDPIScaling = false;
bool FixFullscreenSetting = false;
bool FixMenuTransitionTiming = false;
bool FixProton = false;

// General
bool LaunchWithoutAlice2 = false;
bool PreventAlice2OnExit = false;
char* Alice2Path = nullptr;
int LanguageId = 0;
bool UseConsoleTitleScreen = false;
bool UseOriginalIntroVideos = false;
bool DisableRemasteredModels = false;
bool EnableDevConsole = false;
char* ToggleConsoleBindKey = nullptr;
char* CustomSavePath = nullptr;

// Display
bool ConsolePortHUD = false;
bool EnableControllerIcons = false;
bool UsePS3ControllerIcons = false;
bool HideConsoleAtLaunch = false;
bool DisableLetterbox = false;
bool ForceBorderlessFullscreen = false;
bool EnableVsync = false;
bool AutoResolution = false;
bool CustomResolution = false;
int CustomResolutionWidth = 0;
int CustomResolutionHeight = 0;
bool EnableAltF4Close = 0;

// Graphics
bool AnisotropicTextureFiltering = false;
float MaxAnisotropy = 0;
bool TrilinearTextureFiltering = false;
bool EnhancedLOD = false;
int CustomFPSLimit = 0;
float FOV = 0;
bool AutoFOV = false;

static void ReadConfig()
{
	// Configuration reader initialization
	CIniReader iniReader("VorpalFix.ini");

	// Fixes
	FixSoundRandomization = iniReader.ReadInteger("Fixes", "FixSoundRandomization", 1) == 1;
	FixHardDiskFull = iniReader.ReadInteger("Fixes", "FixHardDiskFull", 1) == 1;
	FixSaveScreenshotBufferOverflow = iniReader.ReadInteger("Fixes", "FixSaveScreenshotBufferOverflow", 1) == 1;
	FixBlinkingAnimationSpeed = iniReader.ReadInteger("Fixes", "FixBlinkingAnimationSpeed", 1) == 1;
	FixStretchedHUD = iniReader.ReadInteger("Fixes", "FixStretchedHUD", 1) == 1;
	FixStretchedFMV = iniReader.ReadInteger("Fixes", "FixStretchedFMV", 1) == 1;
	FixStretchedGUI = iniReader.ReadInteger("Fixes", "FixStretchedGUI", 1) == 1;
	FixDPIScaling = iniReader.ReadInteger("Fixes", "FixDPIScaling", 1) == 1;
	FixFullscreenSetting = iniReader.ReadInteger("Fixes", "FixFullscreenSetting", 1) == 1;
	FixMenuTransitionTiming = iniReader.ReadInteger("Fixes", "FixMenuTransitionTiming", 1) == 1;
	FixProton = iniReader.ReadInteger("Fixes", "FixProton", 0) == 1;

	// General
	LaunchWithoutAlice2 = iniReader.ReadInteger("General", "LaunchWithoutAlice2", 1) == 1;
	PreventAlice2OnExit = iniReader.ReadInteger("General", "PreventAlice2OnExit", 0) == 1;
	Alice2Path = iniReader.ReadString("General", "Alice2Path", ALICE2_DEFAULT_PATH);
	LanguageId = iniReader.ReadInteger("General", "LanguageId", 0);
	UseOriginalIntroVideos = iniReader.ReadInteger("General", "UseOriginalIntroVideos", 0) == 1;
	UseConsoleTitleScreen = iniReader.ReadInteger("General", "UseConsoleTitleScreen", 0) == 1;
	DisableRemasteredModels = iniReader.ReadInteger("General", "DisableRemasteredModels", 0) == 1;
	EnableDevConsole = iniReader.ReadInteger("General", "EnableDevConsole", 0) == 1;
	ToggleConsoleBindKey = iniReader.ReadString("General", "ToggleConsoleBindKey", "F2");
	CustomSavePath = iniReader.ReadString("General", "CustomSavePath", "");

	// Display
	ConsolePortHUD = iniReader.ReadInteger("Display", "ConsolePortHUD", 0) == 1;
	EnableControllerIcons = iniReader.ReadInteger("Display", "EnableControllerIcons", 1) == 1;
	UsePS3ControllerIcons = iniReader.ReadInteger("Display", "UsePS3ControllerIcons", 0) == 1;
	HideConsoleAtLaunch = iniReader.ReadInteger("Display", "HideConsoleAtLaunch", 1) == 1;
	DisableLetterbox = iniReader.ReadInteger("Display", "DisableLetterbox", 0) == 1;
	ForceBorderlessFullscreen = iniReader.ReadInteger("Display", "ForceBorderlessFullscreen", 0) == 1;
	EnableVsync = iniReader.ReadInteger("Display", "EnableVsync", 0) == 1;
	AutoResolution = iniReader.ReadInteger("Display", "AutoResolution", 1) == 1;
	CustomResolution = iniReader.ReadInteger("Display", "CustomResolution", 0) == 1;
	CustomResolutionWidth = iniReader.ReadInteger("Display", "CustomResolutionWidth", 640);
	CustomResolutionHeight = iniReader.ReadInteger("Display", "CustomResolutionHeight", 480);
	EnableAltF4Close = iniReader.ReadInteger("Display", "EnableAltF4Close", 0);

	// Graphics
	AnisotropicTextureFiltering = iniReader.ReadInteger("Graphics", "AnisotropicTextureFiltering", 1) == 1;
	MaxAnisotropy = iniReader.ReadFloat("Graphics", "MaxAnisotropy", 0);
	TrilinearTextureFiltering = iniReader.ReadInteger("Graphics", "TrilinearTextureFiltering", 1) == 1;
	EnhancedLOD = iniReader.ReadInteger("Graphics", "EnhancedLOD", 1) == 1;
	CustomFPSLimit = iniReader.ReadInteger("Graphics", "CustomFPSLimit", 60);
	FOV = iniReader.ReadFloat("Graphics", "FOV", 90.0);
	AutoFOV = iniReader.ReadInteger("Graphics", "AutoFOV", 1) == 1;

	// Set to monitor's refresh rate
	if (CustomFPSLimit == -1)
	{
		CustomFPSLimit = SystemHelper::GetCurrentDisplayFrequency();
	}

	// UseConsoleTitleScreen rely on FixStretchedGUI
	if (!FixStretchedGUI && UseConsoleTitleScreen)
	{
		UseConsoleTitleScreen = false;
	}

	isUsingCustomSaveDir = (CustomSavePath != NULL) && (CustomSavePath[0] != '\0');
	setAlice2Path = strcmp(Alice2Path, ALICE2_DEFAULT_PATH) != 0;
	CvarHooking = FixFullscreenSetting || AutoResolution || EnableDevConsole || EnableVsync || TrilinearTextureFiltering || EnhancedLOD || (CustomFPSLimit != 60) || setAlice2Path;
}

#pragma region

typedef char* (__cdecl* sub_417400)();
sub_417400 GetSavePath = nullptr;

static char* __cdecl GetSavePath_Hook()
{
	if (isUsingCustomSaveDir)
	{
		static char modifiedSavePath[MAX_PATH];
		strcpy(modifiedSavePath, CustomSavePath);

		// Append a backslash if not already present
		size_t len = strlen(modifiedSavePath);
		if (modifiedSavePath[len - 1] != '\\') {
			strcat(modifiedSavePath, "\\");
		}
		return modifiedSavePath;
	}
	return GetSavePath();
}

typedef int(__cdecl* sub_41D1E0)();
sub_41D1E0 CheckDiskFreeSpace = nullptr;

static int __cdecl CheckDiskFreeSpace_Hook()
{
	char* savePath = GetSavePath_Hook();
	unsigned __int64 freeBytesAvailable = 0;
	unsigned __int64 totalNumberOfBytes = 0;
	unsigned __int64 totalNumberOfFreeBytes = 0;

	if (GetDiskFreeSpaceExA(savePath, (PULARGE_INTEGER)&freeBytesAvailable, (PULARGE_INTEGER)&totalNumberOfBytes, (PULARGE_INTEGER)&totalNumberOfFreeBytes))
	{
		// Convert the available free bytes to KB
		unsigned __int64 freeSpaceInKB = freeBytesAvailable >> 10;

		// If the free space is >= 0x80000000, cap it at 0x7FFFFFFF instead of -1
		if (freeSpaceInKB >= 0x80000000)
			freeSpaceInKB = 0x7FFFFFFF;

		return (int)freeSpaceInKB;
	}

	// Return a default value if something goes wrong
	return 2048;
}

typedef int(__cdecl* sub_446050)(float, float, float, float, int, float*, float*, float*, int, const char*, __int16, float*, float*, float, float, int);
sub_446050 SetHUDPosition = nullptr;

static int __cdecl SetHUDPosition_Hook(float x_position, float y_position, float resolution_width, float resolution_height, int a5, float* a6, float* a7, float* a8, int a9, const char* a10, __int16 a11, float* a12, float* a13, float a14, float a15, int a16)
{
	float current_aspect_ratio = resolution_width / resolution_height;
	float scaleX = 1.0f;

	double hud_object_x_position = (x_position * 640.0) / resolution_width;
	float resolution_width_original = resolution_width;

	if (current_aspect_ratio > ASPECT_RATIO_4_3 && FixStretchedHUD)
	{
		scaleX = ASPECT_RATIO_4_3 / current_aspect_ratio;

		if (x_position > 0)
		{
			float center = resolution_width / 2.0f;
			float widthDifference = resolution_width - (resolution_width * scaleX);
			x_position = (x_position - center) * scaleX + center + (widthDifference / 2.0f);

			// Fine tuning
			if (resolution_width_original != 1280 || resolution_height != 800)
			{
				if (resolution_width_original <= 1920)
				{
					x_position -= 1.0f;
				}
				else if (resolution_width_original < 3840)
				{
					x_position -= 2.0f;
				}
				else
				{
					x_position -= 3.0f;
				}
			}
		}

		// Adjust width scaling for the HUD elements on the left
		resolution_width *= scaleX;

		if (x_position < 0)
		{
			x_position = static_cast<double>(resolution_width / 640.0 * hud_object_x_position);

			// Fine tuning
			if (resolution_width_original != 1440 || resolution_height != 900)
			{
				if (resolution_width_original == 1280 && resolution_height == 800)
				{
					x_position += 2.0f;
				}
				else if (resolution_width_original < 1920)
				{
					x_position += 1.0f;
				}
				else if (resolution_width_original < 3840)
				{
					x_position += 2.0f;
				}
				else
				{
					x_position += 3.0f;
				}
			}
		}
	}

	if (ConsolePortHUD)
	{
		float current_width = static_cast<float>(currentWidth);
		float current_height = static_cast<float>(currentHeight);

		if (x_position < 0)
		{
			x_position += current_width / 17.5f;
		}

		if (x_position > 0)
		{
			x_position -= current_width / 17.5f;
		}

		y_position -= current_height / 10.0f;
	}

	return SetHUDPosition(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
}

typedef int(__cdecl* sub_490130)(int, int, int, int, int, int, int);
sub_490130 SetFMVPosition = nullptr;

static int __cdecl SetFMVPosition_Hook(int x_position, int y_position, int resolution_width, int resolution_height, int a5, int a6, int a7)
{
	int video_width;
	int video_height;

	video_height = resolution_height;
	video_width = static_cast<int>(video_height * ASPECT_RATIO_4_3);

	if (video_width > resolution_width)
	{
		video_width = resolution_width;
		video_height = static_cast<int>(video_width / ASPECT_RATIO_4_3);
	}

	x_position = (resolution_width - video_width) / 2;
	y_position = (resolution_height - video_height) / 2;

	return SetFMVPosition(x_position, y_position, video_width, video_height, a5, a6, a7);
}

static void ApplyHook(void* addr, LPVOID hookFunc, LPVOID* originalFunc)
{
	if (!isMHInitialized)
	{
		if (MH_Initialize() == MH_OK)
		{
			isMHInitialized = true;
		}
		else
		{
			MessageBoxA(NULL, "Failed to initialize MinHook!", "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	if (MH_CreateHook(addr, hookFunc, originalFunc) != MH_OK)
	{
		char errorMsg[0x100];
		sprintf_s(errorMsg, "Failed to create hook at address: %p", addr);
		MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
		return;
	}

	if (MH_EnableHook(addr) != MH_OK)
	{
		char errorMsg[0x100];
		sprintf_s(errorMsg, "Failed to enable hook at address: %p", addr);
		MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
		return;
	}
}

typedef int(__stdcall* opengl32_glTexParameterf)(int, int, float);
opengl32_glTexParameterf original_glTexParameterf = nullptr;

static int __stdcall glTexParameterf_Hook(int target, int pname, float param)
{
	// Check if the param is a valid mipmap filter mode
	if (param >= 0x2700 && param <= 0x2703)
	{
		if (!isAnisotropyRetrieved && MaxAnisotropy == 0)
		{
			// Retrieve the maximum anisotropy level supported by the hardware
			glGetFloatv(0x84FF, &MaxAnisotropy);
			isAnisotropyRetrieved = true;
		}

		original_glTexParameterf(target, pname, param);
		return original_glTexParameterf(target, 0x84FE, (float)MaxAnisotropy);
	}
	return original_glTexParameterf(target, pname, param);
}

typedef int(__cdecl* sub_48FC00)(float, float, float, float, float, float, float, float, int);
sub_48FC00 RenderShader = nullptr;
bool isSet = false;

static int __cdecl RenderShader_Hook(float x_position, float y_position, float resolution_width, float resolution_height, float a5, float a6, float a7, float a8, int ShaderHandle)
{
	float current_width = static_cast<float>(currentWidth);
	float current_height = static_cast<float>(currentHeight);

	float current_aspect_ratio = current_width / current_height;

	float black_border_width = (current_width - (current_height * ASPECT_RATIO_4_3)) / 2.0f;

	// Don't do this if black_border_width too big
	if (x_position == LEFT_BORDER_X_ID && current_aspect_ratio <= 2.0f && current_aspect_ratio > 1.5f) // Left border, skip if the image is going to be stretched
	{
		// Calculate the scale factor to match the target height
		float scale_factor = current_height / 720.0f; // Original height of the image is 720

		// Scale the width and height proportionally
		resolution_width = 160.0f * scale_factor;  // Original width is 160
		resolution_height = current_height;        // Match the screen height

		// Align the image to the left edge to cover the black border precisely
		x_position = black_border_width - resolution_width;
	}
	else if (x_position == RIGHT_BORDER_X_ID && current_aspect_ratio <= 2.0f && current_aspect_ratio > 1.5f) // Right border, skip if the image is going to be stretched
	{
		// Calculate the scale factor to match the target height
		float scale_factor = current_height / 720.0f; // Original height of the image is 720

		// Scale the width and height proportionally
		resolution_width = 160.0f * scale_factor;  // Original width is 160
		resolution_height = current_height;        // Match the screen height

		// Position the image to align precisely with the right black border
		x_position = current_width - black_border_width;
	}
	else
	{
		char* ShaderName = *(char**)(SHADERS_CACHE_ADDR + 0x4 * ShaderHandle);
		BYTE isConsoleOpen = (*(BYTE**)CONSOLE_THREAD_PTR_ADDR && *(*(BYTE**)CONSOLE_THREAD_PTR_ADDR + 0xCC)) ? *(*(BYTE**)CONSOLE_THREAD_PTR_ADDR + 0xCC) : 0;

		// Workaround to be able to use the console
		if (isConsoleOpen == 1 && strcmp(ShaderName, "gfx/2d/mouse_arrow") == 0)
		{
			return RenderShader(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		if (!isCursorResized && strcmp(ShaderName, "gfx/2d/mouse_arrow") == 0)
		{
			GameHelper::ResizeCursor(isUsingControllerMenu, currentWidth, currentHeight);
			GameHelper::ResizePopupMessage(currentWidth, currentHeight);
			isCursorResized = true;
		}

		if (!isMainMenuShown && strstr(ShaderName, "main") != NULL)
		{
			GameHelper::ResizeCursor(isUsingControllerMenu, currentWidth, currentHeight);
			isMainMenuShown = true;

			MemoryHelper::WriteMemory<char>(0x4082C3, 0x1B, true);
		}

		if (!isMainMenuShown && strstr(ShaderName, "legalplate") != NULL)
		{
			if (current_width > 1280)
			{
				float scale_factor = (float)current_height / 720.0f;

				resolution_width *= scale_factor;
				resolution_height *= scale_factor;

				x_position = (current_width - resolution_width) / 2.0f;
				y_position = (current_height - resolution_height) / 2.0f;
			}

			return RenderShader(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		if (!isMainMenuShown && strcmp(ShaderName, "title_bg") == 0)
		{
			float image_aspect_ratio = 1280.0f / 720.0f;

			// Determine the scale factor to fill the screen
			float scale_factor;
			if (current_aspect_ratio > image_aspect_ratio)
			{
				// Screen is wider than 16:9; scale based on width to fill horizontally, cropping top and bottom
				scale_factor = (float)current_width / 1280.0f;
			}
			else
			{
				// Screen is taller than 16:9; scale based on height to fill vertically, cropping sides
				scale_factor = (float)current_height / 720.0f;
			}

			// Set scaled dimensions to fill the screen fully
			resolution_width = 1280 * scale_factor;
			resolution_height = 720 * scale_factor;

			// Center the image on the screen
			x_position = (current_width - resolution_width) / 2.0f;
			y_position = (current_height - resolution_height) / 2.0f;

			return RenderShader(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Full screen effects
		if (strcmp(ShaderName, "textures/special/drugfade") == 0 || strcmp(ShaderName, "textures/special/icefade") == 0)
		{
			return RenderShader(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		// Pop-up message
		if (strcmp(ShaderName, "ui/control/press_any_key") == 0)
		{
			return RenderShader(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
		}

		if (current_aspect_ratio >= ASPECT_RATIO_4_3)
		{
			float target_width = current_height * ASPECT_RATIO_4_3;
			float scale_factor = target_width / current_width;
			resolution_width *= scale_factor;
			float horizontal_offset = (current_width - target_width) / 2.0f;

			// Exceptions for some of the in-game assets
			if ((ConsolePortHUD || strcmp(ShaderName, "ui/quicksavecam/quicksavecam") != 0) && strcmp(ShaderName, "ui/dialog/leftFrame") != 0 && strcmp(ShaderName, "ui/dialog/rightFrame") != 0)
			{
				x_position = (x_position * scale_factor) + horizontal_offset;
			}

			if ((ConsolePortHUD && strcmp(ShaderName, "ui/quicksavecam/quicksavecam") == 0))
			{
				x_position = current_width / 6.0f;
				y_position = current_height / 14.25f;
			}

			// Move the dialog boxes to the center
			if (strcmp(ShaderName, "ui/dialog/leftFrame") == 0 || strcmp(ShaderName, "ui/dialog/rightFrame") == 0)
			{
				x_position = (x_position * scale_factor) + horizontal_offset / 1.65f;
				isDialog = true;
			}
			else
			{
				isDialog = false;
			}
		}
	}
	return RenderShader(x_position, y_position, resolution_width, resolution_height, a5, a6, a7, a8, ShaderHandle);
}

typedef void(__cdecl* nullsub_4)();
nullsub_4 SetUIBorder = nullptr;

static void __cdecl SetUIBorder_Hook()
{
	if (isDialog) return;

	int border = GameHelper::GetTga(18, "border1_left");
	RenderShader_Hook(LEFT_BORDER_X_ID, 0, 0, 0, 0.0, 0.0, 1.0, 1.0, *(DWORD*)(border + 4));
	RenderShader_Hook(RIGHT_BORDER_X_ID, 0, 0, 0, 1.0, 0.0, 0.0, 1.0, *(DWORD*)(border + 4));
}

typedef int(__thiscall* sub_4C0A10)(int, float, float, int, int, float, float, float, float, float);
sub_4C0A10 GetGlyphInfo = nullptr;

static int __fastcall GetGlyphInfo_Hook(int this_ptr, int* _EDX, float font_x_position, float font_y_position, int a4, int a5, float a6, float a7, float a8, float font_scale_width, float font_scale_height)
{
	// Don't mess with the console
	if (font_x_position <= 3.0)
	{
		return GetGlyphInfo(this_ptr, font_x_position, font_y_position, a4, a5, a6, a7, a8, font_scale_width, font_scale_height);
	}

	float current_width = static_cast<float>(currentWidth);
	float current_height = static_cast<float>(currentHeight);

	float current_aspect_ratio = current_width / current_height;

	if (current_aspect_ratio > ASPECT_RATIO_4_3)
	{
		// Disable original font scaling, we'll handle it ourselves
		*(BYTE*)(this_ptr + 24) = 1;

		float target_width = current_height * ASPECT_RATIO_4_3;
		float horizontal_offset = (current_width - target_width) / 2.0f;

		float adjusted_font_x_position = (font_x_position * (target_width / 640.0f)) + horizontal_offset;
		float adjusted_font_scale_width;

		// Hack for credits
		if (a8 == -8.0)
		{
			adjusted_font_scale_width = font_scale_width * (target_width / 640.0f) * font_scale_width;
		}
		else
		{
			adjusted_font_scale_width = font_scale_width * (target_width / 640.0f);
		}

		float adjusted_font_y_position = font_y_position * (current_height / 480.0f);
		float adjusted_font_scale_height = font_scale_height * (current_height / 480.0f);

		// Adjust the position of the text for the dialog box
		if (isDialog)
		{
			adjusted_font_x_position = (font_x_position * (target_width / 640.0f)) + horizontal_offset / 1.65f;
		}

		return GetGlyphInfo(this_ptr, adjusted_font_x_position, adjusted_font_y_position, a4, a5, a6, a7, a8, adjusted_font_scale_width, adjusted_font_scale_height);
	}
	else
	{
		return GetGlyphInfo(this_ptr, font_x_position, font_y_position, a4, a5, a6, a7, a8, font_scale_width, font_scale_height);
	}
}

typedef DWORD* (__cdecl* sub_4C5D30)(DWORD*, float, float, float, float, float);
sub_4C5D30 SetAliceMirrorViewportParams = nullptr;

static DWORD* __cdecl SetAliceMirrorViewportParams_Hook(DWORD* a1, float param_x, float param_y, float param_width, float param_height, float param_fov)
{
	DWORD* renderEntity = SetAliceMirrorViewportParams(a1, param_x, param_y, param_width, param_height, param_fov);

	int width = renderEntity[2];
	int height = renderEntity[3];

	float current_width = static_cast<float>(width);
	float current_height = static_cast<float>(height);

	float current_aspect_ratio = current_width / current_height;

	float scale_x = current_width / 640.0f;
	float scale_y = current_height / 480.0f;

	if (current_aspect_ratio > ASPECT_RATIO_4_3)
	{
		float aspect_ratio_adjustment = ASPECT_RATIO_4_3 / current_aspect_ratio;

		int adjusted_width = static_cast<int>(current_width * aspect_ratio_adjustment);
		renderEntity[2] = adjusted_width;

		int x_shift = (width - adjusted_width) / 2;
		renderEntity[0] = x_shift;
	}

	return renderEntity;
}

typedef FILE(__cdecl* sub_43E030)(const char*);
sub_43E030 FS_ZipLoading = nullptr;

static FILE __cdecl FS_ZipLoading_Hook(const char* FileName)
{
	// Skip pak5_mod.pk3
	if (std::strstr(FileName, "pak5_mod.pk3") != NULL)
	{
		FileName = "\x00";
	}

	return FS_ZipLoading(FileName);
}

typedef int(__cdecl* sub_47ABE0)(LPCSTR);
sub_47ABE0 QGL_Init = nullptr;

static int __cdecl QGL_Init_Hook(LPCSTR lpLibFileName)
{
	// Do it once
	if (!isResolutionApplied)
	{
		if (CustomResolution && !ForceBorderlessFullscreen)
		{
			// Set current resolution mode to 0
			int r_mode_ptr = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_PTR_ADDR, false);
			MemoryHelper::WriteMemory<int>(r_mode_ptr + 0x20, 0, false);

			// Replace resolution 0 with the custom one
			MemoryHelper::WriteMemory<int>(DISPLAY_MODE_ARRAY_WIDTH_ADDR, CustomResolutionWidth, false);
			MemoryHelper::WriteMemory<int>(DISPLAY_MODE_ARRAY_HEIGHT_ADDR, CustomResolutionHeight, false);

			isResolutionApplied = true;
		}
	}

	int result = QGL_Init(lpLibFileName);

	if (!isTexParameterfHooked && AnisotropicTextureFiltering)
	{
		int glTexParameterf_ptr = MemoryHelper::ReadMemory<int>(0x1BB8EF0, false);
		ApplyHook((void*)glTexParameterf_ptr, &glTexParameterf_Hook, reinterpret_cast<LPVOID*>(&original_glTexParameterf));
		isTexParameterfHooked = true;
	}

	return result;
}

typedef int(__stdcall* sub_46C600)(HWND, UINT, HDC, HWND);
sub_46C600 lpfnWndProc_MSG = nullptr;

static int __stdcall lpfnWndProc_MSG_Hook(HWND hWnd, UINT Msg, HDC hdc, HWND lParam)
{
	if ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0 && (GetAsyncKeyState(VK_F4) & 0x8000) != 0)
	{
		Msg = WM_CLOSE;
	}

	return lpfnWndProc_MSG(hWnd, Msg, hdc, lParam);
}

typedef int(__cdecl* sub_46FC70)(void*, unsigned __int8, char, unsigned __int8, int);
sub_46FC70 GLW_CreatePFD = nullptr;

static int __cdecl GLW_CreatePFD_Hook(void* pPFD, unsigned __int8 colorbits, char depthbits, unsigned __int8 stencilbits, int stereo)
{
	// Resolution updated, update the variables
	currentWidth = MemoryHelper::ReadMemory<int>(CURRENT_WIDTH_ADDR, false);
	currentHeight = MemoryHelper::ReadMemory<int>(CURRENT_HEIGHT_ADDR, false);

	// Scale the FOV for non-4:3 aspect ratios
	if (AutoFOV)
	{
		float current_width = static_cast<float>(currentWidth);
		float current_height = static_cast<float>(currentHeight);

		float current_aspect_ratio = current_width / current_height;

		float vFOV = 2.0 * atan(tan(90.0 * M_PI / 180.0 / 2.0) / ASPECT_RATIO_4_3);
		FOV = 2.0 * atan(tan(vFOV / 2.0) * current_aspect_ratio) * 180.0 / M_PI;
	}

	if (isMainMenuShown)
	{
		isCursorResized = false;
	}

	return GLW_CreatePFD(pPFD, colorbits, depthbits, stencilbits, stereo);
}

typedef void(__cdecl* sub_420390)(DWORD*, int, int*);
sub_420390 MSG_DoStuff = nullptr;

static void __cdecl UpdateFOV(DWORD* a1, int a2, int* a3)
{
	float* fovPointer = (float*)((DWORD)a2 - 0x50);

	if (*fovPointer != FOV)
	{
		*fovPointer = FOV;
	}

	MSG_DoStuff(a1, a2, a3);
}

typedef int(__cdecl* sub_419910)(const char*, const char*, int);
sub_419910 Cvar_Set = nullptr;

static int __cdecl Cvar_Set_Hook(const char* var_name, const char* value, int flag)
{
	if (TrilinearTextureFiltering && strcmp(var_name, "r_textureMode") == 0)
	{
		value = "GL_LINEAR_MIPMAP_LINEAR";
		flag = 0x10;
	}

	if (EnhancedLOD && strcmp(var_name, "r_lodbias") == 0)
	{
		value = "-2";
		flag = 0x10;
	}

	if (EnhancedLOD && strcmp(var_name, "r_lodCurveError") == 0)
	{
		value = "10000";
		flag = 0x10;
	}

	if (EnableDevConsole && strcmp(var_name, "ui_console") == 0)
	{
		value = "1";
		flag = 0x10;
	}

	if (EnableVsync && strcmp(var_name, "r_swapInterval") == 0)
	{
		value = "1";
		flag = 0x10;
	}

	if (setAlice2Path && strcmp(var_name, "s_Alice2URL") == 0)
	{
		value = Alice2Path;
		flag = 0x10;
	}

	// Read settings from "base" folder, skip it
	if (FixFullscreenSetting && !isDefaultFullscreenSettingSkipped && strcmp(var_name, "r_fullscreen") == 0)
	{
		isDefaultFullscreenSettingSkipped = true;
		return 0;
	}

	if (AutoResolution && !skipAutoResolution && strcmp(var_name, "r_mode") == 0)
	{
		if (flag != 33)
		{
			// From config.cfg
			skipAutoResolution = true;
		}
		else if (strcmp(value, "0") == 0)
		{
			// Since this function is executed after, do it before
			GameHelper::FetchDisplayResolutions();

			// Find the correct r_mode for the current resolution
			auto [screenWidth, screenHeight] = SystemHelper::GetScreenResolution();
			int resolutionNum = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_NUM, false);

			for (int i = 0; i < resolutionNum; i++)
			{
				int screenWidthMode = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_ARRAY_WIDTH_ADDR + (i * 8), false);
				int screenHeightMode = MemoryHelper::ReadMemory<int>(DISPLAY_MODE_ARRAY_HEIGHT_ADDR + (i * 8), false);

				if (screenWidth == screenWidthMode && screenHeight == screenHeightMode)
				{
					static std::string r_mode;
					r_mode = std::to_string(i);
					value = r_mode.c_str();
					break;
				}
			}
		}
	}

	if (CustomFPSLimit != 60 && strcmp(var_name, "com_maxfps") == 0)
	{
		static std::string fpsLimitStr;
		fpsLimitStr = std::to_string(CustomFPSLimit);
		value = fpsLimitStr.c_str();
		flag = 0x10;
	}

	return Cvar_Set(var_name, value, flag);
}

typedef int(__cdecl* sub_42CFF0)();
sub_42CFF0 LoadGameDLL = nullptr;

static int __cdecl LoadGameDLL_Hook()
{
	// Load the game's DLL
	int result = LoadGameDLL();

	// Get handle to "fgamex86.dll" if loaded
	HMODULE gameApiDll = GetModuleHandle(L"fgamex86.dll");

	// If DLL is loaded, get its base address and do the patching
	if (gameApiDll) {
		DWORD gameApiBaseAddress = (DWORD)gameApiDll;

		if (DisableLetterbox)
		{
			MemoryHelper::WriteMemory<int>(gameApiBaseAddress + 0x16CAA3, 0, true);
		}
	}

	return result;
}

typedef HWND(WINAPI* CreateWindowExA_t)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
CreateWindowExA_t fpCreateWindowExA = NULL;

static HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (dwStyle == 0x10C80000)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		dwStyle = WS_VISIBLE + WS_POPUP;

		nWidth = screenWidth;
		nHeight = screenHeight;

		X = 0;
		Y = 0;
	}

	return fpCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

typedef DWORD(__thiscall* sub_4C1AC0)(DWORD*, char*);
sub_4C1AC0 LoadUI = nullptr;

static DWORD __fastcall LoadUI_Hook(DWORD* ptr, int* _ECX, char* ui_path)
{
	int lang = MemoryHelper::ReadMemory<int>(CURRENT_LANG, false);

	const char* langPrefix = "INT";
	switch (lang)
	{
		case 1: langPrefix = "DEU"; break;
		case 2: langPrefix = "FRA"; break;
		case 3: langPrefix = "ESN"; break;
	}

	// Check if a controller is connected
	if (!isUsingControllerMenu)
	{
		if (GameHelper::IsControllerConnected() == 1)
		{
			isUsingControllerMenu = true;

			// Disable mouse navigation
			MemoryHelper::MakeNOP(0x40675E, 2, true);
			MemoryHelper::MakeNOP(0x40676E, 2, true);
		}
	}

	if (UseConsoleTitleScreen && ui_path != NULL && strcmp(ui_path, "ui/title.urc") == 0)
	{
		if (!isUsingControllerMenu)
		{
			ui_path = (char*)malloc(strlen(langPrefix) + strlen("/title.urc") + 1);
			sprintf(ui_path, "%s/title.urc", langPrefix);

			// Press Enter
			MemoryHelper::WriteMemory<char>(0x4082C3, 0x0D, true);
		}
		else
		{
			ui_path = (char*)malloc(strlen(langPrefix) + strlen("/title_console.urc") + 1);
			sprintf(ui_path, "%s/title_console.urc", langPrefix);
		}
	}

	if (UsePS3ControllerIcons)
	{
		char* newLangPrefix = (char*)malloc(strlen(langPrefix) + strlen("_ps3") + 1);
		sprintf(newLangPrefix, "%s_ps3", langPrefix);
		langPrefix = newLangPrefix;
	}

	if (isUsingControllerMenu && ui_path != NULL && strcmp(ui_path, "ui/controls.urc") == 0)
	{
		ui_path = (char*)malloc(strlen(langPrefix) + strlen("/controls2.urc") + 1);
		sprintf(ui_path, "%s/controls2.urc", langPrefix);
	}

	if (isUsingControllerMenu && ui_path != NULL && strcmp(ui_path, "ui/credits.urc") == 0)
	{
		ui_path = (char*)malloc(strlen(langPrefix) + strlen("/credits2.urc") + 1);
		sprintf(ui_path, "%s/credits2.urc", langPrefix);
	}

	if (isUsingControllerMenu && ui_path != NULL && strcmp(ui_path, "ui/loadsave.urc") == 0)
	{
		ui_path = (char*)malloc(strlen(langPrefix) + strlen("/loadsave2.urc") + 1);
		sprintf(ui_path, "%s/loadsave2.urc", langPrefix);
	}

	if (isUsingControllerMenu && ui_path != NULL && strcmp(ui_path, "ui/main.urc") == 0)
	{
		ui_path = (char*)malloc(strlen(langPrefix) + strlen("/main2.urc") + 1);
		sprintf(ui_path, "%s/main2.urc", langPrefix);
	}

	if (isUsingControllerMenu && ui_path != NULL && strcmp(ui_path, "ui/newgame.urc") == 0)
	{
		ui_path = (char*)malloc(strlen(langPrefix) + strlen("/newgame2.urc") + 1);
		sprintf(ui_path, "%s/newgame2.urc", langPrefix);
	}

	if (isUsingControllerMenu && ui_path != NULL && strcmp(ui_path, "ui/quit.urc") == 0)
	{
		ui_path = (char*)malloc(strlen(langPrefix) + strlen("/quit.urc") + 1);
		sprintf(ui_path, "%s/quit.urc", langPrefix);
	}

	return LoadUI(ptr, ui_path);
}

typedef float(__cdecl* sub_423740)(DWORD*, float*);
sub_423740 AliceHeadMovementCoordinates = nullptr;

static float __cdecl AliceHeadMovementCoordinates_Hook(DWORD* a1, float* a2)
{
	int isCursorShown = MemoryHelper::ReadMemory<int>(IS_CURSOR_VISIBLE, false);

	// Make sure Alice is not looking at the top left of the screen
	if (isCursorShown == 1 && isUsingControllerMenu)
	{
		*a1 = 0xC119EB80;
		*(a1 + 1) = 0xC2100000;
	}
	return AliceHeadMovementCoordinates(a1, a2);
}

typedef int(__cdecl* sub_44C1B0)(const char*);
sub_44C1B0 PushMenu = nullptr;

static int __cdecl PushMenu_Hook(const char* menu_name)
{
	if (FixMenuTransitionTiming && strcmp(menu_name, "newgame") == 0)
	{
		MemoryHelper::WriteMemory<int>(0x12EF948, 1200, false);
	}

	if (!isTitleBgSet && strcmp(menu_name, "main") == 0)
	{
		isTitleBgSet = true;
		GameHelper::ResizeCursor(true, currentWidth, currentHeight);
		return PushMenu("title");
	}
	else
	{
		return PushMenu(menu_name);
	}
}

typedef void(__cdecl* sub_44C4F0)(int);
sub_44C4F0 TriggerMainMenu = nullptr;

static void __cdecl TriggerMainMenu_Hook(int a1)
{
	if (isMainMenuShown)
	{
		TriggerMainMenu(a1);
	}
	else
	{
		PushMenu_Hook("main");
	}
}

typedef int(__cdecl* sub_449DF0)();
sub_449DF0 IsGameStarted = nullptr;

static int __cdecl IsGameStarted_Hook()
{
	if (isMainMenuShown)
	{
		return IsGameStarted();
	}
	else
	{
		return 0;
	}
}

typedef void(__cdecl* sub_429600)();
sub_429600 PlayIntroMusic = nullptr;

static void __cdecl PlayIntroMusic_Hook()
{
	if (FixProton && !isRestartedForLinux)
	{
		GameHelper::CallCmd("vid_restart\n", 0);
		isRestartedForLinux = true;
	}
	PlayIntroMusic();
}

#pragma endregion Hooks with MinHook

#pragma region

static void ApplyFixSoundRandomization()
{
	if (!FixSoundRandomization) return;

	unsigned char portedInstructions[0x3C0] = {
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

	MemoryHelper::WriteMemoryRaw(0x00513490, portedInstructions, sizeof(portedInstructions), true);

	MemoryHelper::MakeCALL(0x00402131, 0x005137A0, true);
	MemoryHelper::MakeJMP(0x004348BF, 0x005137A0, true);
	MemoryHelper::MakeJMP(0x0043491F, 0x005136B0, true);
	MemoryHelper::MakeJMP(0x0043494F, 0x00513720, true);
}

static void ApplyFixHardDiskFull()
{
	if (!FixHardDiskFull) return;

	ApplyHook((void*)0x41D1E0, &CheckDiskFreeSpace_Hook, reinterpret_cast<LPVOID*>(&CheckDiskFreeSpace));
}

static void ApplyFixSaveScreenshotBufferOverflow()
{
	if (!FixSaveScreenshotBufferOverflow) return;

	// JMP Redirect
	MemoryHelper::MakeJMP(0x0046D292, 0x00513850, true);

	char leaInstruction[] = { 0x8D, 0x04, 0x40 };
	char addInstruction[] = { 0x05 };
	char pushInstruction[] = { 0x57, 0x50 };

	MemoryHelper::WriteMemoryRaw(0x00513850, leaInstruction, sizeof(leaInstruction), true);
	MemoryHelper::WriteMemoryRaw(0x00513853, addInstruction, sizeof(addInstruction), true);
	MemoryHelper::WriteMemory<int>(0x00513854, 0x1000, true); // Hack: Increase the buffer size by 0x1000
	MemoryHelper::WriteMemoryRaw(0x00513858, pushInstruction, sizeof(pushInstruction), true);

	// Back to the function
	MemoryHelper::MakeJMP(0x0051385A, 0x0046D297, true);
}

static void ApplyFixBlinkingAnimationSpeed()
{
	if (!FixBlinkingAnimationSpeed) return;

	// 20FPS Speed
	int blinkRate = 20;

	// First JMP Redirect
	MemoryHelper::MakeJMP(0x4C6367, 0x513E08, true);

	char cmpInstruction[] = { 0x81, 0xF9 };
	char addInstruction[] = { 0x8B, 0xF0 };

	MemoryHelper::WriteMemoryRaw(0x513E08, cmpInstruction, sizeof(cmpInstruction), true);
	MemoryHelper::WriteMemory<int>(0x513E0A, 100 * CustomFPSLimit / blinkRate, true);
	MemoryHelper::WriteMemoryRaw(0x513E0E, addInstruction, sizeof(addInstruction), true);

	// Back to the function
	MemoryHelper::MakeJMP(0x513E10, 0x4C636C, true);

	// Second JMP Redirect
	MemoryHelper::MakeJMP(0x4C638D, 0x513E20, true);

	char imulInstruction[] = { 0x69, 0xC0 };
	//char subInstruction[] = { 0x2B, 0xC8 };

	MemoryHelper::WriteMemoryRaw(0x513E20, imulInstruction, sizeof(imulInstruction), true);
	MemoryHelper::WriteMemory<int>(0x513E22, 100 * CustomFPSLimit / blinkRate, true);
	//MemoryHelper::WriteMemoryRaw(0x513E26, subInstruction, sizeof(subInstruction), true); // ???
	MemoryHelper::MakeNOP(0x513E26, 2, true);

	MemoryHelper::WriteMemoryRaw(0x513E28, cmpInstruction, sizeof(cmpInstruction), true);
	MemoryHelper::WriteMemory<int>(0x513E2A, 4 * CustomFPSLimit / 60, true);

	// Back to the function
	MemoryHelper::MakeJMP(0x513E2E, 0x4C6395, true);
}

static void ApplyFixStretchedHUD()
{
	if (!FixStretchedHUD && !FixStretchedGUI) return;

	ApplyHook((void*)0x446050, &SetHUDPosition_Hook, reinterpret_cast<LPVOID*>(&SetHUDPosition));

	// hud_item_foldout
	MemoryHelper::WriteMemory<float>(0x5218A8, 258.5f, true);
	// hud_weapon_foldout
	MemoryHelper::WriteMemory<float>(0x5218F8, -258.5f, true);
}

static void ApplyFixStretchedFMV()
{
	if (!FixStretchedFMV) return;

	ApplyHook((void*)0x490130, &SetFMVPosition_Hook, reinterpret_cast<LPVOID*>(&SetFMVPosition));
}

static void ApplyFixStretchedGUI()
{
	if (!FixStretchedGUI) return;

	ApplyHook((void*)0x48FC00, &RenderShader_Hook, reinterpret_cast<LPVOID*>(&RenderShader)); // UI Scaling
	ApplyHook((void*)0x44B100, &SetUIBorder_Hook, reinterpret_cast<LPVOID*>(&SetUIBorder)); // Add the borders
	ApplyHook((void*)0x4C0A10, &GetGlyphInfo_Hook, reinterpret_cast<LPVOID*>(&GetGlyphInfo)); // Font Scaling
	ApplyHook((void*)0x4C5D30, &SetAliceMirrorViewportParams_Hook, reinterpret_cast<LPVOID*>(&SetAliceMirrorViewportParams)); // Scale Alice's 3D model in the settings menu
	MemoryHelper::MakeNOP(0x4D2AB1, 7, true); // dark rectangle when reassigning a control
}

typedef HRESULT(WINAPI* SetProcessDpiAwarenessProc)(PROCESS_DPI_AWARENESS);
typedef BOOL(WINAPI* SetProcessDpiAwarenessContextProc)(DPI_AWARENESS_CONTEXT);

static void ApplyFixDPIScaling()
{
	if (!FixDPIScaling) return;

	if (IsWindows10OrGreater())
	{
		HMODULE user32 = LoadLibrary(L"user32.dll");
		if (user32)
		{
			auto setDpiAwarenessContext = (SetProcessDpiAwarenessContextProc)GetProcAddress(user32, "SetProcessDpiAwarenessContext");
			if (setDpiAwarenessContext)
			{
				setDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
			}
			FreeLibrary(user32);
		}
	}
	else if (IsWindows8Point1OrGreater())
	{
		HMODULE shcore = LoadLibrary(L"shcore.dll");
		if (shcore)
		{
			auto setDpiAwareness = (SetProcessDpiAwarenessProc)GetProcAddress(shcore, "SetProcessDpiAwareness");
			if (setDpiAwareness)
			{
				setDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
			}
			FreeLibrary(shcore);
		}
	}
	else if (IsWindows7OrGreater())
	{
		HMODULE user32 = LoadLibrary(L"user32.dll");
		if (user32)
		{
			typedef BOOL(WINAPI* SetProcessDPIAwareProc)();
			auto setProcessDPIAware = (SetProcessDPIAwareProc)GetProcAddress(user32, "SetProcessDPIAware");
			if (setProcessDPIAware)
			{
				setProcessDPIAware();
			}
			FreeLibrary(user32);
		}
	}
}

static void ApplyFixMenuTransitionTiming()
{
	if (!FixMenuTransitionTiming) return;

	MemoryHelper::WriteMemory<int>(0x4082FC, 0x3200, true);
}

static void ApplyFixProton()
{
	if (!FixProton) return;

	ApplyHook((void*)0x429600, &PlayIntroMusic_Hook, reinterpret_cast<LPVOID*>(&PlayIntroMusic));
}

static void ApplyLaunchWithoutAlice2()
{
	if (!LaunchWithoutAlice2) return;

	MemoryHelper::WriteMemory<int>(0x526A84, 0x00, true);
}

static void ApplyPreventAlice2OnExit()
{
	if (!PreventAlice2OnExit) return;

	MemoryHelper::MakeNOP(0x4642F0, 5, true);
}

static void ApplyLanguageId()
{
	MemoryHelper::WriteMemory<int>(0x461A90, LanguageId, true);
}

static void ApplyUseConsoleTitleScreen()
{
	if (!UseConsoleTitleScreen) return;

	ApplyHook((void*)0x4C1AC0, &LoadUI_Hook, reinterpret_cast<LPVOID*>(&LoadUI));
	ApplyHook((void*)0x44C1B0, &PushMenu_Hook, reinterpret_cast<LPVOID*>(&PushMenu));
	ApplyHook((void*)0x44C4F0, &TriggerMainMenu_Hook, reinterpret_cast<LPVOID*>(&TriggerMainMenu));
	ApplyHook((void*)0x449DF0, &IsGameStarted_Hook, reinterpret_cast<LPVOID*>(&IsGameStarted));
}

static void ApplyUseOriginalIntroVideos()
{
	if (!UseOriginalIntroVideos) return;

	unsigned char portedIntroData[0x270] = {
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

	MemoryHelper::WriteMemoryRaw(0x00513B90, portedIntroData, sizeof(portedIntroData), true);

	MemoryHelper::MakeJMP(0x0044D91F, 0x00513B90, true);
	MemoryHelper::WriteMemory(0x0044E409, 0x00513B90, true);
}

static void ApplyDisableRemasteredModels()
{
	if (!DisableRemasteredModels) return;

	ApplyHook((void*)0x43E030, &FS_ZipLoading_Hook, reinterpret_cast<LPVOID*>(&FS_ZipLoading));
}

static void ApplyEnableDevConsole()
{
	if (!EnableDevConsole) return;

	int toggleConsoleKeyId = GameHelper::GetKeyId(ToggleConsoleBindKey);

	char cmpInstruction[] = { 0x81, 0xFF };
	MemoryHelper::WriteMemoryRaw(0x40823A, cmpInstruction, sizeof(cmpInstruction), true);
	MemoryHelper::WriteMemory<int>(0x40823C, toggleConsoleKeyId, true);
	MemoryHelper::MakeNOP(0x408240, 6, true);
}

static void ApplyCustomSavePath()
{
	if (!isUsingCustomSaveDir && !FixHardDiskFull) return;

	ApplyHook((void*)0x417400, &GetSavePath_Hook, reinterpret_cast<LPVOID*>(&GetSavePath));
}

static void ApplyEnableControllerIcons()
{
	if (!EnableControllerIcons) return;

	if (!UseConsoleTitleScreen)
	{
		ApplyHook((void*)0x4C1AC0, &LoadUI_Hook, reinterpret_cast<LPVOID*>(&LoadUI));
	}

	ApplyHook((void*)0x423740, &AliceHeadMovementCoordinates_Hook, reinterpret_cast<LPVOID*>(&AliceHeadMovementCoordinates));
}

static void ApplyHideConsoleAtLaunch()
{
	if (!HideConsoleAtLaunch) return;

	MemoryHelper::WriteMemory<char>(0x46C28F, 0x00, true);
}

static void ApplyDisableLetterbox()
{
	if (!DisableLetterbox) return;

	ApplyHook((void*)0x42CFF0, &LoadGameDLL_Hook, reinterpret_cast<LPVOID*>(&LoadGameDLL));
}

static void ApplyForceBorderlessFullscreen()
{
	if (!ForceBorderlessFullscreen) return;

	ApplyHook(&CreateWindowExA, &CreateWindowExA_Hook, (LPVOID*)&fpCreateWindowExA);
}

static void ApplyCustomResolutionAndAnisotropic()
{
	if (!CustomResolution && !AnisotropicTextureFiltering) return;

	ApplyHook((void*)0x47ABE0, &QGL_Init_Hook, reinterpret_cast<LPVOID*>(&QGL_Init));
}

static void ApplyEnableAltF4Close()
{
	if (!EnableAltF4Close) return;

	ApplyHook((void*)0x46C600, &lpfnWndProc_MSG_Hook, reinterpret_cast<LPVOID*>(&lpfnWndProc_MSG));
}

static void ApplyCustomFOV()
{
	if (!AutoFOV && FOV == 90.0) return;

	ApplyHook((void*)0x420390, &UpdateFOV, reinterpret_cast<LPVOID*>(&MSG_DoStuff));
}

static void ApplyCvarTweaks()
{
	if (!CvarHooking) return;

	ApplyHook((void*)0x419910, &Cvar_Set_Hook, reinterpret_cast<LPVOID*>(&Cvar_Set));
}

static void ApplyResolutionChangeHook()
{
	if (!AutoFOV && !FixStretchedGUI) return;

	ApplyHook((void*)0x46FC70, &GLW_CreatePFD_Hook, reinterpret_cast<LPVOID*>(&GLW_CreatePFD));
}

#pragma endregion Apply Patches

#pragma region

static void Init()
{
	ReadConfig();
	// Fixes
	ApplyFixSoundRandomization();
	ApplyFixHardDiskFull();
	ApplyFixSaveScreenshotBufferOverflow();
	ApplyFixBlinkingAnimationSpeed();
	ApplyFixStretchedHUD();
	ApplyFixStretchedFMV();
	ApplyFixStretchedGUI();
	ApplyFixDPIScaling();
	ApplyFixMenuTransitionTiming();
	ApplyFixProton();
	// General
	ApplyLaunchWithoutAlice2();
	ApplyPreventAlice2OnExit();
	ApplyLanguageId();
	ApplyUseConsoleTitleScreen();
	ApplyUseOriginalIntroVideos();
	ApplyDisableRemasteredModels();
	ApplyEnableDevConsole();
	ApplyCustomSavePath();
	// Display
	ApplyEnableControllerIcons();
	ApplyHideConsoleAtLaunch();
	ApplyDisableLetterbox();
	ApplyForceBorderlessFullscreen();
	ApplyCustomResolutionAndAnisotropic();
	ApplyEnableAltF4Close();
	// Graphics
	ApplyCustomFOV();
	// Misc
	ApplyCvarTweaks();
	ApplyResolutionChangeHook();
}

static void LoadProxyLibrary()
{
	wchar_t systemPath[MAX_PATH];
	GetSystemDirectoryW(systemPath, MAX_PATH);
	lstrcatW(systemPath, L"\\winmm.dll");

	HINSTANCE hL = LoadLibraryExW(systemPath, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!hL)
	{
		DWORD errorCode = GetLastError();
		wchar_t errorMessage[512];

		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), errorMessage, sizeof(errorMessage) / sizeof(wchar_t), NULL);
		MessageBoxW(NULL, errorMessage, L"Error Loading winmm.dll", MB_ICONERROR);
		return;
	}

	winmm.ProxySetup(hL);
}


static BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			LoadProxyLibrary();
			Init();
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
	return TRUE;
}

#pragma endregion Initialization