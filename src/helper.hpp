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

	constexpr uintptr_t MENU_MANAGER_ADDR = 0x11C2750;
	constexpr uintptr_t MAIN_MENU_ADDR = 0x7CCA60;

	enum class ShaderType : int8_t
	{
		Unknown = -1,
		Generic = 0,
		MouseArrow,
		TitleBg,
		QuicksaveCam,
	};

	static std::vector<ShaderType> shaderTypeCache;

	bool DisableCursorScaling = false;

	void(__cdecl* IN_CenterView)() = (void(__cdecl*)())0x406510;
	int(__cdecl* Key_StringToKeynum)(const char*) = (int(__cdecl*)(const char*))0x4076F0;
	int(__cdecl* Key_SetBinding)(int, const char*) = (int(__cdecl*)(int, const char*))0x407870;
	int(__cdecl* CL_Vid_Restart_f)() = (int(__cdecl*)())0x40B2F0;
	int(__cdecl* Cvar_Set2)(const char*, const char*, int) = (int(__cdecl*)(const char*, const char*, int))0x418D90;
	int(__cdecl* unzGetCurrentFileInfo)(void*, uint32_t*, char*, unsigned int, void*, unsigned int, char*, unsigned int) = (int(__cdecl*)(void*, uint32_t*, char*, unsigned int, void*, unsigned int, char*, unsigned int))0x43E560;
	void** (__thiscall* CreateString)(void**, const char*) = (void** (__thiscall*)(void**, const char*))0x441880;
	char* (__cdecl* CL_GetInvItemName)(int) = (char* (__cdecl*)(int))0x441D60;
	int(__cdecl* UI_GetStaticMap)(unsigned int, const char*) = (int(__cdecl*)(unsigned int, const char*))0x44A300;
	void* (__cdecl* Class_New)(size_t) = (void* (__cdecl*)(size_t))0x45CA00;
	void* (__cdecl* Event_New)(size_t) = (void* (__cdecl*)(size_t))0x45E030;
	int(__thiscall* Listener_ProcessEvent)(int, void*) = (int(__thiscall*)(int, void*))0x45F920;
	void* (__thiscall* Event_Init)(void*, const char*) = (void* (__thiscall*)(void*, const char*))0x4605D0;
	int(__cdecl* IsControllerConnected)() = (int(__cdecl*)())0x463130;
	void(__cdecl* Sys_QueEvent)(int, int, int, int, int, void*) = (void(__cdecl*)(int, int, int, int, int, void*))0x464D00;
	const char* (__thiscall* GetWidgetName)(DWORD*) = (const char* (__thiscall*)(DWORD*))0x4B1550;
	void(__thiscall* Menu_SaveCVars)(int, const char*) = (void(__thiscall*)(int, const char*))0x4B9770;
	void(__thiscall* Menu_ForceHide)(int) = (void(__thiscall*)(int))0x4B9B90;
	void(__cdecl*& ri_Printf)(int, const char*, ...) = *(void(__cdecl**)(int, const char*, ...))0x1C1D220;
	int(WINAPI*& qwglDescribePixelFormat)(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR) = *(int(WINAPI**)(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR))0x1BB8AC8;
	BOOL(WINAPI*& qwglSetPixelFormat)(HDC, int, const PIXELFORMATDESCRIPTOR*) = *(BOOL(WINAPI**)(HDC, int, const PIXELFORMATDESCRIPTOR*))0x1BB8D9C;
	HGLRC(WINAPI*& qwglCreateContext)(HDC) = *(HGLRC(WINAPI**)(HDC))0x1BB8EC8;
	BOOL(WINAPI*& qwglMakeCurrent)(HDC, HGLRC) = *(BOOL(WINAPI**)(HDC, HGLRC))0x1BB8F00;
	BOOL(WINAPI*& qwglDeleteContext)(HGLRC) = *(BOOL(WINAPI**)(HGLRC))0x1BB8D7C;

	static int GetCvarInteger(int cvar)
	{
		return *(int*)(cvar + 0x20);
	}

	static float GetCvarFloat(int cvar)
	{
		return *(float*)(cvar + 0x1C);
	}

	// Describe the pixel format into the game's descriptor, then set it on the window
	static bool GLW_SetPixelFormat(HDC hDC, int pixelFormat, PIXELFORMATDESCRIPTOR* pPFD)
	{
		qwglDescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), pPFD);
		return qwglSetPixelFormat(hDC, pixelFormat, pPFD) != FALSE;
	}

	static void AssignCmdKeyId(int keyId, const char* cmd)
	{
		char* KeyCommandName = *(char**)(0x14EA2A8 + 0xC * keyId);

		if (strcmp(KeyCommandName, cmd) != 0)
		{
			Key_SetBinding(keyId, cmd);
		}
	}

	static std::pair<float, float> GetCursorSize(int width, int height)
	{
		const float mouseWidth = 16.0f;
		const float mouseHeight = 32.0f;

		float scaleFactor = DisableCursorScaling ? 1.0f : std::min(width / 640.0f, height / 480.0f);

		return { floorf(mouseWidth * scaleFactor), floorf(mouseHeight * scaleFactor) };
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
		else if (strcmp(name, "title_bg") == 0)
			cached = ShaderType::TitleBg;
		else
			cached = ShaderType::Generic;

		return cached;
	}

	// Number of menus in the menu manager
	static int GetMenuCount()
	{
		return *(int*)(MENU_MANAGER_ADDR + 0x10);
	}

	// The menus can't be changed while one is transitioning
	static bool IsMenuLocked()
	{
		return *(uint8_t*)(MENU_MANAGER_ADDR + 0x20) != 0;
	}

	// Menu shown on top
	static int GetCurrentMenu()
	{
		int* overlayStack = *(int**)(MENU_MANAGER_ADDR + 0x18);
		int* menuStack = *(int**)(MENU_MANAGER_ADDR + 0x1C);

		if (overlayStack && overlayStack[0]) return overlayStack[0];
		return menuStack ? menuStack[0] : 0;
	}

	static const char* GetMenuName(int menu)
	{
		return **(char***)(menu + 0x20);
	}

	// The first widget of a menu is the container of the widgets loaded from its '.urc' file
	static int GetMenuContainer(int menu)
	{
		return (menu && *(int*)(menu + 0x18) > 0) ? **(int**)(menu + 0x14) : 0;
	}

	static int FindMenuByContainer(int container)
	{
		const int* menus = *(int**)(MENU_MANAGER_ADDR + 0xC);

		for (int i = 0; i < GetMenuCount(); i++)
		{
			if (GetMenuContainer(menus[i]) == container) return menus[i];
		}

		return 0;
	}

	// Find the widget of a menu linked to a cvar
	static DWORD* FindMenuWidget(int menu, const char* cvarName)
	{
		const int* widgets = *(int**)(menu + 0x14);

		for (int i = 0; i < *(int*)(menu + 0x18); i++)
		{
			const char* name = GetWidgetName((DWORD*)widgets[i]);
			if (name && strcmp(name, cvarName) == 0) return (DWORD*)widgets[i];
		}

		return nullptr;
	}

	// Number of items added to a list widget with 'additem'
	static int GetWidgetItemCount(DWORD* widget)
	{
		return *(int*)((uintptr_t)widget + 0x1D8);
	}

	// Group of widgets shown in the menu, the settings use one per page
	static const char* GetActiveMenuGroup()
	{
		char** group = *(char***)0x11C187C;
		return (group && *group) ? *group : "";
	}

	// Duration of the fade from black when a widget is shown, set by 'fadeoverlay' in the '.urc' files
	static float& GetFadeOverlayTime(int widget)
	{
		return *(float*)(widget + 0x1C4);
	}

	// Send an event like "showmenu" to a menu
	static void ProcessMenuEvent(int menu, const char* eventName)
	{
		void* event = Event_New(0x58);
		if (!event) return;

		Listener_ProcessEvent(menu, Event_Init(event, eventName));
	}

	// Fit a widget rect in the 4:3 area, 'rectOffset' is where its x, y, width and height are
	static void ScaleWidgetRect(int widget, int rectOffset, float scaleFactor, float widthDifference, int& x, int& width)
	{
		// Same scale as the game, unless the widget is not auto scaled
		float scaleX = *(BYTE*)(widget + 0x1C1) ? 1.0f : MemoryHelper::ReadMemory<int>(0x11C02CC) / 640.0f;

		x = (int)(*(float*)(widget + rectOffset) * scaleX * scaleFactor + widthDifference);
		width = (int)(*(float*)(widget + rectOffset + 0x8) * scaleX * scaleFactor);
	}

	// Check if a point of the light grid contains lighting data
	static bool IsLightGridPointLit(uintptr_t world, int x, int y, int z)
	{
		const int* gridBounds = (int*)(world + 0xE0);

		if (x < 0 || y < 0 || z < 0 || x >= gridBounds[0] || y >= gridBounds[1] || z >= gridBounds[2]) return false;

		const uint8_t* gridPoint = *(uint8_t**)(world + 0xEC) + 8 * (x + y * gridBounds[0] + z * gridBounds[0] * gridBounds[1]);
		return (gridPoint[0] + gridPoint[1] + gridPoint[2]) != 0;
	}
};

namespace MenuHelper
{
	// Controller menus, switched to the device used last
	enum class MenuStyle : int
	{
		PC,
		Xbox,
		PlayStation,
		Count
	};

	struct MenuVariant
	{
		const char* path; // Menu loaded by the game
		const char* pcFile; // Replace the game's file when set
		const char* controllerFile;
		bool hasPS3Icons;
		int containers[(int)MenuStyle::Count];
	};

	static MenuVariant menuVariants[] =
	{
		{ "ui/controls.urc", nullptr,      "/controls2.urc",     true  },
		{ "ui/credits.urc",  nullptr,      "/credits2.urc",      true  },
		{ "ui/loadsave.urc", nullptr,      "/loadsave2.urc",     true  },
		{ "ui/main.urc",     nullptr,      "/main2.urc",         true  },
		{ "ui/newgame.urc",  nullptr,      "/newgame2.urc",      true  },
		{ "ui/quit.urc",     nullptr,      "/quit2.urc",         true  },
		{ "ui/title.urc",    "/title.urc", "/title_console.urc", false },
	};

	static MenuStyle currentMenuStyle = MenuStyle::PC;
	static MenuStyle requestedMenuStyle = MenuStyle::PC;
	static int fadedMenuContainer = 0;
	static ULONGLONG fadedMenuTime = 0;
	static int mouseMoveDistance = 0;
	static ULONGLONG lastMouseMoveTime = 0;

	// Check which controller icons should be shown
	static MenuStyle GetControllerMenuStyle(int usePS3ControllerIcons)
	{
		bool usePS3Icons = (usePS3ControllerIcons == 2) || (usePS3ControllerIcons == 0 && ControllerHelper::GetGamepadStyle() == ControllerHelper::GamepadStyle::PlayStation);
		return usePS3Icons ? MenuStyle::PlayStation : MenuStyle::Xbox;
	}

	// Only load the controller menus that can be shown
	static bool IsMenuStyleUsed(MenuStyle style, int usePS3ControllerIcons, bool useSDLControllerInput)
	{
		switch (style)
		{
			case MenuStyle::Xbox: return usePS3ControllerIcons != 2;
			case MenuStyle::PlayStation: return usePS3ControllerIcons == 2 || (usePS3ControllerIcons == 0 && useSDLControllerInput);
			default: return true;
		}
	}

	static MenuVariant* FindMenuVariant(const char* ui_path, bool useConsoleTitleScreen)
	{
		for (auto& variant : menuVariants)
		{
			// The title screen is only replaced when using the console one
			if (variant.pcFile && !useConsoleTitleScreen) continue;

			if (strcmp(ui_path, variant.path) == 0) return &variant;
		}

		return nullptr;
	}

	static void GetMenuVariantPath(const MenuVariant& variant, MenuStyle style, const char* langPrefix, char* buffer, size_t size)
	{
		if (style == MenuStyle::PC)
		{
			if (variant.pcFile)
				snprintf(buffer, size, "%s%s", langPrefix, variant.pcFile);
			else
				snprintf(buffer, size, "%s", variant.path);
		}
		else if (style == MenuStyle::PlayStation && variant.hasPS3Icons)
		{
			snprintf(buffer, size, "%s_ps3%s", langPrefix, variant.controllerFile);
		}
		else
		{
			snprintf(buffer, size, "%s%s", langPrefix, variant.controllerFile);
		}
	}

	// Find the version of a menu used by a controller style, 0 if the menu has only one version
	static int GetMenuForStyle(int menu, MenuStyle style)
	{
		int container = GameHelper::GetMenuContainer(menu);
		if (!container) return 0;

		for (const auto& variant : menuVariants)
		{
			for (int variantContainer : variant.containers)
			{
				if (variantContainer != container) continue;

				int styleContainer = variant.containers[(int)style];
				if (!styleContainer || styleContainer == container) return menu;

				int styleMenu = GameHelper::FindMenuByContainer(styleContainer);
				return styleMenu ? styleMenu : menu;
			}
		}

		return 0;
	}

	// Add an item to the same list in the other versions of the menus
	static void AddItemToMenuVariants(DWORD* widget, const char* cvarName, void** value, void** label, void(__thiscall* addItem)(DWORD*, void**, void**))
	{
		for (const auto& variant : menuVariants)
		{
			for (int i = 0; i < (int)MenuStyle::Count; i++)
			{
				int container = variant.containers[i];

				// Shared by two styles
				if (!container || (i > 0 && container == variant.containers[i - 1])) continue;

				int menu = GameHelper::FindMenuByContainer(container);
				DWORD* variantWidget = menu ? GameHelper::FindMenuWidget(menu, cvarName) : nullptr;
				if (!variantWidget || variantWidget == widget) continue;

				// The strings are released by the function
				((int*)value)[1]++;
				((int*)label)[1]++;
				addItem(variantWidget, value, label);
			}
		}
	}

	// Remove the fade from black given to a menu when switching
	static void RemoveSwitchFade()
	{
		if (!fadedMenuContainer) return;

		GameHelper::GetFadeOverlayTime(fadedMenuContainer) = 0.0f;
		fadedMenuContainer = 0;
	}

	// Switch the menus to the version of the device used last, return true when switched
	static bool UpdateMenuStyle()
	{
		if (fadedMenuContainer && GetTickCount64() - fadedMenuTime > 500)
		{
			RemoveSwitchFade();
		}

		if (requestedMenuStyle == currentMenuStyle || GameHelper::GetMenuCount() == 0) return false;

		// Wait for the menu transitions
		if (GameHelper::IsMenuLocked()) return false;

		// Wait to be back on the main page of the settings, the other pages can have unsaved changes
		int currentMenu = GameHelper::GetCurrentMenu();
		if (currentMenu && strcmp(GameHelper::GetMenuName(currentMenu), "controls") == 0 && strcmp(GameHelper::GetActiveMenuGroup(), "group_main") != 0) return false;

		currentMenuStyle = requestedMenuStyle;

		// Replace the opened menus, the overlay ones and the top one are visible
		for (uintptr_t stackAddr : { GameHelper::MENU_MANAGER_ADDR + 0x18, GameHelper::MENU_MANAGER_ADDR + 0x1C })
		{
			int* top = *(int**)stackAddr;

			for (int* node = top; node; node = (int*)node[1])
			{
				int menu = GetMenuForStyle(node[0], currentMenuStyle);
				if (!menu || menu == node[0]) continue;

				if (stackAddr == GameHelper::MENU_MANAGER_ADDR + 0x18 || node == top)
				{
					GameHelper::Menu_ForceHide(node[0]);
					GameHelper::Menu_SaveCVars(menu, nullptr);

					// Fade from black like the other menus, the main menu doesn't have it
					int container = GameHelper::GetMenuContainer(menu);
					if (GameHelper::GetFadeOverlayTime(container) == 0.0f)
					{
						RemoveSwitchFade();
						GameHelper::GetFadeOverlayTime(container) = 0.5f;
						fadedMenuContainer = container;
						fadedMenuTime = GetTickCount64();
					}

					GameHelper::ProcessMenuEvent(menu, "showmenu");
				}

				node[0] = menu;
			}
		}

		// Used by the game to check if the main menu is opened
		int mainMenu = GetMenuForStyle(MemoryHelper::ReadMemory<int>(GameHelper::MAIN_MENU_ADDR), currentMenuStyle);
		if (mainMenu)
		{
			MemoryHelper::WriteMemory<int>(GameHelper::MAIN_MENU_ADDR, mainMenu, false);
		}

		return true;
	}
};

namespace SystemHelper
{
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

	static std::vector<std::string> GetLocPk3Files(const std::string& path) 
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

	static BOOL(WINAPI* qwglChoosePixelFormatARB)(HDC, const int*, const FLOAT*, UINT, int*, UINT*) = nullptr;

	static void BootstrapWGLExtensions()
	{
		if (qwglChoosePixelFormatARB)
			return;

		WNDCLASSA wc = {};
		wc.lpfnWndProc = DefWindowProcA;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = "MSAABootstrapWnd";
		RegisterClassA(&wc);

		HWND hwnd = CreateWindowA(wc.lpszClassName, "", 0, 0, 0, 1, 1, NULL, NULL, wc.hInstance, NULL);
		if (!hwnd) return;

		HDC hdc = GetDC(hwnd);
		if (!hdc) { DestroyWindow(hwnd); return; }

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pf = ChoosePixelFormat(hdc, &pfd);
		if (pf && SetPixelFormat(hdc, pf, &pfd))
		{
			HGLRC ctx = wglCreateContext(hdc);
			if (ctx)
			{
				wglMakeCurrent(hdc, ctx);
				qwglChoosePixelFormatARB = (BOOL(__stdcall*)(HDC, const int*, const FLOAT*, UINT, int*, UINT*))wglGetProcAddress("wglChoosePixelFormatARB");
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(ctx);
			}
		}

		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
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

#define GL_FRAMEBUFFER                    0x8D40
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_RENDERBUFFER                   0x8D41
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_MAX_SAMPLES                    0x8D57
#define GL_SAMPLE_BUFFERS                 0x80A8

namespace RenderHelper
{
	struct RenderTarget
	{
		GLuint fbo;
		GLuint color;
		GLuint depth;
		int width;
		int height;
	};

	static void(APIENTRY* qglGenFramebuffers)(GLsizei, GLuint*) = nullptr;
	static void(APIENTRY* qglDeleteFramebuffers)(GLsizei, const GLuint*) = nullptr;
	static void(APIENTRY* qglBindFramebuffer)(GLenum, GLuint) = nullptr;
	static void(APIENTRY* qglFramebufferRenderbuffer)(GLenum, GLenum, GLenum, GLuint) = nullptr;
	static GLenum(APIENTRY* qglCheckFramebufferStatus)(GLenum) = nullptr;
	static void(APIENTRY* qglGenRenderbuffers)(GLsizei, GLuint*) = nullptr;
	static void(APIENTRY* qglDeleteRenderbuffers)(GLsizei, const GLuint*) = nullptr;
	static void(APIENTRY* qglBindRenderbuffer)(GLenum, GLuint) = nullptr;
	static void(APIENTRY* qglRenderbufferStorageMultisample)(GLenum, GLsizei, GLenum, GLsizei, GLsizei) = nullptr;
	static void(APIENTRY* qglBlitFramebuffer)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum) = nullptr;

	static HGLRC ssaaContext = nullptr;
	static float ssaaScale = 1.0f;
	static int nativeWidth = 0;
	static int nativeHeight = 0;
	static RenderTarget sceneTarget = {}; // Supersampled framebuffer the game renders into
	static RenderTarget resolveTarget = {}; // Single-sample copy of the scene when MSAA is used
	static RenderTarget downsampleTargets[2] = {}; // Intermediate steps so that each blit never shrinks more than 2x
	static int downsampleTargetCount = 0;
	static RenderTarget readTarget = {}; // Native resolution copy used by glReadPixels

	static PROC GetFramebufferProc(const char* name)
	{
		PROC proc = wglGetProcAddress(name);

		if (!proc)
		{
			// Fall back to the EXT_framebuffer_object/blit/multisample entry points
			char extName[64];
			sprintf_s(extName, "%sEXT", name);
			proc = wglGetProcAddress(extName);
		}

		return proc;
	}

	static bool LoadFramebufferFunctions()
	{
		qglGenFramebuffers = (void(APIENTRY*)(GLsizei, GLuint*))GetFramebufferProc("glGenFramebuffers");
		qglDeleteFramebuffers = (void(APIENTRY*)(GLsizei, const GLuint*))GetFramebufferProc("glDeleteFramebuffers");
		qglBindFramebuffer = (void(APIENTRY*)(GLenum, GLuint))GetFramebufferProc("glBindFramebuffer");
		qglFramebufferRenderbuffer = (void(APIENTRY*)(GLenum, GLenum, GLenum, GLuint))GetFramebufferProc("glFramebufferRenderbuffer");
		qglCheckFramebufferStatus = (GLenum(APIENTRY*)(GLenum))GetFramebufferProc("glCheckFramebufferStatus");
		qglGenRenderbuffers = (void(APIENTRY*)(GLsizei, GLuint*))GetFramebufferProc("glGenRenderbuffers");
		qglDeleteRenderbuffers = (void(APIENTRY*)(GLsizei, const GLuint*))GetFramebufferProc("glDeleteRenderbuffers");
		qglBindRenderbuffer = (void(APIENTRY*)(GLenum, GLuint))GetFramebufferProc("glBindRenderbuffer");
		qglRenderbufferStorageMultisample = (void(APIENTRY*)(GLenum, GLsizei, GLenum, GLsizei, GLsizei))GetFramebufferProc("glRenderbufferStorageMultisample");
		qglBlitFramebuffer = (void(APIENTRY*)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum))GetFramebufferProc("glBlitFramebuffer");

		return qglGenFramebuffers && qglDeleteFramebuffers && qglBindFramebuffer && qglFramebufferRenderbuffer && qglCheckFramebufferStatus && qglGenRenderbuffers && qglDeleteRenderbuffers && qglBindRenderbuffer && qglRenderbufferStorageMultisample && qglBlitFramebuffer;
	}

	static void DeleteRenderTarget(RenderTarget& target)
	{
		if (target.fbo) qglDeleteFramebuffers(1, &target.fbo);
		if (target.color) qglDeleteRenderbuffers(1, &target.color);
		if (target.depth) qglDeleteRenderbuffers(1, &target.depth);
		target = {};
	}

	static bool CreateRenderTarget(RenderTarget& target, int width, int height, int samples, bool withDepth)
	{
		target.width = width;
		target.height = height;

		qglGenRenderbuffers(1, &target.color);
		qglBindRenderbuffer(GL_RENDERBUFFER, target.color);
		qglRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);

		if (withDepth)
		{
			// The game uses the stencil buffer for its shadows
			qglGenRenderbuffers(1, &target.depth);
			qglBindRenderbuffer(GL_RENDERBUFFER, target.depth);
			qglRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
		}

		qglGenFramebuffers(1, &target.fbo);
		qglBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
		qglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, target.color);

		if (withDepth)
		{
			qglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target.depth);
		}

		qglBindRenderbuffer(GL_RENDERBUFFER, 0);

		return qglCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	static void DeleteSSAATargets()
	{
		DeleteRenderTarget(sceneTarget);
		DeleteRenderTarget(resolveTarget);
		DeleteRenderTarget(readTarget);

		for (int i = 0; i < downsampleTargetCount; i++)
		{
			DeleteRenderTarget(downsampleTargets[i]);
		}

		downsampleTargetCount = 0;
	}

	// Create the supersampled framebuffer, must be called with the game's context current
	static bool InitSSAA(float scale, int width, int height, int msaaSamples)
	{
		HGLRC context = wglGetCurrentContext();

		if (ssaaContext == context)
		{
			DeleteSSAATargets();
		}
		else
		{
			// The previous context has been destroyed along with its objects
			sceneTarget = {};
			resolveTarget = {};
			readTarget = {};
			downsampleTargetCount = 0;
		}

		ssaaContext = nullptr;

		if (scale <= 1.0f || width <= 0 || height <= 0 || !LoadFramebufferFunctions())
		{
			return false;
		}

		// Keep the supersampled framebuffer within the limits of the GPU
		GLint maxRenderbufferSize = 0;
		GLint maxViewportDims[2] = {};
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportDims);

		int maxSize = std::min(maxRenderbufferSize, std::min(maxViewportDims[0], maxViewportDims[1]));
		scale = std::min(scale, static_cast<float>(maxSize) / static_cast<float>(std::max(width, height)));

		if (scale <= 1.0f)
		{
			return false;
		}

		GLint maxSamples = 0;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		msaaSamples = std::min(msaaSamples, static_cast<int>(maxSamples));

		// Use the same scale on both axes so that a fullscreen viewport covers the whole framebuffer
		int scaledWidth = static_cast<int>(width * scale);
		float effectiveScale = static_cast<float>(scaledWidth) / static_cast<float>(width);
		int scaledHeight = static_cast<int>(floorf(height * effectiveScale));

		bool success = CreateRenderTarget(sceneTarget, scaledWidth, scaledHeight, std::max(msaaSamples, 0), true);

		if (success && msaaSamples > 0)
		{
			success = CreateRenderTarget(resolveTarget, scaledWidth, scaledHeight, 0, false);
		}

		// Halve the size until the last blit to the screen is 2x or less
		int stepWidth = scaledWidth;
		int stepHeight = scaledHeight;

		while (success && downsampleTargetCount < 2 && (stepWidth > width * 2 || stepHeight > height * 2))
		{
			stepWidth = std::max(width, (stepWidth + 1) / 2);
			stepHeight = std::max(height, (stepHeight + 1) / 2);
			success = CreateRenderTarget(downsampleTargets[downsampleTargetCount++], stepWidth, stepHeight, 0, false);
		}

		if (success)
		{
			success = CreateRenderTarget(readTarget, width, height, 0, false);
		}

		if (!success)
		{
			DeleteSSAATargets();
			qglBindFramebuffer(GL_FRAMEBUFFER, 0);
			GameHelper::ri_Printf(0, "...SSAA framebuffer incomplete, disabling SSAA\n");
			return false;
		}

		// The game will now render into the supersampled framebuffer
		qglBindFramebuffer(GL_FRAMEBUFFER, sceneTarget.fbo);

		ssaaContext = context;
		ssaaScale = effectiveScale;
		nativeWidth = width;
		nativeHeight = height;

		if (msaaSamples > 0)
		{
			GameHelper::ri_Printf(0, "...SSAA %.2fx enabled, rendering at %dx%d (%dx MSAA)\n", ssaaScale, scaledWidth, scaledHeight, msaaSamples);
		}
		else
		{
			GameHelper::ri_Printf(0, "...SSAA %.2fx enabled, rendering at %dx%d\n", ssaaScale, scaledWidth, scaledHeight);
		}

		return true;
	}

	static bool isSSAASuspended = false;

	static bool IsSSAAActive()
	{
		return ssaaContext && sceneTarget.fbo && !isSSAASuspended && wglGetCurrentContext() == ssaaContext;
	}

	// Convert a rectangle from the game's resolution to the supersampled resolution
	static void ScaleRect(GLint& x, GLint& y, GLsizei& width, GLsizei& height)
	{
		GLint x0 = static_cast<GLint>(floorf(x * ssaaScale));
		GLint y0 = static_cast<GLint>(floorf(y * ssaaScale));
		GLint x1 = static_cast<GLint>(floorf((x + width) * ssaaScale));
		GLint y1 = static_cast<GLint>(floorf((y + height) * ssaaScale));

		x = x0;
		y = y0;
		width = x1 - x0;
		height = y1 - y0;
	}

	static void BlitRenderTarget(const RenderTarget& source, GLuint destinationFbo, int destinationWidth, int destinationHeight)
	{
		qglBindFramebuffer(GL_READ_FRAMEBUFFER, source.fbo);
		qglBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationFbo);

		GLenum filter = (source.width == destinationWidth && source.height == destinationHeight) ? GL_NEAREST : GL_LINEAR;
		qglBlitFramebuffer(0, 0, source.width, source.height, 0, 0, destinationWidth, destinationHeight, GL_COLOR_BUFFER_BIT, filter);
	}

	// Downsample the supersampled framebuffer into the destination framebuffer
	static void ResolveSSAA(GLuint destinationFbo, int destinationWidth, int destinationHeight)
	{
		// Blits are affected by the scissor test and the color mask
		GLboolean isScissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
		GLboolean colorMask[4];
		glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);

		glDisable(GL_SCISSOR_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		const RenderTarget* source = &sceneTarget;

		// Resolve the MSAA samples first, scaling isn't allowed while doing so
		if (resolveTarget.fbo)
		{
			BlitRenderTarget(*source, resolveTarget.fbo, resolveTarget.width, resolveTarget.height);
			source = &resolveTarget;
		}

		for (int i = 0; i < downsampleTargetCount; i++)
		{
			BlitRenderTarget(*source, downsampleTargets[i].fbo, downsampleTargets[i].width, downsampleTargets[i].height);
			source = &downsampleTargets[i];
		}

		BlitRenderTarget(*source, destinationFbo, destinationWidth, destinationHeight);

		if (isScissorEnabled) glEnable(GL_SCISSOR_TEST);
		glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
	}

	// Copy the frame to the window, before the buffers are swapped
	static void PresentSSAA()
	{
		ResolveSSAA(0, nativeWidth, nativeHeight);
	}

	// Make glReadPixels read a native resolution copy of the frame
	static void BindSSAAReadTarget()
	{
		ResolveSSAA(readTarget.fbo, nativeWidth, nativeHeight);
		qglBindFramebuffer(GL_READ_FRAMEBUFFER, readTarget.fbo);
	}

	static void CreateWindowResolveTarget(RenderTarget& target, int width, int height)
	{
		if (target.fbo)
		{
			DeleteRenderTarget(target);
		}

		GLint sampleBuffers = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);

		if (!sampleBuffers || !LoadFramebufferFunctions())
		{
			return;
		}

		if (!CreateRenderTarget(target, width, height, 0, false))
		{
			DeleteRenderTarget(target);
		}

		qglBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	static void CopyResolvedWindow(const RenderTarget& target)
	{
		qglBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		qglBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.fbo);
		qglBlitFramebuffer(0, 0, target.width, target.height, 0, 0, target.width, target.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		qglBindFramebuffer(GL_READ_FRAMEBUFFER, target.fbo);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, target.width, target.height);

		qglBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Let the game render into the supersampled framebuffer again
	static void RestoreSSAABinding()
	{
		qglBindFramebuffer(GL_FRAMEBUFFER, sceneTarget.fbo);
	}

	// Same filter as the game's 'ResampleTexture', without its limit of 1024 pixels per row
	static void ResampleTexture(const uint8_t* in, int inWidth, int inHeight, uint8_t* out, int outWidth, int outHeight)
	{
		std::vector<unsigned> p1(outWidth), p2(outWidth);
		unsigned fracStep = inWidth * 0x10000 / outWidth;

		unsigned frac = fracStep >> 2;
		for (int i = 0; i < outWidth; i++, frac += fracStep) p1[i] = 4 * (frac >> 16);

		frac = 3 * (fracStep >> 2);
		for (int i = 0; i < outWidth; i++, frac += fracStep) p2[i] = 4 * (frac >> 16);

		for (int i = 0; i < outHeight; i++, out += 4 * outWidth)
		{
			const uint8_t* inRow = in + 4 * inWidth * (int)((i + 0.25) * inHeight / outHeight);
			const uint8_t* inRow2 = in + 4 * inWidth * (int)((i + 0.75) * inHeight / outHeight);

			for (int j = 0; j < outWidth; j++)
			{
				for (int c = 0; c < 4; c++)
				{
					out[4 * j + c] = (inRow[p1[j] + c] + inRow[p2[j] + c] + inRow2[p1[j] + c] + inRow2[p2[j] + c]) >> 2;
				}
			}
		}
	}
};

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82

namespace GammaHelper
{
	// Same lookup as the display does with the gamma ramp, applied to the game's frame instead
	static const char* GammaVertexShader = 
	R"(#version 120
		void main()
		{
			gl_Position = gl_Vertex;
		}
	)";

	static const char* GammaFragmentShader = 
	R"(#version 120
		uniform sampler2D frame;
		uniform sampler2D ramp;
		uniform vec2 size;

		void main()
		{
			vec3 color = texture2D(frame, gl_FragCoord.xy / size).rgb;
			vec3 coord = (color * 255.0 + 0.5) / 256.0;
			gl_FragColor = vec4(texture2D(ramp, vec2(coord.r, 0.5)).r, texture2D(ramp, vec2(coord.g, 0.5)).g, texture2D(ramp, vec2(coord.b, 0.5)).b, 1.0);
		}
	)";

	typedef char GLchar;

	static GLuint(APIENTRY* qglCreateShader)(GLenum) = nullptr;
	static void(APIENTRY* qglShaderSource)(GLuint, GLsizei, const GLchar* const*, const GLint*) = nullptr;
	static void(APIENTRY* qglCompileShader)(GLuint) = nullptr;
	static void(APIENTRY* qglGetShaderiv)(GLuint, GLenum, GLint*) = nullptr;
	static void(APIENTRY* qglDeleteShader)(GLuint) = nullptr;
	static GLuint(APIENTRY* qglCreateProgram)() = nullptr;
	static void(APIENTRY* qglAttachShader)(GLuint, GLuint) = nullptr;
	static void(APIENTRY* qglLinkProgram)(GLuint) = nullptr;
	static void(APIENTRY* qglGetProgramiv)(GLuint, GLenum, GLint*) = nullptr;
	static void(APIENTRY* qglUseProgram)(GLuint) = nullptr;
	static GLint(APIENTRY* qglGetUniformLocation)(GLuint, const GLchar*) = nullptr;
	static void(APIENTRY* qglUniform1i)(GLint, GLint) = nullptr;
	static void(APIENTRY* qglUniform2f)(GLint, GLfloat, GLfloat) = nullptr;
	static void(APIENTRY* qglActiveTexture)(GLenum) = nullptr;

	static WORD gammaRamp[3][256] = {};
	static bool isRampIdentity = true;
	static bool isRampDirty = true;

	static HGLRC gammaContext = nullptr;
	static bool isUnavailable = false;
	static GLuint gammaProgram = 0;
	static GLint sizeLocation = -1;
	static GLuint frameTexture = 0;
	static GLuint rampTexture = 0;
	static RenderHelper::RenderTarget resolveTarget = {};
	static int frameWidth = 0;
	static int frameHeight = 0;

	// Fill an identity ramp, what the game sees as the system's current calibration
	static void GetIdentityRamp(WORD* ramp)
	{
		for (int i = 0; i < 256; i++)
		{
			ramp[i] = ramp[256 + i] = ramp[512 + i] = static_cast<WORD>(i * 257);
		}
	}

	// Store the ramp the game wants to apply to the display
	static void SetRamp(const WORD* ramp)
	{
		std::memcpy(gammaRamp, ramp, sizeof(gammaRamp));

		isRampIdentity = true;
		for (int i = 0; i < 256 * 3 && isRampIdentity; i++)
		{
			isRampIdentity = (ramp[i] >> 8) == (i & 0xFF);
		}

		isRampDirty = true;
	}

	static bool LoadShaderFunctions()
	{
		qglCreateShader = (GLuint(APIENTRY*)(GLenum))wglGetProcAddress("glCreateShader");
		qglShaderSource = (void(APIENTRY*)(GLuint, GLsizei, const GLchar* const*, const GLint*))wglGetProcAddress("glShaderSource");
		qglCompileShader = (void(APIENTRY*)(GLuint))wglGetProcAddress("glCompileShader");
		qglGetShaderiv = (void(APIENTRY*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetShaderiv");
		qglDeleteShader = (void(APIENTRY*)(GLuint))wglGetProcAddress("glDeleteShader");
		qglCreateProgram = (GLuint(APIENTRY*)())wglGetProcAddress("glCreateProgram");
		qglAttachShader = (void(APIENTRY*)(GLuint, GLuint))wglGetProcAddress("glAttachShader");
		qglLinkProgram = (void(APIENTRY*)(GLuint))wglGetProcAddress("glLinkProgram");
		qglGetProgramiv = (void(APIENTRY*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetProgramiv");
		qglUseProgram = (void(APIENTRY*)(GLuint))wglGetProcAddress("glUseProgram");
		qglGetUniformLocation = (GLint(APIENTRY*)(GLuint, const GLchar*))wglGetProcAddress("glGetUniformLocation");
		qglUniform1i = (void(APIENTRY*)(GLint, GLint))wglGetProcAddress("glUniform1i");
		qglUniform2f = (void(APIENTRY*)(GLint, GLfloat, GLfloat))wglGetProcAddress("glUniform2f");
		qglActiveTexture = (void(APIENTRY*)(GLenum))wglGetProcAddress("glActiveTexture");

		return qglCreateShader && qglShaderSource && qglCompileShader && qglGetShaderiv && qglDeleteShader && qglCreateProgram && qglAttachShader && qglLinkProgram && qglGetProgramiv && qglUseProgram && qglGetUniformLocation && qglUniform1i && qglUniform2f && qglActiveTexture;
	}

	static GLuint CompileShader(GLenum type, const char* source)
	{
		GLuint shader = qglCreateShader(type);
		qglShaderSource(shader, 1, &source, nullptr);
		qglCompileShader(shader);

		GLint status = 0;
		qglGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			qglDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	// Create the shader for the current context, the objects of a previous context are gone with it
	static bool InitContext()
	{
		HGLRC context = wglGetCurrentContext();

		if (!context) return false;
		if (context == gammaContext) return !isUnavailable;

		gammaContext = context;
		isUnavailable = true;
		gammaProgram = frameTexture = rampTexture = 0;
		resolveTarget = {};
		frameWidth = frameHeight = 0;
		isRampDirty = true;

		GLuint vertexShader = LoadShaderFunctions() ? CompileShader(GL_VERTEX_SHADER, GammaVertexShader) : 0;
		GLuint fragmentShader = vertexShader ? CompileShader(GL_FRAGMENT_SHADER, GammaFragmentShader) : 0;

		if (fragmentShader)
		{
			gammaProgram = qglCreateProgram();
			qglAttachShader(gammaProgram, vertexShader);
			qglAttachShader(gammaProgram, fragmentShader);
			qglLinkProgram(gammaProgram);

			GLint status = 0;
			qglGetProgramiv(gammaProgram, GL_LINK_STATUS, &status);
			if (!status) gammaProgram = 0;
		}

		if (vertexShader) qglDeleteShader(vertexShader);
		if (fragmentShader) qglDeleteShader(fragmentShader);

		if (!gammaProgram)
		{
			GameHelper::ri_Printf(0, "...gamma shader unavailable\n");
			return false;
		}

		sizeLocation = qglGetUniformLocation(gammaProgram, "size");
		qglUseProgram(gammaProgram);
		qglUniform1i(qglGetUniformLocation(gammaProgram, "frame"), 0);
		qglUniform1i(qglGetUniformLocation(gammaProgram, "ramp"), 1);
		qglUseProgram(0);

		glGenTextures(1, &frameTexture);
		glGenTextures(1, &rampTexture);

		isUnavailable = false;
		return true;
	}

	static void SetupTexture(GLuint texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	// Apply the gamma ramp to the frame in the window's back buffer, before it is presented
	static void ApplyGammaRamp(int width, int height)
	{
		if (isRampIdentity || width <= 0 || height <= 0 || !InitContext()) return;

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// The frame is read from the window, even when the game renders in the supersampled framebuffer
		if (RenderHelper::qglBindFramebuffer)
		{
			RenderHelper::qglBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		qglActiveTexture(GL_TEXTURE1);
		if (isRampDirty)
		{
			WORD rampTexels[256][3];
			for (int i = 0; i < 256; i++)
			{
				rampTexels[i][0] = gammaRamp[0][i];
				rampTexels[i][1] = gammaRamp[1][i];
				rampTexels[i][2] = gammaRamp[2][i];
			}

			SetupTexture(rampTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, 256, 1, 0, GL_RGB, GL_UNSIGNED_SHORT, rampTexels);
			isRampDirty = false;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, rampTexture);
		}

		qglActiveTexture(GL_TEXTURE0);
		if (width != frameWidth || height != frameHeight)
		{
			SetupTexture(frameTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			frameWidth = width;
			frameHeight = height;

			RenderHelper::CreateWindowResolveTarget(resolveTarget, width, height);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, frameTexture);
		}

		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_CULL_FACE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		if (resolveTarget.fbo)
		{
			RenderHelper::CopyResolvedWindow(resolveTarget);
		}
		else
		{
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
		}

		RenderHelper::isSSAASuspended = true;
		glViewport(0, 0, width, height);

		qglUseProgram(gammaProgram);
		qglUniform2f(sizeLocation, static_cast<float>(width), static_cast<float>(height));

		glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();

		qglUseProgram(0);
		glPopAttrib();
		RenderHelper::isSSAASuspended = false;
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

	static bool ApplyHookReplaceable(void* addr, LPVOID hookFunc, LPVOID* originalFunc)
	{
		if (!InitializeMinHook()) return false;

		MH_STATUS status = MH_CreateHook(addr, hookFunc, originalFunc);
		if (status == MH_ERROR_ALREADY_CREATED)
		{
			MH_RemoveHook(addr);
			status = MH_CreateHook(addr, hookFunc, originalFunc);
		}

		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to create hook at address: %p\nError: %s", addr, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return false;
		}

		status = MH_EnableHook(addr);
		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to enable hook at address: %p\nError: %s", addr, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return false;
		}

		return true;
	}

	static void ApplyHookAPI(LPCWSTR moduleName, LPCSTR apiName, LPVOID hookFunc, LPVOID* originalFunc)
	{
		if (!InitializeMinHook()) return;

		LPVOID target = nullptr;
		MH_STATUS status = MH_CreateHookApiEx(moduleName, apiName, hookFunc, originalFunc, &target);
		if (status != MH_OK)
		{
			char errorMsg[0x100];
			sprintf_s(errorMsg, "Failed to create hook for API: %s\nError: %s", apiName, MH_StatusToString(status));
			MessageBoxA(NULL, errorMsg, "Error", MB_ICONERROR | MB_OK);
			return;
		}

		status = MH_EnableHook(target);
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
};