# author: LostPeter
# time:   2022-10-29

name=stb-0.02

folderSrc="../$name"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.h
do 
    cp -rf $file $folderDst
done