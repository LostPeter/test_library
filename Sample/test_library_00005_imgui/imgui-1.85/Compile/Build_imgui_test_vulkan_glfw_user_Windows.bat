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

set name_sample=test_library_00005_imgui_006_vulkan_glfw_user
if "%debug%" == "debug" (
    set name_project=%name_sample%_d
) else (
    set name_project=%name_sample%
)
set name_exe=%name_project%.exe

echo %name_project%
echo %debug%

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
)
mkdir %build_folder%

@rem Bin folder/file
set bin_folder="..\..\..\..\Bin\Windows"
if not exist %bin_folder% (
    mkdir %bin_folder%
)
set bin_file=%bin_folder%"\"%name_exe%
if exist %bin_file% (
    del /S/Q %bin_file%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../"%name_sample%"/"
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_project%".exe" "..\..\..\..\..\..\Bin\Windows\"%name_exe%
) else (
    cmake "../../../"%name_sample%"/"
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_project%".exe" "..\..\..\..\..\..\Bin\Windows\"%name_exe%
)


cd ..
cd ..
cd ..
cd Compile