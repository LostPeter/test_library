@REM author: LostPeter
@REM time:   2022-10-29

@echo off
set debug=%1
echo %debug%

set name="abseil-20211102.0"
if "%debug%" == "debug" (
    set name_project=%name%_d
    set name_lib=%name%_d.lib
    set name_build=Debug
) else (
    set name_project=%name%
    set name_lib=%name%.lib
    set name_build=Release
)

@rem build folder
set build_folder="..\Build\Windows\"%name_project%
if exist %build_folder% (
    rmdir /S/Q %build_folder%
)
mkdir %build_folder%


@rem Plugins folder/file
set plugins_folder="..\..\..\..\Plugins\Windows"
if not exist %plugins_folder% (
    mkdir %plugins_folder%
)
set plugins_libfile=%plugins_folder%"\"%name_lib%
if exist %plugins_libfile% (
    del /S/Q %plugins_libfile%
)


cd ..
cd Build
cd Windows
cd %name_project%

if "%debug%" == "debug" (
    cmake -DDEBUG=1 "../../../%name%/"
    msbuild absl.sln /p:configuration=debug
) else (
    cmake "../../../%name%/"
    msbuild absl.sln /p:configuration=release
)

set lib_gen=".\libGen"
if exist %lib_gen% (
    rmdir /S/Q %lib_gen%
)
mkdir %lib_gen%
cd %lib_gen%
set libSrc=..\absl
lib.exe /out:%name_lib% %libSrc%\base\%name_build%\absl_base.lib %libSrc%\base\%name_build%\absl_log_severity.lib %libSrc%\base\%name_build%\absl_malloc_internal.lib %libSrc%\base\%name_build%\absl_raw_logging_internal.lib %libSrc%\base\%name_build%\absl_scoped_set_env.lib %libSrc%\base\%name_build%\absl_spinlock_wait.lib %libSrc%\base\%name_build%\absl_strerror.lib %libSrc%\base\%name_build%\absl_throw_delegate.lib %libSrc%\container\%name_build%\absl_hashtablez_sampler.lib %libSrc%\container\%name_build%\absl_raw_hash_set.lib %libSrc%\debugging\%name_build%\absl_debugging_internal.lib %libSrc%\debugging\%name_build%\absl_demangle_internal.lib %libSrc%\debugging\%name_build%\absl_examine_stack.lib %libSrc%\debugging\%name_build%\absl_failure_signal_handler.lib %libSrc%\debugging\%name_build%\absl_leak_check.lib %libSrc%\debugging\%name_build%\absl_leak_check_disable.lib %libSrc%\debugging\%name_build%\absl_stacktrace.lib %libSrc%\debugging\%name_build%\absl_symbolize.lib %libSrc%\flags\%name_build%\absl_flags.lib %libSrc%\flags\%name_build%\absl_flags_commandlineflag.lib %libSrc%\flags\%name_build%\absl_flags_commandlineflag_internal.lib %libSrc%\flags\%name_build%\absl_flags_config.lib %libSrc%\flags\%name_build%\absl_flags_internal.lib %libSrc%\flags\%name_build%\absl_flags_marshalling.lib %libSrc%\flags\%name_build%\absl_flags_parse.lib %libSrc%\flags\%name_build%\absl_flags_private_handle_accessor.lib %libSrc%\flags\%name_build%\absl_flags_program_name.lib %libSrc%\flags\%name_build%\absl_flags_reflection.lib %libSrc%\flags\%name_build%\absl_flags_usage.lib %libSrc%\flags\%name_build%\absl_flags_usage_internal.lib %libSrc%\hash\%name_build%\absl_city.lib %libSrc%\hash\%name_build%\absl_hash.lib %libSrc%\hash\%name_build%\absl_low_level_hash.lib %libSrc%\numeric\%name_build%\absl_int128.lib %libSrc%\profiling\%name_build%\absl_exponential_biased.lib %libSrc%\profiling\%name_build%\absl_periodic_sampler.lib %libSrc%\random\%name_build%\absl_random_distributions.lib %libSrc%\random\%name_build%\absl_random_internal_distribution_test_util.lib %libSrc%\random\%name_build%\absl_random_internal_platform.lib %libSrc%\random\%name_build%\absl_random_internal_pool_urbg.lib %libSrc%\random\%name_build%\absl_random_internal_randen.lib %libSrc%\random\%name_build%\absl_random_internal_randen_hwaes.lib %libSrc%\random\%name_build%\absl_random_internal_randen_hwaes_impl.lib %libSrc%\random\%name_build%\absl_random_internal_randen_slow.lib %libSrc%\random\%name_build%\absl_random_internal_seed_material.lib %libSrc%\random\%name_build%\absl_random_seed_gen_exception.lib %libSrc%\random\%name_build%\absl_random_seed_sequences.lib %libSrc%\status\%name_build%\absl_status.lib %libSrc%\status\%name_build%\absl_statusor.lib %libSrc%\strings\%name_build%\absl_cord.lib %libSrc%\strings\%name_build%\absl_cord_internal.lib %libSrc%\strings\%name_build%\absl_cordz_functions.lib %libSrc%\strings\%name_build%\absl_cordz_handle.lib %libSrc%\strings\%name_build%\absl_cordz_info.lib %libSrc%\strings\%name_build%\absl_cordz_sample_token.lib %libSrc%\strings\%name_build%\absl_str_format_internal.lib %libSrc%\strings\%name_build%\absl_strings_internal.lib %libSrc%\strings\%name_build%\absl_strings.lib %libSrc%\synchronization\%name_build%\absl_graphcycles_internal.lib %libSrc%\synchronization\%name_build%\absl_synchronization.lib %libSrc%\time\%name_build%\absl_civil_time.lib %libSrc%\time\%name_build%\absl_time.lib %libSrc%\time\%name_build%\absl_time_zone.lib %libSrc%\types\%name_build%\absl_bad_any_cast_impl.lib %libSrc%\types\%name_build%\absl_bad_optional_access.lib %libSrc%\types\%name_build%\absl_bad_variant_access.lib

copy /Y ".\"%name_lib% "..\..\..\..\..\..\..\Plugins\Windows\"%name_lib%

cd ..

cd ..
cd ..
cd ..
cd Compile



set folderSrc="..\"%name%"\absl"
set folderDst="..\..\..\..\Include\"%name%"\absl"
if exist %folderDst% (
    rmdir /S/Q %folderDst%
)
mkdir %folderDst%

@REM 1, algorithm
mkdir %folderDst%"\algorithm"
xcopy /S /E /Y /F "%folderSrc%\algorithm\*.h" %folderDst%"\algorithm\"

@REM 2, base
mkdir %folderDst%"\base"
mkdir %folderDst%"\base\internal"
xcopy /S /E /Y /F "%folderSrc%\base\*.h" %folderDst%"\base\"
xcopy /S /E /Y /F "%folderSrc%\base\internal\*.h" %folderDst%"\base\internal\"

@REM 3, cleanup
mkdir %folderDst%"\cleanup"
mkdir %folderDst%"\cleanup\internal"
xcopy /S /E /Y /F "%folderSrc%\cleanup\*.h" %folderDst%"\cleanup\"
xcopy /S /E /Y /F "%folderSrc%\cleanup\internal\*.h" %folderDst%"\cleanup\internal\"

@REM 4, container
mkdir %folderDst%"\container"
mkdir %folderDst%"\container\internal"
xcopy /S /E /Y /F "%folderSrc%\container\*.h" %folderDst%"\container\"
xcopy /S /E /Y /F "%folderSrc%\container\internal\*.h" %folderDst%"\container\internal\"

@REM 5, debugging
mkdir %folderDst%"\debugging"
mkdir %folderDst%"\debugging\internal"
xcopy /S /E /Y /F "%folderSrc%\debugging\*.h" %folderDst%"\debugging\"
xcopy /S /E /Y /F "%folderSrc%\debugging\internal\*.h" %folderDst%"\debugging\internal\"

@REM 6, flags
mkdir %folderDst%"\flags"
mkdir %folderDst%"\flags\internal"
xcopy /S /E /Y /F "%folderSrc%\flags\*.h" %folderDst%"\flags\"
xcopy /S /E /Y /F "%folderSrc%\flags\internal\*.h" %folderDst%"\flags\internal\"

@REM 7, functional
mkdir %folderDst%"\functional"
mkdir %folderDst%"\functional\internal"
xcopy /S /E /Y /F "%folderSrc%\functional\*.h" %folderDst%"\functional\"
xcopy /S /E /Y /F "%folderSrc%\functional\internal\*.h" %folderDst%"\functional\internal\"

@REM 8, hash
mkdir %folderDst%"\hash"
mkdir %folderDst%"\hash\internal"
xcopy /S /E /Y /F "%folderSrc%\hash\*.h" %folderDst%"\hash\"
xcopy /S /E /Y /F "%folderSrc%\hash\internal\*.h" %folderDst%"\hash\internal\"

@REM 9, memory
mkdir %folderDst%"\memory"
xcopy /S /E /Y /F "%folderSrc%\memory\*.h" %folderDst%"\memory\"

@REM 10, meta
mkdir %folderDst%"\meta"
xcopy /S /E /Y /F "%folderSrc%\meta\*.h" %folderDst%"\meta\"

@REM 11, numeric
mkdir %folderDst%"\numeric"
mkdir %folderDst%"\numeric\internal"
xcopy /S /E /Y /F "%folderSrc%\numeric\*.h" %folderDst%"\numeric\"
xcopy /S /E /Y /F "%folderSrc%\numeric\internal\*.h" %folderDst%"\numeric\internal\"

@REM 12, profiling
mkdir %folderDst%"\profiling"
mkdir %folderDst%"\profiling\internal"
xcopy /S /E /Y /F "%folderSrc%\profiling\internal\*.h" %folderDst%"\profiling\internal\"

@REM 13, random
mkdir %folderDst%"\random"
mkdir %folderDst%"\random\internal"
xcopy /S /E /Y /F "%folderSrc%\random\*.h" %folderDst%"\random\"
xcopy /S /E /Y /F "%folderSrc%\random\internal\*.h" %folderDst%"\random\internal\"

@REM 14, status
mkdir %folderDst%"\status"
mkdir %folderDst%"\status\internal"
xcopy /S /E /Y /F "%folderSrc%\status\*.h" %folderDst%"\status\"
xcopy /S /E /Y /F "%folderSrc%\status\internal\*.h" %folderDst%"\status\internal\"

@REM 15, strings
mkdir %folderDst%"\strings"
mkdir %folderDst%"\strings\internal"
xcopy /S /E /Y /F "%folderSrc%\strings\*.h" %folderDst%"\strings\"
xcopy /S /E /Y /F "%folderSrc%\strings\internal\*.h" %folderDst%"\strings\internal\"

@REM 16, synchronization
mkdir %folderDst%"\synchronization"
mkdir %folderDst%"\synchronization\internal"
xcopy /S /E /Y /F "%folderSrc%\synchronization\*.h" %folderDst%"\synchronization\"
xcopy /S /E /Y /F "%folderSrc%\synchronization\internal\*.h" %folderDst%"\synchronization\internal\"

@REM 17, time
mkdir %folderDst%"\time"
mkdir %folderDst%"\time\internal"
xcopy /S /E /Y /F "%folderSrc%\time\*.h" %folderDst%"\time\"
xcopy /S /E /Y /F "%folderSrc%\time\internal\*.h" %folderDst%"\time\internal\"

@REM 18, types
mkdir %folderDst%"\types"
mkdir %folderDst%"\types\internal"
xcopy /S /E /Y /F "%folderSrc%\types\*.h" %folderDst%"\types\"
xcopy /S /E /Y /F "%folderSrc%\types\internal\*.h" %folderDst%"\types\internal\"

@REM 19, utility
mkdir %folderDst%"\utility"
xcopy /S /E /Y /F "%folderSrc%\utility\*.h" %folderDst%"\utility\"