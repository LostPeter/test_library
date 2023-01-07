#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

./Build_spdlog_test_MacOS.sh test_library_00023_spdlog_001_LogBase $debug
./Build_spdlog_test_MacOS.sh test_library_00023_spdlog_002_LogStdOut $debug
./Build_spdlog_test_MacOS.sh test_library_00023_spdlog_003_LogFileOut $debug