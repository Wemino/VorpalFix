# VorpalFix

<p align="center">
  <img src="assets/VorpalFix_Logo.png">
</p>

A patch that addresses various technical issues and adds quality-of-life improvements to the 2011 remaster of American McGee's Alice, without altering the original game's artistic vision or overwriting any original game files.

## How to Install
> [!NOTE]
> This patch will only work on the 2011 remaster of American McGee's Alice and **will not work on the 2000 version** of the game.
>
> **Download**: [VorpalFix.zip](https://github.com/Wemino/VorpalFix/releases/latest/download/VorpalFix.zip)
>
> Extract the contents of the zip file into the game’s folder, in the same directory as the `alice.exe` file.
> 
> <img width="180" src="assets/vf_install.png">
> 
> This patch allows the game to launch independently (by executing `alice.exe`), so it is not necessary to install it alongside Alice: Madness Returns.

### Steam Deck/Linux Specific Instructions (Windows users can skip this)
> [!WARNING]
> To launch the game on Steam Deck or Linux, open the game’s properties in Steam and include `WINEDLLOVERRIDES="winmm.dll=n,b" %command%` in the launch options.

## Features
- **Fix missing sound effects** - Fix the missing sound effects introduced in the 2011 remaster, affecting most weapons and the Cheshire Cat summon.

- **Fix "Hard Disk Full"** - Fixed the "Hard Disk Full" error message incorrectly appearing when the game is installed on a disk with more than 2TB of free space.

- **Fix multiple crashes** - Fixed a crash that occurred when saving at resolutions where the width is not a multiple of 4 (e.g., 1366x768), as well as another crash that could happen while fetching the resolution list.

- **Fix particle effects not displaying properly at certain distances** - Fixed an issue where re-materialization effects for Alice's throwable weapons were inconsistent, sometimes failing to appear.

- **Fix Alice's blinking animation speed in the settings menu** - Fixed an issue where Alice's blinking animation played too quickly at high frame rates.

- **Fix fullscreen setting not saving.**

- **Fix multiple menu issues** - Texture bleeding, inconsistent "Back" button animation, and minor problems.

- **Fix multiple oversights in the script files** - Typos in texture/sound paths.

- **Workaround for a Proton specific issue related to the menus when playing on Linux** - Set `FixProton = 1` in `VorpalFix.ini` (https://github.com/ValveSoftware/Proton/issues/8179)
  
- **Fix DPI Scaling on high-DPI displays.**

- **Fix remastered character textures not loading** - Fixed an issue where multiple typos in the archive containing the remastered textures prevented them from loading. Affects **Cheshire Cat**, **Card Guards** and **Mock Turtle**.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/pak5_Vanilla.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/pak5_VF.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla</td>
      <td align="center">VorpalFix with "FixPak5" enabled</td>
    </tr>
  </table>
</div>

- **Fix Menu stretching in widescreen resolutions** - Fixed the stretching of menus and screenshots taken for the save file thumbnail in widescreen resolutions.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/Menu_Vanilla.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/Menu_VF.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla Menu (16:9)</td>
      <td align="center">VorpalFix Menu (16:9)</td>
    </tr>
  </table>
</div>

- **Fix HUD stretching in widescreen resolutions:**

<div align="center">
  <table style="width: 130%; max-width: 1200px;">
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/HUD_Vanilla.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/HUD_VF.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla HUD (16:9)</td>
      <td align="center">VorpalFix HUD (16:9)</td>
    </tr>
  </table>
</div>

- **Fix FMV stretching in widescreen resolutions:**

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/FMV_Vanilla.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/FMV_VF.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla FMV (16:9)</td>
      <td align="center">VorpalFix FMV (16:9)</td>
    </tr>
  </table>
</div>

- **Allow the game to launch without Alice: Madness Returns** - The 2011 remaster is designed to launch with the `-RunningFromAlice2` option, this check has been disabled by default, allowing `alice.exe` to launch independently.

- **Prevent Alice: Madness Returns from launching on exit** - On exit, the game checks the installation path of Alice: Madness Returns to relaunch it. Set `PreventAlice2OnExit = 1` in `VorpalFix.ini`

- **Add a dedicated menu to edit some of the settings of `VorpalFix.ini`** - Delete `pak7_VorpalFix_menu.pk3` to remove.

<div align="center">
  <table>
    <tr>
      <td width="100%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/VF_Menu.jpg"></td>
    </tr>
    <tr>
      <td align="center">VorpalFix Settings Menu</td>
    </tr>
  </table>
</div>

- **Provide the option to disable the remastered models without modifying game files** - Added the option to disable the remastered models without modifying game files. Adjust in VorpalFix menu.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/pak5_On.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/pak5_Off.jpg"></td>
    </tr>
    <tr>
      <td align="center">2011 Textures (Remastered)</td>
      <td align="center">2000 Textures (Original)</td>
    </tr>
  </table>
</div>

- **Restore the "American McGee's Alice" title screen from the Xbox 360/PlayStation 3 version** - Restored the "American McGee's Alice" title screen, as seen in the Xbox 360/PlayStation 3 version.

- **Provide the option to switch the game's supported language** - Added the option to switch between English, German, French, or Spanish as the game's supported language. Adjust in VorpalFix menu.

- **Provide the option to display the EA and Rogue Entertainment intro videos** - Added the option to enable the previously disabled EA and Rogue Entertainment intro videos from the remaster. Set `UseOriginalIntroVideos = 1` in `VorpalFix.ini`.

- **Provide the option to set a custom path for saving the game's data** - Added the option to set a custom path for saving the game's data. Edit `CustomSavePath` in `VorpalFix.ini`.

- **Provide the option to disable the Winsock initialization process** - Added the option to disable the Winsock initialization process, a remnant from the Quake 3 engine.

- **Additional controller commands:**
  - Pressing the Left Stick while pressing any D-pad button will save the current weapon to that D-pad key.
  - Pressing the Right Stick + 'A' will execute a quick load.
  - Pressing the Right Stick + 'B' will execute a quick save.
 
- **Provide the option to use the HUD position similar to the Xbox 360/PlayStation 3 console version** - Added the option to position the HUD like in the Xbox 360/PlayStation 3 console version.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/HUD_PC.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/HUD_Console.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla HUD Position</td>
      <td align="center">VorpalFix Console Style HUD Position</td>
    </tr>
  </table>
</div>

- **Provide the option to disable the letterboxing during cutscenes** - Adjust in VorpalFix menu.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/Letterbox_On.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/Letterbox_Off.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla</td>
      <td align="center">VorpalFix (Letterbox Disabled)</td>
    </tr>
  </table>
</div>
 
- **Enable support for controller icons in the menus** - Xbox or PlayStation 3 controller icons are displayed in the menus when a controller is detected at startup. Adjust the controller style in the VorpalFix menu.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/Controller_Xbox.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/Controller_PS3.jpg"></td>
    </tr>
    <tr>
      <td align="center">Xbox 360 Controller Icons</td>
      <td align="center">PlayStation 3 Controller Icons</td>
    </tr>
  </table>
</div>

- **Provide the option to launch the game in borderless fullscreen mode** - Set `ForceBorderlessFullscreen = 1` in `VorpalFix.ini`.

- **VSync and Custom Frame Rate Options** - Added the option to enable VSync and set a custom frame rate limit, including an option to match the monitor's refresh rate. Both can be adjusted in the VorpalFix menu.  

- **FOV Fix** - Auto-scales 4:3 FOV for wider aspect ratios.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/FOV_Vanilla.jpg"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/FOV_VF.jpg"></td>
    </tr>
    <tr>
      <td align="center">Vanilla 16:9 FOV</td>
      <td align="center">VorpalFix Corrected 16:9 FOV</td>
    </tr>
  </table>
</div>

- **Add support for texture filtering** - Added support for Anisotropic and Trilinear texture filtering to improve texture quality.

<div align="center">
  <table>
    <tr>
      <td width="50%"><a href="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/AF_Off_Full.png"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/AF_Off.png"></a></td>
      <td width="50%"><a href="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/AF_On_Full.png"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/AF_On.png"></a></td>
    </tr>
    <tr>
      <td align="center"><a href="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/anisotropic_off.png">Vanilla</a></td>
      <td align="center"><a href="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/anisotropic_on.png">VorpalFix Anisotropic Filtering (16x)</a></td>
    </tr>
  </table>
</div>

- **No Model LOD Bias** - Forces the highest quality models to be rendered at all distances.

<div align="center">
  <table>
    <tr>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/LOD_Off.png"></td>
      <td width="50%"><img style="width:100%" src="https://raw.githubusercontent.com/Wemino/VorpalFix/refs/heads/main/assets/LOD_On.png"></td>
    </tr>
    <tr>
      <td align="center">Vanilla</td>
      <td align="center">VorpalFix (No Model LOD Bias)</td>
    </tr>
  </table>
</div>

## Credits
- [MinHook](https://github.com/TsudaKageyu/minhook) for hooking.
- [mINI](https://github.com/metayeti/mINI) for INI file handling.
- [Leanny](https://github.com/Leanny) help with some algorithms.
- [CRASHARKI](https://github.com/CRASHARKI) help with fixing and improving the menu.

## Licensing Exception
This project is licensed under the GNU General Public License (GPL) version 3.0. **However, Electronic Arts (EA)** and **any third parties explicitly authorized by EA** (including but not limited to GOG, Steam, or other distribution platforms) are hereby granted an irrevocable, perpetual, and worldwide exemption from all GPL 3.0 requirements. This exemption allows EA and authorized third parties to use, modify, distribute, sublicense, or bundle this codebase in any form (including proprietary/closed-source derivative works) for the purpose of re-releasing, preserving, maintaining, or commercially exploiting *American McGee’s Alice*, without any obligation to disclose source code, copyleft restrictions, or other GPL 3.0 terms.

This exception applies solely to EA and its authorized third parties. All other users remain subject to the terms of the GPL 3.0.
