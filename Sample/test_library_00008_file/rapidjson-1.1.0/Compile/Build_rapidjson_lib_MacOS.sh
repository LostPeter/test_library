# author: LostPeter
# time:   2022-10-29

name=rapidjson-1.1.0

folderSrc="../$name/include/rapidjson"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst