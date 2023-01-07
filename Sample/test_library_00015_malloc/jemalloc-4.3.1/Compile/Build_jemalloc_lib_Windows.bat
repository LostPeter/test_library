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

set name="jemalloc-4.3.1"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
    set name_lib_jet=%name%"_jet_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
    set name_lib_jet=%name%"_jet.lib"
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
set plugins_libfile_jet=%plugins_folder%"\"%name_lib_jet%
if exist %plugins_libfile_jet% (
    del /S/Q %plugins_libfile_jet%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name%/"
    msbuild jemalloc.sln /p:configuration=debug
    copy /Y ".\Debug\jemalloc.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Debug\jemalloc_jet.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_jet%
%
) else (
    cmake "../../../%name%/"
    msbuild jemalloc.sln /p:configuration=release
    copy /Y ".\Release\jemalloc.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Release\jemalloc_jet.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_jet%
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

xcopy /S /E /Y /F "..\%name%\include\*.h" %include_folder%"\"