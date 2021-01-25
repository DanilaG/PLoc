project(LDTC_TESTS)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.1
        UPDATE_DISCONNECTED 1
)

FetchContent_MakeAvailable(googletest)

macro(package_add_test TESTNAME)
    include_directories(../Src)
    include_directories(../Src/Structures)
    add_executable(${TESTNAME} ${ARGN} TestTools.h TestTools.cpp)
    target_link_libraries(${TESTNAME} gtest gmock gtest_main PLoc)
    set_property(TARGET ${TESTNAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${TESTNAME}>)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
    set(ALL_TEST ${ALL_TEST} ${ARGN})
endmacro()

package_add_test(FULL_LDTC_TESTS Tests.cpp)
