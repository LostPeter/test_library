# author: LostPeter
# time:   2022-10-29

name=sol2-3.3.0

folderSrc="../$name/include/sol"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -r $folderSrc $folderDst