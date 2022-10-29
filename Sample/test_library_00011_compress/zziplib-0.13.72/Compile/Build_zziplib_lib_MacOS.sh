# author: LostPeter
# time:   2022-10-29

debug=${1}

name=zziplib-0.13.72
namewrap=zzipwraplib-0.13.72
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_lib_wrap="lib"$namewrap"_d.a"
else
    name_project=$name
    name_lib="lib"$name".a"
    name_lib_wrap="lib"$namewrap".a"
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


if [ "$debug" == "debug" ]; then
    cp -rf "../Build/MacOS/"$name_project"/zzip/libzzip.a" "../../../../Plugins/MacOS/"$name_lib
    cp -rf "../Build/MacOS/"$name_project"/zzipwrap/libzzipwrap.a" "../../../../Plugins/MacOS/"$name_lib_wrap
else
    cp -rf "../Build/MacOS/"$name_project"/zzip/libzzip-0.a" "../../../../Plugins/MacOS/"$name_lib
    cp -rf "../Build/MacOS/"$name_project"/zzipwrap/libzzipwrap-0.a" "../../../../Plugins/MacOS/"$name_lib_wrap
fi


folderSrc="../"$name"/zzip"
folderDst="../../../../Include/"$name"/zzip"
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done


folderSrcWrap="../"$name"/zzipwrap"
folderDstWrap="../../../../Include/"$name"/zzipwrap"
rm -rf $folderDstWrap
mkdir -p $folderDstWrap

for file in ${folderSrcWrap}/*.h
do 
    cp -rf $file $folderDstWrap
done