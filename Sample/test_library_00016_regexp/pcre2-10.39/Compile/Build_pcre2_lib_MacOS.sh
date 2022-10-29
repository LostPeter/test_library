# author: LostPeter
# time:   2022-10-29

debug=${1}

name=pcre2-10.39
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
else
    name_project=$name
    name_lib="lib"$name".a"
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

cp -rf "../Build/MacOS/"$name_project"/libpcre2-8.a" "../../../../Plugins/MacOS/"$name_lib

folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -rf "../Build/MacOS/"$name_project"/pcre2.h" $folderDst