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

namespace GameHelper
{
	typedef int(__cdecl* sub_4158F0)(char*, char);
	sub_4158F0 CallCmd = (sub_4158F0)0x4158F0;

	typedef signed int(__cdecl* sub_46F850)();
	sub_46F850 FetchDisplayResolutions = (sub_46F850)0x46F850;

	typedef int(__cdecl* sub_4076F0)(char*);
	sub_4076F0 GetKeyId = (sub_4076F0)0x4076F0;

	typedef int(__cdecl* sub_44A300)(unsigned int, const char*);
	sub_44A300 GetTga = (sub_44A300)0x44A300;

	// XInputGetState
	typedef int(__cdecl* sub_463130)();
	sub_463130 IsControllerConnected = (sub_463130)0x463130;

	static void __cdecl ResizeCursor(bool hide, int width, int height)
	{
		int ImageNum = MemoryHelper::ReadMemory<int>(0x1BCCEEC, false);
		int ImageIndex = 0x1BCCEF0;

		const int originalWidth = 640;
		const int originalHeight = 480;

		for (int i = 0; i < ImageNum; i++)
		{
			const char* currentTexture = (const char*)ImageIndex;

			if (strcmp(currentTexture, "gfx/2d/mouse_arrow.tga") == 0)
			{
				// Calculate scale factors
				float widthScale = static_cast<float>(width) / originalWidth;
				float heightScale = static_cast<float>(height) / originalHeight;

				// Use the smallest scale factor to maintain the aspect ratio
				float scaleFactor = std::min(widthScale, heightScale);

				// Calculate the new scaled sizes
				int scaledMouseWidth = static_cast<int>(16 * scaleFactor);
				int scaledMouseHeight = static_cast<int>(32 * scaleFactor);

				if (hide)
				{
					scaledMouseWidth = 0;
					scaledMouseHeight = 0;
				}

				MemoryHelper::WriteMemory<int>(ImageIndex + 0x40, scaledMouseWidth, false);
				MemoryHelper::WriteMemory<int>(ImageIndex + 0x44, scaledMouseHeight, false);
				break;
			}

			ImageIndex += 0x80;
		}
	}

	static void __cdecl ResizePopupMessage(int width, int height)
	{
		int ImageNum = MemoryHelper::ReadMemory<int>(0x1BCCEEC, false);
		int ImageIndex = 0x1BCCEF0;

		for (int i = 0; i < ImageNum; i++)
		{
			const char* currentTexture = (const char*)ImageIndex;

			if (strcmp(currentTexture, "ui/control/press_any_key.tga") == 0)
			{
				int image_width = MemoryHelper::ReadMemory<int>(ImageIndex + 0x48, false);

				float current_width = static_cast<float>(width);
				float current_height = static_cast<float>(height);

				float target_width = current_height * 4.0f / 3.0f;
				float scale_factor = target_width / current_width;
				image_width *= scale_factor;

				MemoryHelper::WriteMemory<int>(ImageIndex + 0x48, image_width, false);
			}

			ImageIndex += 0x80;
		}
	}
};

namespace SystemHelper
{
	static DWORD GetCurrentDisplayFrequency()
	{
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);

		if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		{
			return devMode.dmDisplayFrequency;
		}
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
};