#include <SDL3/SDL.h>

namespace MemoryHelper
{
	template <typename T> static bool WriteMemory(uintptr_t address, T value, bool disableProtection = true)
	{
		DWORD oldProtect;
		if (disableProtection)
		{
			if (!VirtualProtect(reinterpret_cast <LPVOID> (address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
		}
		*reinterpret_cast <T*> (address) = value;
		if (disableProtection)
		{
			VirtualProtect(reinterpret_cast <LPVOID> (address), sizeof(T), oldProtect, &oldProtect);
		}
		return true;
	}

	static bool WriteMemoryRaw(uintptr_t address, const void* data, size_t size, bool disableProtection = true)
	{
		DWORD oldProtect;
		if (disableProtection)
		{
			if (!VirtualProtect(reinterpret_cast <LPVOID> (address), size, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
		}
		std::memcpy(reinterpret_cast <void*> (address), data, size);
		if (disableProtection)
		{
			VirtualProtect(reinterpret_cast <LPVOID> (address), size, oldProtect, &oldProtect);
		}
		return true;
	}

	static bool MakeNOP(uintptr_t address, size_t count, bool disableProtection = true)
	{
		DWORD oldProtect;
		if (disableProtection)
		{
			if (!VirtualProtect(reinterpret_cast <LPVOID> (address), count, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
		}
		std::memset(reinterpret_cast <void*> (address), 0x90, count);
		if (disableProtection)
		{
			VirtualProtect(reinterpret_cast <LPVOID> (address), count, oldProtect, &oldProtect);
		}
		return true;
	}

	static bool MakeCALL(uintptr_t srcAddress, uintptr_t destAddress, bool disableProtection = true)
	{
		DWORD oldProtect;
		if (disableProtection)
		{
			if (!VirtualProtect(reinterpret_cast <LPVOID> (srcAddress), 5, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
		}
		uintptr_t relativeAddress = destAddress - srcAddress - 5; *reinterpret_cast <uint8_t*> (srcAddress) = 0xE8; // CALL opcode
		*reinterpret_cast <uintptr_t*> (srcAddress + 1) = relativeAddress;
		if (disableProtection)
		{
			VirtualProtect(reinterpret_cast <LPVOID> (srcAddress), 5, oldProtect, &oldProtect);
		}
		return true;
	}

	static bool MakeJMP(uintptr_t srcAddress, uintptr_t destAddress, bool disableProtection = true)
	{
		DWORD oldProtect;
		if (disableProtection)
		{
			if (!VirtualProtect(reinterpret_cast <LPVOID> (srcAddress), 5, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
		}
		uintptr_t relativeAddress = destAddress - srcAddress - 5; *reinterpret_cast <uint8_t*> (srcAddress) = 0xE9; // JMP opcode
		*reinterpret_cast <uintptr_t*> (srcAddress + 1) = relativeAddress;
		if (disableProtection)
		{
			VirtualProtect(reinterpret_cast <LPVOID> (srcAddress), 5, oldProtect, &oldProtect);
		}
		return true;
	}

	template <typename T> static T ReadMemory(uintptr_t address, bool disableProtection = false)
	{
		DWORD oldProtect;
		if (disableProtection)
		{
			if (!VirtualProtect(reinterpret_cast <LPVOID> (address), sizeof(T), PAGE_EXECUTE_READ, &oldProtect)) return T();
		}
		T value = *reinterpret_cast <T*> (address);
		if (disableProtection)
		{
			VirtualProtect(reinterpret_cast <LPVOID> (address), sizeof(T), oldProtect, &oldProtect);
		}
		return value;
	}
};

namespace IniHelper
{
	mINI::INIFile iniFile("VorpalFix.ini");
	mINI::INIStructure iniReader;

	void Init()
	{
		iniFile.read(iniReader);
	}

	void Save()
	{
		iniFile.write(iniReader);
	}

	char* ReadString(const char* sectionName, const char* valueName, const char* defaultValue)
	{
		char* result = new char[255];
		try
		{
			if (iniReader.has(sectionName) && iniReader.get(sectionName).has(valueName))
			{
				std::string value = iniReader.get(sectionName).get(valueName);

				if (!value.empty() && (value.front() == '\"' || value.front() == '\''))
					value.erase(0, 1);
				if (!value.empty() && (value.back() == '\"' || value.back() == '\''))
					value.erase(value.size() - 1);

				strncpy(result, value.c_str(), 254);
				result[254] = '\0';
				return result;
			}
		}
		catch (...) {}

		strncpy(result, defaultValue, 254);
		result[254] = '\0';
		return result;
	}

	float ReadFloat(const char* sectionName, const char* valueName, float defaultValue)
	{
		try
		{
			if (iniReader.has(sectionName) && iniReader.get(sectionName).has(valueName))
			{
				const std::string& s = iniReader.get(sectionName).get(valueName);
				if (!s.empty())
					return std::stof(s);
			}
		}
		catch (...) {}
		return defaultValue;
	}

	int ReadInteger(const char* sectionName, const char* valueName, int defaultValue)
	{
		try
		{
			if (iniReader.has(sectionName) && iniReader.get(sectionName).has(valueName))
			{
				const std::string& s = iniReader.get(sectionName).get(valueName);
				if (!s.empty())
					return std::stoi(s);
			}
		}
		catch (...) {}
		return defaultValue;
	}
};

namespace GameHelper
{
	constexpr float ASPECT_RATIO_4_3 = 4.0f / 3.0f;

	enum class ShaderType : int8_t
	{
		Unknown = -1,
		Generic = 0,
		MouseArrow,
		TitleBg,
		DrugFade,
		IceFade,
		PressAnyKey,
		Slider2Bar,
		Slider2Top,
		Slider2Indicator,
		UiBar,
		CreditsAlice,
		CreditsBill,
		QuicksaveCam,
	};

	static std::vector<ShaderType> shaderTypeCache;

	bool DisableCursorScaling = false;

	int(__cdecl* GetKeyId)(const char*) = (int(__cdecl*)(const char*))0x4076F0;
	int(__cdecl* Bind)(int, const char*) = (int(__cdecl*)(int, const char*))0x407870;
	int(__cdecl* VidRestart)() = (int(__cdecl*)())0x40B2F0;
	int(__cdecl* UpdateCvar)(const char*, const char*, int) = (int(__cdecl*)(const char*, const char*, int))0x418D90;
	char* (__cdecl* GetWeaponName)(int) = (char* (__cdecl*)(int))0x441D60;
	int(__cdecl* UI_GetStaticMap)(unsigned int, const char*) = (int(__cdecl*)(unsigned int, const char*))0x44A300;
	int(__cdecl* IsControllerConnected)() = (int(__cdecl*)())0x463130;

	static void AssignCmdKeyId(int keyId, const char* cmd)
	{
		char* KeyCommandName = *(char**)(0x14EA2A8 + 0xC * keyId);

		if (strcmp(KeyCommandName, cmd) != 0)
		{
			Bind(keyId, cmd);
		}
	}

	static int FindShaderIndex(const char* texturePath)
	{
		int ShaderNum = MemoryHelper::ReadMemory<int>(0x1BCCEEC);
		int ShaderIndex = 0x1BCCEF0;

		for (int i = 0; i < ShaderNum; i++)
		{
			const char* currentTexture = (const char*)ShaderIndex;

			if (strcmp(currentTexture, texturePath) == 0)
			{
				return ShaderIndex;
			}

			ShaderIndex += 0x80;
		}

		return -1;
	}

	static void __cdecl ResizeCursor(bool hide, int width, int height)
	{
		int ImageIndex = FindShaderIndex("gfx/2d/mouse_arrow.tga");

		if (ImageIndex == -1) return;

		const int originalWidth = 640;
		const int originalHeight = 480;

		const int mouseWidth = 16;
		const int mouseHeight = 32;

		if (hide)
		{
			MemoryHelper::WriteMemory<int>(ImageIndex + 0x40, 0, false);
			MemoryHelper::WriteMemory<int>(ImageIndex + 0x44, 0, false);
			return;
		}

		float widthScale = static_cast<float>(width) / originalWidth;
		float heightScale = static_cast<float>(height) / originalHeight;

		float scaleFactor = std::min(widthScale, heightScale);

		int scaledMouseWidth = static_cast<int>(mouseWidth * scaleFactor);
		int scaledMouseHeight = static_cast<int>(mouseHeight * scaleFactor);

		if (DisableCursorScaling)
		{
			MemoryHelper::WriteMemory<int>(ImageIndex + 0x40, mouseWidth, false);
			MemoryHelper::WriteMemory<int>(ImageIndex + 0x44, mouseHeight, false);
		}
		else
		{
			MemoryHelper::WriteMemory<int>(ImageIndex + 0x40, scaledMouseWidth, false);
			MemoryHelper::WriteMemory<int>(ImageIndex + 0x44, scaledMouseHeight, false);
		}
	}

	static void __cdecl ResizePopupMessage(int width, int height)
	{
		int ImageIndex = FindShaderIndex("ui/control/press_any_key.tga");

		if (ImageIndex == -1) return;

		int image_width = MemoryHelper::ReadMemory<int>(ImageIndex + 0x48);

		float current_width = static_cast<float>(width);
		float current_height = static_cast<float>(height);

		float target_width = current_height * ASPECT_RATIO_4_3;
		float scale_factor = target_width / current_width;
		image_width = static_cast<int>(image_width * scale_factor);

		MemoryHelper::WriteMemory<int>(ImageIndex + 0x48, image_width, false);
	}

	static void ResetShaderCache()
	{
		shaderTypeCache.clear();
	}

	static ShaderType ClassifyShader(int handle, const char* name)
	{
		if (handle >= static_cast<int>(shaderTypeCache.size()))
			shaderTypeCache.resize(handle + 128, ShaderType::Unknown);

		ShaderType& cached = shaderTypeCache[handle];
		if (cached != ShaderType::Unknown)
			return cached;

		if (strcmp(name, "gfx/2d/mouse_arrow") == 0)
			cached = ShaderType::MouseArrow;
		else if (strcmp(name, "ui/quicksavecam/quicksavecam") == 0)
			cached = ShaderType::QuicksaveCam;
		else if (strcmp(name, "textures/special/drugfade") == 0)
			cached = ShaderType::DrugFade;
		else if (strcmp(name, "textures/special/icefade") == 0)
			cached = ShaderType::IceFade;
		else if (strcmp(name, "ui/control/press_any_key") == 0)
			cached = ShaderType::PressAnyKey;
		else if (strcmp(name, "ui/control/slider2_bar") == 0)
			cached = ShaderType::Slider2Bar;
		else if (strcmp(name, "ui/control/slider2_top") == 0)
			cached = ShaderType::Slider2Top;
		else if (strcmp(name, "ui/control/slider2_indicator") == 0)
			cached = ShaderType::Slider2Indicator;
		else if (strcmp(name, "ui/bar") == 0)
			cached = ShaderType::UiBar;
		else if (strcmp(name, "ui/credits/alice") == 0)
			cached = ShaderType::CreditsAlice;
		else if (strcmp(name, "ui/credits/bill") == 0)
			cached = ShaderType::CreditsBill;
		else if (strcmp(name, "title_bg") == 0)
			cached = ShaderType::TitleBg;
		else
			cached = ShaderType::Generic;

		return cached;
	}
};

namespace SystemHelper
{
	static bool IsNative()
	{
		// env check
		const char* protonEnvVars[] = 
		{
			"STEAM_COMPAT_DATA_PATH",
			"STEAM_COMPAT_TOOL_PATHS",
			"STEAM_COMPAT_PROTON",
			"STEAM_COMPAT_MOUNTS",
			"PROTON_CRASH_REPORT_DIR",
			"PROTON_USE_XALIA",
			"SteamVirtualGamepadInfo_Proton",
			"SteamDeck",
			"SteamOS"
		};

		for (const char* var : protonEnvVars)
		{
			if (std::getenv(var))
				return false;
		}

		// Fallback: dll check
		HMODULE hntdll = GetModuleHandleA("ntdll.dll");
		if (hntdll && GetProcAddress(hntdll, "wine_get_version"))
			return false;

		// Fallback: registry check
		HKEY hKey;
		if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Wine", 0, KEY_READ, &hKey) == ERROR_SUCCESS) 
		{
			RegCloseKey(hKey);
			return false;
		}

		return true;
	}

	static DWORD GetCurrentDisplayFrequency()
	{
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);

		if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		{
			return devMode.dmDisplayFrequency;
		}
		return 60;
	}

	static std::pair<DWORD, DWORD> GetScreenResolution()
	{
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);

		if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		{
			return { devMode.dmPelsWidth, devMode.dmPelsHeight };
		}
		return { 0, 0 };
	}

	std::vector<std::string> GetLocPk3Files(const std::string& path) 
	{
		std::vector<std::string> pk3Files;

		if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
		{
			return pk3Files;
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) 
		{
			if (entry.is_regular_file() && entry.path().extension() == ".pk3") 
			{
				std::string relativePath = entry.path().string();
				relativePath = relativePath.substr(relativePath.find("loc/"));

				pk3Files.push_back(relativePath);
			}
		}

		return pk3Files;
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
};

namespace HookHelper
{
	bool isMHInitialized = false;

	static bool InitializeMinHook()
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
				return false;
			}
		}
		return true;
	}

	static void ApplyHook(void* addr, LPVOID hookFunc, LPVOID* originalFunc)
	{
		if (!InitializeMinHook()) return;

		MH_STATUS status = MH_CreateHook(addr, hookFunc, originalFunc);
		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to create hook at address: %p\nError: %s", addr, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return;
		}

		status = MH_EnableHook(addr);
		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to enable hook at address: %p\nError: %s", addr, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	static void ApplyHookAPI(LPCWSTR moduleName, LPCSTR apiName, LPVOID hookFunc, LPVOID* originalFunc)
	{
		if (!InitializeMinHook()) return;

		MH_STATUS status = MH_CreateHookApi(moduleName, apiName, hookFunc, originalFunc);
		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to create hook for API: %s\nError: %s", apiName, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return;
		}

		status = MH_EnableHook(MH_ALL_HOOKS);
		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to enable hook for API: %s\nError: %s", apiName, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
};

namespace StringHelper
{
	const char* IntegerToCString(int value) 
	{
		static thread_local char buffer[32];
		snprintf(buffer, sizeof(buffer), "%d", value);
		return buffer;
	}

	const char* FloatToCString(float value, int precision = 0)
	{
		static thread_local char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.*f", precision, value);
		return buffer;
	}

	const char* BoolToCString(bool value)
	{
		return value ? "1" : "0";
	}

	bool stricmp(const char* str1, const char* str2) 
	{
		if (!str1 || !str2) 
		{
			return false;
		}

		while (*str1 && *str2) 
		{
			if (tolower(static_cast<unsigned char>(*str1)) != tolower(static_cast<unsigned char>(*str2))) 
			{
				return false;
			}
			++str1;
			++str2;
		}

		return *str1 == '\0' && *str2 == '\0';
	}

	static std::unordered_map<std::string, std::string> pathCache;

	const char* ConstructPath(const char* prefix, const char* suffix)
	{
		std::string key = std::string(prefix) + suffix;

		auto it = pathCache.find(key);
		if (it != pathCache.end())
		{
			return it->second.c_str();
		}

		pathCache[key] = key;
		return pathCache[key].c_str();
	}
};

namespace ControllerHelper
{
	// ==========================================================
	// Types and Enums
	// ==========================================================

	enum class GamepadStyle
	{
		Xbox,
		PlayStation,
		Nintendo,
		Unknown
	};

	struct GamepadCapabilities
	{
		GamepadStyle style = GamepadStyle::Unknown;
		const char* name = nullptr;
		Uint16 vendorId = 0;
		Uint16 productId = 0;
	};

	// ==========================================================
	// Static State
	// ==========================================================

	static SDL_Gamepad* s_pGamepad = nullptr;
	static GamepadCapabilities s_capabilities;

	// ==========================================================
	// Controller Database
	// ==========================================================

	static bool LoadGamepadMappings()
	{
		const char* paths[] =
		{
			"gamecontrollerdb.txt",
			"../gamecontrollerdb.txt",
		};

		for (const char* path : paths)
		{
			int result = SDL_AddGamepadMappingsFromFile(path);
			if (result >= 0)
			{
				return true;
			}
		}

		return false;
	}

	// ==========================================================
	// Capability Detection
	// ==========================================================

	static GamepadStyle DetectGamepadStyle(SDL_Gamepad* pGamepad)
	{
		if (!pGamepad)
			return GamepadStyle::Unknown;

		SDL_GamepadType type = SDL_GetGamepadType(pGamepad);

		switch (type)
		{
			case SDL_GAMEPAD_TYPE_XBOX360:
			case SDL_GAMEPAD_TYPE_XBOXONE:
				return GamepadStyle::Xbox;

			case SDL_GAMEPAD_TYPE_PS3:
			case SDL_GAMEPAD_TYPE_PS4:
			case SDL_GAMEPAD_TYPE_PS5:
				return GamepadStyle::PlayStation;

			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
				return GamepadStyle::Nintendo;

			default:
				return GamepadStyle::Xbox;
		}
	}

	static void LoadGamepadCapabilities(SDL_Gamepad* pGamepad)
	{
		s_capabilities = GamepadCapabilities();

		if (!pGamepad)
			return;

		s_capabilities.style = DetectGamepadStyle(pGamepad);
		s_capabilities.name = SDL_GetGamepadName(pGamepad);
		s_capabilities.vendorId = SDL_GetGamepadVendor(pGamepad);
		s_capabilities.productId = SDL_GetGamepadProduct(pGamepad);
	}

	// ==========================================================
	// Initialization / Shutdown
	// ==========================================================

	bool InitializeSDLGamepad()
	{
		if (!SDL_Init(SDL_INIT_GAMEPAD))
		{
			return false;
		}

		LoadGamepadMappings();

		return true;
	}

	void ShutdownSDLGamepad()
	{
		if (s_pGamepad)
		{
			SDL_CloseGamepad(s_pGamepad);
			s_pGamepad = nullptr;
		}

		s_capabilities = GamepadCapabilities();

		SDL_Quit();
	}

	// ==========================================================
	// Accessors
	// ==========================================================

	SDL_Gamepad* GetGamepad()
	{
		return s_pGamepad;
	}

	const GamepadCapabilities& GetCapabilities()
	{
		return s_capabilities;
	}

	GamepadStyle GetGamepadStyle()
	{
		return s_capabilities.style;
	}

	bool IsConnected()
	{
		return s_pGamepad && SDL_GamepadConnected(s_pGamepad);
	}

	// ==========================================================
	// Event Processing
	// ==========================================================

	static void OnGamepadConnected(SDL_JoystickID deviceId)
	{
		if (s_pGamepad)
			return;

		s_pGamepad = SDL_OpenGamepad(deviceId);
		if (s_pGamepad)
		{
			LoadGamepadCapabilities(s_pGamepad);
		}
	}

	static void OnGamepadDisconnected(SDL_JoystickID deviceId)
	{
		if (!s_pGamepad)
			return;

		if (deviceId == SDL_GetGamepadID(s_pGamepad))
		{
			SDL_CloseGamepad(s_pGamepad);
			s_pGamepad = nullptr;
			s_capabilities = GamepadCapabilities();
		}
	}

	static void ProcessSDLEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_GAMEPAD_ADDED:
					OnGamepadConnected(event.gdevice.which);
					break;

				case SDL_EVENT_GAMEPAD_REMOVED:
					OnGamepadDisconnected(event.gdevice.which);
					break;
			}
		}
	}

	// ==========================================================
	// Main Poll Function
	// ==========================================================

	DWORD PollController(XINPUT_STATE* pState)
	{
		ProcessSDLEvents();

		// Check if controller is connected
		if (!s_pGamepad || !SDL_GamepadConnected(s_pGamepad))
		{
			return ERROR_DEVICE_NOT_CONNECTED; // 1167
		}

		// Packet number (increment each call)
		static DWORD s_packetNumber = 0;
		pState->dwPacketNumber = ++s_packetNumber;

		// ==========================================================
		// Buttons
		// ==========================================================

		WORD buttons = 0;

		// D-Pad
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))
			buttons |= XINPUT_GAMEPAD_DPAD_UP;
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
			buttons |= XINPUT_GAMEPAD_DPAD_DOWN;
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
			buttons |= XINPUT_GAMEPAD_DPAD_LEFT;
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
			buttons |= XINPUT_GAMEPAD_DPAD_RIGHT;

		// Start/Back
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_START))
			buttons |= XINPUT_GAMEPAD_START;
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_BACK))
			buttons |= XINPUT_GAMEPAD_BACK;

		// Thumbstick clicks
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_LEFT_STICK))
			buttons |= XINPUT_GAMEPAD_LEFT_THUMB;
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_RIGHT_STICK))
			buttons |= XINPUT_GAMEPAD_RIGHT_THUMB;

		// Shoulders
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER))
			buttons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
		if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER))
			buttons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;

		// Face buttons, swap A/B and X/Y for Nintendo
		bool swapButtons = (s_capabilities.style == GamepadStyle::Nintendo);

		if (swapButtons)
		{
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_EAST))
				buttons |= XINPUT_GAMEPAD_A;
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_SOUTH))
				buttons |= XINPUT_GAMEPAD_B;
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_NORTH))
				buttons |= XINPUT_GAMEPAD_X;
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_WEST))
				buttons |= XINPUT_GAMEPAD_Y;
		}
		else
		{
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_SOUTH))
				buttons |= XINPUT_GAMEPAD_A;
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_EAST))
				buttons |= XINPUT_GAMEPAD_B;
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_WEST))
				buttons |= XINPUT_GAMEPAD_X;
			if (SDL_GetGamepadButton(s_pGamepad, SDL_GAMEPAD_BUTTON_NORTH))
				buttons |= XINPUT_GAMEPAD_Y;
		}

		pState->Gamepad.wButtons = buttons;

		// ==========================================================
		// Triggers (SDL: 0-32767 -> XInput: 0-255)
		// ==========================================================

		Sint16 leftTrigger = SDL_GetGamepadAxis(s_pGamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
		Sint16 rightTrigger = SDL_GetGamepadAxis(s_pGamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);

		pState->Gamepad.bLeftTrigger = static_cast<BYTE>((leftTrigger * 255) / 32767);
		pState->Gamepad.bRightTrigger = static_cast<BYTE>((rightTrigger * 255) / 32767);

		// ==========================================================
		// Thumbsticks (SDL Y-axis is inverted compared to XInput)
		// ==========================================================

		auto SafeNegate = [](Sint16 value) -> SHORT
		{
			if (value == -32768)
				return 32767;
			return -value;
		};

		pState->Gamepad.sThumbLX = SDL_GetGamepadAxis(s_pGamepad, SDL_GAMEPAD_AXIS_LEFTX);
		pState->Gamepad.sThumbLY = SafeNegate(SDL_GetGamepadAxis(s_pGamepad, SDL_GAMEPAD_AXIS_LEFTY));
		pState->Gamepad.sThumbRX = SDL_GetGamepadAxis(s_pGamepad, SDL_GAMEPAD_AXIS_RIGHTX);
		pState->Gamepad.sThumbRY = SafeNegate(SDL_GetGamepadAxis(s_pGamepad, SDL_GAMEPAD_AXIS_RIGHTY));

		return ERROR_SUCCESS; // 0
	}
};