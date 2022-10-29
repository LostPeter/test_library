@REM author: LostPeter
@REM time:   2022-10-29

set name="rapidxml-1.13"

set include_folder="..\..\..\..\Include\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\%name%\*.hpp" %include_folder%"\"