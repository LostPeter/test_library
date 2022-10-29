# author: LostPeter
# time:   2022-10-29

name=mytinystl-2.0.0

folderSrc="../$name/MyTinySTL"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst