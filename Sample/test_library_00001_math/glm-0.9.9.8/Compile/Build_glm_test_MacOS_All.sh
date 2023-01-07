#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

./Build_glm_test_MacOS.sh test_library_00001_glm_001_vec $debug
./Build_glm_test_MacOS.sh test_library_00001_glm_002_matrix $debug
./Build_glm_test_MacOS.sh test_library_00001_glm_003_quaternion $debug