@REM #****************************************************************************
@REM # test_library - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-29
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

set debug=%1

call ./Build_spdlog_test_Windows.bat test_library_00023_spdlog_001_LogBase %debug%
call ./Build_spdlog_test_Windows.bat test_library_00023_spdlog_002_LogStdOut %debug%
call ./Build_spdlog_test_Windows.bat test_library_00023_spdlog_003_LogFileOut %debug%