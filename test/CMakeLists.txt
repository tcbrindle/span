
add_executable(test_span11
    catch_main.cpp
    test_span.cpp
)
target_link_libraries(test_span11 PUBLIC span)
target_compile_features(test_span11 PUBLIC cxx_std_11)

add_executable(test_span14
    catch_main.cpp
    test_span.cpp
)
target_link_libraries(test_span14 PUBLIC span)
target_compile_features(test_span14 PUBLIC cxx_std_14)

add_executable(test_span17
    catch_main.cpp
    test_span.cpp
    )
target_link_libraries(test_span17 PUBLIC span)
target_compile_features(test_span17 PUBLIC cxx_std_17)