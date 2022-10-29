# author: LostPeter
# time:   2022-10-29

debug=${1}

name=libsquish-1.15
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

if [ "$debug" == "debug" ]; then
    cp -rf "../Build/MacOS/"$name_project"/libsquishd.a" "../../../../Plugins/MacOS/"$name_lib
else
    cp -rf "../Build/MacOS/"$name_project"/libsquish.a" "../../../../Plugins/MacOS/"$name_lib
fi

folderSrc="../"$name
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done