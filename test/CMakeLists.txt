add_executable(solutionTest SolutionTest.cc)

target_link_libraries(solutionTest PRIVATE
    solution
    GTest::GTest
    GTest::Main
    -Wl,--copy-dt-needed-entries
    ${GFLAGS_LIBRARIES}
    ${GLOG_LIBRARIES}
    ${IBERTY_LIBRARY}
)

add_test(
    NAME
        unit
    COMMAND
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/solutionTest
    )