#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

name_tcmalloc="tcmalloc-9.9.5"
name=gperftools-2.9.1
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib_tcmalloc="lib"$name_tcmalloc"_d.a"
else
    name_project=$name
    name_lib_tcmalloc="lib"$name_tcmalloc".a"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Plugins/MacOS/"$name_lib_tcmalloc
mkdir -p "../../../../Plugins/MacOS"

rm -rf "../../../../Include/"$name
mkdir -p "../../../../Include/"$name

cd ..
cd Build
cd MacOS
cd $name_project

#lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DGPERFTOOLS_BUILD_STATIC=1 ../../../$name/
else
    cmake -DGPERFTOOLS_BUILD_STATIC=1 ../../../$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project"/libtcmalloc.a" "../../../../Plugins/MacOS/"$name_lib_tcmalloc
cp -rf "../Build/MacOS/"$name_project"/gperftools/tcmalloc.h" "../../../../Include/"$name