@echo off

set folders_to_compress=pak6_VorpalFix pak7_VorpalFix_menu loc\DEU\base_g_VorpalFix loc\ESN\base_s_VorpalFix loc\FRA\base_f_VorpalFix loc\INT\base_e_VorpalFix
set seven_zip="%ProgramFiles%\7-Zip\7z.exe"

setlocal enabledelayedexpansion

for %%F in (%folders_to_compress%) do (
    set "full_path=%%F"
    set "folder_name=%%~nxF"
    set "output_file=%USERPROFILE%\Desktop\base\%%F.pk3"

    for %%D in ("!output_file!") do if not exist "%%~dpD" mkdir "%%~dpD"

    echo Compressing %%F to !output_file!...
    %seven_zip% a "!output_file!" "./%%F/*" -tzip -mm=Deflate -mx=9

    if !ERRORLEVEL! NEQ 0 (
        echo Error occurred while compressing %%F
    ) else (
        echo Successfully compressed %%F to !output_file!
    )
)

endlocal
echo All specified folders have been processed.
pause