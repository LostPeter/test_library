# author: LostPeter
# time:   2022-10-29

debug=${1}

name=protobuf-3.19.4
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_lib_src="libprotobufd.a"
else
    name_project=$name
    name_lib="lib"$name".a"
    name_lib_src="libprotobuf.a"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Plugins/MacOS/"$name_lib
mkdir -p "../../../../Plugins/MacOS"



cd ..
cd Build
cd MacOS
cd $name_project

# lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../$name/cmake/
else
    cmake ../../../$name/cmake/
fi
make
make DESTDIR=./lib install


cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project"/"$name_lib_src "../../../../Plugins/MacOS/"$name_lib
cp "../Build/MacOS/"$name_project"/lib/usr/local/bin/protoc" "../../../../Bin/MacOS/protoc"
cp "../Build/MacOS/"$name_project"/lib/usr/local/bin/protoc-3.19.4.0" "../../../../Bin/MacOS/protoc-3.19.4.0"


folderDst="../../../../Include/"$name"/"
rm -rf $folderDst
mkdir -p $folderDst
cp -rf "../Build/MacOS/"$name_project"/lib/usr/local/include/" $folderDst