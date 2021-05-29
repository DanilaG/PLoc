project(PLoc_Modeling2)

set(HEADERS
        Src2/Algorithm.h
        Src2/Combiner.h
        Src2/ErrorGenerator.h
        Src2/Experimenter.h
        Src2/FactoryFromJSON.h
        Src2/Scene.h
        )

set(EXECUTABLE
        main.cpp
        Src2/ErrorGenerator.cpp
        Src2/Experimenter.cpp
        Src2/FactoryFromJSON.cpp
        )

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v2.2.1)

if(NOT cxxopts_POPULATED)
    FetchContent_Populate(cxxopts)
    include_directories(${cxxopts_SOURCE_DIR}/include)
endif()

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.7.2)

if(NOT magic_enum_POPULATED)
    FetchContent_Populate(magic_enum)
    include_directories(${magic_enum_SOURCE_DIR}/include)
endif()

include_directories(
        ../../Src
        ../../Src/Structures
)

add_executable(${PROJECT_NAME} ${HEADERS} ${EXECUTABLE})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json PLoc)
