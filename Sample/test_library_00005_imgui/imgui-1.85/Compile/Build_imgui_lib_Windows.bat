@REM author: LostPeter
@REM time:   2022-10-29

@echo off
set mode=%1
set debug=%2

@REM mode only can be dx12, gl3, gles3_android, vulkan_glfw, vulkan_android
set m_dx12=dx12
set m_gl3=gl3
set m_gles_android=gles3_android
set m_vulkan_glfw=vulkan_glfw
set m_vulkan_android=vulkan_android

if %mode% neq %m_dx12% (
    if %mode% neq %m_gl3% (
        if %mode% neq %m_gles_android% (
            if %mode% neq %m_vulkan_glfw% (
                if %mode% neq %m_vulkan_android% (
                    echo "wrong mode type: [%mode%], only can be [%m_dx12%][%m_gl3%][%m_gles_android%][%m_vulkan_glfw%][%m_vulkan_android%]"
                    exit
                )
            )     
        )
    )
)
echo "mode: %mode%, %debug%"


set name_base=imgui
set name=%name_base%-1.85

if "%debug%" == "debug" (
    set name_project=%name%"_"%mode%"_d"
    set name_lib=%name%"_"%mode%"_d.lib"
) else (
    set name_project=%name%"_"%mode%
    set name_lib=%name%"_"%mode%".lib"
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

set bin_floder="..\..\..\..\Bin\Windows"
if not exist %bin_floder% (
    mkdir %bin_floder%
)

cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 -DPLATFORM_MODE=%mode% ../../../%name%/
    msbuild "%name_project%".sln /p:configuration=debug
    copy /Y ".\Debug\"%name_lib% "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
) else (
    cmake -DPLATFORM_MODE=%mode% ../../../%name%/
    msbuild "%name_project%".sln /p:configuration=release
    copy /Y ".\Release\"%name_lib% "..\..\..\..\..\..\Plugins\Windows\"%name_lib%
)


cd ..
cd ..
cd ..
cd Compile

set include_folder="..\..\..\..\Include\"%name%"_"%mode%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\%name%\%name_base%\*.h" %include_folder%"\"%name_base%"\"
if %mode% == %m_dx12% (
    xcopy /S /E /Y /F "..\%name%\impl_dx12\*.h" %include_folder%"\impl_dx12\"
    xcopy /S /E /Y /F "..\%name%\impl_win32\*.h" %include_folder%"\impl_win32\"
) else if %mode% == %m_gl3% (
    xcopy /S /E /Y /F "..\%name%\impl_gl3\*.h" %include_folder%"\impl_gl3\"
) else if %mode% == %m_gles_android% (
    xcopy /S /E /Y /F "..\%name%\impl_gles3_android\*.h" %include_folder%"\impl_gles3_android\"
) else if %mode% == %m_vulkan_glfw% (
    xcopy /S /E /Y /F "..\%name%\impl_glfw\*.h" %include_folder%"\impl_glfw\"
    xcopy /S /E /Y /F "..\%name%\impl_vulkan\*.h" %include_folder%"\impl_vulkan\"
) else if %mode% == %m_vulkan_android% (
    xcopy /S /E /Y /F "..\%name%\impl_gles3_android\*.h" %include_folder%"\impl_gles3_android\"
)

@echo over