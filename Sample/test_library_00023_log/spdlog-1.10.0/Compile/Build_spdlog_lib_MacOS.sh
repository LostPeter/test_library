# author: LostPeter
# time:   2022-10-29

name=spdlog-1.10.0

folderSrc="../$name/include/spdlog"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -r $folderSrc $folderDst