# author: LostPeter
# time:   2022-10-29

debug=${1}

name_sample="test_library_00005_imgui_005_vulkan_glfw"
if [ "$debug" == "debug" ]; then
    name_project=$name_sample"_d"
else
    name_project=$name_sample
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Bin/MacOS/"$name_project
mkdir -p "../../../../Bin/MacOS"


cd ..
cd Build
cd MacOS
cd $name_project

if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 ../../../$name_sample/
else
    cmake ../../../$name_sample/
fi
make

cd ..
cd ..
cd ..
cd Compile

cp -rf "../Build/MacOS/$name_project/$name_project" "../../../../Bin/MacOS"