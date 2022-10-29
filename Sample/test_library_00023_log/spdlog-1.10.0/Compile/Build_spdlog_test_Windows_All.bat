@REM author: LostPeter
@REM time:   2022-10-29

set debug=%1

call ./Build_spdlog_test_Windows.bat test_library_00023_spdlog_001_LogBase %debug%
call ./Build_spdlog_test_Windows.bat test_library_00023_spdlog_002_LogStdOut %debug%
call ./Build_spdlog_test_Windows.bat test_library_00023_spdlog_003_LogFileOut %debug%