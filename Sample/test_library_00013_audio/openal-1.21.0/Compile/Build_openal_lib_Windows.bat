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

set name="openal-1.21.0"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib=%name%"_d.lib"
    set name_lib_common=%name%"_common_d.lib"
    set name_lib_ex_common=%name%"_ex-common_d.lib"
) else (
    set name_project=%name%
    set name_lib=%name%".lib"
   set name_lib_common=%name%"_common.lib"
    set name_lib_ex_common=%name%"_ex-common.lib"
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
set plugins_libfile_common=%plugins_folder%"\"%name_lib_common%
if exist %plugins_libfile_common% (
    del /S/Q %plugins_libfile_common%
)
set plugins_libfile_ex_common=%plugins_folder%"\"%name_lib_ex_common%
if exist %plugins_libfile_ex_common% (
    del /S/Q %plugins_libfile_ex_common%
)

cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name%/"
    msbuild OpenAL.sln /p:configuration=debug
    copy /Y ".\Debug\OpenAL32.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Debug\common.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_common%
    copy /Y ".\Debug\ex-common.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_ex_common%
) else (
    cmake "../../../%name%/"
    msbuild OpenAL.sln /p:configuration=release
    copy /Y ".\Release\OpenAL32.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Release\common.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_common%
    copy /Y ".\Release\ex-common.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_ex_common%
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

xcopy /S /E /Y /F "..\%name%\include\AL\*.h" %include_folder%"\AL\"