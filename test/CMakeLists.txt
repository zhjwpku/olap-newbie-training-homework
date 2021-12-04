add_executable(solutionTest SolutionTest.cc)

target_link_libraries(solutionTest PRIVATE
    solution
    GTest::GTest
    GTest::Main
)

add_test(
    NAME
        unit
    COMMAND
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/solutionTest
    )