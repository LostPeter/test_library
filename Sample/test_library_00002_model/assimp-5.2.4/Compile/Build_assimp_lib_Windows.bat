@REM #****************************************************************************
@REM # test_library - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-29
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

@echo off
set debug=%1
echo %debug%

set name="assimp-5.2.4"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
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
    cmake -DDEBUG=1 "../../../%name%/" -A x64
    msbuild Assimp.sln /p:configuration=debug
    copy /Y ".\lib\Debug\assimp-vc142-mtd.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    xcopy /S /E /Y /F ".\include\"
) else (
    cmake "../../../%name%/" -A x64
    msbuild Assimp.sln /p:configuration=release
    copy /Y ".\lib\Release\assimp-vc142-mt.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
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

xcopy /S /E /Y /F "..\%name%\include\assimp\" %include_folder%"\assimp"
xcopy /S /E /Y /F "..\Build\Windows\%name_project%\include\assimp\config.h" %include_folder%"\assimp\"