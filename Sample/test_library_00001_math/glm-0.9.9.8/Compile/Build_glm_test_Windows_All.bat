@REM #****************************************************************************
@REM # test_library - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-29
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

set debug=%1

call ./Build_glm_test_Windows.bat test_library_00001_glm_001_vec %debug%
call ./Build_glm_test_Windows.bat test_library_00001_glm_002_matrix %debug%
call ./Build_glm_test_Windows.bat test_library_00001_glm_003_quaternion %debug%