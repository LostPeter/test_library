#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

./Build_lua_test_MacOS.sh test_library_00010_lua_001_Base $debug
./Build_lua_test_MacOS.sh test_library_00010_lua_002_C $debug
./Build_lua_test_MacOS.sh test_library_00010_lua_003_Cpp $debug