# # # # sol2
# The MIT License (MIT)
# 
# Copyright (c) 2013-2022 Rapptz, ThePhD, and contributors
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# # # # sol2 tests - environment

file(GLOB sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	source/*.cpp)

sol2_create_basic_test(sol2.tests.exceptions sol2::sol2 ${sources})
sol2_create_basic_test(sol2.tests.exceptions.SOL_ALL_SAFETIES_ON sol2::sol2 ${sources})
target_compile_options(sol2.tests.exceptions
	PRIVATE
	${--allow-unreachable-code})
target_compile_options(sol2.tests.exceptions.SOL_ALL_SAFETIES_ON
	PRIVATE
	${--allow-unreachable-code})
target_compile_definitions(sol2.tests.exceptions.SOL_ALL_SAFETIES_ON
	PRIVATE
	SOL_ALL_SAFETIES_ON=1)

if (WAY_TOO_BUSTED_RIGHT_NOW)

sol2_create_basic_test(sol2.tests.exceptions.SOL_EXCEPTIONS_ALWAYS_UNSAFE sol2::sol2 ${sources})
target_compile_options(sol2.tests.exceptions.SOL_EXCEPTIONS_ALWAYS_UNSAFE
	PRIVATE
	${--allow-unreachable-code})
target_compile_definitions(sol2.tests.exceptions.SOL_EXCEPTIONS_ALWAYS_UNSAFE
	PRIVATE
	SOL_EXCEPTIONS_ALWAYS_UNSAFE=0)

endif()
