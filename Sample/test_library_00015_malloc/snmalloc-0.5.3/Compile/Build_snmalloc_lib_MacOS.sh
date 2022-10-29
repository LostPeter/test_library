# author: LostPeter
# time:   2022-10-29

name=snmalloc-0.5.3

folderSrc="../$name/src/"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst