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
		PressAnyKey,
		QuicksaveCam,
	};

	static std::vector<ShaderType> shaderTypeCache;

	bool DisableCursorScaling = false;

	void(__cdecl* IN_CenterView)() = (void(__cdecl*)())0x406510;
	int(__cdecl* Key_StringToKeynum)(const char*) = (int(__cdecl*)(const char*))0x4076F0;
	int(__cdecl* Key_SetBinding)(int, const char*) = (int(__cdecl*)(int, const char*))0x407870;
	int(__cdecl* CL_Vid_Restart_f)() = (int(__cdecl*)())0x40B2F0;
	int(__cdecl* Cvar_Set2)(const char*, const char*, int) = (int(__cdecl*)(const char*, const char*, int))0x418D90;
	void** (__thiscall* CreateString)(void**, const char*) = (void** (__thiscall*)(void**, const char*))0x441880;
	char* (__cdecl* CL_GetInvItemName)(int) = (char* (__cdecl*)(int))0x441D60;
	int(__cdecl* UI_GetStaticMap)(unsigned int, const char*) = (int(__cdecl*)(unsigned int, const char*))0x44A300;
	int(__cdecl* IsControllerConnected)() = (int(__cdecl*)())0x463130;
	void(__cdecl* Sys_QueEvent)(int, int, int, int, int, void*) = (void(__cdecl*)(int, int, int, int, int, void*))0x464D00;
	const char* (__thiscall* GetWidgetName)(DWORD*) = (const char* (__thiscall*)(DWORD*))0x4B1550;
	void(__cdecl*& ri_Printf)(int, const char*, ...) = *(void(__cdecl**)(int, const char*, ...))0x1C1D220;

	static void AssignCmdKeyId(int keyId, const char* cmd)
	{
		char* KeyCommandName = *(char**)(0x14EA2A8 + 0xC * keyId);

		if (strcmp(KeyCommandName, cmd) != 0)
		{
			Key_SetBinding(keyId, cmd);
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

	// Check if a point of the light grid contains lighting data
	static bool IsLightGridPointLit(uintptr_t world, int x, int y, int z)
	{
		const int* gridBounds = (int*)(world + 0xE0);

		if (x < 0 || y < 0 || z < 0 || x >= gridBounds[0] || y >= gridBounds[1] || z >= gridBounds[2]) return false;

		const uint8_t* gridPoint = *(uint8_t**)(world + 0xEC) + 8 * (x + y * gridBounds[0] + z * gridBounds[0] * gridBounds[1]);
		return (gridPoint[0] + gridPoint[1] + gridPoint[2]) != 0;
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

	extern BOOL(WINAPI* qwglChoosePixelFormatARB)(HDC, const int*, const FLOAT*, UINT, int*, UINT*);

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

	static bool IsSSAAActive()
	{
		return ssaaContext && sceneTarget.fbo && wglGetCurrentContext() == ssaaContext;
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

	// Let the game render into the supersampled framebuffer again
	static void RestoreSSAABinding()
	{
		qglBindFramebuffer(GL_FRAMEBUFFER, sceneTarget.fbo);
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