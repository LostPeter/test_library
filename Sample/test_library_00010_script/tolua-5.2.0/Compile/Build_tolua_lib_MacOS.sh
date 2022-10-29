# author: LostPeter
# time:   2022-10-29

debug=${1}

name=tolua-5.2.0
if [ "$debug" == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
    name_tolua="tolua_d"
else
    name_project=$name
    name_lib="lib"$name".a"
    name_tolua="tolua"
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
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DLUA_VERSION="lua-5.4.3" -DLUA_LIB="MacOS/liblua-5.4.3_d.a" ../../../$name/ 
else
    cmake -DLUA_VERSION="lua-5.4.3" -DLUA_LIB="MacOS/liblua-5.4.3.a" ../../../$name/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/"$name_project"/libtolua_lib.a" "../../../../Plugins/MacOS/"$name_lib
cp -rf "../Build/MacOS/"$name_project"/tolua" "../../../../Plugins/MacOS/"$name_tolua

folderSrc="../$name/include"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done

