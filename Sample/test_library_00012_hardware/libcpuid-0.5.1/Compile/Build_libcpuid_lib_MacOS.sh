#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

name=libcpuid-0.5.1
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib=$name"_d.a"
else
    name_project=$name
    name_lib=$name".a"
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

cp -rf "../Build/MacOS/"$name_project"/libcpuid/libcpuid.a" "../../../../Plugins/MacOS/"$name_lib


folderSrc="../"$name"/libcpuid"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/libcpuid*.h
do 
    cp -rf $file $folderDst
done