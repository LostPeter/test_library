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

set namebase="bullet-3.21"
set name="bullet3-3.21"
if "%debug%" == "debug" (
    set name_project=%name%"_d"
    set name_lib_linearmath=%namebase%"_linearmath_d.lib"
    set name_lib_softbody=%namebase%"_softbody_d.lib"
    set name_lib_inversedynamics=%namebase%"_inversedynamics_d.lib"
    set name_lib_dynamics=%namebase%"_dynamics_d.lib"
    set name_lib_collision=%namebase%"_collision_d.lib"
    set name_lib_fileloader=%namebase%"_fileloader_d.lib"
    set name_lib3_opencl_clew=%name%"_opencl_clew_d.lib"
    set name_lib3_geometry=%name%"_geometry_d.lib"
    set name_lib3_dynamics=%name%"_dynamics_d.lib"
    set name_lib3_common=%name%"_common_d.lib"
    set name_lib3_collision=%name%"_collision_d.lib"
) else (
    set name_project=%name%
    set name_lib_linearmath=%namebase%"_linearmath.lib"
    set name_lib_softbody=%namebase%"_softbody.lib"
    set name_lib_inversedynamics=%namebase%"_inversedynamics.lib"
    set name_lib_dynamics=%namebase%"_dynamics.lib"
    set name_lib_collision=%namebase%"_collision.lib"
    set name_lib_fileloader=%namebase%"_fileloader.lib"
    set name_lib3_opencl_clew=%name%"_opencl_clew.lib"
    set name_lib3_geometry=%name%"_geometry.lib"
    set name_lib3_dynamics=%name%"_dynamics.lib"
    set name_lib3_common=%name%"_common.lib"
    set name_lib3_collision=%name%"_collision.lib"
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
set plugins_lib_linearmath=%plugins_folder%"\"%name_lib_linearmath%
if exist %plugins_lib_linearmath% (
    del /S/Q %plugins_lib_linearmath%
)
set plugins_lib_softbody=%plugins_folder%"\"%name_lib_softbody%
if exist %plugins_lib_softbody% (
    del /S/Q %plugins_lib_softbody%
)
set plugins_lib_inversedynamics=%plugins_folder%"\"%name_lib_inversedynamics%
if exist %plugins_lib_inversedynamics% (
    del /S/Q %plugins_lib_inversedynamics%
)
set plugins_lib_dynamics=%plugins_folder%"\"%name_lib_dynamics%
if exist %plugins_lib_dynamics% (
    del /S/Q %plugins_lib_dynamics%
)
set plugins_lib_collision=%plugins_folder%"\"%name_lib_collision%
if exist %plugins_lib_collision% (
    del /S/Q %plugins_lib_collision%
)
set plugins_lib_fileloader=%plugins_folder%"\"%name_lib_fileloader%
if exist %plugins_lib_fileloader% (
    del /S/Q %plugins_lib_fileloader%
)
set plugins_lib3_opencl_clew=%plugins_folder%"\"%name_lib3_opencl_clew%
if exist %plugins_lib3_opencl_clew% (
    del /S/Q %plugins_lib3_opencl_clew%
)
set plugins_lib3_geometry=%plugins_folder%"\"%name_lib3_geometry%
if exist %plugins_lib3_geometry% (
    del /S/Q %plugins_lib3_geometry%
)
set plugins_lib3_dynamics=%plugins_folder%"\"%name_lib3_dynamics%
if exist %plugins_lib3_dynamics% (
    del /S/Q %plugins_lib3_dynamics%
)
set plugins_lib3_common=%plugins_folder%"\"%name_lib3_common%
if exist %plugins_lib3_common% (
    del /S/Q %plugins_lib3_common%
)
set plugins_lib3_collision=%plugins_folder%"\"%name_lib3_collision%
if exist %plugins_lib3_collision% (
    del /S/Q %plugins_lib3_collision%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name%/"
    msbuild BULLET_PHYSICS.sln /p:configuration=debug
    copy /Y ".\lib\Debug\LinearMath_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_linearmath%
    copy /Y ".\lib\Debug\BulletSoftBody_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_softbody%
    copy /Y ".\lib\Debug\BulletInverseDynamics_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_inversedynamics%
    copy /Y ".\lib\Debug\BulletDynamics_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_dynamics%
    copy /Y ".\lib\Debug\BulletCollision_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_collision%
    copy /Y ".\lib\Debug\Bullet2FileLoader_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_fileloader%
    copy /Y ".\lib\Debug\Bullet3OpenCL_clew_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_opencl_clew%
    copy /Y ".\lib\Debug\Bullet3Geometry_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_geometry%
    copy /Y ".\lib\Debug\Bullet3Dynamics_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_dynamics%
    copy /Y ".\lib\Debug\Bullet3Common_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_common%
    copy /Y ".\lib\Debug\Bullet3Collision_Debug.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_collision%
) else (
    cmake "../../../%name%/"
    msbuild BULLET_PHYSICS.sln /p:configuration=release
    copy /Y ".\lib\Release\LinearMath.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_linearmath%
    copy /Y ".\lib\Release\BulletSoftBody.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_softbody%
    copy /Y ".\lib\Release\BulletInverseDynamics.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_inversedynamics%
    copy /Y ".\lib\Release\BulletDynamics.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_dynamics%
    copy /Y ".\lib\Release\BulletCollision.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_collision%
    copy /Y ".\lib\Release\Bullet2FileLoader.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib_fileloader%
    copy /Y ".\lib\Release\Bullet3OpenCL_clew.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_opencl_clew%
    copy /Y ".\lib\Release\Bullet3Geometry.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_geometry%
    copy /Y ".\lib\Release\Bullet3Dynamics.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_dynamics%
    copy /Y ".\lib\Release\Bullet3Common.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_common%
    copy /Y ".\lib\Release\Bullet3Collision.lib" "..\..\..\..\..\..\Plugins\Windows\"%name_lib3_collision%
)


cd ..
cd ..
cd ..
cd Compile

set folderSrc="..\%name%\src"
set folderDst="..\..\..\..\Include\"%name%
if exist %folderDst% (
    rmdir /S/Q %folderDst%
)
mkdir %folderDst%
xcopy /Y /F %folderSrc%"\*.h" %folderDst%"\"

@REM 1, linearmath
mkdir -p %folderDst%"\LinearMath"
xcopy /Y /F %folderSrc%"\LinearMath\*.h" %folderDst%"\LinearMath\"


@REM 2, softbody
mkdir -p %folderDst%"\BulletSoftBody"
xcopy /Y /F %folderSrc%"\BulletSoftBody\*.h" %folderDst%"\BulletSoftBody\"


@REM 3, inversedynamics
mkdir -p %folderDst%"\BulletInverseDynamics"
xcopy /Y /F %folderSrc%"\BulletInverseDynamics\*.hpp" %folderDst%"\BulletInverseDynamics\"


@REM 4, dynamics
mkdir -p %folderDst%"\BulletDynamics"
mkdir -p %folderDst%"\BulletDynamics\Character"
mkdir -p %folderDst%"\BulletDynamics\ConstraintSolver"
mkdir -p %folderDst%"\BulletDynamics\Dynamics"
mkdir -p %folderDst%"\BulletDynamics\Featherstone"
mkdir -p %folderDst%"\BulletDynamics\MLCPSolvers"
mkdir -p %folderDst%"\BulletDynamics\Vehicle"
xcopy /Y /F %folderSrc%"\BulletDynamics\Character\*.h" %folderDst%"\BulletDynamics\Character\"
xcopy /Y /F %folderSrc%"\BulletDynamics\ConstraintSolver\*.h" %folderDst%"\BulletDynamics\ConstraintSolver\"
xcopy /Y /F %folderSrc%"\BulletDynamics\Dynamics\*.h" %folderDst%"\BulletDynamics\Dynamics\"
xcopy /Y /F %folderSrc%"\BulletDynamics\Featherstone\*.h" %folderDst%"\BulletDynamics\Featherstone\"
xcopy /Y /F %folderSrc%"\BulletDynamics\MLCPSolvers\*.h" %folderDst%"\BulletDynamics\MLCPSolvers\"
xcopy /Y /F %folderSrc%"\BulletDynamics\Vehicle\*.h" %folderDst%"\BulletDynamics\Vehicle\"


@REM 5, collision
mkdir -p %folderDst%"\BulletCollision"
mkdir -p %folderDst%"\BulletCollision\BroadphaseCollision"
mkdir -p %folderDst%"\BulletCollision\CollisionDispatch"
mkdir -p %folderDst%"\BulletCollision\CollisionShapes"
mkdir -p %folderDst%"\BulletCollision\Gimpact"
mkdir -p %folderDst%"\BulletCollision\NarrowPhaseCollision"
xcopy /Y /F %folderSrc%"\BulletCollision\BroadphaseCollision\*.h" %folderDst%"\BulletCollision\BroadphaseCollision\"
xcopy /Y /F %folderSrc%"\BulletCollision\CollisionDispatch\*.h" %folderDst%"\BulletCollision\CollisionDispatch\"
xcopy /Y /F %folderSrc%"\BulletCollision\CollisionShapes\*.h" %folderDst%"\BulletCollision\CollisionShapes\"
xcopy /Y /F %folderSrc%"\BulletCollision\Gimpact\*.h" %folderDst%"\BulletCollision\Gimpact\"
xcopy /Y /F %folderSrc%"\BulletCollision\NarrowPhaseCollision\*.h" %folderDst%"\BulletCollision\NarrowPhaseCollision\"


@REM 6, fileloader
mkdir -p %folderDst%"\Bullet3Serialize\Bullet2FileLoader"
xcopy /Y /F %folderSrc%"\Bullet3Serialize\Bullet2FileLoader\*.h" %folderDst%"\Bullet3Serialize\Bullet2FileLoader\"


@REM 7, opencl_clew
mkdir -p %folderDst%"\Bullet3OpenCL"
mkdir -p %folderDst%"\Bullet3OpenCL\BroadphaseCollision"
mkdir -p %folderDst%"\Bullet3OpenCL\Initialize"
mkdir -p %folderDst%"\Bullet3OpenCL\NarrowphaseCollision"
mkdir -p %folderDst%"\Bullet3OpenCL\ParallelPrimitives"
mkdir -p %folderDst%"\Bullet3OpenCL\Raycast"
mkdir -p %folderDst%"\Bullet3OpenCL\RigidBody"
xcopy /Y /F %folderSrc%"\Bullet3OpenCL\BroadphaseCollision\*.h" %folderDst%"\Bullet3OpenCL\BroadphaseCollision\"
xcopy /Y /F %folderSrc%"\Bullet3OpenCL\Initialize\*.h" %folderDst%"\Bullet3OpenCL\Initialize\"
xcopy /Y /F %folderSrc%"\Bullet3OpenCL\NarrowphaseCollision\*.h" %folderDst%"\Bullet3OpenCL\NarrowphaseCollision\"
xcopy /Y /F %folderSrc%"\Bullet3OpenCL\ParallelPrimitives\*.h" %folderDst%"\Bullet3OpenCL\ParallelPrimitives\"
xcopy /Y /F %folderSrc%"\Bullet3OpenCL\Raycast\*.h" %folderDst%"\Bullet3OpenCL\Raycast\"
xcopy /Y /F %folderSrc%"\Bullet3OpenCL\RigidBody\*.h" %folderDst%"\Bullet3OpenCL\RigidBody\"


@REM 8, geometry
mkdir -p %folderDst%"\Bullet3Geometry"
xcopy /Y /F %folderSrc%"\Bullet3Geometry\*.h" %folderDst%"\Bullet3Geometry\"


@REM 9, dynamics
mkdir -p %folderDst%"\Bullet3Dynamics"
mkdir -p %folderDst%"\Bullet3Dynamics\ConstraintSolver"
mkdir -p %folderDst%"\Bullet3Dynamics\shared"
xcopy /Y /F %folderSrc%"\Bullet3Dynamics\*.h" %folderDst%"\Bullet3Dynamics\"
xcopy /Y /F %folderSrc%"\Bullet3Dynamics\ConstraintSolver\*.h" %folderDst%"\Bullet3Dynamics\ConstraintSolver\"
xcopy /Y /F %folderSrc%"\Bullet3Dynamics\shared\*.h" %folderDst%"\Bullet3Dynamics\shared\"


@REM 10, common
mkdir -p %folderDst%"\Bullet3Common"
mkdir -p %folderDst%"\Bullet3Common\shared"
xcopy /Y /F %folderSrc%"\Bullet3Common\*.h" %folderDst%"\Bullet3Common\"
xcopy /Y /F %folderSrc%"\Bullet3Common\shared\*.h" %folderDst%"\Bullet3Common\shared\"


@REM 11, collision
mkdir -p %folderDst%"\Bullet3Collision"
mkdir -p %folderDst%"\Bullet3Collision/BroadPhaseCollision"
mkdir -p %folderDst%"\Bullet3Collision/BroadPhaseCollision\shared"
mkdir -p %folderDst%"\Bullet3Collision/NarrowPhaseCollision"
mkdir -p %folderDst%"\Bullet3Collision/NarrowPhaseCollision\shared"
xcopy /Y /F %folderSrc%"\Bullet3Collision\BroadPhaseCollision\*.h" %folderDst%"\Bullet3Collision\BroadPhaseCollision\"
xcopy /Y /F %folderSrc%"\Bullet3Collision\BroadPhaseCollision\shared\*.h" %folderDst%"\Bullet3Collision\BroadPhaseCollision\shared\"
xcopy /Y /F %folderSrc%"\Bullet3Collision\NarrowPhaseCollision\*.h" %folderDst%"\Bullet3Collision\NarrowPhaseCollision\"
xcopy /Y /F %folderSrc%"\Bullet3Collision\NarrowPhaseCollision\shared\*.h" %folderDst%"\Bullet3Collision\NarrowPhaseCollision\shared\"