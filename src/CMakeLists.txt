set(solution_srcs
    Solution.cc
    FileUtils.cc
    Worker.cc
)

add_library(solution STATIC ${solution_srcs})

target_include_directories(solution INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(solution
  PUBLIC project_options
)
