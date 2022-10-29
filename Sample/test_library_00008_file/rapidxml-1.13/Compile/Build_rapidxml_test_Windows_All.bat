@REM author: LostPeter
@REM time:   2022-10-29

set debug=%1

call ./Build_rapidxml_test_Windows.bat test_library_00008_rapidxml_001_Parse %debug%
call ./Build_rapidxml_test_Windows.bat test_library_00008_rapidxml_002_Wrapper %debug%