#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

namebase=bullet-3.21
name=bullet3-3.21
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib_linearmath="lib"$namebase"_linearmath_d.a"
    name_lib_softbody="lib"$namebase"_softbody_d.a"
    name_lib_inversedynamics="lib"$namebase"_inversedynamics_d.a"
    name_lib_dynamics="lib"$namebase"_dynamics_d.a"
    name_lib_collision="lib"$namebase"_collision_d.a"
    name_lib_fileloader="lib"$namebase"_fileloader_d.a"
    name_lib3_opencl_clew="lib"$name"_opencl_clew_d.a"
    name_lib3_geometry="lib"$name"_geometry_d.a"
    name_lib3_dynamics="lib"$name"_dynamics_d.a"
    name_lib3_common="lib"$name"_common_d.a"
    name_lib3_collision="lib"$name"_collision_d.a"
else
    name_project=$name
    name_lib_linearmath="lib"$namebase"_linearmath.a"
    name_lib_softbody="lib"$namebase"_softbody.a"
    name_lib_inversedynamics="lib"$namebase"_inversedynamics.a"
    name_lib_dynamics="lib"$namebase"_dynamics.a"
    name_lib_collision="lib"$namebase"_collision.a"
    name_lib_fileloader="lib"$namebase"_fileloader.a"
    name_lib3_opencl_clew="lib"$name"_opencl_clew.a"
    name_lib3_geometry="lib"$name"_geometry.a"
    name_lib3_dynamics="lib"$name"_dynamics.a"
    name_lib3_common="lib"$name"_common.a"
    name_lib3_collision="lib"$name"_collision.a"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Plugins/MacOS/"$name_lib_linearmath
rm -rf "../../../../Plugins/MacOS/"$name_lib_softbody
rm -rf "../../../../Plugins/MacOS/"$name_lib_inversedynamics
rm -rf "../../../../Plugins/MacOS/"$name_lib_dynamics
rm -rf "../../../../Plugins/MacOS/"$name_lib_collision
rm -rf "../../../../Plugins/MacOS/"$name_lib_fileloader
rm -rf "../../../../Plugins/MacOS/"$name_lib3_opencl_clew
rm -rf "../../../../Plugins/MacOS/"$name_lib3_geometry
rm -rf "../../../../Plugins/MacOS/"$name_lib3_dynamics
rm -rf "../../../../Plugins/MacOS/"$name_lib3_common
rm -rf "../../../../Plugins/MacOS/"$name_lib3_collision

mkdir -p "../../../../Plugins/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../$name/
else
    cmake ../../../$name/
fi
make

cd ..
cd ..
cd ..
cd Compile


folderSrc="../$name/src"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done

#1> linearmath
cp -rf "../Build/MacOS/"$name_project"/src/LinearMath/libLinearMath.a" "../../../../Plugins/MacOS/"$name_lib_linearmath
mkdir -p $folderDst/LinearMath
for file in ${folderSrc}/LinearMath/*.h
do 
    cp -rf $file $folderDst/LinearMath
done

#2> softbody
cp -rf "../Build/MacOS/"$name_project"/src/BulletSoftBody/libBulletSoftBody.a" "../../../../Plugins/MacOS/"$name_lib_softbody
mkdir -p $folderDst/BulletSoftBody
for file in ${folderSrc}/BulletSoftBody/*.h
do 
    cp -rf $file $folderDst/BulletSoftBody
done

#3> inversedynamics
cp -rf "../Build/MacOS/"$name_project"/src/BulletInverseDynamics/libBulletInverseDynamics.a" "../../../../Plugins/MacOS/"$name_lib_inversedynamics
mkdir -p $folderDst/BulletInverseDynamics
for file in ${folderSrc}/BulletInverseDynamics/*.hpp
do 
    cp -rf $file $folderDst/BulletInverseDynamics
done

#4> dynamics
cp -rf "../Build/MacOS/"$name_project"/src/BulletDynamics/libBulletDynamics.a" "../../../../Plugins/MacOS/"$name_lib_dynamics
mkdir -p $folderDst/BulletDynamics
mkdir -p $folderDst/BulletDynamics/Character
mkdir -p $folderDst/BulletDynamics/ConstraintSolver
mkdir -p $folderDst/BulletDynamics/Dynamics
mkdir -p $folderDst/BulletDynamics/Featherstone
mkdir -p $folderDst/BulletDynamics/MLCPSolvers
mkdir -p $folderDst/BulletDynamics/Vehicle
for file in ${folderSrc}/BulletDynamics/Character/*.h
do 
    cp -rf $file $folderDst/BulletDynamics/Character
done
for file in ${folderSrc}/BulletDynamics/ConstraintSolver/*.h
do 
    cp -rf $file $folderDst/BulletDynamics/ConstraintSolver
done
for file in ${folderSrc}/BulletDynamics/Dynamics/*.h
do 
    cp -rf $file $folderDst/BulletDynamics/Dynamics
done
for file in ${folderSrc}/BulletDynamics/Featherstone/*.h
do 
    cp -rf $file $folderDst/BulletDynamics/Featherstone
done
for file in ${folderSrc}/BulletDynamics/MLCPSolvers/*.h
do 
    cp -rf $file $folderDst/BulletDynamics/MLCPSolvers
done
for file in ${folderSrc}/BulletDynamics/Vehicle/*.h
do 
    cp -rf $file $folderDst/BulletDynamics/Vehicle
done

#5> collision
cp -rf "../Build/MacOS/"$name_project"/src/BulletCollision/libBulletCollision.a" "../../../../Plugins/MacOS/"$name_lib_collision
mkdir -p $folderDst/BulletCollision
mkdir -p $folderDst/BulletCollision/BroadphaseCollision
mkdir -p $folderDst/BulletCollision/CollisionDispatch
mkdir -p $folderDst/BulletCollision/CollisionShapes
mkdir -p $folderDst/BulletCollision/Gimpact
mkdir -p $folderDst/BulletCollision/NarrowPhaseCollision
for file in ${folderSrc}/BulletCollision/BroadphaseCollision/*.h
do 
    cp -rf $file $folderDst/BulletCollision/BroadphaseCollision
done
for file in ${folderSrc}/BulletCollision/CollisionDispatch/*.h
do 
    cp -rf $file $folderDst/BulletCollision/CollisionDispatch
done
for file in ${folderSrc}/BulletCollision/CollisionShapes/*.h
do 
    cp -rf $file $folderDst/BulletCollision/CollisionShapes
done
for file in ${folderSrc}/BulletCollision/Gimpact/*.h
do 
    cp -rf $file $folderDst/BulletCollision/Gimpact
done
for file in ${folderSrc}/BulletCollision/NarrowPhaseCollision/*.h
do 
    cp -rf $file $folderDst/BulletCollision/NarrowPhaseCollision
done

#6> fileloader
cp -rf "../Build/MacOS/"$name_project"/src/Bullet3Serialize/Bullet2FileLoader/libBullet2FileLoader.a" "../../../../Plugins/MacOS/"$name_lib_fileloader
mkdir -p $folderDst/Bullet3Serialize/Bullet2FileLoader
for file in ${folderSrc}/Bullet3Serialize/Bullet2FileLoader/*.h
do 
    cp -rf $file $folderDst/Bullet3Serialize/Bullet2FileLoader
done

#7> opencl_clew
cp -rf "../Build/MacOS/"$name_project"/src/Bullet3OpenCL/libBullet3OpenCL_clew.a" "../../../../Plugins/MacOS/"$name_lib3_opencl_clew
mkdir -p $folderDst/Bullet3OpenCL
mkdir -p $folderDst/Bullet3OpenCL/BroadphaseCollision
mkdir -p $folderDst/Bullet3OpenCL/Initialize
mkdir -p $folderDst/Bullet3OpenCL/NarrowphaseCollision
mkdir -p $folderDst/Bullet3OpenCL/ParallelPrimitives
mkdir -p $folderDst/Bullet3OpenCL/Raycast
mkdir -p $folderDst/Bullet3OpenCL/RigidBody
for file in ${folderSrc}/Bullet3OpenCL/BroadphaseCollision/*.h
do 
    cp -rf $file $folderDst/Bullet3OpenCL/BroadphaseCollision
done
for file in ${folderSrc}/Bullet3OpenCL/Initialize/*.h
do 
    cp -rf $file $folderDst/Bullet3OpenCL/Initialize
done
for file in ${folderSrc}/Bullet3OpenCL/NarrowphaseCollision/*.h
do 
    cp -rf $file $folderDst/Bullet3OpenCL/NarrowphaseCollision
done
for file in ${folderSrc}/Bullet3OpenCL/ParallelPrimitives/*.h
do 
    cp -rf $file $folderDst/Bullet3OpenCL/ParallelPrimitives
done
for file in ${folderSrc}/Bullet3OpenCL/Raycast/*.h
do 
    cp -rf $file $folderDst/Bullet3OpenCL/Raycast
done
for file in ${folderSrc}/Bullet3OpenCL/RigidBody/*.h
do 
    cp -rf $file $folderDst/Bullet3OpenCL/RigidBody
done

#8> geometry
cp -rf "../Build/MacOS/"$name_project"/src/Bullet3Geometry/libBullet3Geometry.a" "../../../../Plugins/MacOS/"$name_lib3_geometry
mkdir -p $folderDst/Bullet3Geometry
for file in ${folderSrc}/Bullet3Geometry/*.h
do 
    cp -rf $file $folderDst/Bullet3Geometry
done

#9> dynamics
cp -rf "../Build/MacOS/"$name_project"/src/Bullet3Dynamics/libBullet3Dynamics.a" "../../../../Plugins/MacOS/"$name_lib3_dynamics
mkdir -p $folderDst/Bullet3Dynamics
mkdir -p $folderDst/Bullet3Dynamics/ConstraintSolver
mkdir -p $folderDst/Bullet3Dynamics/shared
for file in ${folderSrc}/Bullet3Dynamics/*.h
do 
    cp -rf $file $folderDst/Bullet3Dynamics
done
for file in ${folderSrc}/Bullet3Dynamics/ConstraintSolver/*.h
do 
    cp -rf $file $folderDst/Bullet3Dynamics/ConstraintSolver
done
for file in ${folderSrc}/Bullet3Dynamics/shared/*.h
do 
    cp -rf $file $folderDst/Bullet3Dynamics/shared
done

#10> common
cp -rf "../Build/MacOS/"$name_project"/src/Bullet3Common/libBullet3Common.a" "../../../../Plugins/MacOS/"$name_lib3_common
mkdir -p $folderDst/Bullet3Common
mkdir -p $folderDst/Bullet3Common/shared
for file in ${folderSrc}/Bullet3Common/*.h
do 
    cp -rf $file $folderDst/Bullet3Common
done
for file in ${folderSrc}/Bullet3Common/shared/*.h
do 
    cp -rf $file $folderDst/Bullet3Common/shared
done

#11> collision
cp -rf "../Build/MacOS/"$name_project"/src/Bullet3Collision/libBullet3Collision.a" "../../../../Plugins/MacOS/"$name_lib3_collision
mkdir -p $folderDst/Bullet3Collision
mkdir -p $folderDst/Bullet3Collision/BroadPhaseCollision
mkdir -p $folderDst/Bullet3Collision/BroadPhaseCollision/shared
mkdir -p $folderDst/Bullet3Collision/NarrowPhaseCollision
mkdir -p $folderDst/Bullet3Collision/NarrowPhaseCollision/shared
for file in ${folderSrc}/Bullet3Collision/BroadPhaseCollision/*.h
do 
    cp -rf $file $folderDst/Bullet3Collision/BroadPhaseCollision
done
for file in ${folderSrc}/Bullet3Collision/BroadPhaseCollision/shared/*.h
do 
    cp -rf $file $folderDst/Bullet3Collision/BroadPhaseCollision/shared
done
for file in ${folderSrc}/Bullet3Collision/NarrowPhaseCollision/*.h
do 
    cp -rf $file $folderDst/Bullet3Collision/NarrowPhaseCollision
done
for file in ${folderSrc}/Bullet3Collision/NarrowPhaseCollision/shared/*.h
do 
    cp -rf $file $folderDst/Bullet3Collision/NarrowPhaseCollision/shared
done