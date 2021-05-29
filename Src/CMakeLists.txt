project(PLoc)

set(HEADERS
        Debug/Log.h
        Localization/LocalizationTools.h
        Localization/Localization.h
        Localization/Combiner/Combiner.h
        Localization/Combiner/MeanCombiner.h
        Localization/Combiner/FilteredMeanCombiner.h
        Localization/Combiner/MassCombiner.h
        Localization/Combiner/MedianCombiner.h
        Localization/Combiner/ParallelCombiner.h
        Localization/Combiner/TimeDifferenceCombiner.h
        Localization/Combiner/TriangleCombiner.h
        Structures/Geometric/Points/Point.h
        Structures/Geometric/Points/SpherePoint.h
        Structures/Geometric/Points/TimePoint.h
        )

set(EXECUTABLE
        Debug/Log.cpp
        Localization/Combiner/MeanCombiner.cpp
        Localization/Combiner/FilteredMeanCombiner.cpp
        Localization/Combiner/MassCombiner.cpp
        Localization/Combiner/MedianCombiner.cpp
        Localization/Combiner/ParallelCombiner.cpp
        Localization/Combiner/TimeDifferenceCombiner.cpp
        Localization/Combiner/TriangleCombiner.cpp
        Localization/DirectMethod.cpp
        Localization/Localization.cpp
        Localization/QuadMethod.cpp
        Localization/QPMethod.cpp
        Structures/Geometric/Points/SpherePoint.cpp
        )

include_directories(Structures)

add_library(${PROJECT_NAME} ${HEADERS} ${EXECUTABLE})
