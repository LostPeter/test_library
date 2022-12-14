#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2023-01-07
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

name=libnoise-1.0.0
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib=$name"_d.a"
    nameutil_lib="libnoiseutils-1.0.0_d.a"
else
    name_project=$name
    name_lib=$name".a"
    nameutil_lib="libnoiseutils-1.0.0.a"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Plugins/MacOS/"$name_lib
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

cp -rf "../Build/MacOS/"$name_project"/src/libnoise.a" "../../../../Plugins/MacOS/"$name_lib
cp -rf "../Build/MacOS/"$name_project"/noiseutils/libnoiseutils-static.a" "../../../../Plugins/MacOS/"$nameutil_lib

folderSrc="../"$name
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst
mkdir -p $folderDst"/noiseutils"

cp -rf $folderSrc"/src/noise" $folderDst

for file in ${folderSrc}/noiseutils/*.h
do 
    cp -rf $file $folderDst"/noiseutils"
done
