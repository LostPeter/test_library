# author: LostPeter
# time:   2022-10-29

name=glm-0.9.9.8

folderSrc="../$name/include/glm"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst