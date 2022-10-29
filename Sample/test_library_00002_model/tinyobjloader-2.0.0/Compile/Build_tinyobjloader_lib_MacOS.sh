# author: LostPeter
# time:   2022-10-29

name=tinyobjloader-2.0.0

folderSrc="../$name"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done