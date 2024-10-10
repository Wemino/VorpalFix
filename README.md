# VorpalFix
A plugin that addresses a variety of technical issues and adds quality-of-life improvements to the 2011 remaster of American McGee's Alice.

## Features
### Fixes
- Fix the random sound selection function. (e.g., Cheshire Cat summon, Vorpal Blade hit sounds and much more)
- Fix "Hard Disk Full" when the game is installed on a disk with more than 2TB of free space.
- Fix crashing when saving at certain resolutions.
- Fix Alice's blinking animation playing too quickly at high frame rates in the settings menu.
- Fix HUD stretching in widescreen resolutions. [Before & After](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/hud.gif)
- Fix FMV stretching in widescreen resolutions. [Before & After](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/fmv.gif)
- Fix GUI stretching in widescreen resolutions. [Before & After](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/gui.gif)

### General
- Allow the game to launch without Alice: Madness Returns.
- Prevent Alice: Madness Returns from launching on exit. (when installed with Alice: Madness Returns)
- Allow to switch the game's supported language. (English, German, French, or Spanish)
- Provide the option to display the EA and Rogue Entertainment intro videos that were disabled in the re-release.
- Allow the option to disable the use of the remastered models.
- Allow the use of the developer console from the original game.
 
### Display
- Allow to hide the popup console at launch.
- Force the game to start in fullscreen mode.
- Provide the option to enable VSync.
- Custom resolution settings.

### Graphics
- Allow to enable trilinear texture filtering for better texture quality.
- Force highest quality models with enhanced LOD.
- Allow to set a custom frame rate limit, with the option to match the monitor’s refresh rate.
- Configure the Field of View (FOV), with an option for automatic adjustment based on aspect ratio.

## How to Install
> [!NOTE]
> This plugin will only work on the 2011 re-release/remaster of American McGee's Alice and **will not work on the 2000 version** of the game.
>
> **Download**: [VorpalFix.zip](https://github.com/Wemino/VorpalFix/releases/latest/download/VorpalFix.zip)
>
> Extract the contents of the zip file into the game’s folder, next to the `alice.exe` file.

### Steam Deck/Linux Specific Instructions (Windows users can skip this)
> [!WARNING]
> To launch the game on Steam Deck or Linux, open the game’s properties in Steam and include `WINEDLLOVERRIDES="winmm.dll=n,b" %command%` in the launch options.

## Plugin Configuration
- Modify the `VorpalFix.ini` file to adjust the plugin’s settings.

## Credits
- [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) for ASI loading.
- [MinHook](https://github.com/TsudaKageyu/minhook) for hooking.
- [injector](https://github.com/thelink2012/injector) for memory patching.
- [Leanny](https://github.com/Leanny) help with some algorithms.
