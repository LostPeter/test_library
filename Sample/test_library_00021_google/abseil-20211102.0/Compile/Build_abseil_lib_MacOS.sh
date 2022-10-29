# author: LostPeter
# time:   2022-10-29

debug=${1}

name=abseil-20211102.0
if [ $debug == "debug" ]; then
    name_project=$name"_d"
    name_lib="lib"$name"_d.a"
else
    name_project=$name
    name_lib="lib"$name".a"
fi

rm -rf "../Build/MacOS/"$name_project
mkdir -p "../Build/MacOS/"$name_project

rm -rf "../../../../Plugins/MacOS/"$name_lib
mkdir -p "../../../../Plugins/MacOS"



cd ..
cd Build
cd MacOS
cd $name_project

# lib
if [ "$debug" == "debug" ]; then
    cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug ../../../$name/
else
    cmake ../../../$name/
fi
make

mkdir -p "./libGen"
cd "./libGen"
libSrc="../absl"
# base
ar x $libSrc"/base/libabsl_base.a"
ar x $libSrc"/base/libabsl_log_severity.a"
ar x $libSrc"/base/libabsl_malloc_internal.a"
ar x $libSrc"/base/libabsl_raw_logging_internal.a"
ar x $libSrc"/base/libabsl_scoped_set_env.a"
ar x $libSrc"/base/libabsl_spinlock_wait.a"
ar x $libSrc"/base/libabsl_strerror.a"
ar x $libSrc"/base/libabsl_throw_delegate.a"
# container
ar x $libSrc"/container/libabsl_hashtablez_sampler.a"
ar x $libSrc"/container/libabsl_raw_hash_set.a"
# debugging
ar x $libSrc"/debugging/libabsl_debugging_internal.a"
ar x $libSrc"/debugging/libabsl_demangle_internal.a"
ar x $libSrc"/debugging/libabsl_examine_stack.a"
ar x $libSrc"/debugging/libabsl_failure_signal_handler.a"
ar x $libSrc"/debugging/libabsl_leak_check_disable.a"
ar x $libSrc"/debugging/libabsl_leak_check.a"
ar x $libSrc"/debugging/libabsl_stacktrace.a"
ar x $libSrc"/debugging/libabsl_symbolize.a"
# flags
ar x $libSrc"/flags/libabsl_flags_commandlineflag_internal.a"
ar x $libSrc"/flags/libabsl_flags_commandlineflag.a"
ar x $libSrc"/flags/libabsl_flags_config.a"
ar x $libSrc"/flags/libabsl_flags_internal.a"
ar x $libSrc"/flags/libabsl_flags_marshalling.a"
ar x $libSrc"/flags/libabsl_flags_parse.a"
ar x $libSrc"/flags/libabsl_flags_private_handle_accessor.a"
ar x $libSrc"/flags/libabsl_flags_program_name.a"
ar x $libSrc"/flags/libabsl_flags_reflection.a"
ar x $libSrc"/flags/libabsl_flags_usage_internal.a"
ar x $libSrc"/flags/libabsl_flags_usage.a"
ar x $libSrc"/flags/libabsl_flags.a"
# hash
ar x $libSrc"/hash/libabsl_city.a"
ar x $libSrc"/hash/libabsl_hash.a"
ar x $libSrc"/hash/libabsl_low_level_hash.a"
# numeric
ar x $libSrc"/numeric/libabsl_int128.a"
# profiling
ar x $libSrc"/profiling/libabsl_exponential_biased.a"
ar x $libSrc"/profiling/libabsl_periodic_sampler.a"
# random
ar x $libSrc"/random/libabsl_random_distributions.a"
ar x $libSrc"/random/libabsl_random_internal_distribution_test_util.a"
ar x $libSrc"/random/libabsl_random_internal_platform.a"
ar x $libSrc"/random/libabsl_random_internal_pool_urbg.a"
ar x $libSrc"/random/libabsl_random_internal_randen_hwaes_impl.a"
ar x $libSrc"/random/libabsl_random_internal_randen_hwaes.a"
ar x $libSrc"/random/libabsl_random_internal_randen_slow.a"
ar x $libSrc"/random/libabsl_random_internal_randen.a"
ar x $libSrc"/random/libabsl_random_internal_seed_material.a"
ar x $libSrc"/random/libabsl_random_seed_gen_exception.a"
ar x $libSrc"/random/libabsl_random_seed_sequences.a"
# status
ar x $libSrc"/status/libabsl_status.a"
ar x $libSrc"/status/libabsl_statusor.a"
# strings
ar x $libSrc"/strings/libabsl_cord_internal.a"
ar x $libSrc"/strings/libabsl_cord.a"
ar x $libSrc"/strings/libabsl_cordz_functions.a"
ar x $libSrc"/strings/libabsl_cordz_handle.a"
ar x $libSrc"/strings/libabsl_cordz_info.a"
ar x $libSrc"/strings/libabsl_cordz_sample_token.a"
ar x $libSrc"/strings/libabsl_str_format_internal.a"
ar x $libSrc"/strings/libabsl_strings_internal.a"
ar x $libSrc"/strings/libabsl_strings.a"
# synchronization
ar x $libSrc"/synchronization/libabsl_graphcycles_internal.a"
ar x $libSrc"/synchronization/libabsl_synchronization.a"
# time
ar x $libSrc"/time/libabsl_civil_time.a"
ar x $libSrc"/time/libabsl_time_zone.a"
ar x $libSrc"/time/libabsl_time.a"
# types
ar x $libSrc"/types/libabsl_bad_any_cast_impl.a"
ar x $libSrc"/types/libabsl_bad_optional_access.a"
ar x $libSrc"/types/libabsl_bad_variant_access.a"

ar cru "../"$name_lib *.o

cp -rf "../"$name_lib "../../../../../../../Plugins/MacOS/"$name_lib

cd ..

cd ..
cd ..
cd ..
cd Compile


folderSrc="../"$name"/absl"
folderDst="../../../../Include/"$name"/absl"
rm -rf $folderDst
mkdir -p $folderDst
#1, algorithm
mkdir -p $folderDst/algorithm
for file in ${folderSrc}/algorithm/*.h
do 
    cp -rf $file $folderDst/algorithm
done
#2, base
mkdir -p $folderDst/base/internal
for file in ${folderSrc}/base/*.h
do 
    cp -rf $file $folderDst/base
done
for file in ${folderSrc}/base/internal/*.h
do 
    cp -rf $file $folderDst/base/internal
done
#3, cleanup
mkdir -p $folderDst/cleanup/internal
for file in ${folderSrc}/cleanup/*.h
do 
    cp -rf $file $folderDst/cleanup
done
for file in ${folderSrc}/cleanup/internal/*.h
do 
    cp -rf $file $folderDst/cleanup/internal
done
#4, container
mkdir -p $folderDst/container/internal
for file in ${folderSrc}/container/*.h
do 
    cp -rf $file $folderDst/container
done
for file in ${folderSrc}/container/internal/*.h
do 
    cp -rf $file $folderDst/container/internal
done
#5, debugging
mkdir -p $folderDst/debugging/internal
for file in ${folderSrc}/debugging/*.h
do 
    cp -rf $file $folderDst/debugging
done
for file in ${folderSrc}/debugging/internal/*.h
do 
    cp -rf $file $folderDst/debugging/internal
done
#6, flags
mkdir -p $folderDst/flags/internal
for file in ${folderSrc}/flags/*.h
do 
    cp -rf $file $folderDst/flags
done
for file in ${folderSrc}/flags/internal/*.h
do 
    cp -rf $file $folderDst/flags/internal
done
#7, functional
mkdir -p $folderDst/functional/internal
for file in ${folderSrc}/functional/*.h
do 
    cp -rf $file $folderDst/functional
done
for file in ${folderSrc}/functional/internal/*.h
do 
    cp -rf $file $folderDst/functional/internal
done
#8, hash
mkdir -p $folderDst/hash/internal
for file in ${folderSrc}/hash/*.h
do 
    cp -rf $file $folderDst/hash
done
for file in ${folderSrc}/hash/internal/*.h
do 
    cp -rf $file $folderDst/hash/internal
done
#9, memory
mkdir -p $folderDst/memory
for file in ${folderSrc}/memory/*.h
do 
    cp -rf $file $folderDst/memory
done
#10, meta
mkdir -p $folderDst/meta
for file in ${folderSrc}/meta/*.h
do 
    cp -rf $file $folderDst/meta
done
#11, numeric
mkdir -p $folderDst/numeric/internal
for file in ${folderSrc}/numeric/*.h
do 
    cp -rf $file $folderDst/numeric
done
for file in ${folderSrc}/numeric/internal/*.h
do 
    cp -rf $file $folderDst/numeric/internal
done
#12, profiling
mkdir -p $folderDst/profiling/internal
for file in ${folderSrc}/profiling/internal/*.h
do 
    cp -rf $file $folderDst/profiling/internal
done
#13, random
mkdir -p $folderDst/random/internal
for file in ${folderSrc}/random/*.h
do 
    cp -rf $file $folderDst/random
done
for file in ${folderSrc}/random/internal/*.h
do 
    cp -rf $file $folderDst/random/internal
done
#14, status
mkdir -p $folderDst/status/internal
for file in ${folderSrc}/status/*.h
do 
    cp -rf $file $folderDst/status
done
for file in ${folderSrc}/status/internal/*.h
do 
    cp -rf $file $folderDst/status/internal
done
#15, strings
mkdir -p $folderDst/strings/internal
for file in ${folderSrc}/strings/*.h
do 
    cp -rf $file $folderDst/strings
done
for file in ${folderSrc}/strings/internal/*.h
do 
    cp -rf $file $folderDst/strings/internal
done
#16, synchronization
mkdir -p $folderDst/synchronization/internal
for file in ${folderSrc}/synchronization/*.h
do 
    cp -rf $file $folderDst/synchronization
done
for file in ${folderSrc}/synchronization/internal/*.h
do 
    cp -rf $file $folderDst/synchronization/internal
done
#17, time
mkdir -p $folderDst/time/internal
for file in ${folderSrc}/time/*.h
do 
    cp -rf $file $folderDst/time
done
for file in ${folderSrc}/time/internal/*.h
do 
    cp -rf $file $folderDst/time/internal
done
#18, types
mkdir -p $folderDst/types/internal
for file in ${folderSrc}/types/*.h
do 
    cp -rf $file $folderDst/types
done
for file in ${folderSrc}/types/internal/*.h
do 
    cp -rf $file $folderDst/types/internal
done
#19, utility
mkdir -p $folderDst/utility
for file in ${folderSrc}/utility/*.h
do 
    cp -rf $file $folderDst/utility
done