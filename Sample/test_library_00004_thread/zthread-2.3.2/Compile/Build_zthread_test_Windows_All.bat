@REM #****************************************************************************
@REM # test_library - Copyright (C) 2022 by LostPeter
@REM # 
@REM # Author: LostPeter
@REM # Time:   2022-10-29
@REM #
@REM # This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
@REM #****************************************************************************/

set debug=%1

call ./Build_zthread_test_Windows.bat test_library_00004_zthread_001_Runnable %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_002_Quit %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_003_Executor %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_004_Priority %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_005_CountedPtr %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_006_Guard %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_007_ThreadLocal %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_008_TaskCancel %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_009_Interrupt %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_010_Condition %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_011_ProducerConsumer %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_012_Queue %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_013_DeadLock %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_014_ReadWriteLock %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_015_Semaphore %debug%
call ./Build_zthread_test_Windows.bat test_library_00004_zthread_016_ThreadPool %debug%