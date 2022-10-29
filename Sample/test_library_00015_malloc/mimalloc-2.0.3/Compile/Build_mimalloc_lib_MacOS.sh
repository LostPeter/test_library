# author: LostPeter
# time:   2022-10-29

debug=${1}

name=mimalloc-2.0.3
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_lib_src="libmimalloc-debug.a"
else
    name_project=$name
    name_lib="lib"$name".a"
    name_lib_src="libmimalloc.a"
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

cp -rf "../Build/MacOS/"$name_project"/"$name_lib_src "../../../../Plugins/MacOS/"$name_lib


folderSrc="../"$name"/include"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done