@REM author: LostPeter
@REM time:   2022-10-29

set name="rapidjson-1.1.0"

set include_folder="..\..\..\..\Include\"%name%
if exist %include_folder% (
    rmdir /S/Q %include_folder%
)
mkdir %include_folder%

xcopy /S /E /Y /F "..\%name%\include\rapidjson" %include_folder%"\rapidjson\"