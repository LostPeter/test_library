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

set name_tcmalloc="tcmalloc-9.9.5"
set name="gperftools-2.9.1"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib_tcmalloc=%name_tcmalloc%"_d.lib"
) else (
    set name_project=%name%
    set name_lib_tcmalloc=%name_tcmalloc%".lib"
)

@REM build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
)
mkdir %build_folder%


@REM Plugins folder/file
set plugins_folder="..\..\..\..\Plugins\Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"\"%name_lib_tcmalloc%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)

set include_folder="..\..\..\..\Include\"%name_tcmalloc%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%  

cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name%/"
    msbuild gperftools.sln /p:configuration=debug
    copy /Y ".\Debug\tcmalloc_minimal_static.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_tcmalloc%
    copy /Y ".\gperftools\tcmalloc.h" "..\..\..\..\..\..\Include\"%name_tcmalloc%
) else (
    cmake "../../../%name%/"
    msbuild gperftools.sln /p:configuration=release
    copy /Y ".\Release\tcmalloc_minimal_static.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_tcmalloc%
    copy /Y ".\gperftools\tcmalloc.h" "..\..\..\..\..\..\Include\"%name_tcmalloc%
)


cd ..
cd ..
cd ..
cd Compile