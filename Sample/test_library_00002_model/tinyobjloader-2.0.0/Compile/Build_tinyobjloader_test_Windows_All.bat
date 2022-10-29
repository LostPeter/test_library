@REM author: LostPeter
@REM time:   2022-10-29

set debug=%1

call ./Build_tinyobjloader_test_Windows.bat test_library_00002_tinyobjloader_001_mesh %debug%
call ./Build_tinyobjloader_test_Windows.bat test_library_00002_tinyobjloader_002_image %debug%
call ./Build_tinyobjloader_test_Windows.bat test_library_00002_tinyobjloader_003_material %debug%