@REM author: LostPeter
@REM time:   2022-10-29

@echo off
set debug=%1
echo %debug%

set name="protobuf-3.19.4"
if "%debug%" == "debug" (
    set name_project=%name%_d
    set name_lib=%name%_d.lib
    set name_build=Debug
) else (
    set name_project=%name%
    set name_lib=%name%.lib
    set name_build=Release
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
    cmake -DDEBUG=1 "../../../%name%/cmake/"
    msbuild protobuf.sln /p:configuration=debug
    copy /Y ".\Debug\libprotobufd.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Debug\protoc.exe" "..\..\..\..\..\..\Bin\Windows\protoc_d.exe"
) else (
    cmake "../../../%name%/cmake/"
    msbuild protobuf.sln /p:configuration=release
    copy /Y ".\Release\libprotobuf.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
    copy /Y ".\Release\protoc.exe" "..\..\..\..\..\..\Bin\Windows\protoc.exe"
)

cd ..
cd ..
cd ..
cd Compile

set include_folder="..\..\..\..\Include\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%"\google\protobuf"


xcopy /S /E /Y /F "..\%name%\src\google\protobuf\*.h" %include_folder%"\google\protobuf\"