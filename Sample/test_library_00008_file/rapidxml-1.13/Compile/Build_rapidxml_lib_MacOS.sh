# author: LostPeter
# time:   2022-10-29

name=rapidxml-1.13

folderSrc="../$name"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

for file in ${folderSrc}/*.hpp
do 
    cp -rf $file $folderDst
done