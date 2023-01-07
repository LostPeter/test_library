#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

mode=${1}
debug=${2}

# mode only can be gl3, gles3_android, gles3_ios, vulkan_glfw, vulkan_android, metal
m_gl3="gl3"
m_gles_android="gles3_android"
m_gles_ios="gles3_ios"
m_vulkan_glfw="vulkan_glfw"
m_vulkan_android="vulkan_android"
m_metal="metal"

if [ [ "$mode" != "$m_gl3" ]\
    && [ "$mode" != "$m_gles_android" ]\
    && [ "$mode" != "$m_gles_ios" ]\
    && [ "$mode" != "$m_vulkan_glfw" ]\
    && [ "$mode" != "$m_vulkan_android" ]\
    &&[ "$mode" != "$m_metal" ]; then

    echo "wrong mode type: ["$mode"], only can be [$m_gl3][$m_gles_android][$m_gles_ios][$m_vulkan_glfw][$m_vulkan_android][$m_metal]"
    exit 1
fi
echo "mode: $mode, $debug"

name_base="imgui"
name="${name_base}-1.85"

if [ "$debug" == "debug" ]; then
    name_project=$name"_"$mode"_d"
    name_dylib="lib"$name"_"$mode"_d.dylib"
else
    name_project=$name"_"$mode
    name_dylib="lib"$name"_"$mode".dylib"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Plugins/MacOS/"$name_dylib
mkdir -p "../../../../Plugins/MacOS"

cd ..
cd Build
cd MacOS
cd $name_project

#dylib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DPLATFORM_MODE=$mode ../../../$name/
else
    cmake -DPLATFORM_MODE=$mode ../../../$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project/$name_dylib "../../../../Plugins/MacOS/"$name_dylib

folderSrc="../$name/$name_base"
folderDstRoot="../../../../Include/"$name"_"$mode
rm -rf $folderDstRoot
mkdir -p $folderDstRoot
folderDst=$folderDstRoot"/"$name_base
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done

if [ "$mode" == "$m_gl3" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_gles_android" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_gles_ios" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_vulkan_glfw" ]; then 
    echo "mode: $mode"

    folderSrc_Glfw="../"$name"/impl_glfw"
    floderSrc_Vulkan="../"$name"/impl_vulkan"
    folderDst_Glfw="../../../../Include/"$name"_"$mode"/impl_glfw/"
    folderDst_Vulkan="../../../../Include/"$name"_"$mode"/impl_vulkan/"
    mkdir -p $folderDst_Glfw
    mkdir -p $folderDst_Vulkan

    for file in ${folderSrc_Glfw}/*.h
    do 
        cp -rf $file $folderDst_Glfw
    done

    for file in ${floderSrc_Vulkan}/*.h
    do 
        cp -rf $file $folderDst_Vulkan
    done

elif [ "$mode" == "$m_vulkan_android" ]; then 
    echo "mode: $mode"

elif [ "$mode" == "$m_metal" ]; then 
    echo "mode: $mode"

else 
    echo "mode: $mode, unknown !"
    
fi