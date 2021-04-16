
cmake_minimum_required(VERSION 3.16.3)
project(soft_heap)

set(CMAKE_CXX_STANDARD 14)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

set(files
        src/SoftHeap/Element.cpp
        src/SoftHeap/BaseNode.h
        src/KaplanSoftHeap/TreeNode.cpp
        src/SoftSequenceHeap/SequenceNode.cpp
        )

add_executable(ApproxSortGenerator src/ApproxSortGenerator.cpp ${files} src/utils/ApproxSortUtils.cpp)
add_executable(KthElementGenerator src/KthElementGenerator.cpp ${files} src/utils/KthElementUtils.cpp)


# TESTS
add_executable(HeapStructureTest test/HeapStructureTest.cpp ${files})
target_link_libraries(HeapStructureTest gtest_main)

add_executable(NodeTest test/KaplanSoftHeapTest/TreeNodeTest.cpp ${files})
target_link_libraries(NodeTest gtest_main)

add_executable(SoftHeapTest test/SoftHeapTest.cpp ${files})
target_link_libraries(SoftHeapTest gtest_main)

add_executable(SequenceTest test/SoftSequenceHeapTest/SequenceNodeTest.cpp ${files})
target_link_libraries(SequenceTest gtest_main)