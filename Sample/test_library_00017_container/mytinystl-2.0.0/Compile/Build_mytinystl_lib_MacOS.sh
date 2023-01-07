#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

name=mytinystl-2.0.0

folderSrc="../$name/MyTinySTL"
folderDst="../../../../Include/"$name
rm -rf $folderDst
mkdir -p $folderDst

cp -pr $folderSrc $folderDst