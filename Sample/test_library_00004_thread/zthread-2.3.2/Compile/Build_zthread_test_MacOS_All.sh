#****************************************************************************
# test_library - Copyright (C) 2022 by LostPeter
# 
# Author: LostPeter
# Time:   2022-10-29
#
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
#****************************************************************************/

debug=${1}

./Build_zthread_test_MacOS.sh test_library_00004_zthread_001_Runnable $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_002_Quit $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_003_Executor $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_004_Priority $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_005_CountedPtr $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_006_Guard $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_007_ThreadLocal $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_008_TaskCancel $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_009_Interrupt $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_010_Condition $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_011_ProducerConsumer $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_012_Queue $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_013_DeadLock $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_014_ReadWriteLock $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_015_Semaphore $debug
./Build_zthread_test_MacOS.sh test_library_00004_zthread_016_ThreadPool $debug