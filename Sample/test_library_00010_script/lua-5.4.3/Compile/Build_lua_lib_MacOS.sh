# author: LostPeter
# time:   2022-10-29

debug=${1}

name=lua-5.4.3
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_lua="lua_d"
    name_luac="luac_d"
else
    name_project=$name
    name_lib="lib"$name".a"
    name_lua="lua"
    name_luac="luac"
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

cp -rf "../Build/MacOS/"$name_project"/liblua.a" "../../../../Plugins/MacOS/"$name_lib
cp -rf "../Build/MacOS/$name_project/lua" "../../../../Plugins/MacOS/"${name_lua}
cp -rf "../Build/MacOS/$name_project/luac" "../../../../Plugins/MacOS/"${name_luac}


folderSrc="../$name/src"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done

