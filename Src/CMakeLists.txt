project(PLoc)

set(HEADERS
        Debug/Log.h
        Localization/LocalizationTools.h
        Localization/Localization.h
        Localization/Combiner/Combiner.h
        Localization/Combiner/MeanCombiner.h
        Localization/Combiner/FilteredMeanCombiner.h
        Structures/Geometric/Points/Point.h
        Structures/Geometric/Points/TimePoint.h
        )

set(EXECUTABLE
        Debug/Log.cpp
        Localization/Combiner/MeanCombiner.cpp
        Localization/Combiner/FilteredMeanCombiner.cpp
        Localization/DirectMethod.cpp
        Localization/Localization.cpp
        Localization/QuadMethod.cpp
        Localization/VectorMethod.cpp
        )

include_directories(Structures)

add_library(${PROJECT_NAME} ${HEADERS} ${EXECUTABLE})
