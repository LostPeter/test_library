@REM author: LostPeter
@REM time:   2022-10-29

set debug=%1

call ./Build_lua_test_Windows.bat test_library_00010_lua_001_Base %debug%
call ./Build_lua_test_Windows.bat test_library_00010_lua_002_C %debug%
call ./Build_lua_test_Windows.bat test_library_00010_lua_003_Cpp %debug%