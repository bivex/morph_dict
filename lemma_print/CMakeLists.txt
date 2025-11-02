declare_cmake_min_version()

project(lemma_print)

add_executable (${PROJECT_NAME}
        lemma_print.cpp
)

target_link_libraries(${PROJECT_NAME} 
    lemmatizer_base_lib
)



