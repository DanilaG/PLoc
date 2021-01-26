project(PLoc_Modeling)

set(HEADERS
        )

set(EXECUTABLE
        main.cpp
        )

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1)

if(NOT cxxopts_POPULATED)
    FetchContent_Populate(cxxopts)
    include_directories(${cxxopts_SOURCE_DIR}/include)
endif()

include_directories(../Src)
include_directories(../Src/Structures)

add_executable(${PROJECT_NAME} ${HEADERS} ${EXECUTABLE})