declare_cmake_min_version()

add_library(AgramtabLib
        agramtab.cpp
        agramtab.h
        EngGramTab.cpp
        EngGramTab.h
        RusGramTab.cpp
        RusGramTab.h
        UkrGramTab.cpp
        UkrGramTab.h
        GerGramTab.cpp
        GerGramTab.h
        SpaGramTab.cpp
        SpaGramTab.h
        FreGramTab.cpp
        FreGramTab.h
        PorGramTab.cpp
        PorGramTab.h
        LatGramTab.cpp
        LatGramTab.h

        GramInfo.h
 )

target_link_libraries(AgramtabLib
        morphology_common
)

add_subdirectory (tests/CheckGramTab)