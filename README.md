# VorpalFix

<p align="center">
  <img src="assets/VorpalFix_Logo.png">
</p>

A patch that addresses a variety of technical issues and adds quality-of-life improvements to the 2011 remaster of American McGee's Alice.

## How to Install
> [!NOTE]
> This patch will only work on the 2011 remaster of American McGee's Alice and **will not work on the 2000 version** of the game.
>
> **Download**: [VorpalFix.zip](https://github.com/Wemino/VorpalFix/releases/latest/download/VorpalFix.zip)
>
> Extract the contents of the zip file into the game’s folder, next to the `alice.exe` file:
> 
> <img width="180" src="assets/vf_install.png">
> 
> This patch allows the game to launch independently (by executing `alice.exe`), so it is not necessary to install it alongside Alice: Madness Returns.

### Steam Deck/Linux Specific Instructions (Windows users can skip this)
> [!WARNING]
> To launch the game on Steam Deck or Linux, open the game’s properties in Steam and include `WINEDLLOVERRIDES="winmm.dll=n,b" %command%` in the launch options.

## Configuration
- Modify the VorpalFix.ini file to adjust the settings for the fix.

- Some configurations can be edited through the in-game VorpalFix menu: 
![VorpalFix Menu](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/menu.png)

If you prefer not to have the in-game VorpalFix menu, you can remove it by deleting the file `pak7_VorpalFix_menu.pk3`

## Features
### Fixes
- Fix missing sound effects. (e.g., Cheshire Cat summon, Vorpal Blade hit sounds and much more)
- Fix "Hard Disk Full" when the game is installed on a disk with more than 2TB of free space.
- Fix crashing when saving at resolutions where the width is not a multiple of 4.
- Fix loading issues with certain remastered character textures.
- Fix some particle effects that are not displaying properly at certain distances.
- Fix Alice's blinking animation playing too quickly at high frame rates in the settings menu.
- Fix HUD stretching in widescreen resolutions. [Before & After](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/hud.gif)
- Fix FMV stretching in widescreen resolutions. [Before & After](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/fmv.gif)
- Fix GUI stretching in widescreen resolutions. [Before & After](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/gui.gif)
- Fix DPI Scaling on high-DPI displays.
- Fix the fullscreen setting not saving.
- Fix multiple menu issues.
- Fix multiple oversights in the script files.
- Workaround for a Proton specific issue related to the menus when playing on Linux.

### General
- Allow the game to launch without Alice: Madness Returns.
- Additional controller commands:
  - Pressing the Left Stick while pressing any D-pad button will save the current weapon to that D-pad key.
  - Pressing the Right Stick + 'A' will execute a quick load.
  - Pressing the Right Stick + 'B' will execute a quick save.
- Prevent Alice: Madness Returns from launching on exit. (when installed with Alice: Madness Returns)
- Provide the option to switch the game's supported language. (English, German, French, or Spanish)
- Restore the "American McGee's Alice" title screen from the Xbox 360/PlayStation 3 version.
- Provide the option to display the EA and Rogue Entertainment intro videos that were disabled in the remaster.
- Provide the option to disable the remastered models without modifying game files.
- Allow the use of the developer console from the original game, which was disabled in this remaster.
- Provide the option to set a custom path for saving the game's data.
- Provide the option to disable the Winsock initialization process, which is a remnant from the Quake 3 engine.
 
### Display
- Provide the option to use the HUD position similar to the Xbox 360/PlayStation 3 console version.
- Enable support for Xbox controller icons in the menus when a controller is detected during startup.
- Provide an option to display PlayStation 3 controller icons in place of Xbox controller icons.
- Provide the option to hide the popup console at launch.
- Provide the option to launch the game in borderless fullscreen mode.
- Provide the option to enable VSync.
- Custom resolution settings.

### Graphics
- Automatic adjustment of the Field of View (FOV) based on aspect ratio.
- Add support for Anisotropic Texture Filtering. [Off](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/anisotropic_off.png) VS [On (x16)](https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/anisotropic_on.png)
- Provide the option to enable trilinear texture filtering for better texture quality.
- Provide the option to always prioritize the highest quality models, regardless of distance.
- Allow to set a custom frame rate limit, with the option to match the monitor’s refresh rate.

## Credits
- [MinHook](https://github.com/TsudaKageyu/minhook) for hooking.
- [mINI](https://github.com/metayeti/mINI) for INI file handling.
- [Leanny](https://github.com/Leanny) help with some algorithms.
- [CRASHARKI](https://github.com/CRASHARKI) help with fixing and improving the menu.

## Licensing Exception
This project is licensed under the GNU General Public License (GPL) version 3.0. **However, Electronic Arts (EA)** and **any third parties explicitly authorized by EA** (including but not limited to GOG, Steam, or other distribution platforms) are hereby granted an irrevocable, perpetual, and worldwide exemption from all GPL 3.0 requirements. This exemption allows EA and authorized third parties to use, modify, distribute, sublicense, or bundle this codebase in any form (including proprietary/closed-source derivative works) for the purpose of re-releasing, preserving, maintaining, or commercially exploiting *American McGee’s Alice*, without any obligation to disclose source code, copyleft restrictions, or other GPL 3.0 terms.

This exception applies solely to EA and its authorized third parties. All other users remain subject to the terms of the GPL 3.0.
