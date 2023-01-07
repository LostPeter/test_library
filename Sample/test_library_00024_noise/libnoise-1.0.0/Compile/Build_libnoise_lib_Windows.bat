@REM author: LostPeter
@REM time:   2023-01-07

@echo off
set debug=%1
echo %debug%

set name="libnoise-1.0.0"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
    set nameutil_lib="libnoiseutils-1.0.0_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
    set nameutil_lib="libnoiseutils-1.0.0.lib"
)

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
)
mkdir %build_folder%


@rem Plugins folder/file
set plugins_folder="..\..\..\..\Plugins\Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"\"%name_lib%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name%/"
    msbuild libnoise.sln /p:configuration=debug
    copy /Y ".\Debug\libnoise.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Debug\libnoiseutils.lib" "..\..\..\..\..\..\Plugins\Windows\"%nameutil_lib%
) else (
    cmake "../../../%name%/"
    msbuild libnoise.sln /p:configuration=release
    copy /Y ".\Release\libnoise.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Release\libnoiseutils.lib" "..\..\..\..\..\..\Plugins\Windows\"%nameutil_lib%
)


cd ..
cd ..
cd ..
cd Compile

set include_folder="..\..\..\..\Include\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /E /S /Y /F "..\%name%\src\noise" %include_folder%"\"