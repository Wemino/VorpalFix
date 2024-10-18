struct winmm
{
    FARPROC CloseDriver;
    FARPROC DefDriverProc;
    FARPROC DriverCallback;
    FARPROC DrvGetModuleHandle;
    FARPROC GetDriverModuleHandle;
    FARPROC NotifyCallbackData;
    FARPROC OpenDriver;
    FARPROC SendDriverMessage;
    FARPROC WOW32DriverCallback;
    FARPROC WOW32ResolveMultiMediaHandle;
    FARPROC WOWAppExit;
    FARPROC aux32Message;
    FARPROC auxGetDevCapsA;
    FARPROC auxGetDevCapsW;
    FARPROC auxGetNumDevs;
    FARPROC auxGetVolume;
    FARPROC auxOutMessage;
    FARPROC auxSetVolume;
    FARPROC joy32Message;
    FARPROC joyConfigChanged;
    FARPROC joyGetDevCapsA;
    FARPROC joyGetDevCapsW;
    FARPROC joyGetNumDevs;
    FARPROC joyGetPos;
    FARPROC joyGetPosEx;
    FARPROC joyGetThreshold;
    FARPROC joyReleaseCapture;
    FARPROC joySetCapture;
    FARPROC joySetThreshold;
    FARPROC mci32Message;
    FARPROC mciDriverNotify;
    FARPROC mciDriverYield;
    FARPROC mciExecute;
    FARPROC mciFreeCommandResource;
    FARPROC mciGetCreatorTask;
    FARPROC mciGetDeviceIDA;
    FARPROC mciGetDeviceIDFromElementIDA;
    FARPROC mciGetDeviceIDFromElementIDW;
    FARPROC mciGetDeviceIDW;
    FARPROC mciGetDriverData;
    FARPROC mciGetErrorStringA;
    FARPROC mciGetErrorStringW;
    FARPROC mciGetYieldProc;
    FARPROC mciLoadCommandResource;
    FARPROC mciSendCommandA;
    FARPROC mciSendCommandW;
    FARPROC mciSendStringA;
    FARPROC mciSendStringW;
    FARPROC mciSetDriverData;
    FARPROC mciSetYieldProc;
    FARPROC mid32Message;
    FARPROC midiConnect;
    FARPROC midiDisconnect;
    FARPROC midiInAddBuffer;
    FARPROC midiInClose;
    FARPROC midiInGetDevCapsA;
    FARPROC midiInGetDevCapsW;
    FARPROC midiInGetErrorTextA;
    FARPROC midiInGetErrorTextW;
    FARPROC midiInGetID;
    FARPROC midiInGetNumDevs;
    FARPROC midiInMessage;
    FARPROC midiInOpen;
    FARPROC midiInPrepareHeader;
    FARPROC midiInReset;
    FARPROC midiInStart;
    FARPROC midiInStop;
    FARPROC midiInUnprepareHeader;
    FARPROC midiOutCacheDrumPatches;
    FARPROC midiOutCachePatches;
    FARPROC midiOutClose;
    FARPROC midiOutGetDevCapsA;
    FARPROC midiOutGetDevCapsW;
    FARPROC midiOutGetErrorTextA;
    FARPROC midiOutGetErrorTextW;
    FARPROC midiOutGetID;
    FARPROC midiOutGetNumDevs;
    FARPROC midiOutGetVolume;
    FARPROC midiOutLongMsg;
    FARPROC midiOutMessage;
    FARPROC midiOutOpen;
    FARPROC midiOutPrepareHeader;
    FARPROC midiOutReset;
    FARPROC midiOutSetVolume;
    FARPROC midiOutShortMsg;
    FARPROC midiOutUnprepareHeader;
    FARPROC midiStreamClose;
    FARPROC midiStreamOpen;
    FARPROC midiStreamOut;
    FARPROC midiStreamPause;
    FARPROC midiStreamPosition;
    FARPROC midiStreamProperty;
    FARPROC midiStreamRestart;
    FARPROC midiStreamStop;
    FARPROC mixerClose;
    FARPROC mixerGetControlDetailsA;
    FARPROC mixerGetControlDetailsW;
    FARPROC mixerGetDevCapsA;
    FARPROC mixerGetDevCapsW;
    FARPROC mixerGetID;
    FARPROC mixerGetLineControlsA;
    FARPROC mixerGetLineControlsW;
    FARPROC mixerGetLineInfoA;
    FARPROC mixerGetLineInfoW;
    FARPROC mixerGetNumDevs;
    FARPROC mixerMessage;
    FARPROC mixerOpen;
    FARPROC mixerSetControlDetails;
    FARPROC mmDrvInstall;
    FARPROC mmGetCurrentTask;
    FARPROC mmioAdvance;
    FARPROC mmioAscend;
    FARPROC mmioClose;
    FARPROC mmioCreateChunk;
    FARPROC mmioDescend;
    FARPROC mmioFlush;
    FARPROC mmioGetInfo;
    FARPROC mmioInstallIOProcA;
    FARPROC mmioInstallIOProcW;
    FARPROC mmioOpenA;
    FARPROC mmioOpenW;
    FARPROC mmioRead;
    FARPROC mmioRenameA;
    FARPROC mmioRenameW;
    FARPROC mmioSeek;
    FARPROC mmioSendMessage;
    FARPROC mmioSetBuffer;
    FARPROC mmioSetInfo;
    FARPROC mmioStringToFOURCCA;
    FARPROC mmioStringToFOURCCW;
    FARPROC mmioWrite;
    FARPROC mmsystemGetVersion;
    FARPROC mmTaskBlock;
    FARPROC mmTaskCreate;
    FARPROC mmTaskSignal;
    FARPROC mmTaskYield;
    FARPROC mod32Message;
    FARPROC mxd32Message;
    FARPROC sndPlaySoundA;
    FARPROC sndPlaySoundW;
    FARPROC tid32Message;
    FARPROC timeBeginPeriod;
    FARPROC timeEndPeriod;
    FARPROC timeGetDevCaps;
    FARPROC timeGetSystemTime;
    FARPROC timeGetTime;
    FARPROC timeKillEvent;
    FARPROC timeSetEvent;
    FARPROC waveInAddBuffer;
    FARPROC waveInClose;
    FARPROC waveInGetDevCapsA;
    FARPROC waveInGetDevCapsW;
    FARPROC waveInGetErrorTextA;
    FARPROC waveInGetErrorTextW;
    FARPROC waveInGetID;
    FARPROC waveInGetNumDevs;
    FARPROC waveInGetPosition;
    FARPROC waveInMessage;
    FARPROC waveInOpen;
    FARPROC waveInPrepareHeader;
    FARPROC waveInReset;
    FARPROC waveInStart;
    FARPROC waveInStop;
    FARPROC waveInUnprepareHeader;
    FARPROC waveOutBreakLoop;
    FARPROC waveOutClose;
    FARPROC waveOutGetDevCapsA;
    FARPROC waveOutGetDevCapsW;
    FARPROC waveOutGetErrorTextA;
    FARPROC waveOutGetErrorTextW;
    FARPROC waveOutGetID;
    FARPROC waveOutGetNumDevs;
    FARPROC waveOutGetPitch;
    FARPROC waveOutGetPlaybackRate;
    FARPROC waveOutGetPosition;
    FARPROC waveOutGetVolume;
    FARPROC waveOutMessage;
    FARPROC waveOutOpen;
    FARPROC waveOutPause;
    FARPROC waveOutPrepareHeader;
    FARPROC waveOutReset;
    FARPROC waveOutRestart;
    FARPROC waveOutSetPitch;
    FARPROC waveOutSetPlaybackRate;
    FARPROC waveOutSetVolume;
    FARPROC waveOutUnprepareHeader;
    FARPROC waveOutWrite;
    FARPROC wid32Message;
    FARPROC wod32Message;

    void ProxySetup(HINSTANCE hL)
    {
        CloseDriver = GetProcAddress(hL, "CloseDriver");
        DefDriverProc = GetProcAddress(hL, "DefDriverProc");
        DriverCallback = GetProcAddress(hL, "DriverCallback");
        DrvGetModuleHandle = GetProcAddress(hL, "DrvGetModuleHandle");
        GetDriverModuleHandle = GetProcAddress(hL, "GetDriverModuleHandle");
        NotifyCallbackData = GetProcAddress(hL, "NotifyCallbackData");
        OpenDriver = GetProcAddress(hL, "OpenDriver");
        SendDriverMessage = GetProcAddress(hL, "SendDriverMessage");
        WOW32DriverCallback = GetProcAddress(hL, "WOW32DriverCallback");
        WOW32ResolveMultiMediaHandle = GetProcAddress(hL, "WOW32ResolveMultiMediaHandle");
        WOWAppExit = GetProcAddress(hL, "WOWAppExit");
        aux32Message = GetProcAddress(hL, "aux32Message");
        auxGetDevCapsA = GetProcAddress(hL, "auxGetDevCapsA");
        auxGetDevCapsW = GetProcAddress(hL, "auxGetDevCapsW");
        auxGetNumDevs = GetProcAddress(hL, "auxGetNumDevs");
        auxGetVolume = GetProcAddress(hL, "auxGetVolume");
        auxOutMessage = GetProcAddress(hL, "auxOutMessage");
        auxSetVolume = GetProcAddress(hL, "auxSetVolume");
        joy32Message = GetProcAddress(hL, "joy32Message");
        joyConfigChanged = GetProcAddress(hL, "joyConfigChanged");
        joyGetDevCapsA = GetProcAddress(hL, "joyGetDevCapsA");
        joyGetDevCapsW = GetProcAddress(hL, "joyGetDevCapsW");
        joyGetNumDevs = GetProcAddress(hL, "joyGetNumDevs");
        joyGetPos = GetProcAddress(hL, "joyGetPos");
        joyGetPosEx = GetProcAddress(hL, "joyGetPosEx");
        joyGetThreshold = GetProcAddress(hL, "joyGetThreshold");
        joyReleaseCapture = GetProcAddress(hL, "joyReleaseCapture");
        joySetCapture = GetProcAddress(hL, "joySetCapture");
        joySetThreshold = GetProcAddress(hL, "joySetThreshold");
        mci32Message = GetProcAddress(hL, "mci32Message");
        mciDriverNotify = GetProcAddress(hL, "mciDriverNotify");
        mciDriverYield = GetProcAddress(hL, "mciDriverYield");
        mciExecute = GetProcAddress(hL, "mciExecute");
        mciFreeCommandResource = GetProcAddress(hL, "mciFreeCommandResource");
        mciGetCreatorTask = GetProcAddress(hL, "mciGetCreatorTask");
        mciGetDeviceIDA = GetProcAddress(hL, "mciGetDeviceIDA");
        mciGetDeviceIDFromElementIDA = GetProcAddress(hL, "mciGetDeviceIDFromElementIDA");
        mciGetDeviceIDFromElementIDW = GetProcAddress(hL, "mciGetDeviceIDFromElementIDW");
        mciGetDeviceIDW = GetProcAddress(hL, "mciGetDeviceIDW");
        mciGetDriverData = GetProcAddress(hL, "mciGetDriverData");
        mciGetErrorStringA = GetProcAddress(hL, "mciGetErrorStringA");
        mciGetErrorStringW = GetProcAddress(hL, "mciGetErrorStringW");
        mciGetYieldProc = GetProcAddress(hL, "mciGetYieldProc");
        mciLoadCommandResource = GetProcAddress(hL, "mciLoadCommandResource");
        mciSendCommandA = GetProcAddress(hL, "mciSendCommandA");
        mciSendCommandW = GetProcAddress(hL, "mciSendCommandW");
        mciSendStringA = GetProcAddress(hL, "mciSendStringA");
        mciSendStringW = GetProcAddress(hL, "mciSendStringW");
        mciSetDriverData = GetProcAddress(hL, "mciSetDriverData");
        mciSetYieldProc = GetProcAddress(hL, "mciSetYieldProc");
        mid32Message = GetProcAddress(hL, "mid32Message");
        midiConnect = GetProcAddress(hL, "midiConnect");
        midiDisconnect = GetProcAddress(hL, "midiDisconnect");
        midiInAddBuffer = GetProcAddress(hL, "midiInAddBuffer");
        midiInClose = GetProcAddress(hL, "midiInClose");
        midiInGetDevCapsA = GetProcAddress(hL, "midiInGetDevCapsA");
        midiInGetDevCapsW = GetProcAddress(hL, "midiInGetDevCapsW");
        midiInGetErrorTextA = GetProcAddress(hL, "midiInGetErrorTextA");
        midiInGetErrorTextW = GetProcAddress(hL, "midiInGetErrorTextW");
        midiInGetID = GetProcAddress(hL, "midiInGetID");
        midiInGetNumDevs = GetProcAddress(hL, "midiInGetNumDevs");
        midiInMessage = GetProcAddress(hL, "midiInMessage");
        midiInOpen = GetProcAddress(hL, "midiInOpen");
        midiInPrepareHeader = GetProcAddress(hL, "midiInPrepareHeader");
        midiInReset = GetProcAddress(hL, "midiInReset");
        midiInStart = GetProcAddress(hL, "midiInStart");
        midiInStop = GetProcAddress(hL, "midiInStop");
        midiInUnprepareHeader = GetProcAddress(hL, "midiInUnprepareHeader");
        midiOutCacheDrumPatches = GetProcAddress(hL, "midiOutCacheDrumPatches");
        midiOutCachePatches = GetProcAddress(hL, "midiOutCachePatches");
        midiOutClose = GetProcAddress(hL, "midiOutClose");
        midiOutGetDevCapsA = GetProcAddress(hL, "midiOutGetDevCapsA");
        midiOutGetDevCapsW = GetProcAddress(hL, "midiOutGetDevCapsW");
        midiOutGetErrorTextA = GetProcAddress(hL, "midiOutGetErrorTextA");
        midiOutGetErrorTextW = GetProcAddress(hL, "midiOutGetErrorTextW");
        midiOutGetID = GetProcAddress(hL, "midiOutGetID");
        midiOutGetNumDevs = GetProcAddress(hL, "midiOutGetNumDevs");
        midiOutGetVolume = GetProcAddress(hL, "midiOutGetVolume");
        midiOutLongMsg = GetProcAddress(hL, "midiOutLongMsg");
        midiOutMessage = GetProcAddress(hL, "midiOutMessage");
        midiOutOpen = GetProcAddress(hL, "midiOutOpen");
        midiOutPrepareHeader = GetProcAddress(hL, "midiOutPrepareHeader");
        midiOutReset = GetProcAddress(hL, "midiOutReset");
        midiOutSetVolume = GetProcAddress(hL, "midiOutSetVolume");
        midiOutShortMsg = GetProcAddress(hL, "midiOutShortMsg");
        midiOutUnprepareHeader = GetProcAddress(hL, "midiOutUnprepareHeader");
        midiStreamClose = GetProcAddress(hL, "midiStreamClose");
        midiStreamOpen = GetProcAddress(hL, "midiStreamOpen");
        midiStreamOut = GetProcAddress(hL, "midiStreamOut");
        midiStreamPause = GetProcAddress(hL, "midiStreamPause");
        midiStreamPosition = GetProcAddress(hL, "midiStreamPosition");
        midiStreamProperty = GetProcAddress(hL, "midiStreamProperty");
        midiStreamRestart = GetProcAddress(hL, "midiStreamRestart");
        midiStreamStop = GetProcAddress(hL, "midiStreamStop");
        mixerClose = GetProcAddress(hL, "mixerClose");
        mixerGetControlDetailsA = GetProcAddress(hL, "mixerGetControlDetailsA");
        mixerGetControlDetailsW = GetProcAddress(hL, "mixerGetControlDetailsW");
        mixerGetDevCapsA = GetProcAddress(hL, "mixerGetDevCapsA");
        mixerGetDevCapsW = GetProcAddress(hL, "mixerGetDevCapsW");
        mixerGetID = GetProcAddress(hL, "mixerGetID");
        mixerGetLineControlsA = GetProcAddress(hL, "mixerGetLineControlsA");
        mixerGetLineControlsW = GetProcAddress(hL, "mixerGetLineControlsW");
        mixerGetLineInfoA = GetProcAddress(hL, "mixerGetLineInfoA");
        mixerGetLineInfoW = GetProcAddress(hL, "mixerGetLineInfoW");
        mixerGetNumDevs = GetProcAddress(hL, "mixerGetNumDevs");
        mixerMessage = GetProcAddress(hL, "mixerMessage");
        mixerOpen = GetProcAddress(hL, "mixerOpen");
        mixerSetControlDetails = GetProcAddress(hL, "mixerSetControlDetails");
        mmDrvInstall = GetProcAddress(hL, "mmDrvInstall");
        mmGetCurrentTask = GetProcAddress(hL, "mmGetCurrentTask");
        mmioAdvance = GetProcAddress(hL, "mmioAdvance");
        mmioAscend = GetProcAddress(hL, "mmioAscend");
        mmioClose = GetProcAddress(hL, "mmioClose");
        mmioCreateChunk = GetProcAddress(hL, "mmioCreateChunk");
        mmioDescend = GetProcAddress(hL, "mmioDescend");
        mmioFlush = GetProcAddress(hL, "mmioFlush");
        mmioGetInfo = GetProcAddress(hL, "mmioGetInfo");
        mmioInstallIOProcA = GetProcAddress(hL, "mmioInstallIOProcA");
        mmioInstallIOProcW = GetProcAddress(hL, "mmioInstallIOProcW");
        mmioOpenA = GetProcAddress(hL, "mmioOpenA");
        mmioOpenW = GetProcAddress(hL, "mmioOpenW");
        mmioRead = GetProcAddress(hL, "mmioRead");
        mmioRenameA = GetProcAddress(hL, "mmioRenameA");
        mmioRenameW = GetProcAddress(hL, "mmioRenameW");
        mmioSeek = GetProcAddress(hL, "mmioSeek");
        mmioSendMessage = GetProcAddress(hL, "mmioSendMessage");
        mmioSetBuffer = GetProcAddress(hL, "mmioSetBuffer");
        mmioSetInfo = GetProcAddress(hL, "mmioSetInfo");
        mmioStringToFOURCCA = GetProcAddress(hL, "mmioStringToFOURCCA");
        mmioStringToFOURCCW = GetProcAddress(hL, "mmioStringToFOURCCW");
        mmioWrite = GetProcAddress(hL, "mmioWrite");
        mmsystemGetVersion = GetProcAddress(hL, "mmsystemGetVersion");
        mmTaskBlock = GetProcAddress(hL, "mmTaskBlock");
        mmTaskCreate = GetProcAddress(hL, "mmTaskCreate");
        mmTaskSignal = GetProcAddress(hL, "mmTaskSignal");
        mmTaskYield = GetProcAddress(hL, "mmTaskYield");
        mod32Message = GetProcAddress(hL, "mod32Message");
        mxd32Message = GetProcAddress(hL, "mxd32Message");
        sndPlaySoundA = GetProcAddress(hL, "sndPlaySoundA");
        sndPlaySoundW = GetProcAddress(hL, "sndPlaySoundW");
        tid32Message = GetProcAddress(hL, "tid32Message");
        timeBeginPeriod = GetProcAddress(hL, "timeBeginPeriod");
        timeEndPeriod = GetProcAddress(hL, "timeEndPeriod");
        timeGetDevCaps = GetProcAddress(hL, "timeGetDevCaps");
        timeGetSystemTime = GetProcAddress(hL, "timeGetSystemTime");
        timeGetTime = GetProcAddress(hL, "timeGetTime");
        timeKillEvent = GetProcAddress(hL, "timeKillEvent");
        timeSetEvent = GetProcAddress(hL, "timeSetEvent");
        waveInAddBuffer = GetProcAddress(hL, "waveInAddBuffer");
        waveInClose = GetProcAddress(hL, "waveInClose");
        waveInGetDevCapsA = GetProcAddress(hL, "waveInGetDevCapsA");
        waveInGetDevCapsW = GetProcAddress(hL, "waveInGetDevCapsW");
        waveInGetErrorTextA = GetProcAddress(hL, "waveInGetErrorTextA");
        waveInGetErrorTextW = GetProcAddress(hL, "waveInGetErrorTextW");
        waveInGetID = GetProcAddress(hL, "waveInGetID");
        waveInGetNumDevs = GetProcAddress(hL, "waveInGetNumDevs");
        waveInGetPosition = GetProcAddress(hL, "waveInGetPosition");
        waveInMessage = GetProcAddress(hL, "waveInMessage");
        waveInOpen = GetProcAddress(hL, "waveInOpen");
        waveInPrepareHeader = GetProcAddress(hL, "waveInPrepareHeader");
        waveInReset = GetProcAddress(hL, "waveInReset");
        waveInStart = GetProcAddress(hL, "waveInStart");
        waveInStop = GetProcAddress(hL, "waveInStop");
        waveInUnprepareHeader = GetProcAddress(hL, "waveInUnprepareHeader");
        waveOutBreakLoop = GetProcAddress(hL, "waveOutBreakLoop");
        waveOutClose = GetProcAddress(hL, "waveOutClose");
        waveOutGetDevCapsA = GetProcAddress(hL, "waveOutGetDevCapsA");
        waveOutGetDevCapsW = GetProcAddress(hL, "waveOutGetDevCapsW");
        waveOutGetErrorTextA = GetProcAddress(hL, "waveOutGetErrorTextA");
        waveOutGetErrorTextW = GetProcAddress(hL, "waveOutGetErrorTextW");
        waveOutGetID = GetProcAddress(hL, "waveOutGetID");
        waveOutGetNumDevs = GetProcAddress(hL, "waveOutGetNumDevs");
        waveOutGetPitch = GetProcAddress(hL, "waveOutGetPitch");
        waveOutGetPlaybackRate = GetProcAddress(hL, "waveOutGetPlaybackRate");
        waveOutGetPosition = GetProcAddress(hL, "waveOutGetPosition");
        waveOutGetVolume = GetProcAddress(hL, "waveOutGetVolume");
        waveOutMessage = GetProcAddress(hL, "waveOutMessage");
        waveOutOpen = GetProcAddress(hL, "waveOutOpen");
        waveOutPause = GetProcAddress(hL, "waveOutPause");
        waveOutPrepareHeader = GetProcAddress(hL, "waveOutPrepareHeader");
        waveOutReset = GetProcAddress(hL, "waveOutReset");
        waveOutRestart = GetProcAddress(hL, "waveOutRestart");
        waveOutSetPitch = GetProcAddress(hL, "waveOutSetPitch");
        waveOutSetPlaybackRate = GetProcAddress(hL, "waveOutSetPlaybackRate");
        waveOutSetVolume = GetProcAddress(hL, "waveOutSetVolume");
        waveOutUnprepareHeader = GetProcAddress(hL, "waveOutUnprepareHeader");
        waveOutWrite = GetProcAddress(hL, "waveOutWrite");
        wid32Message = GetProcAddress(hL, "wid32Message");
        wod32Message = GetProcAddress(hL, "wod32Message");
    }
} winmm;

__declspec(naked) void Hook_CloseDriver() { _asm { jmp[winmm.CloseDriver] } }
__declspec(naked) void Hook_DefDriverProc() { _asm { jmp[winmm.DefDriverProc] } }
__declspec(naked) void Hook_DriverCallback() { _asm { jmp[winmm.DriverCallback] } }
__declspec(naked) void Hook_DrvGetModuleHandle() { _asm { jmp[winmm.DrvGetModuleHandle] } }
__declspec(naked) void Hook_GetDriverModuleHandle() { _asm { jmp[winmm.GetDriverModuleHandle] } }
__declspec(naked) void Hook_NotifyCallbackData() { _asm { jmp[winmm.NotifyCallbackData] } }
__declspec(naked) void Hook_OpenDriver() { _asm { jmp[winmm.OpenDriver] } }
__declspec(naked) void Hook_SendDriverMessage() { _asm { jmp[winmm.SendDriverMessage] } }
__declspec(naked) void Hook_WOW32DriverCallback() { _asm { jmp[winmm.WOW32DriverCallback] } }
__declspec(naked) void Hook_WOW32ResolveMultiMediaHandle() { _asm { jmp[winmm.WOW32ResolveMultiMediaHandle] } }
__declspec(naked) void Hook_WOWAppExit() { _asm { jmp[winmm.WOWAppExit] } }
__declspec(naked) void Hook_aux32Message() { _asm { jmp[winmm.aux32Message] } }
__declspec(naked) void Hook_auxGetDevCapsA() { _asm { jmp[winmm.auxGetDevCapsA] } }
__declspec(naked) void Hook_auxGetDevCapsW() { _asm { jmp[winmm.auxGetDevCapsW] } }
__declspec(naked) void Hook_auxGetNumDevs() { _asm { jmp[winmm.auxGetNumDevs] } }
__declspec(naked) void Hook_auxGetVolume() { _asm { jmp[winmm.auxGetVolume] } }
__declspec(naked) void Hook_auxOutMessage() { _asm { jmp[winmm.auxOutMessage] } }
__declspec(naked) void Hook_auxSetVolume() { _asm { jmp[winmm.auxSetVolume] } }
__declspec(naked) void Hook_joy32Message() { _asm { jmp[winmm.joy32Message] } }
__declspec(naked) void Hook_joyConfigChanged() { _asm { jmp[winmm.joyConfigChanged] } }
__declspec(naked) void Hook_joyGetDevCapsA() { _asm { jmp[winmm.joyGetDevCapsA] } }
__declspec(naked) void Hook_joyGetDevCapsW() { _asm { jmp[winmm.joyGetDevCapsW] } }
__declspec(naked) void Hook_joyGetNumDevs() { _asm { jmp[winmm.joyGetNumDevs] } }
__declspec(naked) void Hook_joyGetPos() { _asm { jmp[winmm.joyGetPos] } }
__declspec(naked) void Hook_joyGetPosEx() { _asm { jmp[winmm.joyGetPosEx] } }
__declspec(naked) void Hook_joyGetThreshold() { _asm { jmp[winmm.joyGetThreshold] } }
__declspec(naked) void Hook_joyReleaseCapture() { _asm { jmp[winmm.joyReleaseCapture] } }
__declspec(naked) void Hook_joySetCapture() { _asm { jmp[winmm.joySetCapture] } }
__declspec(naked) void Hook_joySetThreshold() { _asm { jmp[winmm.joySetThreshold] } }
__declspec(naked) void Hook_mci32Message() { _asm { jmp[winmm.mci32Message] } }
__declspec(naked) void Hook_mciDriverNotify() { _asm { jmp[winmm.mciDriverNotify] } }
__declspec(naked) void Hook_mciDriverYield() { _asm { jmp[winmm.mciDriverYield] } }
__declspec(naked) void Hook_mciExecute() { _asm { jmp[winmm.mciExecute] } }
__declspec(naked) void Hook_mciFreeCommandResource() { _asm { jmp[winmm.mciFreeCommandResource] } }
__declspec(naked) void Hook_mciGetCreatorTask() { _asm { jmp[winmm.mciGetCreatorTask] } }
__declspec(naked) void Hook_mciGetDeviceIDA() { _asm { jmp[winmm.mciGetDeviceIDA] } }
__declspec(naked) void Hook_mciGetDeviceIDFromElementIDA() { _asm { jmp[winmm.mciGetDeviceIDFromElementIDA] } }
__declspec(naked) void Hook_mciGetDeviceIDFromElementIDW() { _asm { jmp[winmm.mciGetDeviceIDFromElementIDW] } }
__declspec(naked) void Hook_mciGetDeviceIDW() { _asm { jmp[winmm.mciGetDeviceIDW] } }
__declspec(naked) void Hook_mciGetDriverData() { _asm { jmp[winmm.mciGetDriverData] } }
__declspec(naked) void Hook_mciGetErrorStringA() { _asm { jmp[winmm.mciGetErrorStringA] } }
__declspec(naked) void Hook_mciGetErrorStringW() { _asm { jmp[winmm.mciGetErrorStringW] } }
__declspec(naked) void Hook_mciGetYieldProc() { _asm { jmp[winmm.mciGetYieldProc] } }
__declspec(naked) void Hook_mciLoadCommandResource() { _asm { jmp[winmm.mciLoadCommandResource] } }
__declspec(naked) void Hook_mciSendCommandA() { _asm { jmp[winmm.mciSendCommandA] } }
__declspec(naked) void Hook_mciSendCommandW() { _asm { jmp[winmm.mciSendCommandW] } }
__declspec(naked) void Hook_mciSendStringA() { _asm { jmp[winmm.mciSendStringA] } }
__declspec(naked) void Hook_mciSendStringW() { _asm { jmp[winmm.mciSendStringW] } }
__declspec(naked) void Hook_mciSetDriverData() { _asm { jmp[winmm.mciSetDriverData] } }
__declspec(naked) void Hook_mciSetYieldProc() { _asm { jmp[winmm.mciSetYieldProc] } }
__declspec(naked) void Hook_mid32Message() { _asm { jmp[winmm.mid32Message] } }
__declspec(naked) void Hook_midiConnect() { _asm { jmp[winmm.midiConnect] } }
__declspec(naked) void Hook_midiDisconnect() { _asm { jmp[winmm.midiDisconnect] } }
__declspec(naked) void Hook_midiInAddBuffer() { _asm { jmp[winmm.midiInAddBuffer] } }
__declspec(naked) void Hook_midiInClose() { _asm { jmp[winmm.midiInClose] } }
__declspec(naked) void Hook_midiInGetDevCapsA() { _asm { jmp[winmm.midiInGetDevCapsA] } }
__declspec(naked) void Hook_midiInGetDevCapsW() { _asm { jmp[winmm.midiInGetDevCapsW] } }
__declspec(naked) void Hook_midiInGetErrorTextA() { _asm { jmp[winmm.midiInGetErrorTextA] } }
__declspec(naked) void Hook_midiInGetErrorTextW() { _asm { jmp[winmm.midiInGetErrorTextW] } }
__declspec(naked) void Hook_midiInGetID() { _asm { jmp[winmm.midiInGetID] } }
__declspec(naked) void Hook_midiInGetNumDevs() { _asm { jmp[winmm.midiInGetNumDevs] } }
__declspec(naked) void Hook_midiInMessage() { _asm { jmp[winmm.midiInMessage] } }
__declspec(naked) void Hook_midiInOpen() { _asm { jmp[winmm.midiInOpen] } }
__declspec(naked) void Hook_midiInPrepareHeader() { _asm { jmp[winmm.midiInPrepareHeader] } }
__declspec(naked) void Hook_midiInReset() { _asm { jmp[winmm.midiInReset] } }
__declspec(naked) void Hook_midiInStart() { _asm { jmp[winmm.midiInStart] } }
__declspec(naked) void Hook_midiInStop() { _asm { jmp[winmm.midiInStop] } }
__declspec(naked) void Hook_midiInUnprepareHeader() { _asm { jmp[winmm.midiInUnprepareHeader] } }
__declspec(naked) void Hook_midiOutCacheDrumPatches() { _asm { jmp[winmm.midiOutCacheDrumPatches] } }
__declspec(naked) void Hook_midiOutCachePatches() { _asm { jmp[winmm.midiOutCachePatches] } }
__declspec(naked) void Hook_midiOutClose() { _asm { jmp[winmm.midiOutClose] } }
__declspec(naked) void Hook_midiOutGetDevCapsA() { _asm { jmp[winmm.midiOutGetDevCapsA] } }
__declspec(naked) void Hook_midiOutGetDevCapsW() { _asm { jmp[winmm.midiOutGetDevCapsW] } }
__declspec(naked) void Hook_midiOutGetErrorTextA() { _asm { jmp[winmm.midiOutGetErrorTextA] } }
__declspec(naked) void Hook_midiOutGetErrorTextW() { _asm { jmp[winmm.midiOutGetErrorTextW] } }
__declspec(naked) void Hook_midiOutGetID() { _asm { jmp[winmm.midiOutGetID] } }
__declspec(naked) void Hook_midiOutGetNumDevs() { _asm { jmp[winmm.midiOutGetNumDevs] } }
__declspec(naked) void Hook_midiOutGetVolume() { _asm { jmp[winmm.midiOutGetVolume] } }
__declspec(naked) void Hook_midiOutLongMsg() { _asm { jmp[winmm.midiOutLongMsg] } }
__declspec(naked) void Hook_midiOutMessage() { _asm { jmp[winmm.midiOutMessage] } }
__declspec(naked) void Hook_midiOutOpen() { _asm { jmp[winmm.midiOutOpen] } }
__declspec(naked) void Hook_midiOutPrepareHeader() { _asm { jmp[winmm.midiOutPrepareHeader] } }
__declspec(naked) void Hook_midiOutReset() { _asm { jmp[winmm.midiOutReset] } }
__declspec(naked) void Hook_midiOutSetVolume() { _asm { jmp[winmm.midiOutSetVolume] } }
__declspec(naked) void Hook_midiOutShortMsg() { _asm { jmp[winmm.midiOutShortMsg] } }
__declspec(naked) void Hook_midiOutUnprepareHeader() { _asm { jmp[winmm.midiOutUnprepareHeader] } }
__declspec(naked) void Hook_midiStreamClose() { _asm { jmp[winmm.midiStreamClose] } }
__declspec(naked) void Hook_midiStreamOpen() { _asm { jmp[winmm.midiStreamOpen] } }
__declspec(naked) void Hook_midiStreamOut() { _asm { jmp[winmm.midiStreamOut] } }
__declspec(naked) void Hook_midiStreamPause() { _asm { jmp[winmm.midiStreamPause] } }
__declspec(naked) void Hook_midiStreamPosition() { _asm { jmp[winmm.midiStreamPosition] } }
__declspec(naked) void Hook_midiStreamProperty() { _asm { jmp[winmm.midiStreamProperty] } }
__declspec(naked) void Hook_midiStreamRestart() { _asm { jmp[winmm.midiStreamRestart] } }
__declspec(naked) void Hook_midiStreamStop() { _asm { jmp[winmm.midiStreamStop] } }
__declspec(naked) void Hook_mixerClose() { _asm { jmp[winmm.mixerClose] } }
__declspec(naked) void Hook_mixerGetControlDetailsA() { _asm { jmp[winmm.mixerGetControlDetailsA] } }
__declspec(naked) void Hook_mixerGetControlDetailsW() { _asm { jmp[winmm.mixerGetControlDetailsW] } }
__declspec(naked) void Hook_mixerGetDevCapsA() { _asm { jmp[winmm.mixerGetDevCapsA] } }
__declspec(naked) void Hook_mixerGetDevCapsW() { _asm { jmp[winmm.mixerGetDevCapsW] } }
__declspec(naked) void Hook_mixerGetID() { _asm { jmp[winmm.mixerGetID] } }
__declspec(naked) void Hook_mixerGetLineControlsA() { _asm { jmp[winmm.mixerGetLineControlsA] } }
__declspec(naked) void Hook_mixerGetLineControlsW() { _asm { jmp[winmm.mixerGetLineControlsW] } }
__declspec(naked) void Hook_mixerGetLineInfoA() { _asm { jmp[winmm.mixerGetLineInfoA] } }
__declspec(naked) void Hook_mixerGetLineInfoW() { _asm { jmp[winmm.mixerGetLineInfoW] } }
__declspec(naked) void Hook_mixerGetNumDevs() { _asm { jmp[winmm.mixerGetNumDevs] } }
__declspec(naked) void Hook_mixerMessage() { _asm { jmp[winmm.mixerMessage] } }
__declspec(naked) void Hook_mixerOpen() { _asm { jmp[winmm.mixerOpen] } }
__declspec(naked) void Hook_mixerSetControlDetails() { _asm { jmp[winmm.mixerSetControlDetails] } }
__declspec(naked) void Hook_mmDrvInstall() { _asm { jmp[winmm.mmDrvInstall] } }
__declspec(naked) void Hook_mmGetCurrentTask() { _asm { jmp[winmm.mmGetCurrentTask] } }
__declspec(naked) void Hook_mmioAdvance() { _asm { jmp[winmm.mmioAdvance] } }
__declspec(naked) void Hook_mmioAscend() { _asm { jmp[winmm.mmioAscend] } }
__declspec(naked) void Hook_mmioClose() { _asm { jmp[winmm.mmioClose] } }
__declspec(naked) void Hook_mmioCreateChunk() { _asm { jmp[winmm.mmioCreateChunk] } }
__declspec(naked) void Hook_mmioDescend() { _asm { jmp[winmm.mmioDescend] } }
__declspec(naked) void Hook_mmioFlush() { _asm { jmp[winmm.mmioFlush] } }
__declspec(naked) void Hook_mmioGetInfo() { _asm { jmp[winmm.mmioGetInfo] } }
__declspec(naked) void Hook_mmioInstallIOProcA() { _asm { jmp[winmm.mmioInstallIOProcA] } }
__declspec(naked) void Hook_mmioInstallIOProcW() { _asm { jmp[winmm.mmioInstallIOProcW] } }
__declspec(naked) void Hook_mmioOpenA() { _asm { jmp[winmm.mmioOpenA] } }
__declspec(naked) void Hook_mmioOpenW() { _asm { jmp[winmm.mmioOpenW] } }
__declspec(naked) void Hook_mmioRead() { _asm { jmp[winmm.mmioRead] } }
__declspec(naked) void Hook_mmioRenameA() { _asm { jmp[winmm.mmioRenameA] } }
__declspec(naked) void Hook_mmioRenameW() { _asm { jmp[winmm.mmioRenameW] } }
__declspec(naked) void Hook_mmioSeek() { _asm { jmp[winmm.mmioSeek] } }
__declspec(naked) void Hook_mmioSendMessage() { _asm { jmp[winmm.mmioSendMessage] } }
__declspec(naked) void Hook_mmioSetBuffer() { _asm { jmp[winmm.mmioSetBuffer] } }
__declspec(naked) void Hook_mmioSetInfo() { _asm { jmp[winmm.mmioSetInfo] } }
__declspec(naked) void Hook_mmioStringToFOURCCA() { _asm { jmp[winmm.mmioStringToFOURCCA] } }
__declspec(naked) void Hook_mmioStringToFOURCCW() { _asm { jmp[winmm.mmioStringToFOURCCW] } }
__declspec(naked) void Hook_mmioWrite() { _asm { jmp[winmm.mmioWrite] } }
__declspec(naked) void Hook_mmsystemGetVersion() { _asm { jmp[winmm.mmsystemGetVersion] } }
__declspec(naked) void Hook_mmTaskBlock() { _asm { jmp[winmm.mmTaskBlock] } }
__declspec(naked) void Hook_mmTaskCreate() { _asm { jmp[winmm.mmTaskCreate] } }
__declspec(naked) void Hook_mmTaskSignal() { _asm { jmp[winmm.mmTaskSignal] } }
__declspec(naked) void Hook_mmTaskYield() { _asm { jmp[winmm.mmTaskYield] } }
__declspec(naked) void Hook_mod32Message() { _asm { jmp[winmm.mod32Message] } }
__declspec(naked) void Hook_mxd32Message() { _asm { jmp[winmm.mxd32Message] } }
__declspec(naked) void Hook_sndPlaySoundA() { _asm { jmp[winmm.sndPlaySoundA] } }
__declspec(naked) void Hook_sndPlaySoundW() { _asm { jmp[winmm.sndPlaySoundW] } }
__declspec(naked) void Hook_tid32Message() { _asm { jmp[winmm.tid32Message] } }
__declspec(naked) void Hook_timeBeginPeriod() { _asm { jmp[winmm.timeBeginPeriod] } }
__declspec(naked) void Hook_timeEndPeriod() { _asm { jmp[winmm.timeEndPeriod] } }
__declspec(naked) void Hook_timeGetDevCaps() { _asm { jmp[winmm.timeGetDevCaps] } }
__declspec(naked) void Hook_timeGetSystemTime() { _asm { jmp[winmm.timeGetSystemTime] } }
__declspec(naked) void Hook_timeGetTime() { _asm { jmp[winmm.timeGetTime] } }
__declspec(naked) void Hook_timeKillEvent() { _asm { jmp[winmm.timeKillEvent] } }
__declspec(naked) void Hook_timeSetEvent() { _asm { jmp[winmm.timeSetEvent] } }
__declspec(naked) void Hook_waveInAddBuffer() { _asm { jmp[winmm.waveInAddBuffer] } }
__declspec(naked) void Hook_waveInClose() { _asm { jmp[winmm.waveInClose] } }
__declspec(naked) void Hook_waveInGetDevCapsA() { _asm { jmp[winmm.waveInGetDevCapsA] } }
__declspec(naked) void Hook_waveInGetDevCapsW() { _asm { jmp[winmm.waveInGetDevCapsW] } }
__declspec(naked) void Hook_waveInGetErrorTextA() { _asm { jmp[winmm.waveInGetErrorTextA] } }
__declspec(naked) void Hook_waveInGetErrorTextW() { _asm { jmp[winmm.waveInGetErrorTextW] } }
__declspec(naked) void Hook_waveInGetID() { _asm { jmp[winmm.waveInGetID] } }
__declspec(naked) void Hook_waveInGetNumDevs() { _asm { jmp[winmm.waveInGetNumDevs] } }
__declspec(naked) void Hook_waveInGetPosition() { _asm { jmp[winmm.waveInGetPosition] } }
__declspec(naked) void Hook_waveInMessage() { _asm { jmp[winmm.waveInMessage] } }
__declspec(naked) void Hook_waveInOpen() { _asm { jmp[winmm.waveInOpen] } }
__declspec(naked) void Hook_waveInPrepareHeader() { _asm { jmp[winmm.waveInPrepareHeader] } }
__declspec(naked) void Hook_waveInReset() { _asm { jmp[winmm.waveInReset] } }
__declspec(naked) void Hook_waveInStart() { _asm { jmp[winmm.waveInStart] } }
__declspec(naked) void Hook_waveInStop() { _asm { jmp[winmm.waveInStop] } }
__declspec(naked) void Hook_waveInUnprepareHeader() { _asm { jmp[winmm.waveInUnprepareHeader] } }
__declspec(naked) void Hook_waveOutBreakLoop() { _asm { jmp[winmm.waveOutBreakLoop] } }
__declspec(naked) void Hook_waveOutClose() { _asm { jmp[winmm.waveOutClose] } }
__declspec(naked) void Hook_waveOutGetDevCapsA() { _asm { jmp[winmm.waveOutGetDevCapsA] } }
__declspec(naked) void Hook_waveOutGetDevCapsW() { _asm { jmp[winmm.waveOutGetDevCapsW] } }
__declspec(naked) void Hook_waveOutGetErrorTextA() { _asm { jmp[winmm.waveOutGetErrorTextA] } }
__declspec(naked) void Hook_waveOutGetErrorTextW() { _asm { jmp[winmm.waveOutGetErrorTextW] } }
__declspec(naked) void Hook_waveOutGetID() { _asm { jmp[winmm.waveOutGetID] } }
__declspec(naked) void Hook_waveOutGetNumDevs() { _asm { jmp[winmm.waveOutGetNumDevs] } }
__declspec(naked) void Hook_waveOutGetPitch() { _asm { jmp[winmm.waveOutGetPitch] } }
__declspec(naked) void Hook_waveOutGetPlaybackRate() { _asm { jmp[winmm.waveOutGetPlaybackRate] } }
__declspec(naked) void Hook_waveOutGetPosition() { _asm { jmp[winmm.waveOutGetPosition] } }
__declspec(naked) void Hook_waveOutGetVolume() { _asm { jmp[winmm.waveOutGetVolume] } }
__declspec(naked) void Hook_waveOutMessage() { _asm { jmp[winmm.waveOutMessage] } }
__declspec(naked) void Hook_waveOutOpen() { _asm { jmp[winmm.waveOutOpen] } }
__declspec(naked) void Hook_waveOutPause() { _asm { jmp[winmm.waveOutPause] } }
__declspec(naked) void Hook_waveOutPrepareHeader() { _asm { jmp[winmm.waveOutPrepareHeader] } }
__declspec(naked) void Hook_waveOutReset() { _asm { jmp[winmm.waveOutReset] } }
__declspec(naked) void Hook_waveOutRestart() { _asm { jmp[winmm.waveOutRestart] } }
__declspec(naked) void Hook_waveOutSetPitch() { _asm { jmp[winmm.waveOutSetPitch] } }
__declspec(naked) void Hook_waveOutSetPlaybackRate() { _asm { jmp[winmm.waveOutSetPlaybackRate] } }
__declspec(naked) void Hook_waveOutSetVolume() { _asm { jmp[winmm.waveOutSetVolume] } }
__declspec(naked) void Hook_waveOutUnprepareHeader() { _asm { jmp[winmm.waveOutUnprepareHeader] } }
__declspec(naked) void Hook_waveOutWrite() { _asm { jmp[winmm.waveOutWrite] } }
__declspec(naked) void Hook_wid32Message() { _asm { jmp[winmm.wid32Message] } }
__declspec(naked) void Hook_wod32Message() { _asm { jmp[winmm.wod32Message] } }