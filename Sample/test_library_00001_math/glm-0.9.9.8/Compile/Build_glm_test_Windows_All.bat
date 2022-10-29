@REM author: LostPeter
@REM time:   2022-10-29

set debug=%1

call ./Build_glm_test_Windows.bat test_library_00001_glm_001_vec %debug%
call ./Build_glm_test_Windows.bat test_library_00001_glm_002_matrix %debug%
call ./Build_glm_test_Windows.bat test_library_00001_glm_003_quaternion %debug%