@REM #****************************************************************************
@REM # test_library - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-29
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

set debug=%1

call ./Build_lua_test_Windows.bat test_library_00010_lua_001_Base %debug%
call ./Build_lua_test_Windows.bat test_library_00010_lua_002_C %debug%
call ./Build_lua_test_Windows.bat test_library_00010_lua_003_Cpp %debug%